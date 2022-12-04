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
#include "graphic.h"
#include "shapes.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/***        macro definitions                                               ***/
/******************************************************************************/

/******************************************************************************/
/***        type definitions                                                ***/
/******************************************************************************/

class Layer {
  public:
    Layer(uint32_t width, uint32_t height, Graphic::Rotate rotate = Graphic::ROTATE_DEFAULT,
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
    Layer &Clear(Graphic::Color color);

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

    /**
     * @brief Get invertColor
     */
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
     * @brief Draw absolute pixel
     */
    void DrawAbsolute(Shape::Point point, Graphic::Color color);

    void DrawAbsolute(Shape::VerticalLine line, Graphic::Color color);
    void DrawAbsolute(Shape::HorizontalLine line, Graphic::Color color);

    /**
     * @brief Draw relative pixel
     */
    void Draw(Shape::Point point, Graphic::Color color);

    /**
     * @brief Draw line
     */
    void Draw(Shape::Line line, Graphic::Color color);

    /**
     * @brief Draw horizontal line
     */
    void Draw(Shape::HorizontalLine line, Graphic::Color color);

    /**
     * @brief Draw vertical line
     */
    void Draw(Shape::VerticalLine line, Graphic::Color color);

    /**
     * @brief Draw circle
     */
    void Draw(Shape::Circle circle, Graphic::Color color);

    /**
     * @brief Draw filled circle
     */
    void DrawFilled(Shape::Circle circle, Graphic::Color color);

    /**
     * @brief Draw rectangle
     */
    void Draw(Shape::Rectangle rectangle, Graphic::Color color);

    /**
     * @brief Draw filled rectangle
     */
    void DrawFilled(Shape::Rectangle rectangle, Graphic::Color color);

  protected:
    uint8_t *new_image = nullptr;
    uint8_t *old_image = nullptr;
    int32_t width;
    int32_t height;


    Graphic::Rotate rotate;

    Graphic::Padding textPadding;

    bool invertColor;

  private:
    static void drawAbsolute(uint8_t *image, bool color);
    static void drawAbsolute(uint8_t *image, int32_t x, bool color);
};

/******************************************************************************/
/***        exported variables                                              ***/
/******************************************************************************/

/******************************************************************************/
/***        exported functions                                              ***/
/******************************************************************************/

#define GetMatrix(bitmap, w, x, y) (bitmap[(y) * (w) + (x)])

#define LoopMatrix(w, h, x, y)                                                 \
    for (auto i = (x); i < (x) + (w); i++)                                     \
        for (auto j = (y); j < (y) + (h); j++)

#define LoopLine(start, count)                                                 \
    for (auto i = (start); i < (start) + (count); i++)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GRAPHIC_LAYER_H */

/******************************************************************************/
/***        END OF FILE                                                     ***/
/******************************************************************************/
