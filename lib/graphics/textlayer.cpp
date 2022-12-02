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

constexpr uint16_t CHAR_LF = 0x000A;
constexpr uint16_t CHAR_SPACE = 0x0020;

static const CodePoint breaks[] = {
    CHAR_SPACE, // space
    CHAR_LF,    // LF
    0x002c,     // '.'
    0x002e,     // ','
    0x0000,     // EOS
};

// 中文标点
static const CodePoint ch_breaks[] = {
    0x201c, // '“'
    0x3002, // '。'
    0x300a, // '《'
    0xff0c, // '，'
    0x0000, // EOS
};

TextLayer::TextLayer(const Layer &layer) : Layer(layer) {}

TextLayer::TextLayer(const TextLayer &layer) : Layer(layer) {}

TextLayer::TextLayer(uint32_t width, uint32_t height, int32_t rotate)
    : Layer(width, height, rotate) {}

void TextLayer::drawGlyph(const Graphic::GlyphInfo *glyph, Font *font,
                          const unsigned char *bitmap) {
    int32_t x, y;

    x = font->Scale(glyph->cp, glyph->x + glyph->leftSideBearing);
    y = font->Scale(glyph->cp, glyph->y) + glyph->iy0 + glyph->ascent;

#ifndef NDEBUG
    debug("%d %d %d %d\n", x, y, x + glyph->width, y + glyph->height);
    this->Draw(Shape::Rectangle(x, y, x + glyph->width, y + glyph->height),
               invertColor ? Graphic::Color::WB : Graphic::Color::BW);
#endif

    LoopMatrix(glyph->width, glyph->height, x, y) {
        uint8_t iColor = GetMatrix(bitmap, glyph->width, i - x, j - y) >> 6;
        auto color = Graphic::CastColor(iColor);

        if (color != Graphic::Color::WW) {
            Draw(Shape::Point(i, j), color);
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

TextLayer &TextLayer::SetLineHeight(float lineHeight) {
    this->lineHeightScale = lineHeight;
    return *this;
}

TextLayer &TextLayer::SetFont(Font *font) {
    this->font = font;
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

float TextLayer::calcGlyphOffset(const CodePoint *start, int num) {
    assert(num > 0);

    int ix0, ix1, advanceWidth;
    float sum = 0;
    const CodePoint *cp;
    for (cp = start; cp - start < num; cp++) {
        font->GetCodepointBitmapBox(cp, &ix0, 0, &ix1, 0);
        font->GetCodepointHMetrics(cp, &advanceWidth, 0);

        sum += advanceWidth;
        sum += font->GetCodepointKernAdvance(cp, cp + 1);
    }
    // For last glyph, use width instead of advance width.
    sum = sum - advanceWidth + font->Unscale(cp, ix1 - ix0);

    return sum;
}

inline bool isBreak(const CodePoint &cp) {
    return CodePoint::StrChr(breaks, cp) != nullptr;
}

inline bool isChBreak(const CodePoint &cp) {
    return !cp.IsEmpty() && cp > 0x2000 &&
           CodePoint::StrChr(ch_breaks, cp) != nullptr;
}

bool TextLayer::lineFeed(const CodePoint *cp, int32_t x) {
    int32_t maxLineLength = GetRelativeWidth() - textPadding.paddingRight;

    /* Auto linefeed for latin chars */
    if (isBreak(*cp)) {
        auto nextBreak = CodePoint::FindNextBreak(cp + 1, breaks);
        if (nextBreak == nullptr) {
            return false;
        }
        auto assumedLineLength =
            font->Scale(cp, x + calcGlyphOffset(cp, nextBreak - cp));
        return assumedLineLength > maxLineLength;
    }
    /* Auto linefeed for Chinese chars */
    if (isChBreak(*(cp + 2))) {
        auto assumedLineLength =
            font->Scale(cp, x + calcGlyphOffset(cp, 3));
        return assumedLineLength > maxLineLength;
    }

    return false;
}

void TextLayer::getGlyphInfo(Graphic::GlyphInfo *glyph, const CodePoint *cp,
                             float x, float y) {
    int ix0, iy0, ix1, iy1;

    font->GetCodepointBitmapBox(cp, &ix0, &iy0, &ix1, &iy1);
    font->GetCodepointHMetrics(cp, &glyph->advanced, &glyph->leftSideBearing);

    glyph->x = x;
    glyph->y = y;
    glyph->width = ix1 - ix0;
    glyph->height = iy1 - iy0;
    glyph->iy0 = iy0;
    glyph->cp = cp;

    auto ncp = cp + 1;
    if (!ncp->IsEmpty()) {
        glyph->kern = font->GetCodepointKernAdvance(cp, ncp);
    }
}

#define NextLine(x, y, xStart, yOffset, stopCondition)                         \
    {                                                                          \
        x = xStart;                                                            \
        y += yOffset;                                                          \
        if (stopCondition) {                                                   \
            charNum = cp - cps;                                                \
            break;                                                             \
        }                                                                      \
    }

TextLayer &TextLayer::TypeSetting(Text::Direction direction) {
    // Debug checks
    assert_is_initialized(font);
    assert_is_initialized(codepoints);

    float assumedLineLength;

    // initialize
    glyphInfo.reset(new Graphic::GlyphInfo[charNum]);

    // head of arrays
    auto cps = codepoints.get() + 1;
    auto glyph = glyphInfo.get();

    /* Smoother unscaled x */
    float x = font->Unscale(cps, textPadding.paddingLeft),
          y = font->Unscale(cps, textPadding.paddingTop);
    float left = x,
          lineHeight =
              font->Unscale(cps, font->GetLineHeight(cps, lineHeightScale));

    int maxLineLength = GetRelativeWidth() - textPadding.paddingRight;
    int maxHeight = GetRelativeHeight() - textPadding.paddingBottom;

    /* Codepoints always starts with 0xFEFF */
    for (auto cp = cps; cp - cps < charNum - 1; cp++) {
        /* Handle LF */
        if (*cp < CHAR_SPACE) {
            if (*cp == CHAR_LF) { // LF, of course
                goto next_line;
            } else { // Invalid char?
                continue;
            }
        }

        getGlyphInfo(glyph, cp, x, y);

        assumedLineLength =
            glyph->x + glyph->leftSideBearing + font->Unscale(cp, glyph->width);
        if (font->Scale(cp, assumedLineLength) >= maxLineLength) {
            glyph->x = left;
            glyph->y += lineHeight;
            NextLine(x, y, left, lineHeight,
                     font->Scale(cp, y + lineHeight) > maxHeight);
        }

        /* move to start of the next codepoint */
        x += glyph->advanced + font->Unscale(cp, glyph->kern);

        glyph++;

        if (lineFeed(cp, x)) {
        next_line:
            NextLine(x, y, left, lineHeight,
                     font->Scale(cp, y + lineHeight) > maxHeight);
            if (*cp == CHAR_SPACE) {
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

    int lineWidth = GetRelativeWidth() - textPadding.paddingRight;
    Graphic::GlyphInfo::AdjustAlign(glyphInfo.get(), charNum, textAlign,
                                    lineWidth, font);

    std::unique_ptr<unsigned char[]> bitmap;

    for (auto glyph = glyphInfo.get(); glyph->cp != nullptr; glyph++) {
        auto cp = glyph->cp;
        bitmap.reset(font->GetCodepointBitmap(cp, 0, 0, 0, 0));
        glyph->ascent = font->GetScaledAscent(cp);
        drawGlyph(glyph, font, bitmap.get());
    }
}
