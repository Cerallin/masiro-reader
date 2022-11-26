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

TextLayer::TextLayer(const Layer &layer) : Layer(layer) {}

TextLayer::TextLayer(const TextLayer &layer) : Layer(layer) {}

TextLayer::TextLayer(uint32_t width, uint32_t height, int32_t rotate)
    : Layer(width, height, rotate) {}

void TextLayer::drawGlyph(const Graphic::GlyphInfo *glyph, Font *font,
                          const unsigned char *bitmap) {
    int32_t x, y;

    x = font->Scale(glyph->cp, glyph->x + glyph->leftSideBearing);
    y = glyph->y + glyph->iy0 + glyph->ascent;

#ifndef NDEBUG
    debug("%d %d %d %d\n", x, y, x + glyph->width, y + glyph->height);
    this->DrawRectangle(x, y, x + glyph->width, y + glyph->height,
                        invertColor ? Graphic::Color::WB : Graphic::Color::BW);
#endif

    LoopMatrix(glyph->width, glyph->height, x, y) {
        int color = GetMatrix(bitmap, glyph->width, i - x, j - y) >> 6;
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

TextLayer &TextLayer::SetText(char *str) {
    size_t srcLen = strlen(str), destLen = (srcLen + 4) * 2;
    codepoints.reset(new CodePoint[destLen / 2]);

    auto cps = codepoints.get();
    try {
        charNum = CodePoint::StrToUnicode(str, srcLen, &cps, destLen);
    } catch (const ConvertFailedException &e) {
        codepoints.release();
        throw;
    }

    return *this;
}

TextLayer &TextLayer::SetTextAlign(Graphic::TextAlign textAlign) {
    this->textAlign = textAlign;
    return *this;
}

TextLayer &TextLayer::SetTextPadding(Graphic::TextPadding textPadding) {
    this->textPadding = textPadding;
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
                                         Graphic::GlyphInfo *glyph, int x,
                                         int y, int iy0) {
    font->GetCodepointHMetrics(codepoint, &glyph->advancedWith,
                               &glyph->leftSideBearing);
    font->GetCodepointBitmap(codepoint, &glyph->width, &glyph->height, 0, 0);

    glyph->x = x;
    glyph->y = y;
    glyph->iy0 = iy0;
    glyph->cp = codepoint;
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

TextLayer &TextLayer::TypeSetting() {
    // Debug checks
    assert_is_initialized(font);
    assert_is_initialized(codepoints);

    glyphInfo.reset(new Graphic::GlyphInfo[charNum]);

    // Next line flag
    int flagNextLine = 0;

    auto cps = codepoints.get();
    auto glyph = glyphInfo.get();

    /* Smoother unscaled x */
    float x = font->Unscale(cps, textPadding.paddingLeft), left = x;
    int y = textPadding.paddingTop;
    // TODO: customize lineHeight
    // FIXME: need a robust method to calculate line height
    int lineHeight = font->GetLineHeight(cps + 1, 1.0f);
    int maxLineLength = GetRelativeWidth() - textPadding.paddingRight;
    int maxHeight = GetRelativeHeight() - textPadding.paddingBottom;

    /* Codepoints always starts with 0xFEFF */
    for (auto c = cps + 1; c - cps < charNum; c++, glyph++) {
        auto nextCodePoint = c + 1;

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
        calcCodePointTypeSetting(c, glyph, x, y, iy0);

        if (font->Scale(c, x + glyph->leftSideBearing +
                               font->Unscale(c, glyph->width)) >=
            maxLineLength) {
            NextLine(x, y, left - glyph->leftSideBearing, lineHeight);
        }

        /* 调整x */
        x += glyph->advancedWith;

        if (!nextCodePoint->IsEmpty()) {
            /* 调整字距 */
            glyph->kern = font->GetCodepointKernAdvance(c, nextCodePoint);
            x += glyph->kern;
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
    assert_is_initialized(glyphInfo);

    unsigned char *bitmap;

    // TODO: maxLineLength as a property.
    int maxLineLength = GetRelativeWidth() - textPadding.paddingRight;

    Graphic::GlyphInfo::AdjustAlign(codepoints.get(), glyphInfo.get(), charNum,
                                    textAlign, maxLineLength, font);

    for (auto glyph = glyphInfo.get(); glyph->cp != nullptr; glyph++) {
        auto cp = glyph->cp;
        glyph->ascent = font->GetScaledAscent(cp);
        bitmap = font->GetCodepointBitmap(cp, 0, 0, 0, 0);
        drawGlyph(glyph, font, bitmap);
        // TODO: prealloc to optimize
        font->FreeBitmap(cp, bitmap);
    }
}

TextLayer &TextLayer::SetFont(Font *font) {
    this->font = font;
    return *this;
}
