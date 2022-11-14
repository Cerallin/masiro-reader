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

#include <new>
#include <stdio.h>

int FontFace::LoadFont(const char *fontFilePath) {
    FILE *fontFile = fopen(fontFilePath, "rb");
    if (!fontFile) {
        perror("LoadFont");
        setState(ERROR);
        return -1;
    }

    fseek(fontFile, 0, SEEK_END);
    auto size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);

    fontBuffer = std::make_unique<unsigned char[]>(size);

    debug("Loading font from %s\n", fontBuffer.get());

    fread(fontBuffer.get(), size, 1, fontFile);
    fclose(fontFile);

    if (!stbtt_InitFont(&fontInfo, fontBuffer.get(), 0)) {
        setState(ERROR);
        return -1;
    }

    setState(LOADED);
    return 0;
}

const stbtt_fontinfo *FontFace::GetFontInfo() const { return &fontInfo; }

int FontFace::FindGlyphIndex(int codepoint) const {
    return stbtt_FindGlyphIndex(&fontInfo, codepoint);
}

FontFaceState FontFace::GetState() const { return this->state; }

void FontFace::setState(FontFaceState newState) { this->state = newState; }