/**
 * Masiro Reader: a EPD book reader.
 * Copyright (C) 2022  Cerallin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "textnode.h"

#define CHAR_LF    0x000A
#define CHAR_SPACE 0x0020

const CodePoint breaks[] = {
    CHAR_SPACE, // space
    CHAR_LF,    // LF
    0x002c,     // '.'
    0x002e,     // ','
    0x0000,     // EOS
};

const CodePoint ch_breaks[] = {
    0x201c, // '“'
    0x3002, // '。'
    0x300a, // '《'
    0xff0c, // ','
    0x0000, // EOS
};

#define NextLine(x, y, left, lineHeight)                                       \
    {                                                                          \
        x = left;                                                              \
        y += lineHeight;                                                       \
        if (y + lineHeight > maxHeight) {                                      \
            charNum = c - codepoints;                                          \
            break;                                                             \
        }                                                                      \
        if (x == CHAR_SPACE) {                                                 \
            x++;                                                               \
            continue;                                                          \
        }                                                                      \
    }

extern FontFace defaultFontFace;

TextNode::TextNode() {
    FontFamily fontFamily(&defaultFontFace);
    innerText = nullptr;
    innerTextLength = 0;
}

void TextNode::SetInnerText(CodePoint *str, size_t len) {
    innerText = str;
    innerTextLength = len;
}

void TextNode::Render(void) {

    // Next line flag
    int flagNextLine = 0;
    /* 获取字符的边框（边界） */
    int ix0, iy0, ix1, iy1;
    unsigned char *bitmap;

    /* Smoother unscaled x */
    float x = font->Unscale(textPadding.paddingLeft), left = x;
    int y = textPadding.paddingTop;
    int lineHeight = font->GetLineHeight(1.5f);

    int maxLineLength = GetRelativeWidth() - textPadding.paddingRight;
    int maxHeight = GetRelativeHeight() - textPadding.paddingBottom;

    /* Codepoints always starts with 0xFEFF */
    for (auto c = codepoints + 1; c - codepoints < charNum; c++) {
        auto next_c = c + 1;
        auto ts = &this->typeSetting[c - codepoints - 1];

        /* Handle LF */
        if (*c < CHAR_SPACE) { // space
            if (*c == CHAR_LF) {
                // Goto next line
                NextLine(x, y, left, lineHeight);
                continue;
            } else { // Invalid char?
                continue;
            }
        }

        /* Auto linefeed for latin chars */
        if (u16String::FindNextChar(breaks, *c) != nullptr) {
            auto next_break = u16String::FindNextBreak(c + 1, breaks);
            if (next_break != nullptr) {
                int advanceWidth;
                int sum = 0;
                for (auto tmp = c; tmp < next_break; tmp++) {
                    font->GetCodepointBitmapBox(*tmp, &ix0, 0, 0, 0);
                    font->GetCodepointHMetrics(*tmp, &advanceWidth, 0);
                    sum += font->Unscale(ix0) + advanceWidth;
                    sum += font->GetCodepointKernAdvance(*tmp, *(tmp + 1));
                }
                flagNextLine = (font->Scale(x + sum) > maxLineLength);
            }
        }
        // TODO: Auto lf for Chinese chars
        else if (*(c + 2) && *(c + 2) > 0x2000 &&
                 u16String::FindNextChar(ch_breaks, *(c + 2)) != nullptr) {
            int advanceWidth;
            int sum = 0;
            for (auto tmp = c; tmp < c + 3; tmp++) {
                font->GetCodepointBitmapBox(*tmp, &ix0, 0, 0, 0);
                font->GetCodepointHMetrics(*tmp, &advanceWidth, 0);
                sum += font->Unscale(ix0) + advanceWidth;
                sum += font->GetCodepointKernAdvance(*tmp, *(tmp + 1));
            }
            flagNextLine = (font->Scale(x + sum) > maxLineLength);
        }

        font->GetCodepointBitmapBox(*c, &ix0, &iy0, 0, 0);
        x += font->Unscale(ix0);

        font->GetCodepointHMetrics(*c, &ts->advancedWith, &ts->leftSideBearing);
        font->GetCodepointBitmap(*c, &ts->width, &ts->height, 0, 0);

        if (font->Scale(x + ts->leftSideBearing + font->Unscale(ts->width)) >=
            maxLineLength) {
            NextLine(x, y, left - ts->leftSideBearing, lineHeight);
        }

        ts->x = x;
        ts->y = y;
        ts->iy0 = iy0;

        /* 调整x */
        x += ts->advancedWith;

        if (*next_c) {
            /* 调整字距 */
            ts->kern = font->GetCodepointKernAdvance(*c, *next_c);
            x += ts->kern;
        }

        if (flagNextLine) {
            flagNextLine = 0;
            NextLine(x, y, left, lineHeight);
            if (*c == CHAR_SPACE) {
                continue;
            }
        }
    }

    TextTypeSetting::AdjustAlign(typeSetting, charNum, textAlign, maxLineLength,
                                 font);

    auto ts = typeSetting;
    int ascent = font->GetScaledAscent();
    for (auto c = codepoints + 1; c - codepoints < charNum; c++, ts++) {
        bitmap = font->GetCodepointBitmap(*c, 0, 0, 0, 0);
        DrawCharAt(font->Scale(ts->x + ts->leftSideBearing),
                   ts->y + ts->iy0 + ascent, bitmap, ts->width, ts->height,
                   font);
    }

    font->FreeBitmap(bitmap);
}
