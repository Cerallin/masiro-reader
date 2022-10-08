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

#ifndef GRAPHIC_IMAGE_LAYER_H
#define GRAPHIC_IMAGE_LAYER_H

#include "bmp.h"
#include "layer.h"

class ImageLayer : public Layer {
  public:
    ImageLayer(const Layer &layer);
    ImageLayer(uint32_t width, uint32_t height,
               int32_t rotate = ROTATE_DEFAULT);
    virtual ~ImageLayer() = default;

    int LoadFrom(BMPImage *image);
    int LoadFrom(const char *imageFile);
};

#endif /* GRAPHIC_IMAGE_LAYER_H */
