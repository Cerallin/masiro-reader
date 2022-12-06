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
#include "text.h"

#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

class TextLayer : public Layer {
  public:
    TextLayer(const Layer &layer);
    TextLayer(const TextLayer &layer);
    TextLayer(uint32_t width, uint32_t height,
              Graphic::Rotate rotate = Graphic::ROTATE_DEFAULT);

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

    TextLayer &SetTextAlign(Text::Align textAlign);

    TextLayer &SetLineHeight(float lineHeight);

    TextLayer &SetWritingMode(Text::WritingMode mode);

    TextLayer &SetPadding(Graphic::Padding textPadding);
    TextLayer &SetPadding(int padding);
    TextLayer &SetPadding(int paddingX, int paddingY);
    TextLayer &SetPadding(int paddingLeft, int paddingTop, int paddingRight,
                          int paddingBottom);

    /**
     * @brief Calculate glyphs positions.
     */
    TextLayer &TypeSetting();

    /**
     * @brief Draw glyphs.
     * Must be called after TextLayer::TypeSetting.
     */
    void Render();

  private:
    void drawGlyph(const Text::GlyphInfo *glyphInfo, Font *font,
                   const unsigned char *bitmap);

    float calcGlyphOffset(const CodePoint *start, int num);
    void getGlyphInfo(Text::GlyphInfo *glyph, const CodePoint *cp, float x,
                      float y);

    bool lineFeed(const CodePoint *cp, int32_t pos, int32_t maxLine);

    std::unique_ptr<CodePoint[]> codepoints;
    std::unique_ptr<Text::GlyphInfo[]> glyphInfo;

    Text::Align textAlign = Text::AlignStart;
    Font *font = nullptr;

    ssize_t charNum;

    Text::WritingMode mode = Text::Horizontal_TB;

    float lineHeightScale = 1.0f;
};

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GRAPHIC_TEXT_LAYER_H */
