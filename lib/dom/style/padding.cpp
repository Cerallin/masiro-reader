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

#include "style.h"

namespace Style {

HasPadding::HasPadding() { SetPadding(0); }

const Padding *HasPadding::GetPadding() { return &padding; }

void HasPadding::SetPadding(int32_t paddingValue) {
    SetPadding(paddingValue, paddingValue);
}

void HasPadding::SetPadding(int32_t paddingX, int32_t paddingY) {
    SetPadding(paddingX, paddingY, paddingX, paddingY);
}

void HasPadding::SetPadding(int32_t paddingLeft, int32_t paddingTop,
                            int32_t paddingRight, int32_t paddingBottom) {
    padding = {
        .paddingLeft = paddingLeft,
        .paddingRight = paddingRight,
        .paddingTop = paddingTop,
        .paddingBottom = paddingBottom,
    };
}

}; // namespace Style