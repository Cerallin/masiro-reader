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

#include "fontface.h"
#include <stdint.h>

typedef uint16_t CodePoint;

class Font {
  public:
    Font(FontFace *fontFace, float fontSize = 32.0f);
    ~Font();

    void GetCodepointHMetrics(int codepoint, int *advanceWidth,
                              int *leftSideBearing);
    void GetCodepointBitmapBox(int codepoint, int *ix0, int *iy0, int *ix1,
                               int *iy1);
    unsigned char *GetCodepointBitmap(int codepoint, int *width, int *height,
                                      int *xoff, int *yoff);
    int GetCodepointKernAdvance(int cp1, int cp2);

    void FreeBitmap(unsigned char *bitmap);

    FontFace *GetFontFace();

    int GetScaledAscent();
    int GetLineHeight(float scale = 1.f);

    float Scale(float num);
    float Unscale(float num);

  private:
    FontFace *fontFace;
    float fontSize;
    float fontScale;

    int ascent;
    int descent;
    int lineGap;
};

#endif /* FONT_FONT_H */