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

#ifndef _Class
#error "Please specify class name for layer traits"
#endif

_Class &_Class::SetFrontImage(uint8_t *front) {
    this->new_image = front;
    return *this;
}

_Class &_Class::SetBackImage(uint8_t *back) {
    this->old_image = back;
    return *this;
}

_Class &_Class::SetImages(uint8_t *image) {
    this->new_image = image;
    this->old_image = image + width * height / 8;
    return *this;
}

_Class &_Class::Init() {
    Clear(Graphic::Color::WW);
    return *this;
}

_Class &_Class::SetRotate(int32_t rotate) {
    this->rotate = rotate;
    return *this;
}

_Class &_Class::SetInvertColor(bool flag) {
    if (invertColor == flag)
        return *this;

    invertColor = flag;

    int32_t w = width / 8;

    LoopMatrix(w, height, 0, 0) {
        new_image[i + j * w] = ~new_image[i + j * w];
        old_image[i + j * w] = ~old_image[i + j * w];
    }

    return *this;
}

_Class &_Class::SetRelativeHeight(int32_t len) {
    if (rotate == Graphic::ROTATE_0 || rotate == Graphic::ROTATE_180)
        this->height = len;
    else
        this->width = len;

    return *this;
}

_Class &_Class::SetRelativeWidth(int32_t len) {
    if (rotate == Graphic::ROTATE_0 || rotate == Graphic::ROTATE_180)
        this->width = len;
    else
        this->height = len;

    return *this;
}
