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

#include "style.h"

namespace Graphic {

#define ScanLine(needle, lineStart)                                            \
    for (needle = lineStart; needle->y == lineStart->y; needle++, cp++)

#define BackSpace(needle, cp)                                                  \
    {                                                                          \
        needle--;                                                              \
        cp--;                                                                  \
    }

void GlyphInfo::AdjustAlign(const CodePoint *codepoints,
                                  GlyphInfo *glyphInfos, ssize_t len,
                                  TextAlign align, int lineWidth, Font *font) {
    int stringWidth;
    float offset;
    const CodePoint *cp = codepoints;
    GlyphInfo *needle = glyphInfos, *lineStart = glyphInfos;

    if (align == AlignLeft) // default
        return;

    do {
        ScanLine(needle, lineStart) {} // Search for EOL
        BackSpace(needle, cp);

        stringWidth = font->Scale(cp, needle->x) + needle->width;
        if (align == AlignRight) {
            offset = font->Unscale(cp, lineWidth - stringWidth);
            ScanLine(needle, lineStart) { needle->x += offset; }
        } else if (align == AlignCenter) {
            offset = font->Unscale(cp, lineWidth - stringWidth) / 2;
            ScanLine(needle, lineStart) { needle->x += offset; }
        }
        lineStart = needle;
    } while (needle - glyphInfos < len);
}

} // namespace Graphic
