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

#include "debug.h"

#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC // make stb private
#include "fontface.h"

#include <cstdio>
#include <exception>
#include <new>
#include <system_error>

char LoadFontFailedException::error_msg[80];

void FontFace::LoadFont(const char *filename) {
    FILE *fontFile = fopen(filename, "rb");
    if (!fontFile) {
        std::system_error e(errno, std::generic_category());
        handle_exception(e);
        setState(ERROR);
        throw LoadFontFailedException(filename);
    }

    fseek(fontFile, 0, SEEK_END);
    auto size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);

    fontBuffer = std::make_unique<unsigned char[]>(size);
    fread(fontBuffer.get(), size, 1, fontFile);
    fclose(fontFile);

    if (!stbtt_InitFont(&fontInfo, fontBuffer.get(), 0)) {
        std::runtime_error e("init failed.");
        handle_exception(e);
        throw LoadFontFailedException(filename);
    }

    setState(LOADED);
}

const stbtt_fontinfo *FontFace::GetFontInfo() const { return &fontInfo; }

int FontFace::FindGlyphIndex(const CodePoint *codepoint) const {
    return stbtt_FindGlyphIndex(&fontInfo, codepoint->GetValue());
}

FontFaceState FontFace::GetState() const { return this->state; }

void FontFace::setState(FontFaceState newState) { this->state = newState; }
