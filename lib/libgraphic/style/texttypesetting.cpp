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
    for (needle = lineStart; needle->y == lineStart->y; needle++)

void TextTypeSetting::AdjustAlign(TextTypeSetting *typeSettings, size_t len,
                                  TextAlign align, int lineWidth, Font *font) {
    int stringWidth;
    float offset;
    TextTypeSetting *needle = typeSettings, *lineStart = typeSettings;

    if (align == AlignLeft) // default
        return;

    do {
        ScanLine(needle, lineStart) {} // Search for EOL
        // If this line is set here, it would be interesting.
        // lineStart = needle;
        needle--;

        stringWidth = font->Scale(needle->x) + needle->width;
        if (align == AlignRight) {
            offset = font->Unscale(lineWidth - stringWidth);
            ScanLine(needle, lineStart) { needle->x += offset; }
        } else if (align == AlignCenter) {
            offset = font->Unscale(lineWidth - stringWidth);
            ScanLine(needle, lineStart) { needle->x += offset; }
        }
        lineStart = needle;
    } while (needle - typeSettings < len);
}

} // namespace Graphic