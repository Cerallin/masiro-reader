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

HasMargin::HasMargin() { SetMargin(0); }

const Margin *HasMargin::GetMargin() { return &margin; }

void HasMargin::SetMargin(int32_t marginValue) {
    SetMargin(marginValue, marginValue);
}

void HasMargin::SetMargin(int32_t marginX, int32_t marginY) {
    SetMargin(marginX, marginY, marginX, marginY);
}

void HasMargin::SetMargin(int32_t marginLeft, int32_t marginTop,
                          int32_t marginRight, int32_t marginBottom) {
    margin = {
        .marginLeft = marginLeft,
        .marginRight = marginRight,
        .marginTop = marginTop,
        .marginBottom = marginBottom,
    };
}

}; // namespace Style