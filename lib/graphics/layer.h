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

/******************************************************************************/
/***        include files                                                   ***/
/******************************************************************************/

#include "epd37/epd2in13b.h"
#include "fonts/font.h"
#include "style.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

namespace Graphic {

enum Rotate {
    ROTATE_0 = 1,
    ROTATE_90 = 2,
    ROTATE_180 = 4,
    ROTATE_270 = 8,
};

enum Color {
    WW = 0,
    WB = 1,
    BW = 2,
    BB = 3,
};

} // namespace Graphic

class Layer {
  public:
    Layer(uint32_t width, uint32_t height, int32_t rotate = ROTATE_DEFAULT,
          bool invertColor = false);
    Layer(const Layer &layer);
    ~Layer() = default;

    #define _Class Layer
    #include "traits/layersetters.h"

    /**
     * @brief Get size of an image (front or back)
     *
     * @returns size in bytes
     */
    size_t GetMemSize() const;

    /**
     * @brief Clear all pixels
     *
     * @param color color to paint
     */
    Layer &Clear(int32_t color);

    /**
     * @brief Get width, NOT affected by rotate
     */
    int32_t GetWidth() const;

    /**
     * @brief Get relative width, affected by rotate
     */
    int32_t GetRelativeWidth() const;

    /**
     * @brief Get height, NOT affected by rotate
     */
    int32_t GetHeight() const;

    /**
     * @brief Get relative height, affected by rotate
     */
    int32_t GetRelativeHeight() const;

    /**
     * @brief Get rotate
     */
    int32_t GetRotate() const;

    bool GetInvertColor(void) const;

    /**
     * @brief Get new image
     *
     * @returns new image pointer
     */
    uint8_t *GetNewImage(void) const;

    /**
     * @brief Get old image
     *
     * @returns old image pointer
     */
    uint8_t *GetOldImage() const;

    /**
     * @brief
     *
     * @param[in] x
     * @param[in] y
     * @param[in] color
     */
    void DrawAbsolutePixel(int32_t x, int32_t y, int32_t color);

    /**
     * @brief this draws a pixel by the coordinates.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] color
     */
    void DrawPixel(int32_t x, int32_t y, int32_t color);

    /**
     * @brief this draws a line on the frame buffer.
     *
     * @param[in] x0
     * @param[in] y0
     * @param[in] x1
     * @param[in] y1
     * @param[in] color
     */
    void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                  int32_t color);

    /**
     * @brief this draws a horizontal line on the frame buffer.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] width
     * @param[in] color
     */
    void DrawHorizontalLine(int32_t x, int32_t y, int32_t width, int32_t color);

    /**
     * @brief this draws a vertical line on the frame buffer.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] height
     * @param[in] color
     */
    void DrawVerticalLine(int32_t x, int32_t y, int32_t height, int32_t color);

    /**
     * @brief this draws a rectangle.
     *
     * @param[in] x0
     * @param[in] y0
     * @param[in] x1
     * @param[in] y1
     * @param[in] color
     */
    void DrawRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                       int32_t color);

    /**
     * @brief this draws a filled rectangle.
     *
     * @param[in] x0
     * @param[in] y0
     * @param[in] x1
     * @param[in] y1
     * @param[in] color
     */
    void DrawFilledRectangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
                             int32_t color);

    /**
     * @brief this draws a circle.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] radius
     * @param[in] color
     */
    void DrawCircle(int32_t x, int32_t y, int32_t radius, int32_t color);

    /**
     * @brief this draws a filled circle.
     *
     * @param[in] x
     * @param[in] y
     * @param[in] radius
     * @param[in] color
     */
    void DrawFilledCircle(int32_t x, int32_t y, int32_t radius, int32_t color);

  protected:
    uint8_t *new_image = nullptr;
    uint8_t *old_image = nullptr;
    int32_t width;
    int32_t height;
    int32_t rotate;
    bool invertColor;

    static Graphic::Rotate ROTATE_DEFAULT;
};

/******************************************************************************/
/***        exported variables                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

#define GetMatrix(bitmap, w, x, y) (bitmap[(y) * (w) + (x)])

#define LoopMatrix(w, h, x, y)                                                 \
    for (auto i = x; i < x + w; i++)                                           \
        for (auto j = y; j < y + h; j++)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GRAPHIC_LAYER_H */

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/