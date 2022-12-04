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

#include "imagelayer.h"
#include "traits/layersetters.cpp"

#include <cstdint>
#include <cstring>
#include <exception>

ImageLayer::ImageLayer(uint32_t width, uint32_t height, int32_t rotate)
    : Layer(width, height, rotate) {}

ImageLayer::ImageLayer(const Layer &layer) : Layer(layer) {}

void ImageLayer::LoadFrom(BMPImage *image) {
    if (this->width != image->GetWidth() ||
        this->height != image->GetHeight()) {
        char error_msg[80];
        snprintf(error_msg, 80,
                 "Image size incompatible, layer size: [%" PRId32 "x%" PRId32
                 "], image size: [%" PRId32 "x%" PRId32 "]",
                 this->width, this->height, image->GetWidth(),
                 image->GetHeight());
        throw std::runtime_error(error_msg);
    }

    // Debug block
    {
        assert_is_initialized(image->GetFrontImage());
        assert_is_initialized(image->GetBackImage());

        assert_is_initialized(new_image);
        assert_is_initialized(old_image);
    }

    auto memSize = GetMemSize();
    std::memcpy(new_image, image->GetFrontImage(), memSize);
    std::memcpy(old_image, image->GetBackImage(), memSize);

    if (invertColor) {
        InvertColor();
    }
}

void ImageLayer::LoadFrom(const char *imageFile) {
    assert_is_initialized(new_image);
    assert_is_initialized(old_image);

    BMPImage image(GetWidth(), GetHeight(), GetNewImage(), GetOldImage());
    image.Load(imageFile);

    if (invertColor) {
        InvertColor();
    }
}
