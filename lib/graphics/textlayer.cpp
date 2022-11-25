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

#include "debug.h"

#include "textlayer.h"

#include <cstdio>
#include <cstring>
#include <new>

#include "traits/layersetters.cpp"

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
            charNum = c - cps;                                                 \
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

void TextLayer::drawGlyph(const CodePoint *cp,
                          const Graphic::TextTypeSetting *ts, Font *font,
                          const unsigned char *bitmap) {
    int32_t x, y;

    x = font->Scale(cp, ts->x + ts->leftSideBearing);
    y = ts->y + ts->iy0 + ts->ascent;

#ifndef NDEBUG
    debug("%d %d %d %d\n", x, y, x + ts->width, y + ts->height);
    this->DrawRectangle(x, y, x + ts->width, y + ts->height,
                        invertColor ? Graphic::Color::WB : Graphic::Color::BW);
#endif

    LoopMatrix(ts->width, ts->height, x, y) {
        int color = GetMatrix(bitmap, ts->width, i - x, j - y) >> 6;
        // FIXME This is some kind of "alpha" tunnel
        if (this->invertColor) {
            if (color != Graphic::Color::WW)
                DrawPixel(i, j, ~color);
        } else {
            if (color != Graphic::Color::BB)
                DrawPixel(i, j, color);
        }
    };
}

int TextLayer::SetText(char *str) {
    size_t srcLen = strlen(str), destLen = (srcLen + 4) * 2;
    codepoints = std::make_unique<CodePoint[]>(destLen / 2);
    if (codepoints == nullptr) {
        return -1;
    }

    auto cps = codepoints.get();
    charNum = CodePoint::StrToUnicode(str, srcLen, &cps, destLen);
    if (charNum == -1) {
        codepoints.~unique_ptr();
        return -1;
    }

    typeSetting = std::make_unique<Graphic::TextTypeSetting[]>(charNum);
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

void TextLayer::calcCodePointSize(const CodePoint *codepoint, int *ix0,
                                  int *iy0, int *ix1, int *iy1,
                                  int *advanceWidth) {
    font->GetCodepointBitmapBox(codepoint, ix0, ix1, iy0, iy1);
    font->GetCodepointHMetrics(codepoint, advanceWidth, 0);
}

void TextLayer::calcCodePointTypeSetting(const CodePoint *codepoint,
                                         Graphic::TextTypeSetting *ts, int x,
                                         int y, int iy0) {
    font->GetCodepointHMetrics(codepoint, &ts->advancedWith,
                               &ts->leftSideBearing);
    font->GetCodepointBitmap(codepoint, &ts->width, &ts->height, 0, 0);

    ts->x = x;
    ts->y = y;
    ts->iy0 = iy0;
}

int TextLayer::calcLineWidth(const CodePoint *start,
                             const CodePoint *nextBreak) {
    int ix0, advanceWidth;

    int sum = 0;
    for (auto tmp = start; tmp < nextBreak; tmp++) {
        this->calcCodePointSize(tmp, &ix0, 0, 0, 0, &advanceWidth);
        sum += font->Unscale(tmp, ix0) + advanceWidth;
        sum += font->GetCodepointKernAdvance(tmp, (tmp + 1));
    }

    return sum;
}

TextLayer &TextLayer::CalcTypeSetting() {
    // Debug checks
    assert_is_initialized(font);
    assert_is_initialized(codepoints);
    assert_is_initialized(typeSetting);

    // Next line flag
    int flagNextLine = 0;

    auto cps = codepoints.get();

    /* Smoother unscaled x */
    float x = font->Unscale(cps, textPadding.paddingLeft), left = x;
    int y = textPadding.paddingTop;
    // TODO: customize lineHeight
    // FIXME: need a robust method to calculate line height
    int lineHeight = font->GetLineHeight(cps + 1, 1.0f);
    int maxLineLength = GetRelativeWidth() - textPadding.paddingRight;
    int maxHeight = GetRelativeHeight() - textPadding.paddingBottom;

    /* Codepoints always starts with 0xFEFF */
    for (auto c = cps + 1; c - cps < charNum; c++) {
        auto nextCodePoint = c + 1;
        auto ts = &this->typeSetting[c - cps - 1];

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
            auto nextBreak = CodePoint::FindNextBreak(c + 1, breaks);
            if (nextBreak != nullptr) {
                flagNextLine =
                    (font->Scale(c, x + calcLineWidth(c, nextBreak)) >
                     maxLineLength);
            }
        }

        // TODO: Auto lf for Chinese chars
        else {
            auto ctmp = c + 2;
            if (!ctmp->IsEmpty() && ctmp->GetValue() > 0x2000 &&
                CodePoint::StrChr(ch_breaks, *ctmp) != nullptr) {
                int sum = calcLineWidth(c, c + 3);
                flagNextLine = (font->Scale(ctmp, x + sum) > maxLineLength);
            }
        }

        int ix0, iy0;
        font->GetCodepointBitmapBox(c, &ix0, &iy0, 0, 0);
        x += font->Unscale(c, ix0);
        calcCodePointTypeSetting(c, ts, x, y, iy0);

        if (font->Scale(c, x + ts->leftSideBearing +
                               font->Unscale(c, ts->width)) >= maxLineLength) {
            NextLine(x, y, left - ts->leftSideBearing, lineHeight);
        }

        /* 调整x */
        x += ts->advancedWith;

        if (!nextCodePoint->IsEmpty()) {
            /* 调整字距 */
            ts->kern = font->GetCodepointKernAdvance(c, nextCodePoint);
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

    return *this;
}

void TextLayer::Render() {
    // Debug checks
    assert_is_initialized(font);
    assert_is_initialized(codepoints);
    assert_is_initialized(typeSetting);

    unsigned char *bitmap;

    // TODO: maxLineLength as a property.
    int maxLineLength = GetRelativeWidth() - textPadding.paddingRight;

    Graphic::TextTypeSetting::AdjustAlign(codepoints.get(), typeSetting.get(),
                                          charNum, textAlign, maxLineLength,
                                          font);

    auto ts = typeSetting.get();
    auto cps = codepoints.get();
    // TODO: 让ts存储codepoint
    for (auto c = cps + 1; c - cps < charNum; c++, ts++) {
        if (*c < CHAR_SPACE)
            continue;

        int ascent = font->GetScaledAscent(c);
        // FIXME: not sure if this causes mem leak.
        bitmap = font->GetCodepointBitmap(c, 0, 0, 0, 0);
        ts->ascent = ascent;
        drawGlyph(c, ts, font, bitmap);
    }

    // font->FreeBitmap(bitmap);
}

TextLayer &TextLayer::SetFont(Font *font) {
    this->font = font;
    return *this;
}
