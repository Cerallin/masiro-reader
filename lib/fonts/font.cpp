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

void Font::GetCodepointBitmapBox(const CodePoint *codepoint, int *ix0, int *iy0,
                                 int *ix1, int *iy1) {
    auto fontInfo = getFontFace(codepoint)->GetFontInfo();
    stbtt_GetCodepointBitmapBox(fontInfo, codepoint->GetValue(), 0,
                                getFontScale(codepoint), ix0, iy0, ix1, iy1);
    stbtt_FindGlyphIndex(fontInfo, codepoint->GetValue());
}

unsigned char *Font::GetCodepointBitmap(const CodePoint *codepoint, int *width,
                                        int *height, int *xoff, int *yoff) {
    return stbtt_GetCodepointBitmap(
        getFontFace(codepoint)->GetFontInfo(), 0, getFontScale(codepoint),
        codepoint->GetValue(), width, height, xoff, yoff);
}

int Font::GetCodepointKernAdvance(const CodePoint *cp1, const CodePoint *cp2) {
    // FIXME what if cp1 and cp2 are found in different font faces?
    auto ff1 = getFontFace(cp1);
    return stbtt_GetCodepointKernAdvance(ff1->GetFontInfo(), cp1->GetValue(),
                                         cp2->GetValue());
}

void Font::FreeBitmap(const CodePoint *codepoint, unsigned char *bitmap) {
    stbtt_FreeBitmap(bitmap, getFontFace(codepoint)->GetFontInfo()->userdata);
}

int Font::GetScaledAscent(const CodePoint *codepoint) {
    auto metrics = getMetrics(codepoint);
    return Scale(codepoint, metrics->ascent);
}

int Font::GetLineHeight(const CodePoint *codepoint, float scale) {
    auto metrics = getMetrics(codepoint);
    return roundeven(scale * metrics->GetLineHeight());
}

float Font::Scale(const CodePoint *codepoint, float num) {
    return roundevenf(num * getFontScale(codepoint));
}

float Font::Unscale(const CodePoint *codepoint, float num) {
    return roundevenf(num / getFontScale(codepoint));
}

inline float Font::getFontScale(const CodePoint *codepoint) {
    auto metrics = getMetrics(codepoint);
    return metrics->scale;
}

inline const FontFace *Font::getFontFace(const CodePoint *codepoint) {
    return fontFamily->GetFontFace(codepoint);
}

inline const FontMetrics *Font::getMetrics(const CodePoint *codepoint) {
    auto index = fontFamily->GetFontFaceIndex(codepoint);
    return &metricsList[index];
}
