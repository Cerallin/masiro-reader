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

#include "font.h"

class FontFamily {
  public:
    FontFamily(FontFace *defaultFontFace);
    FontFamily(FontFace *defaultFontFace, FontFace *italicFontFace);
    FontFamily(FontFace *defaultFontFace, FontFace *fallbackFontFace,
               FontFace *italicFontFace, FontFace *italicFallbackFontFace);

    virtual ~FontFamily() = default;

  private:
    FontFace *regularFontFace;
    FontFace *fallbackFontFace;
    FontFace *italicFontFace;
    FontFace *italicFallbackFontFace;
};

#endif /* FONT_FONT_FAMILY_H */
