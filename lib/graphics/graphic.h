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

#ifndef GRAPHIC_GRAPHIC_H
#define GRAPHIC_GRAPHIC_H

#include "fonts/font.h"

#include <cstddef>
#include <cstdint>

namespace Graphic {

enum Rotate {
    ROTATE_0 = 1,
    ROTATE_90 = 2,
    ROTATE_180 = 4,
    ROTATE_270 = 8,
};

enum Color {
    WW = 0,
    WB = 1,
    BW = 2,
    BB = 3,
};

Color CastColor(uint8_t color);
Color InvertColor(Color color);
Color InvertColor(int color);

typedef enum {
    AlignStart,
    AlignEnd,
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

class GlyphInfo {
  public:
    static void AdjustAlign(GlyphInfo *glyphInfos, ssize_t len, TextAlign align,
                            int lineWidth, Font *font);

    float x = 0;         // unscaled absolute x
    float y = 0;         // unscaled absolute y
    int ix0 = 0;         // x offset
    int iy0 = 0;         // y offset
    int width = 0;       // scaled bitmap width
    int height = 0;      // scaled bitmap height
    int advanced = 0;    // unscaled advance width
    int sideBearing = 0; // unscaled left side bearing
    int kern = 0;        // scaled kerning
    int ascent = 0;      // ascent

    bool rotate = false;

    const CodePoint *cp = nullptr;
};

} // namespace Graphic

namespace Text {

enum WritingMode {
    // Horizontal, from top to bottom
    Horizontal_TB = 1,
    // Vertical, from left to right
    Vertical_LR = 2,
    // Vertical, from right to left
    Vertical_RL = 4,
};

constexpr bool Horizontal(WritingMode mode) { return mode == Horizontal_TB; }

constexpr bool Vertical(WritingMode mode) {
    return mode == Vertical_LR || mode == Vertical_RL;
}

} // namespace Text

#endif /* GRAPHIC_GRAPHIC_H */
