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

#include "graphic.h"

namespace Graphic {

Color CastColor(int color) {
    color = color & 0x03;
    Color map[] = {
        Color::WW,
        Color::WB,
        Color::BW,
        Color::BB,
    };
    return map[color];
}

Color InvertColor(int color) {
    color = (~color) & 0x03;
    Color map[] = {
        Color::BB,
        Color::BW,
        Color::WB,
        Color::WW,
    };
    return map[color];
}

Color InvertColor(Color color) {
    switch (color) {
    case Color::WW:
        return Color::BB;
    case Color::WB:
        return Color::BW;
    case Color::BW:
        return Color::WB;
    case Color::BB:
    default:
        return Color::WW;
    }
}

} // namespace Graphic
