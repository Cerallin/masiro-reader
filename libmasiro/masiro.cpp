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

#include "config.h"

#include "masiro.h"

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Function to initialize the library.
 */
void __init() {
    BufferPool::Init();
    // TODO: check EPD
}

void masiro_version() {
    fprintf(stderr, "Masiro-Reader Library v" PACKAGE_VERSION " (" ARCH ")\n"
                    "Copyright 2022\tCerallin <cerallin@cerallin.top>\n\n");
}

void save_image(Layer &layer, const char *filename) {
    BMPImage image(layer.GetWidth(), layer.GetHeight(), layer.GetNewImage(),
                   layer.GetOldImage());
    image.Save(filename);
}

#ifdef __cplusplus
}
#endif