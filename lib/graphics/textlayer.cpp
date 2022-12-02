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
    CHAR_LF,    // LF
    CHAR_SPACE, // ' '
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

TextLayer &TextLayer::SetWritingMode(Text::WritingMode mode) {
    this->mode = mode;
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

void TextLayer::drawGlyph(const Graphic::GlyphInfo *glyph, Font *font,
                          const unsigned char *bitmap) {
    int32_t x, y;
    if (Text::Horizontal(mode)) {
        x = font->Scale(glyph->cp, glyph->x + glyph->sideBearing);
        y = font->Scale(glyph->cp, glyph->y) + glyph->iy0 + glyph->ascent;
    } else if (Text::Vertical(mode)) {
        x = font->Scale(glyph->cp, glyph->y) + glyph->ix0;
        y = font->Scale(glyph->cp, glyph->x + glyph->sideBearing);
    }

#ifndef NDEBUG
    if (glyph->cp->GetValue() < 0xff) {
        debug("%c: ", (char)glyph->cp->GetValue());
    }
    debug("%3d x %3d\t+ %3d x %3d\n", x, y, x + glyph->width,
          y + glyph->height);

    if (glyph->width && glyph->height) {
        this->Draw(Shape::Rectangle(x, y, x + glyph->width, y + glyph->height),
                   Graphic::Color::WB);

        if (Text::Horizontal(mode)) {
            this->Draw(Shape::Rectangle(
                           font->Scale(glyph->cp, glyph->x),
                           font->Scale(glyph->cp, glyph->y),
                           font->Scale(glyph->cp, glyph->x + glyph->advanced),
                           y + glyph->height),
                       Graphic::Color::BW);
        } else if (Text::Vertical(mode)) {
            this->Draw(Shape::Rectangle(
                           font->Scale(glyph->cp, glyph->y),
                           font->Scale(glyph->cp, glyph->x),
                           font->Scale(glyph->cp, glyph->y) + glyph->width,
                           font->Scale(glyph->cp, glyph->x + glyph->advanced)),
                       Graphic::Color::BW);
        }
    }
#endif

    LoopMatrix(glyph->width, glyph->height, x, y) {
        uint8_t iColor = GetMatrix(bitmap, glyph->width, i - x, j - y) >> 6;
        auto color = Graphic::CastColor(iColor);

        if (color != Graphic::Color::WW) {
            Draw(Shape::Point(i, j), color);
        }
    };
}

float TextLayer::calcGlyphOffset(const CodePoint *start, int num) {
    assert(num > 0);

    int ix0, iy0, ix1, iy1, advance, sideBearing;
    float sum = 0;
    const CodePoint *cp;
    for (cp = start + 1; cp - start < num; cp++) {
        font->GetCodepointBitmapBox(cp, &ix0, &iy0, &ix1, &iy1);
        if (Text::Horizontal(mode)) {
            font->GetCodepointHMetrics(cp, &advance, &sideBearing);
        } else if (Text::Vertical(mode)) {
            font->GetCodepointVMetrics(cp, &advance, &sideBearing);
        }

        sum += advance;
        sum += font->GetCodepointKernAdvance(cp, cp + 1);
    }
    // For last glyph, use width instead of advance width.
    sum = sum - advance + sideBearing;
    if (Text::Horizontal(mode)) {
        sum += font->Unscale(cp, ix1 - ix0);
    } else if (Text::Vertical(mode)) {
        sum += font->Unscale(cp, iy1 - iy0);
    }

    return sum;
}

inline bool isBreak(const CodePoint &cp) {
    return CodePoint::FindNextChar(breaks, cp) != nullptr;
}

inline bool isChBreak(const CodePoint &cp) {
    return !cp.IsEmpty() && cp > 0x2000 &&
           CodePoint::FindNextChar(ch_breaks, cp) != nullptr;
}

bool TextLayer::lineFeed(const CodePoint *cp, int32_t pos, int32_t maxLine) {
    /* Auto linefeed for latin chars */
    if (isBreak(*cp)) {
        auto nextBreak = CodePoint::FindNextBreak(cp + 1, breaks);
        if (nextBreak == nullptr) {
            return false;
        }
        auto assumedLineLength =
            font->Scale(cp, pos + calcGlyphOffset(cp, nextBreak - cp));
        return assumedLineLength > maxLine;
    }
    /* Auto linefeed for Chinese chars */
    if (isChBreak(*(cp + 2))) {
        auto assumedLineLength = font->Scale(cp, pos + calcGlyphOffset(cp, 3));
        return assumedLineLength > maxLine;
    }

    return false;
}

void TextLayer::getGlyphInfo(Graphic::GlyphInfo *glyph, const CodePoint *cp,
                             float textDirection, float lineDirection) {
    int ix0, iy0, ix1, iy1;
    font->GetCodepointBitmapBox(cp, &ix0, &iy0, &ix1, &iy1);

    if (Text::Horizontal(mode)) {
        font->GetCodepointHMetrics(cp, &glyph->advanced, &glyph->sideBearing);
        glyph->ascent = font->GetScaledAscent(cp);
    } else if (Text::Vertical(mode)) {
        font->GetCodepointVMetrics(cp, &glyph->advanced, &glyph->sideBearing);
        glyph->ascent = font->GetScaledVertAscent(cp);
    }

    glyph->x = textDirection;
    glyph->y = lineDirection;
    glyph->width = ix1 - ix0;
    glyph->height = iy1 - iy0;
    glyph->ix0 = ix0;
    glyph->iy0 = iy0;
    glyph->cp = cp;

    auto ncp = cp + 1;
    if (!ncp->IsEmpty()) {
        glyph->kern = font->GetCodepointKernAdvance(cp, ncp);
    }
}

bool overflow(const Graphic::GlyphInfo *glyph, const Font *font, int max) {
    float assumedLineLength;
    assumedLineLength =
        glyph->x + glyph->sideBearing + font->Unscale(glyph->cp, glyph->width);
    return font->Scale(glyph->cp, assumedLineLength) >= max;
}

#define NextLine(x, y, start, lineHeight, stopCondition)                       \
    {                                                                          \
        x = start;                                                             \
        y += lineHeight;                                                       \
        if (stopCondition) {                                                   \
            charNum = cp - cps;                                                \
            break;                                                             \
        }                                                                      \
    }

TextLayer &TextLayer::TypeSetting() {
    // Debug checks
    assert_is_initialized(font);
    assert_is_initialized(codepoints);

    // initialize
    glyphInfo.reset(new Graphic::GlyphInfo[charNum]);

    // head of arrays
    auto cps = codepoints.get() + 1;
    auto glyph = glyphInfo.get();

    float textStart, lineHeight;

    /* Smoother unscaled */
    float textDirection, lineDirection;
    // max length of lines
    int maxLine;
    // max length of each line
    int maxText;

    if (Text::Horizontal(mode)) {
        textStart = font->Unscale(cps, textPadding.paddingLeft);
        lineHeight =
            font->Unscale(cps, font->GetLineHeight(cps, lineHeightScale));

        textDirection = textStart;
        lineDirection = font->Unscale(cps, textPadding.paddingTop);

        maxLine = GetRelativeHeight() - textPadding.paddingBottom;
        maxText = GetRelativeWidth() - textPadding.paddingRight;
    } else if (Text::Vertical(mode)) {
        textStart = font->Unscale(cps, textPadding.paddingTop);
        lineHeight =
            font->Unscale(cps, font->GetLineWidth(cps, lineHeightScale));

        textDirection = textStart;
        lineDirection = font->Unscale(cps, textPadding.paddingLeft);

        if (mode == Text::Vertical_RL) {
            lineDirection -= lineHeight;
            lineHeight = -lineHeight;
        }

        maxLine = GetRelativeWidth() - textPadding.paddingRight;
        maxText = GetRelativeHeight() - textPadding.paddingBottom;
    }

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

        getGlyphInfo(glyph, cp, textDirection, lineDirection);

        if (overflow(glyph, font, maxText)) {
            if (*cp == CHAR_SPACE) {
                while (*cp == CHAR_SPACE) {
                    cp++;
                }

                cp--;
                continue;
            }
            glyph->x = textStart;
            glyph->y += lineHeight;
            NextLine(textDirection, lineDirection, textStart, lineHeight,
                     font->Scale(cp, lineDirection + lineHeight) > maxLine);
        }

        /* move to start of the next codepoint */
        textDirection += glyph->advanced + font->Unscale(cp, glyph->kern);

        glyph++;

        if (lineFeed(cp, textStart, maxText)) {
        next_line:
            NextLine(textDirection, lineDirection, textStart, lineHeight,
                     font->Scale(cp, lineDirection + lineHeight) > maxLine);
        }
    }

    return *this;
}

void TextLayer::Render() {
    // Debug checks
    assert_is_initialized(font);
    assert_is_initialized(glyphInfo);

    int lineWidth;

    if (Text::Horizontal(mode)) {
        lineWidth = GetRelativeWidth() - textPadding.paddingRight;
    } else if (Text::Vertical(mode)) {
        lineWidth = GetRelativeHeight() - textPadding.paddingBottom;
    }
    Graphic::GlyphInfo::AdjustAlign(glyphInfo.get(), charNum, textAlign,
                                    lineWidth, font);

    std::unique_ptr<unsigned char[]> bitmap;

    for (auto glyph = glyphInfo.get(); glyph->cp != nullptr; glyph++) {
        auto cp = glyph->cp;
        bitmap.reset(font->GetCodepointBitmap(cp, 0, 0, 0, 0));
        drawGlyph(glyph, font, bitmap.get());
    }
}
