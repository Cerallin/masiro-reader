/*******************************************************************************
 * MIT License
 *
 * Copyright (c) 2021 by Lilygo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "epdpaint.h"
#include <new>
#include <string.h>

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

/******************************************************************************/
/***        local function prototypes                                       ***/
/******************************************************************************/

/******************************************************************************/
/***        exported variables                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        local variables                                                 ***/
/******************************************************************************/

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        local functions                                                 ***/
/******************************************************************************/

Paint::Paint(unsigned char *new_image, unsigned char *old_image, uint32_t width,
             uint32_t height, int32_t rotate)
    : new_image(new_image), old_image(old_image),
      /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
      width(width % 8 ? width + 8 - (width % 8) : width), height(height),
      rotate(rotate), invertColor(false) {}

Paint::Paint(uint32_t width, uint32_t height, int32_t rotate)
    : /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
      width(width % 8 ? width + 8 - (width % 8) : width), height(height),
      rotate(rotate) {}

Paint::Paint(const Paint &paint)
    : new_image(paint.new_image), old_image(paint.old_image),
      width(paint.width), height(paint.height), rotate(paint.rotate),
      invertColor(paint.invertColor) {}

Paint &Paint::Init() {
    Clear(COLOR_WW);
    return *this;
}

void Paint::Clear(int32_t colored) {
    for (int32_t x = 0; x < this->width; x++) {
        for (int32_t y = 0; y < this->height; y++) {
            DrawAbsolutePixel(x, y, colored);
        }
    }
}

Paint &Paint::SetInvertColor(bool flag) {
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

bool Paint::GetInvertColor() { return invertColor; }

void Paint::DrawAbsolutePixel(int32_t x, int32_t y, int32_t colored) {
    if (invertColor) {
        colored = (~colored) & 0x03;
    }

    if (colored & 0x01) {
        new_image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
    } else {
        new_image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
    }

    if (colored & 0x02) {
        old_image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
    } else {
        old_image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
    }
}

uint8_t *Paint::GetNewImage(void) { return this->new_image; }

uint8_t *Paint::GetOldImage(void) { return this->old_image; }

int32_t Paint::GetWidth(void) { return width; }

int32_t Paint::GetRelativeWidth(void) {
    if (rotate == ROTATE_0 || rotate == ROTATE_180)
        return this->width;
    else
        return this->height;
}

int32_t Paint::GetRelativeHeight(void) {
    if (rotate == ROTATE_0 || rotate == ROTATE_180)
        return this->height;
    else
        return this->width;
}

int32_t Paint::GetHeight(void) { return height; }

Paint &Paint::SetHeight(int32_t height) {
    this->height = height;
    return *this;
}

int32_t Paint::GetRotate(void) { return this->rotate; }

Paint &Paint::SetRotate(int32_t rotate) {
    this->rotate = rotate;
    return *this;
}

void Paint::DrawPixel(int32_t x, int32_t y, int32_t colored) {
    if (rotate == ROTATE_0) {
        DrawAbsolutePixel(x, y, colored);
    } else if (rotate == ROTATE_90) {
        DrawAbsolutePixel(width - y, x, colored);
    } else if (rotate == ROTATE_180) {
        DrawAbsolutePixel(width - x, height - y, colored);
    } else if (rotate == ROTATE_270) {
        DrawAbsolutePixel(y, height - x, colored);
    }
}

void Paint::DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                     int32_t colored) {
    /* Bresenham algorithm */
    int32_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
    int32_t sx = x0 < x1 ? 1 : -1;
    int32_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
    int32_t sy = y0 < y1 ? 1 : -1;
    int32_t err = dx + dy;

    while ((x0 != x1) && (y0 != y1)) {
        DrawPixel(x0, y0, colored);
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

void Paint::DrawHorizontalLine(int32_t x, int32_t y, int32_t line_width,
                               int32_t colored) {
    int32_t i;
    for (i = x; i < x + line_width; i++) {
        DrawPixel(i, y, colored);
    }
}

void Paint::DrawVerticalLine(int32_t x, int32_t y, int32_t line_height,
                             int32_t colored) {
    int32_t i;
    for (i = y; i < y + line_height; i++) {
        DrawPixel(x, i, colored);
    }
}

void Paint::DrawRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                          int32_t colored) {
    int32_t min_x, min_y, max_x, max_y;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, colored);
    DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, colored);
    DrawVerticalLine(min_x, min_y, max_y - min_y + 1, colored);
    DrawVerticalLine(max_x, min_y, max_y - min_y + 1, colored);
}

void Paint::DrawFilledRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                                int32_t colored) {
    int32_t min_x, min_y, max_x, max_y;
    int32_t i;
    min_x = x1 > x0 ? x0 : x1;
    max_x = x1 > x0 ? x1 : x0;
    min_y = y1 > y0 ? y0 : y1;
    max_y = y1 > y0 ? y1 : y0;

    for (i = min_x; i <= max_x; i++) {
        DrawVerticalLine(i, min_y, max_y - min_y + 1, colored);
    }
}

void Paint::DrawCircle(int32_t x, int32_t y, int32_t radius, int32_t colored) {
    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
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

void Paint::DrawFilledCircle(int32_t x, int32_t y, int32_t radius,
                             int32_t colored) {
    /* Bresenham algorithm */
    int32_t x_pos = -radius;
    int32_t y_pos = 0;
    int32_t err = 2 - 2 * radius;
    int32_t e2;

    do {
        DrawPixel(x - x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y + y_pos, colored);
        DrawPixel(x + x_pos, y - y_pos, colored);
        DrawPixel(x - x_pos, y - y_pos, colored);
        DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
        DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
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

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/