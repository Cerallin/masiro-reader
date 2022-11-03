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

#ifndef GRAPHIC_FRAME_H
#define GRAPHIC_FRAME_H

#include "epd2in13b.h"
#include "layer.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

class Frame {
  public:
    Frame(const uint8_t *old_image, const uint8_t *new_image);
    Frame(const Layer *layer);
    ~Frame() = default;

    void Display(const Epd *epd);

    bool isDisplayed() const;

  private:
    const uint8_t *old_image = nullptr;
    const uint8_t *new_image = nullptr;

    bool displayed = false;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
