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

#ifndef FONT_FONT_FAMILY_H
#define FONT_FONT_FAMILY_H

#include "codepoint.h"
#include "fontface.h"

#include <vector>

class FontFamily {
  public:
    FontFamily(FontFace *defaultFontFace);
    ~FontFamily() = default;

    /**
     * @brief Get all font faces.
     *
     * @return font face vector
     */
    std::vector<const FontFace *> GetFontFace() const;

    /**
     * @brief Add a font face
     *
     * @param fontFace to insert
     */
    void AddFontFace(const FontFace *fontFace);

    /**
     * @brief Get specific font face
     *
     * @return nullptr if not found
     */
    const FontFace *GetFontFace(const CodePoint *codepoint,
                                bool italic = false) const;

    /**
     * @brief Get index of the vector
     *
     * @returns -1 if not found.
    */
    ssize_t GetFontFaceIndex(const CodePoint *codepoint, bool italic = false);

  private:
    std::vector<const FontFace *> ffList;
};

#endif /* FONT_FONT_FAMILY_H */
