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

FontFamily::FontFamily(FontFace *defaultFontFace) {
    regularFontFace = defaultFontFace;
    fallbackFontFace = nullptr;
    italicFontFace = nullptr;
    italicFallbackFontFace = nullptr;
}

FontFamily::FontFamily(FontFace *defaultFontFace, FontFace *italicFontFace) {
    regularFontFace = defaultFontFace;
    fallbackFontFace = nullptr;
    italicFontFace = italicFontFace;
    italicFallbackFontFace = nullptr;
}

FontFamily::FontFamily(FontFace *defaultFontFace, FontFace *fallbackFontFace,
                       FontFace *italicFontFace, FontFace *italicFallback) {
    regularFontFace = defaultFontFace;
    fallbackFontFace = fallbackFontFace;
    italicFontFace = italicFontFace;
    italicFallbackFontFace = italicFallback;
}
