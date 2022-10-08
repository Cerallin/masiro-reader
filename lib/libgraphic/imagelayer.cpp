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

#include "imagelayer.h"
#include <string.h>

ImageLayer::ImageLayer(uint32_t width, uint32_t height, int32_t rotate)
    : Layer(width, height, rotate) {};

ImageLayer::ImageLayer(const Layer &layer) : Layer(layer) {}

int ImageLayer::LoadFrom(BMPImage *image) {
    if (this->width != image->GetWidth() ||
        this->height != image->GetHeight()) {
        return -1;
    }

    memcpy(new_image, image->GetFrontImage(), width * height / 8);
    memcpy(old_image, image->GetBackImage(), width * height / 8);

    return 0;
}
