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

#include <math.h>
#include <new>
#include <stdio.h>
#include <string.h>

Font::Font(FontFace *fontFace, float fontSize)
    : fontFace(fontFace), fontSize(fontSize) {
    auto fontInfo = fontFace->GetFontInfo();

    fontScale = stbtt_ScaleForPixelHeight(fontFace->GetFontInfo(), fontSize);
    /**
     * 获取垂直方向上的度量
     * ascent：字体从基线到顶部的高度；
     * descent：基线到底部的高度，通常为负值；
     * lineGap：两个字体之间的间距；
     */
    stbtt_GetFontVMetrics(fontInfo, &ascent, &descent, &lineGap);
};

Font::~Font(){};

void Font::GetCodepointHMetrics(int codepoint, int *advanceWidth,
                                int *leftSideBearing) {
    stbtt_GetCodepointHMetrics(fontFace->GetFontInfo(), codepoint, advanceWidth,
                               leftSideBearing);
}

void Font::GetCodepointBitmapBox(int codepoint, int *ix0, int *iy0, int *ix1,
                                 int *iy1) {
    stbtt_GetCodepointBitmapBox(fontFace->GetFontInfo(), codepoint, 0,
                                fontScale, ix0, iy0, ix1, iy1);
}

unsigned char *Font::GetCodepointBitmap(int codepoint, int *width, int *height,
                                        int *xoff, int *yoff) {
    return stbtt_GetCodepointBitmap(fontFace->GetFontInfo(), 0, fontScale,
                                    codepoint, width, height, xoff, yoff);
}

int Font::GetCodepointKernAdvance(int cp1, int cp2) {
    return stbtt_GetCodepointKernAdvance(fontFace->GetFontInfo(), cp1, cp2);
}

void Font::FreeBitmap(unsigned char *bitmap) {
    stbtt_FreeBitmap(bitmap, fontFace->GetFontInfo()->userdata);
}

FontFace *Font::GetFontFace() { return fontFace; }

int Font::GetScaledAscent() { return Scale(ascent); }

int Font::GetLineHeight(float scale) {
    return roundeven(scale * fontScale * (ascent - descent + lineGap));
}

float Font::Scale(float num) { return roundevenf(num * fontScale); }

float Font::Unscale(float num) { return roundevenf(num / fontScale); }
