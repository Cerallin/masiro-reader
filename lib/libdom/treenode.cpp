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

#include "treenode.h"

#define IterateChildren(n)                                                     \
    for (TreeNode *n = firstChild; n != nullptr; n = n->next)

using namespace DOM;

TreeNode::TreeNode() {
    prev = next = nullptr;
    firstChild = lastChild = children = nullptr;
}

TreeNode::~TreeNode() {}

TreeNode *TreeNode::GetFirstChild() { return firstChild; }

TreeNode *TreeNode::GetLastChild() { return lastChild; }

void TreeNode::PushChildren(TreeNode *node) {
    if (lastChild != nullptr) {
        lastChild->next = node;
    } else {
        firstChild = node;
    }

    node->prev = lastChild;
    node->next = nullptr;

    lastChild = node;
}

TreeNode *TreeNode::PopChildren() {
    TreeNode *res = lastChild;

    if (res == nullptr)
        return res;

    lastChild = lastChild->prev;
    if (lastChild != nullptr) {
        lastChild->next = nullptr;
    }

    return res;
}

void TreeNode::UnshiftChildren(TreeNode *node) {
    node->next = firstChild;
    if (firstChild != nullptr) {
        firstChild->prev = node;
    } else {
        lastChild = node;
    }
    firstChild = node;
}

TreeNode *TreeNode::ShiftChildren() {
    TreeNode *res = firstChild;

    if (res == nullptr)
        return nullptr;

    firstChild = res->next;
    if (firstChild != nullptr) {
        firstChild->prev = nullptr;
    } else {
        lastChild = nullptr;
    }

    return res;
}

TreeNode *TreeNode::GetChildren() { return firstChild; }

size_t TreeNode::CountChildren() {
    size_t count = 0;
    IterateChildren(n) { count++; }

    return count;
}

void TreeNode::SetParent(TreeNode *node) { parent = node; }

TreeNode *TreeNode::GetParent() { return parent; }
