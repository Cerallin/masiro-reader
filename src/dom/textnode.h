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

#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include "node.h"
#include "fontfamily.h"

/**
 * @brief Text node, always the leaf node
 *
 */
class TextNode : public Node {
  public:
    TextNode();
    virtual ~TextNode();

    void SetInnerText(CodePoint *, size_t len);

    /**
     * @brief Render content.
     *
     */
    void Render(void);

  private:
    /**
     * @brief Test to display.
     *
     */
    CodePoint *innerText;

    /**
     * @brief Length of text to display.
     *
     * @note In case innerText is too long.
     */
    size_t innerTextLength;

    FontFamily fontFamily;

    float fontSize;
};

#endif
