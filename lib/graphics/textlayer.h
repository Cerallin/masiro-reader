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

#include "fonts/codepoint.h"
#include "layer.h"
#include "style.h"

#ifdef __cplusplus
extern "C" {
#endif

class TextLayer : public Layer {
  public:
    TextLayer(const Layer &layer);
    TextLayer(const TextLayer &layer);
    TextLayer(uint32_t width, uint32_t height, int32_t rotate = ROTATE_DEFAULT);

    ~TextLayer() = default;

#undef _Class
#define _Class TextLayer
#include "traits/layersetters.h"

    /**
     * @brief Set text.
     *
     * @param str Unicode series starts with 0xFEFF
     * @return TextLayer &
     */
    TextLayer &SetText(char *str);

    TextLayer &SetFont(Font *font);

    TextLayer &SetTextAlign(Graphic::TextAlign textAlign);

    TextLayer &SetTextPadding(Graphic::TextPadding textPadding);
    TextLayer &SetTextPadding(int padding);
    TextLayer &SetTextPadding(int paddingX, int paddingY);
    TextLayer &SetTextPadding(int paddingLeft, int paddingTop, int paddingRight,
                              int paddingBottom);

    TextLayer &CalcTypeSetting();

    /**
     * @brief Render the characters
     *
     */
    void Render();

  private:
    void drawGlyph(const CodePoint *cp,
                   const Graphic::TextTypeSetting *typeSetting, Font *font,
                   const unsigned char *bitmap);

    void calcCodePointSize(const CodePoint *codepoint, int *ix0, int *iy0,
                           int *ix1, int *iy1, int *advanceWidth);

    int calcLineWidth(const CodePoint *start, const CodePoint *nextBreak);

    void calcCodePointTypeSetting(const CodePoint *c,
                                  Graphic::TextTypeSetting *ts, int x, int y,
                                  int iy0);

    std::unique_ptr<CodePoint[]> codepoints = nullptr;
    Font *font = nullptr;

    std::unique_ptr<Graphic::TextTypeSetting[]> typeSetting = nullptr;
    Graphic::TextAlign textAlign = Graphic::AlignLeft;
    Graphic::TextPadding textPadding = {0, 0, 0, 0};

    ssize_t charNum;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GRAPHIC_TEXT_LAYER_H */