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

#ifndef GRAPHIC_PARAGRAPH_H
#define GRAPHIC_PARAGRAPH_H

#include "epd2in13b.h"

#include "layer.h"
#include "text.h"

class Paragraph : public Layer {
  public:
    Paragraph(uint32_t width, uint32_t height, Font *font,
              TextAlign textAlign = AlignLeft, int32_t rotate = ROTATE_DEFAULT);

    Paragraph(uint32_t width, uint32_t height, CodePoint *codepoints,
              Font *font, TextAlign textAlign = AlignLeft,
              int32_t rotate = ROTATE_DEFAULT);
    virtual ~Paragraph();

    /**
     * @brief Set text.
     *
     * @param str Unicode series starts with 0xFEFF
     * @return int
     */
    int SetText(char *str);

    void SetTextAlign(TextAlign textAlign);

    void SetTextPadding(TextPadding textPadding);
    void SetTextPadding(int padding);
    void SetTextPadding(int paddingX, int paddingY);
    void SetTextPadding(int paddingLeft, int paddingTop, int paddingRight,
                        int paddingBottom);

    /**
     * @brief Render the characters
     *
     */
    void Render();

  private:
    Font *font;
    CodePoint *codepoints;
    TextTypeSetting *typeSetting;
    TextAlign textAlign;
    TextPadding textPadding;
    size_t charNum;
};

#endif