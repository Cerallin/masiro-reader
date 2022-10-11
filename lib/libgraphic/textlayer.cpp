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

#include "textlayer.h"
#include "debug.h"

#include <new>
#include <stdio.h>
#include <string.h>

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

TextLayer::TextLayer(uint32_t width, uint32_t height, Font *font,
                     Graphic::TextAlign textAlign, int32_t rotate)
    : Layer(width, height, rotate), font(font), textAlign(textAlign) {}

TextLayer::TextLayer(uint32_t width, uint32_t height, CodePoint *codepoints,
                     Font *font, Graphic::TextAlign textAlign, int32_t rotate)
    : Layer(width, height, rotate), codepoints(codepoints), font(font),
      textAlign(textAlign) {}

TextLayer::TextLayer(const Layer &layer, Graphic::TextAlign textAlign)
    : Layer(layer), textAlign(textAlign) {}

TextLayer::~TextLayer() {
    if (codepoints != nullptr)
        delete[] codepoints;
    if (typeSetting != nullptr)
        delete[] typeSetting;
}

void TextLayer::DrawGlyph(const Graphic::TextTypeSetting *ts, Font *font,
                          const unsigned char *bitmap) {
    int32_t x, y;

    x = font->Scale(ts->x + ts->leftSideBearing);
    y = ts->y + ts->iy0 + ts->ascent;

    LoopMatrix(ts->width, ts->height, x, y) {
        int color = GetMatrix(bitmap, ts->width, i - x, j - y) >> 6;
        // FIXME This is some kind of "alpha" tunnel
        if (this->invertColor) {
            if (color != COLOR_WW)
                DrawPixel(i, j, ~color);
        } else {
            if (color != COLOR_BB)
                DrawPixel(i, j, color);
        }
    };
}

int TextLayer::SetText(char *str) {
    size_t srcLen = strlen(str), destLen = (srcLen + 4) * 2;
    codepoints = new (std::nothrow) CodePoint[destLen / 2]();
    if (codepoints == nullptr) {
        return -1;
    }

    charNum = CodePoint::StrToUnicode(str, srcLen, &codepoints, destLen);
    if (charNum == -1) {
        delete[] codepoints;
        return -1;
    }

    typeSetting = new (std::nothrow) Graphic::TextTypeSetting[charNum]();
    if (typeSetting == nullptr) {
        delete[] codepoints;
        return -1;
    }
    return 0;
}

TextLayer &TextLayer::SetTextAlign(Graphic::TextAlign textAlign) {
    textAlign = textAlign;
    return *this;
}

TextLayer &TextLayer::SetTextPadding(Graphic::TextPadding textPadding) {
    textPadding = textPadding;
    return *this;
}

TextLayer &TextLayer::SetTextPadding(int padding) {
    textPadding.paddingLeft = padding;
    textPadding.paddingRight = padding;
    textPadding.paddingTop = padding;
    textPadding.paddingBottom = padding;
    return *this;
}

TextLayer &TextLayer::SetTextPadding(int paddingX, int paddingY) {
    textPadding.paddingLeft = paddingX;
    textPadding.paddingRight = paddingX;
    textPadding.paddingTop = paddingY;
    textPadding.paddingBottom = paddingY;
    return *this;
}

TextLayer &TextLayer::SetTextPadding(int paddingLeft, int paddingTop,
                                     int paddingRight, int paddingBottom) {
    textPadding.paddingLeft = paddingLeft;
    textPadding.paddingRight = paddingRight;
    textPadding.paddingTop = paddingTop;
    textPadding.paddingBottom = paddingBottom;
    return *this;
}

void TextLayer::Render() {
    // Debug checks
    debug_is_initialized(font);
    debug_is_initialized(codepoints);
    debug_is_initialized(typeSetting);

    // Next line flag
    int flagNextLine = 0;
    /* Border of a glyph */
    int ix0, iy0, ix1, iy1;
    unsigned char *bitmap;

    /* Smoother unscaled x */
    float x = font->Unscale(textPadding.paddingLeft), left = x;
    int y = textPadding.paddingTop;
    int lineHeight = font->GetLineHeight(1.2f);

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
        if (CodePoint::StrChr(breaks, *c) != nullptr) {
            auto next_break = CodePoint::FindNextBreak(c + 1, breaks);
            if (next_break != nullptr) {
                int advanceWidth;
                int sum = 0;
                for (auto tmp = c; tmp < next_break; tmp++) {
#ifndef NDEBUG
                    font->GetCodepointBitmapBox(tmp->GetValue(), &ix0, &ix1,
                                                &iy0, &iy1);
                    this->DrawRectangle(ix0, iy0, ix1, iy1, COLOR_BB);
#else
                    font->GetCodepointBitmapBox(tmp->GetValue(), &ix0, 0, 0, 0);
#endif
                    font->GetCodepointHMetrics(tmp->GetValue(), &advanceWidth,
                                               0);
                    sum += font->Unscale(ix0) + advanceWidth;
                    sum += font->GetCodepointKernAdvance(tmp->GetValue(),
                                                         (tmp + 1)->GetValue());
                }
                flagNextLine = (font->Scale(x + sum) > maxLineLength);
            }
        }

        // TODO: Auto lf for Chinese chars
        else if (!(c + 2)->IsEmpty() && (c + 2)->GetValue() > 0x2000 &&
                 CodePoint::StrChr(ch_breaks, *(c + 2)) != nullptr) {
            int advanceWidth;
            int sum = 0;
            for (auto tmp = c; tmp < c + 3; tmp++) {
                font->GetCodepointBitmapBox(tmp->GetValue(), &ix0, 0, 0, 0);
                font->GetCodepointHMetrics(tmp->GetValue(), &advanceWidth, 0);
                sum += font->Unscale(ix0) + advanceWidth;
                sum += font->GetCodepointKernAdvance(tmp->GetValue(),
                                                     (tmp + 1)->GetValue());
            }
            flagNextLine = (font->Scale(x + sum) > maxLineLength);
        }

        font->GetCodepointBitmapBox(c->GetValue(), &ix0, &iy0, 0, 0);
        x += font->Unscale(ix0);

        font->GetCodepointHMetrics(c->GetValue(), &ts->advancedWith,
                                   &ts->leftSideBearing);
        font->GetCodepointBitmap(c->GetValue(), &ts->width, &ts->height, 0, 0);

        if (font->Scale(x + ts->leftSideBearing + font->Unscale(ts->width)) >=
            maxLineLength) {
            NextLine(x, y, left - ts->leftSideBearing, lineHeight);
        }

        ts->x = x;
        ts->y = y;
        ts->iy0 = iy0;

        /* 调整x */
        x += ts->advancedWith;

        if (!next_c->IsEmpty()) {
            /* 调整字距 */
            ts->kern = font->GetCodepointKernAdvance(c->GetValue(),
                                                     next_c->GetValue());
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

    Graphic::TextTypeSetting::AdjustAlign(typeSetting, charNum, textAlign,
                                          maxLineLength, font);

    auto ts = typeSetting;
    int ascent = font->GetScaledAscent();
    for (auto c = codepoints + 1; c - codepoints < charNum; c++, ts++) {
        // FIXME not sure if this causes mem leak.
        bitmap = font->GetCodepointBitmap(c->GetValue(), 0, 0, 0, 0);
        ts->ascent = ascent;
        DrawGlyph(ts, font, bitmap);
    }

    font->FreeBitmap(bitmap);
}

TextLayer &TextLayer::SetFont(Font *font) {
    this->font = font;
    return *this;
}
