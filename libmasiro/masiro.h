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

#ifndef MASIRO_H
#define MASIRO_H

#include "display.h"
#include "graphics.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Function to initialize the shared library, do not call it yourself.
*/
void __init() __attribute__((constructor));

void masiro_version();

/**
 * save layer image to file.
 *
 * @throw ImageSaveException
 */
void save_image(Layer &layer, const char *filename);

#ifdef __cplusplus
}
#endif

#endif /* MASIRO_H */
