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

#ifndef CODEPOINT_H
#define CODEPOINT_H

#include <cstdint>
#include <cstdio>

class CodePoint {
  public:
    CodePoint();
    CodePoint(uint16_t cp);
    ~CodePoint();

    void SetValue(uint16_t cp);
    void SetValue(const CodePoint *cp);

    uint16_t GetValue() const;

    bool IsEmpty() const;
    bool IsLatin() const;

    const bool operator<(const CodePoint &cp) const;
    const bool operator<=(const CodePoint &cp) const;
    const bool operator>(const CodePoint &cp) const;
    const bool operator>=(const CodePoint &cp) const;
    const bool operator==(const CodePoint &cp) const;

    const bool operator<(uint16_t cp) const;
    const bool operator<=(uint16_t cp) const;
    const bool operator>(uint16_t cp) const;
    const bool operator>=(uint16_t cp) const;
    const bool operator==(uint16_t cp) const;

    static CodePoint *FindNextBreak(const CodePoint *str,
                                    const CodePoint *needle);

    static CodePoint *StrChr(const CodePoint *str, CodePoint c);

    static CodePoint *Strpbrk(const CodePoint *str, const CodePoint *needle);

    static CodePoint *FindNextChar(const CodePoint *str, CodePoint c);

    /**
     * @brief Convert string to unicode, returns -1 on error and 0 on success.
     *
     * @param str string to convert
     * @param unicodeStr storage result
     * @return int
     */
    static int StrToUnicode(char *str, size_t srcLen, CodePoint **unicodeStr,
                            size_t destLen);

  private:
    uint16_t value;
};

#endif
