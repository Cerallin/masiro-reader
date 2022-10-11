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

#include "codepoint.h"

#include <assert.h>
#include <iconv.h>
#include <new>
#include <string.h>

CodePoint::CodePoint() { this->value = 0; }

CodePoint::CodePoint(uint16_t cp) : value(cp) {}

CodePoint::~CodePoint() {}

void CodePoint::SetValue(uint16_t cp) { this->value = cp; }

void CodePoint::SetValue(const CodePoint *cp) { this->value = cp->value; }

uint16_t CodePoint::GetValue() { return this->value; }

bool CodePoint::IsEmpty() const { return this->value == 0; }

const bool CodePoint::operator<(const CodePoint &cp) const {
    return value < cp.value;
}

const bool CodePoint::operator<=(const CodePoint &cp) const {
    return value <= cp.value;
}

const bool CodePoint::operator>(const CodePoint &cp) const {
    return value > cp.value;
}

const bool CodePoint::operator>=(const CodePoint &cp) const {
    return value >= cp.value;
}

const bool CodePoint::operator==(const CodePoint &cp) const {
    return value == cp.value;
}

const bool CodePoint::operator<(uint16_t cp) const { return value < cp; }

const bool CodePoint::operator<=(uint16_t cp) const { return value <= cp; }

const bool CodePoint::operator>(uint16_t cp) const { return value > cp; }

const bool CodePoint::operator>=(uint16_t cp) const { return value >= cp; }

const bool CodePoint::operator==(uint16_t cp) const { return value == cp; }

CodePoint *CodePoint::StrChr(const CodePoint *str, CodePoint c) {
    for (auto p_str = str; !p_str->IsEmpty(); p_str++) {
        if (*p_str == c)
            return (CodePoint *)p_str;
    }

    return nullptr;
}

CodePoint *CodePoint::Strpbrk(const CodePoint *str, const CodePoint *needle) {
    for (auto p_str = str; !p_str->IsEmpty(); p_str++) {
        for (auto n_ptr = needle; !n_ptr->IsEmpty(); n_ptr++) {
            if (*p_str == *n_ptr)
                return (CodePoint *)p_str;
        }
    }

    return nullptr;
}

bool CodePoint::IsLatin() const {
    if (value >= 0x1EFF)
        return false;

    if (value < 0x00FF)
        return true;
    if (value < 0x017F && value > 0x0100)
        return true;
    if (value < 0x024F && value > 0x0180)
        return true;
    if (value < 0x1EFF && value > 0x1E00)
        return true;

    return false;
}

CodePoint *CodePoint::FindNextBreak(const CodePoint *str,
                                    const CodePoint *needle) {
    for (auto p_str = str; !p_str->IsEmpty(); p_str++) {
        if (!p_str->IsLatin())
            return (CodePoint *)p_str;
        for (auto n_ptr = needle; !n_ptr->IsEmpty(); n_ptr++) {
            if (*p_str == *n_ptr)
                return (CodePoint *)p_str;
        }
    }

    return nullptr;
}

int CodePoint::StrToUnicode(char *str, size_t srcLen, CodePoint **unicodeStr,
                            size_t destLen) {
    size_t srcLeft = srcLen, unicodeLeft = destLen;

    char *buffer = (char *)*unicodeStr;
    char *inbuf = str, *outbuff = buffer;

    iconv_t cd = iconv_open("UTF-16", "UTF-8");
    if (cd == (iconv_t)-1) {
        return -1;
    }

    if (iconv(cd, &inbuf, &srcLeft, &outbuff, &unicodeLeft) == (size_t)-1) {
        perror("iconv");
        return -1;
    }
    iconv_close(cd);

    // TODO endianess check
    assert(*(uint16_t *)buffer == 0xFEFF);

    static_assert(sizeof(CodePoint) == sizeof(uint16_t),
                  "Size of CodePoint should be equal to uint16_t");
    static_assert(offsetof(CodePoint, value) == 0,
                  "Content of CodePoint should be equal to uint16_t");
    // 当CodePoint的二进制内容与uint16_t完全一致时可以这样转换
    *unicodeStr = (CodePoint *)buffer;

    return (CodePoint *)outbuff - (CodePoint *)buffer;
}

CodePoint *CodePoint::FindNextChar(const CodePoint *str, CodePoint c) {
    for (auto p_str = str; !p_str->IsEmpty(); p_str++) {
        if (*p_str == c)
            return (CodePoint *)p_str;
    }

    return nullptr;
}