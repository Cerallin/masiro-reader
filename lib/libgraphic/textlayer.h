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

#ifndef GRAPHIC_TEXT_LAYER_H
#define GRAPHIC_TEXT_LAYER_H

#include "codepoint.h"
#include "layer.h"
#include "style.h"

#ifdef __cplusplus
extern "C" {
#endif

class TextLayer : public Layer {
  public:
    TextLayer(uint32_t width, uint32_t height, Font *font,
              Graphic::TextAlign textAlign = Graphic::AlignLeft,
              int32_t rotate = ROTATE_DEFAULT);

    TextLayer(uint32_t width, uint32_t height, CodePoint *codepoints,
              Font *font, Graphic::TextAlign textAlign = Graphic::AlignLeft,
              int32_t rotate = ROTATE_DEFAULT);

    TextLayer(const Layer &layer,
              Graphic::TextAlign textAlign = Graphic::AlignLeft);

    virtual ~TextLayer();

    OVERRIDE_LAYER(TextLayer);

    /**
     * @brief Set text.
     *
     * @param str Unicode series starts with 0xFEFF
     * @return int
     */
    int SetText(char *str);

    TextLayer &SetFont(Font *font);

    TextLayer &SetTextAlign(Graphic::TextAlign textAlign);

    TextLayer &SetTextPadding(Graphic::TextPadding textPadding);
    TextLayer &SetTextPadding(int padding);
    TextLayer &SetTextPadding(int paddingX, int paddingY);
    TextLayer &SetTextPadding(int paddingLeft, int paddingTop, int paddingRight,
                              int paddingBottom);

    /**
     * @brief Render the characters
     *
     */
    void Render();

  private:
    /**
     * @brief Draw a single char, with no coordinate checks
     *
     * @param x
     * @param y
     * @param codepoint
     * @param font
     */
    void DrawGlyph(const Graphic::TextTypeSetting *typeSetting, Font *font,
                   const unsigned char *bitmap);

    CodePoint *codepoints = nullptr;
    Font *font = nullptr;

    Graphic::TextTypeSetting *typeSetting = nullptr;
    Graphic::TextAlign textAlign = Graphic::AlignLeft;
    Graphic::TextPadding textPadding = {0, 0, 0, 0};

    ssize_t charNum;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GRAPHIC_TEXT_LAYER_H */
