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

#include "frame.h"

Frame::Frame(const uint8_t *old_image, const uint8_t *new_image) {}

Frame::Frame(const Layer *layer)
    : old_image(layer->GetOldImage()), new_image(layer->GetNewImage()) {}

void Frame::Display(const Epd *epd) {
    epd->DisplayFrame(old_image, new_image);
    this->displayed = true;
}

bool Frame::isDisplayed() const { return this->displayed; }
