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

#ifndef DOM_NODE_H
#define DOM_NODE_H

#include <stddef.h>

namespace DOM {

class TreeNode {
  public:
    TreeNode();
    virtual ~TreeNode();

    TreeNode *GetFirstChild();
    TreeNode *GetLastChild();

    void PushChildren(TreeNode *node);
    TreeNode *PopChildren();

    void UnshiftChildren(TreeNode *node);
    TreeNode *ShiftChildren();

    TreeNode *GetChildren();
    size_t CountChildren();

    // TODO: traverse

  private:
    /**
     * @brief Previous node.
     *
     */
    TreeNode *prev;

    /**
     * @brief Next node.
     *
     */
    TreeNode *next;

    /**
     * @brief First child.
     *
     */
    TreeNode *firstChild;

    /**
     * @brief Last child.
     *
     */
    TreeNode *lastChild;

    /**
     * @brief Children nodes.
     *
     */
    TreeNode *children;
}; // Class TreeNode

}; // namespace DOM

#endif /* DOM_NODE_H */
