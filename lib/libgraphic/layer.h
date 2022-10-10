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

#ifndef GRAPHIC_LAYER_H
#define GRAPHIC_LAYER_H

#include "epd2in13b.h"
#include "epdpaint.h"
#include "font.h"
#include "style.h"

#define ROTATE_DEFAULT ROTATE_270

#define OVERRIDE_0_ARGS_FUNC(MyClass, BaseClass, Method)                       \
    MyClass &Method() { return (MyClass &)BaseClass::Method(); };

#define OVERRIDE_1_ARGS_FUNC(MyClass, BaseClass, Method, type1)                \
    MyClass &Method(type1 arg1) { return (MyClass &)BaseClass::Method(arg1); };

#define OVERRIDE_2_ARGS_FUNC(MyClass, BaseClass, Method, type1, type2)         \
    MyClass &Method(type1 arg1, type2 arg2) {                                  \
        return (MyClass &)BaseClass::Method(arg1, arg2);                       \
    };

class Layer : public Paint {
  public:
    Layer(uint32_t width, uint32_t height, int32_t rotate = ROTATE_DEFAULT);
    Layer(const Layer &layer);
    virtual ~Layer() = default;

    void Display(Epd *epd);

    /**
     * @brief Set front image
     *
     * @param front
     * @return Layer &
     */
    Layer &SetFrontImage(uint8_t *front);
    Layer &SetBackImage(uint8_t *back);
    Layer &SetImages(uint8_t *image);

    size_t GetMemSize();

    OVERRIDE_0_ARGS_FUNC(Layer, Paint, Init);

    OVERRIDE_1_ARGS_FUNC(Layer, Paint, SetHeight, int32_t);

    OVERRIDE_1_ARGS_FUNC(Layer, Paint, SetRotate, int32_t);

    OVERRIDE_1_ARGS_FUNC(Layer, Paint, SetInvertColor, bool);
};

#define OVERRIDE_LAYER(MyClass)                                                \
    OVERRIDE_1_ARGS_FUNC(MyClass, Layer, SetFrontImage, uint8_t *);            \
    OVERRIDE_1_ARGS_FUNC(MyClass, Layer, SetBackImage, uint8_t *);             \
    OVERRIDE_1_ARGS_FUNC(MyClass, Layer, SetImages, uint8_t *);                \
    OVERRIDE_0_ARGS_FUNC(MyClass, Layer, Init);                                \
    OVERRIDE_1_ARGS_FUNC(MyClass, Layer, SetHeight, int32_t);                  \
    OVERRIDE_1_ARGS_FUNC(MyClass, Layer, SetRotate, int32_t);                  \
    OVERRIDE_1_ARGS_FUNC(MyClass, Layer, SetInvertColor, bool);

#endif /* GRAPHIC_LAYER_H */
