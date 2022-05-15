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

#ifndef FONT_U16STRING_H
#define FONT_U16STRING_H

#include <stdint.h>
#include <stdlib.h>

typedef uint16_t CodePoint;

namespace u16String {
/**
 * @brief Convert string to unicode.
 *
 * @param str string to convert
 * @param unicodeStr storage result
 * @return string length
 */
size_t StrToUnicode(char *str, CodePoint **unicodeStr);

CodePoint *FindNextBreak(const CodePoint *str, const CodePoint *needle);

CodePoint *FindNextChar(const CodePoint *str, CodePoint c);
}; // namespace u16String

#endif
