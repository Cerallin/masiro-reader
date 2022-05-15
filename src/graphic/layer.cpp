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

#include <string.h>

Layer::Layer(uint32_t width, uint32_t height, int32_t rotate)
    : Paint(width, height, rotate){};

void Layer::DrawCharAt(int32_t x, int32_t y, unsigned char *bitmap, int width,
                       int height, Font *font) {
    LoopMatrix(width, height, x, y) {
        int color = GetMatrix(bitmap, width, i - x, j - y) >> 6;
        if (color != COLOR_WW)
            DrawPixel(i, j, color);
    };
}

int Layer::LoadFrom(BMPImage *image) {
    if (this->width != image->GetWidth() ||
        this->height != image->GetHeight()) {
        return -1;
    }

    memcpy(new_image, image->GetFrontImge(), width * height / 8);
    memcpy(old_image, image->GetBackImge(), width * height / 8);

    return 0;
}

void Layer::Display(Epd *epd) { epd->DisplayFrame(old_image, new_image); }
