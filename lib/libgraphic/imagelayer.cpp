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
#include "debug.h"
#include <string.h>

#include "traits/layersetters.cpp"

ImageLayer::ImageLayer(uint32_t width, uint32_t height, int32_t rotate)
    : Layer(width, height, rotate) {}

ImageLayer::ImageLayer(const Layer &layer) : Layer(layer) {}

int ImageLayer::LoadFrom(BMPImage *image) {
    auto memSize = GetMemSize();

    if (this->width != image->GetWidth() ||
        this->height != image->GetHeight()) {
        return -1;
    }

    // Debug block
    {
        assert_is_initialized(image->GetFrontImage());
        assert_is_initialized(image->GetBackImage());

        assert_is_initialized(new_image);
        assert_is_initialized(old_image);
    }

    memcpy(new_image, image->GetFrontImage(), memSize);
    memcpy(old_image, image->GetBackImage(), memSize);

    return 0;
}

int ImageLayer::LoadFrom(const char *imageFile) {
    assert_is_initialized(new_image);
    assert_is_initialized(old_image);

    BMPImage image(GetWidth(), GetHeight(), GetNewImage(), GetOldImage());
    return image.Load(imageFile);
}
