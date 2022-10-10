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

#include <assert.h>

Layer::Layer(uint32_t width, uint32_t height, int32_t rotate)
    : Paint(width, height, rotate){};

Layer::Layer(const Layer &layer) : Paint(layer) {}

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
    size_t size = width * height;
    return size;
}
