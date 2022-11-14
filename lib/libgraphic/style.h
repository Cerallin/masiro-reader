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

#ifndef GRAPHIC_STYLE_H
#define GRAPHIC_STYLE_H

#include "font.h"
#include <stddef.h>

namespace Graphic {

typedef enum {
    AlignLeft,
    AlignRight,
    AlignCenter,
    AlignJustify, // TODO: not implemented
} TextAlign;

class TextPadding {
  public:
    int paddingLeft;
    int paddingTop;
    int paddingRight;
    int paddingBottom;
};

class TextTypeSetting {
  public:
    static void AdjustAlign(const CodePoint *codepoints, TextTypeSetting *typeSettings,
                            ssize_t len, TextAlign align, int lineWidth,
                            Font *font);

    float x;             // unscaled absolute x, to be justified
    int y;               // absolute y, determing chars in the same line
    int iy0;             // y offset
    int width;           // scaled bitmap width
    int height;          // scaled bitmap height
    int advancedWith;    // unscaled advance width
    int leftSideBearing; // unscaled left side bearing
    int kern;            // scaled kerning
    int ascent;          // ascent
};

} // namespace Graphic

#endif /* GRAPHIC_STYLE_H */
