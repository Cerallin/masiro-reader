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

#include "layer.h"

#include "debug.h"

Rotate Layer::ROTATE_DEFAULT = ROTATE_270;

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

void Layer::Display(Epd *epd) { epd->DisplayFrame(old_image, new_image); }

Layer &Layer::SetFrontImage(uint8_t *front) {
    this->new_image = front;
    return *this;
}

Layer &Layer::SetBackImage(uint8_t *back) {
    this->old_image = back;
    return *this;
}

Layer &Layer::SetImages(uint8_t *image) {
    this->new_image = image;
    this->old_image = image + width * height / 8;
    return *this;
}

size_t Layer::GetMemSize() {
    size_t size = width * height / 8;
    return size;
}

Layer &Layer::Init() {
    Clear(Color::WW);
    return *this;
}

Layer &Layer::Clear(int32_t color) {
    LoopMatrix(this->width, this->height, 0, 0) {
        DrawAbsolutePixel(i, j, color);
    }

    return *this;
}

Layer &Layer::SetInvertColor(bool flag) {
    if (invertColor == flag)
        return *this;

    invertColor = flag;

    int32_t w = width / 8;

    LoopMatrix(w, height, 0, 0) {
        new_image[i + j * w] = ~new_image[i + j * w];
        old_image[i + j * w] = ~old_image[i + j * w];
    }

    return *this;
}

bool Layer::GetInvertColor() { return invertColor; }

void Layer::DrawAbsolutePixel(int32_t x, int32_t y, int32_t color) {
    if (invertColor) {
        color = (~color) & 0x03;
    }

    if (color & 0x01) {
        new_image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
    } else {
        new_image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
    }

    if (color & 0x02) {
        old_image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
    } else {
        old_image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
    }
}

uint8_t *Layer::GetNewImage(void) { return this->new_image; }

uint8_t *Layer::GetOldImage(void) { return this->old_image; }

int32_t Layer::GetWidth(void) { return width; }

int32_t Layer::GetRelativeWidth(void) {
    if (rotate == ROTATE_0 || rotate == ROTATE_180)
        return this->width;
    else
        return this->height;
}

int32_t Layer::GetRelativeHeight(void) {
    if (rotate == ROTATE_0 || rotate == ROTATE_180)
        return this->height;
    else
        return this->width;
}

int32_t Layer::GetHeight(void) { return height; }

Layer &Layer::SetRelativeHeight(int32_t len) {
    if (rotate == ROTATE_0 || rotate == ROTATE_180)
        this->height = len;
    else
        this->width = len;

    return *this;
}

Layer &Layer::SetRelativeWidth(int32_t len) {
    if (rotate == ROTATE_0 || rotate == ROTATE_180)
        this->width = len;
    else
        this->height = len;

    return *this;
}

int32_t Layer::GetRotate(void) { return this->rotate; }

Layer &Layer::SetRotate(int32_t rotate) {
    this->rotate = rotate;
    return *this;
}

void Layer::DrawPixel(int32_t x, int32_t y, int32_t color) {
    if (rotate == ROTATE_0) {
        DrawAbsolutePixel(x, y, color);
    } else if (rotate == ROTATE_90) {
        DrawAbsolutePixel(width - y, x, color);
    } else if (rotate == ROTATE_180) {
        DrawAbsolutePixel(width - x, height - y, color);
    } else if (rotate == ROTATE_270) {
        DrawAbsolutePixel(y, height - x, color);
    }
}

void Layer::DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                     int32_t color) {
    /* Bresenham algorithm */
    int32_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy;

    while ((x0 != x1) && (y0 != y1)) {
        DrawPixel(x0, y0, color);
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

void Layer::DrawHorizontalLine(int32_t x, int32_t y, int32_t line_width,
                               int32_t color) {
    int32_t i;
    for (i = x; i < x + line_width; i++) {
        DrawPixel(i, y, color);
    }
}

void Layer::DrawVerticalLine(int32_t x, int32_t y, int32_t line_height,
                             int32_t color) {
    int32_t i;
    for (i = y; i < y + line_height; i++) {
        DrawPixel(x, i, color);
    }
}

void Layer::DrawRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                          int32_t color) {
    int32_t min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, color);
    DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, color);
    DrawVerticalLine(min_x, min_y, max_y - min_y + 1, color);
    DrawVerticalLine(max_x, min_y, max_y - min_y + 1, color);
}

void Layer::DrawFilledRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                                int32_t color) {
    int32_t min_x, min_y, max_x, max_y;
    int32_t i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (i = min_x; i <= max_x; i++) {
        DrawVerticalLine(i, min_y, max_y - min_y + 1, color);
    }
}

void Layer::DrawCircle(int32_t x, int32_t y, int32_t radius, int32_t color) {
    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do {
        DrawPixel(x - x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y - y_pos, color);
        DrawPixel(x - x_pos, y - y_pos, color);
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

void Layer::DrawFilledCircle(int32_t x, int32_t y, int32_t radius,
                             int32_t color) {
    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do {
        DrawPixel(x - x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y + y_pos, color);
        DrawPixel(x + x_pos, y - y_pos, color);
        DrawPixel(x - x_pos, y - y_pos, color);
        DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, color);
        DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, color);
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