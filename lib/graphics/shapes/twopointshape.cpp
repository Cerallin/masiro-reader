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

#include "shapes.h"

#include <algorithm>

using namespace Shape;

TwoPointShape::TwoPointShape(Point topLeft, Point bottomRight)
    : topLeft(topLeft), bottomRight(bottomRight) {
    auto x = std::minmax(topLeft.x, bottomRight.x);
    auto y = std::minmax(topLeft.y, bottomRight.y);

    topLeft.x = std::get<0>(x);
    topLeft.y = std::get<0>(y);
    bottomRight.x = std::get<1>(x);
    bottomRight.y = std::get<1>(y);
}

TwoPointShape::TwoPointShape(coordinate x0, coordinate y0, coordinate x1,
                             coordinate y1)
    : TwoPointShape(Point(x0, y0), Point(x1, y1)) {}

Rectangle::Rectangle(Point topLeft, Point bottomRight)
    : TwoPointShape(topLeft, bottomRight) {}

Rectangle::Rectangle(coordinate x0, coordinate y0, coordinate x1, coordinate y1)
    : TwoPointShape(x0, y0, x1, y1) {}

Line::Line(Point start, Point end) : TwoPointShape(start, end) {}

Line::Line(coordinate x0, coordinate y0, coordinate x1, coordinate y1)
    : TwoPointShape(x0, y0, x1, y1) {}

HorizontalLine::HorizontalLine(coordinate x0, coordinate x1, coordinate y)
    : Line(Point(x0, y), Point(x1, y)) {}

VerticalLine::VerticalLine(coordinate x, coordinate y0, coordinate y1)
    : Line(Point(x, y0), Point(x, y1)) {}
