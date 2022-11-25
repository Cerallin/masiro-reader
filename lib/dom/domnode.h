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

#ifndef DOM_DOM_H
#define DOM_DOM_H

#include <stdint.h>

#include "domtag.h"
#include "style.h"
#include "treenode.h"

namespace DOM {

class DOMNode : public TreeNode {
  public:
    DOMNode(int32_t width, int32_t height);
    virtual ~DOMNode() = default;

    void SetWidth(int32_t width);

    void SetHeight(int32_t height);

    void SetTag(DOMTag tagValue);

    /**
     * @brief Styles.
     *
     */
    Style::Styles styles;

  private:
    /**
     * @brief HTML tag.
     *
     */
    DOMTag tag;

    /**
     * @brief Width of graph area.
     *
     */
    int32_t width;

    /**
     * @brief Height of graph area.
     *
     */
    int32_t height;

    /**
     * @brief Real width of content.
     *
     */
    int32_t realWidth;

    /**
     * @brief Real height of content.
     *
     */
    int32_t realHeight;
}; // Class DOM

}; // namespace DOM

#endif