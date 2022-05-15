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

#include "domnode.h"

namespace DOM {

DOMNode::DOMNode(int32_t width, int32_t height) {
    SetWidth(width);
    SetHeight(height);
}

void DOMNode::SetWidth(int32_t width) { this->width = width; }

void DOMNode::SetHeight(int32_t height) { this->height = height; }

}; // namespace DOM
