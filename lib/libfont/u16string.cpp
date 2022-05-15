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

#include "u16string.h"

#include <assert.h>
#include <iconv.h>
#include <new>
#include <string.h>

namespace u16String {

size_t StrToUnicode(char *str, CodePoint **unicodeStr) {
    iconv_t cd = iconv_open("UTF-16", "UTF-8");
    if (cd == (iconv_t)-1) {
        return 0;
    }

    size_t srcLeft = strlen(str), unicodeLeft = srcLeft * 2 + 10;
    char *buffer = new (std::nothrow) char[unicodeLeft]();
    if (buffer == nullptr) {
        return 0;
    }

    char *inbuf = str, *outbuff = buffer;
    if (iconv(cd, &inbuf, &srcLeft, &outbuff, &unicodeLeft) == -1) {
        iconv_close(cd);
        return 0;
    }
    iconv_close(cd);

    assert(*(CodePoint *)buffer == 0xFEFF);
    *unicodeStr = (CodePoint *)buffer;

    return (CodePoint *)outbuff - (CodePoint *)buffer - 1;
}

CodePoint *FindNextChar(const CodePoint *str, CodePoint c) {
    for (auto p_str = str; *p_str; p_str++) {
        if (*p_str == c)
            return (CodePoint *)p_str;
    }

    return nullptr;
}

inline bool isLatin(CodePoint c) {
    if (c >= 0x1EFF)
        return false;

    if (c < 0x00FF)
        return true;
    if (c < 0x017F && c > 0x0100)
        return true;
    if (c < 0x024F && c > 0x0180)
        return true;
    if (c < 0x1EFF && c > 0x1E00)
        return true;

    return false;
}

CodePoint *FindNextBreak(const CodePoint *str, const CodePoint *needle) {
    for (auto p_str = str; *p_str; p_str++) {
        if (!isLatin(*p_str))
            return (CodePoint *)p_str;
        for (auto n_ptr = needle; *n_ptr; n_ptr++) {
            if (*p_str == *n_ptr)
                return (CodePoint *)p_str;
        }
    }

    return nullptr;
}

} // namespace u16String
