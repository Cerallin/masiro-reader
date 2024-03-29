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

#define STB_TRUETYPE_IMPLEMENTATION
#include "font.h"

#include <cmath>
#include <cstdio>
#include <cstring>

Font::Font(FontFamily *fontFamily, float fontSize)
    : fontFamily(fontFamily), fontSize(fontSize) {
    const auto ffList = fontFamily->GetFontFace();

    metricsList.reserve(ffList.size());
    for (const auto ff : ffList) {
        auto fontInfo = ff->GetFontInfo();
        auto scale = stbtt_ScaleForPixelHeight(fontInfo, fontSize);

        FontMetrics metrics(scale);
        metrics.FontVMetrics(fontInfo);
        metrics.FontHMetrics(fontInfo);

        metricsList.push_back(metrics);
    }
};

Font::Font(FontFace *fontFace, float fontSize)
    : Font(new FontFamily(fontFace), fontSize) {}

void Font::GetCodepointHMetrics(const CodePoint *codepoint, int *advanceWidth,
                                int *leftSideBearing) {
    stbtt_GetCodepointHMetrics(getFontFace(codepoint)->GetFontInfo(),
                               codepoint->GetValue(), advanceWidth,
                               leftSideBearing);
}

void Font::GetCodepointVMetrics(const CodePoint *codepoint, int *advanceHeight,
                                int *topSideBearing) {
    stbtt_GetCodepointVMetrics(getFontFace(codepoint)->GetFontInfo(),
                               codepoint->GetValue(), advanceHeight,
                               topSideBearing);
}

void Font::GetCodepointBitmapBox(const CodePoint *codepoint, int *ix0, int *iy0,
                                 int *ix1, int *iy1) {
    auto fontInfo = getFontFace(codepoint)->GetFontInfo();
    stbtt_GetCodepointBitmapBox(fontInfo, codepoint->GetValue(),
                                getFontScale(codepoint),
                                getFontScale(codepoint), ix0, iy0, ix1, iy1);
    stbtt_FindGlyphIndex(fontInfo, codepoint->GetValue());
}

unsigned char *Font::GetCodepointBitmap(const CodePoint *codepoint, int *width,
                                        int *height, int *xoff, int *yoff) {
    return stbtt_GetCodepointBitmap(
        getFontFace(codepoint)->GetFontInfo(), getFontScale(codepoint),
        getFontScale(codepoint), codepoint->GetValue(), width, height, xoff,
        yoff);
}

int Font::GetCodepointKernAdvance(const CodePoint *cp1, const CodePoint *cp2) {
    // FIXME what if cp1 and cp2 are found in different font faces?
    auto ff1 = getFontFace(cp1);
    return stbtt_GetCodepointKernAdvance(ff1->GetFontInfo(), cp1->GetValue(),
                                         cp2->GetValue());
}

int Font::GetScaledAscent(const CodePoint *codepoint) const {
    auto metrics = getMetrics(codepoint);
    return Scale(codepoint, metrics->ascent);
}

int Font::GetScaledVertAscent(const CodePoint *codepoint) const {
    auto metrics = getMetrics(codepoint);
    return Scale(codepoint, metrics->vertTypoAscender);
}

int Font::GetLineHeight(const CodePoint *codepoint, float scale) const {
    auto metrics = getMetrics(codepoint);
    return roundeven(scale * metrics->GetLineHeight());
}

int Font::GetLineWidth(const CodePoint *codepoint, float scale) const {
    auto metrics = getMetrics(codepoint);
    return roundeven(scale * metrics->GetLineWidth());
}

float Font::Scale(const CodePoint *codepoint, float num) const {
    return roundevenf(num * getFontScale(codepoint));
}

float Font::Unscale(const CodePoint *codepoint, float num) const {
    return roundevenf(num / getFontScale(codepoint));
}

inline float Font::getFontScale(const CodePoint *codepoint) const {
    auto metrics = getMetrics(codepoint);
    return metrics->scale;
}

inline const FontFace *Font::getFontFace(const CodePoint *codepoint) const {
    return fontFamily->GetFontFace(codepoint);
}

inline const FontMetrics *Font::getMetrics(const CodePoint *codepoint) const {
    auto index = fontFamily->GetFontFaceIndex(codepoint);
    return &metricsList[index];
}
