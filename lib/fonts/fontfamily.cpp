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

#include "fontfamily.h"

constexpr size_t FF_LIST_SIZE = 4;

FontFamily::FontFamily(FontFace *defaultFontFace) {
    ffList.reserve(FF_LIST_SIZE);
    ffList.push_back(defaultFontFace);
}

std::vector<const FontFace *> FontFamily::GetFontFace() const { return ffList; }

void FontFamily::AddFontFace(const FontFace *fontFace) {
    ffList.push_back(fontFace);
}

const FontFace *FontFamily::GetFontFace(const CodePoint *codepoint,
                                        bool italic) const {
    for (const FontFace *ff : ffList) {
        if (ff->FindGlyphIndex(codepoint)) {
            return ff;
        }
    }

    return nullptr;
}

ssize_t FontFamily::GetFontFaceIndex(const CodePoint *codepoint, bool italic) {
    for (size_t i = 0; i < ffList.size(); i++) {
        const auto ff = ffList[i];
        if (ff->FindGlyphIndex(codepoint)) {
            return i;
        }
    }

    return -1;
}
