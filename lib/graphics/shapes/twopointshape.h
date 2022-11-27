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

#ifndef GRAPHIC_SHAPE_TWO_POINT_SHAPE_H
#define GRAPHIC_SHAPE_TWO_POINT_SHAPE_H

namespace Shape {

class TwoPointShape {
  public:
    TwoPointShape(Point start, Point end);
    TwoPointShape(coordinate x0, coordinate y0, coordinate x1, coordinate y1);

    Point start, end;
};

class Rectangle : public TwoPointShape {
  public:
    Rectangle(Point start, Point end);
    Rectangle(coordinate x0, coordinate y0, coordinate x1, coordinate y1);
};

class Line : public TwoPointShape {
  public:
    Line(Point start, Point end);
    Line(coordinate x0, coordinate y0, coordinate x1, coordinate y1);
};

class HorizontalLine : public Line {
  public:
    HorizontalLine(coordinate x0, coordinate x1, coordinate y);
};

class VerticalLine : public Line {
  public:
    VerticalLine(coordinate x, coordinate y0, coordinate y1);
};

} // namespace Shape

#endif /* GRAPHIC_SHAPE_TWO_POINT_SHAPE_H */
