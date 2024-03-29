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

#ifndef FONT_FONT_H
#define FONT_FONT_H

#include "codepoint.h"
#include "fontfamily.h"
#include "fontmetrics.h"

#include <cstdint>
#include <memory>

class Font {
  public:
    Font(FontFace *fontFace, float = 32.0f);
    Font(FontFamily *fontFamily, float fontSize = 32.0f);
    ~Font() = default;

    void GetCodepointHMetrics(const CodePoint *codepoint, int *advanceWidth,
                              int *leftSideBearing);
    void GetCodepointVMetrics(const CodePoint *codepoint, int *advanceHeight,
                              int *topSideBearing);
    void GetCodepointBitmapBox(const CodePoint *codepoint, int *ix0, int *iy0,
                               int *ix1, int *iy1);
    unsigned char *GetCodepointBitmap(const CodePoint *codepoint, int *width,
                                      int *height, int *xoff, int *yoff);
    int GetCodepointKernAdvance(const CodePoint *cp1, const CodePoint *cp2);

    FontFamily *GetFontFamily();

    int GetScaledAscent(const CodePoint *codepoint) const;
    int GetScaledVertAscent(const CodePoint *codepoint) const;
    int GetLineHeight(const CodePoint *codepoint, float scale = 1.f) const;
    int GetLineWidth(const CodePoint *codepoint, float scale = 1.f) const;

    float Scale(const CodePoint *codepoint, float num) const;
    float Unscale(const CodePoint *codepoint, float num) const;

  private:
    std::unique_ptr<FontFamily> fontFamily;
    std::vector<FontMetrics> metricsList;

    float fontSize;

    inline float getFontScale(const CodePoint *codepoint) const;
    inline const FontFace *getFontFace(const CodePoint *codepoint) const;
    inline const FontMetrics *getMetrics(const CodePoint *codepoint) const;
};

#endif /* FONT_FONT_H */
