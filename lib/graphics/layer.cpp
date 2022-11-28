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

#include "debug.h"

#include "layer.h"

#include "traits/layersetters.cpp"

Graphic::Rotate Layer::ROTATE_DEFAULT = Graphic::ROTATE_270;

Layer::Layer(uint32_t width, uint32_t height, int32_t rotate,
             bool invertColor) {
    /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
    this->width = width % 8 ? width + 8 - (width % 8) : width;
    this->height = height;
    this->rotate = rotate;
    this->invertColor = invertColor;
};

Layer::Layer(const Layer &layer)
    : new_image(layer.new_image), old_image(layer.old_image),
      width(layer.width), height(layer.height), rotate(layer.rotate),
      invertColor(layer.invertColor) {}

size_t Layer::GetMemSize() const {
    size_t size = width * height / 8;
    return size;
}

Layer &Layer::Clear(Graphic::Color color) {
    LoopMatrix(this->width, this->height, 0, 0) {
        DrawAbsolute(Shape::Point(i, j), color);
    }

    return *this;
}

bool Layer::GetInvertColor() const { return invertColor; }

void Layer::drawAbsolute(uint8_t *image, bool color) {
    if (color) {
        *image = 0x00;
    } else {
        *image = ~((uint8_t)0x00);
    }
}

void Layer::drawAbsolute(uint8_t *image, int32_t x, bool color) {
    if (color) {
        *image &= ~(0x80 >> (x % 8));
    } else {
        *image |= 0x80 >> (x % 8);
    }
}

void Layer::DrawAbsolute(Shape::Point point, Graphic::Color color) {
    auto x = point.x, y = point.y;

    if (this->invertColor) {
        color = Graphic::InvertColor(color);
    }

    drawAbsolute(&new_image[(x + y * this->width) / 8], x, color & 0x01);
    drawAbsolute(&old_image[(x + y * this->width) / 8], x, color & 0x02);
}

uint8_t *Layer::GetNewImage(void) const { return this->new_image; }

uint8_t *Layer::GetOldImage(void) const { return this->old_image; }

int32_t Layer::GetWidth(void) const { return width; }

int32_t Layer::GetRelativeWidth(void) const {
    if (rotate == Graphic::ROTATE_0 || rotate == Graphic::ROTATE_180)
        return this->width;
    else
        return this->height;
}

int32_t Layer::GetRelativeHeight(void) const {
    if (rotate == Graphic::ROTATE_0 || rotate == Graphic::ROTATE_180)
        return this->height;
    else
        return this->width;
}

int32_t Layer::GetHeight(void) const { return height; }

int32_t Layer::GetRotate(void) const { return this->rotate; }

void Layer::Draw(Shape::Point pixel, Graphic::Color color) {
    auto x = pixel.x, y = pixel.y;

    if (rotate == Graphic::ROTATE_0) {
        DrawAbsolute(Shape::Point(x, y), color);
    } else if (rotate == Graphic::ROTATE_90) {
        DrawAbsolute(Shape::Point(width - y, x), color);
    } else if (rotate == Graphic::ROTATE_180) {
        DrawAbsolute(Shape::Point(width - x, height - y), color);
    } else if (rotate == Graphic::ROTATE_270) {
        DrawAbsolute(Shape::Point(y, height - x), color);
    }
}

void Layer::Draw(Shape::Line line, Graphic::Color color) {
    auto x0 = line.start.x, y0 = line.start.y;
    auto x1 = line.end.x, y1 = line.end.y;

    /* Bresenham algorithm */
    int32_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy;

    while ((x0 != x1) && (y0 != y1)) {
        Draw(Shape::Point(x0, y0), color);
        if (2 * err >= dy) {
            err += dy;
            x0 += sx;
        }
        if (2 * err <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Layer::DrawAbsolute(Shape::HorizontalLine line, Graphic::Color color) {
    auto width = line.end.x - line.start.x + 1;
    auto x0 = line.start.x, y = line.end.y;

    assert(width > 0);

    int32_t preBits = 8 - x0 % 8, preCount = (x0 + preBits) / 8;
    int32_t byteCount = (width - preBits) / 8, postBits = (width - preBits) % 8;

    if (preBits > width) {
        preBits = width;
    }

    LoopLine(x0, preBits) { DrawAbsolute(Shape::Point(i, y), color); }
    LoopLine(x0 + preBits + byteCount * 8, postBits) {
        DrawAbsolute(Shape::Point(i, y), color);
    }

    if (this->invertColor) {
        color = Graphic::InvertColor(color);
    }

    LoopLine(preCount, byteCount) {
        drawAbsolute(&new_image[(i * 8 + y * this->width) / 8], color & 0x01);
        drawAbsolute(&old_image[(i * 8 + y * this->width) / 8], color & 0x02);
    }
}

void Layer::DrawAbsolute(Shape::VerticalLine line, Graphic::Color color) {
    auto x = line.end.x;
    auto height = line.end.y - line.start.y + 1;

    assert(height > 0);
    LoopLine(line.start.y, height) { DrawAbsolute(Shape::Point(x, i), color); }
}

void Layer::Draw(Shape::HorizontalLine line, Graphic::Color color) {
    auto x0 = line.start.x, y0 = line.start.y;
    auto x1 = line.end.x;

    if (rotate == Graphic::ROTATE_0) {
        DrawAbsolute(line, color);
    } else if (rotate == Graphic::ROTATE_90) {
        auto absoluteLine = Shape::VerticalLine(width - y0, x0, x1);
        DrawAbsolute(absoluteLine, color);
    } else if (rotate == Graphic::ROTATE_180) {
        auto absoluteLine =
            Shape::HorizontalLine(width - x1, width - x0, height - y0);
        DrawAbsolute(absoluteLine, color);
    } else if (rotate == Graphic::ROTATE_270) {
        auto absoluteLine = Shape::VerticalLine(y0, height - x1, height - x0);
        DrawAbsolute(absoluteLine, color);
    }
}

void Layer::Draw(Shape::VerticalLine line, Graphic::Color color) {
    auto x0 = line.start.x, y0 = line.start.y;
    auto y1 = line.end.y;

    if (rotate == Graphic::ROTATE_0) {
        DrawAbsolute(line, color);
    } else if (rotate == Graphic::ROTATE_90) {
        auto absoluteLine = Shape::HorizontalLine(width - y1, width - y0, x0);
        DrawAbsolute(absoluteLine, color);
    } else if (rotate == Graphic::ROTATE_180) {
        auto absoluteLine =
            Shape::VerticalLine(width - x0, height - y0, height - y1);
        DrawAbsolute(absoluteLine, color);
    } else if (rotate == Graphic::ROTATE_270) {
        auto absoluteLine = Shape::HorizontalLine(y0, y1, height - x0);
        DrawAbsolute(absoluteLine, color);
    }
}

void Layer::Draw(Shape::Rectangle rectangle, Graphic::Color color) {
    auto x0 = rectangle.start.x, y0 = rectangle.start.y;
    auto x1 = rectangle.end.x, y1 = rectangle.end.y;

    Draw(Shape::HorizontalLine(x0, x1, y0), color);
    Draw(Shape::HorizontalLine(x0, x1, y1), color);
    Draw(Shape::VerticalLine(x0, y0, y1), color);
    Draw(Shape::VerticalLine(x1, y0, y1), color);
}

void Layer::DrawFilled(Shape::Rectangle rectangle, Graphic::Color color) {
    auto x0 = rectangle.start.x;
    auto width = rectangle.end.x - rectangle.start.x;
    auto y0 = rectangle.start.y, y1 = rectangle.end.y;

    assert(width >= 0);
    assert(y1 - y0 >= 0);

    LoopLine(x0, width + 1) { Draw(Shape::VerticalLine(i, y0, y1), color); }
}

void Layer::Draw(Shape::Circle circle, Graphic::Color color) {
    auto x = circle.center.x, y = circle.center.y;
    auto radius = circle.radius;

    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do {
        Draw(Shape::Point(x - x_pos, y + y_pos), color);
        Draw(Shape::Point(x + x_pos, y + y_pos), color);
        Draw(Shape::Point(x + x_pos, y - y_pos), color);
        Draw(Shape::Point(x - x_pos, y - y_pos), color);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}

void Layer::DrawFilled(Shape::Circle circle, Graphic::Color color) {
    auto x = circle.center.x, y = circle.center.y;
    auto radius = circle.radius;

    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do {
        Draw(Shape::HorizontalLine(x + x_pos, x - x_pos, y + y_pos), color);
        Draw(Shape::HorizontalLine(x + x_pos, x - x_pos, y - y_pos), color);
        e2 = err;
        if (e2 <= y_pos) {
            err += ++y_pos * 2 + 1;
            if (-x_pos == y_pos && e2 <= x_pos) {
                e2 = 0;
            }
        }
        if (e2 > x_pos) {
            err += ++x_pos * 2 + 1;
        }
    } while (x_pos <= 0);
}
