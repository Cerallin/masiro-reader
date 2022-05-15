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

HasPosition::HasPosition() {
    SetPosition(0, 0, 0, 0);
    SetPositionType(PositionRelative);
}

const PositionType *HasPosition::GetPositionType() { return &positionType; }

const Position *HasPosition::GetPosition() { return &position; }

void HasPosition::SetPositionType(PositionType type) { positionType = type; }

void HasPosition::SetPosition(int32_t left, int32_t top, int32_t right,
                              int32_t bottom) {
    position = {
        .left = left,
        .right = right,
        .top = top,
        .bottom = bottom,
    };
}

}; // namespace Style
