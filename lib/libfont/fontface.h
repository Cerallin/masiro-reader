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

#ifndef FONT_FONT_FACE_H
#define FONT_FONT_FACE_H

#include "stb_truetype.h"
#include <memory>

enum FontFaceState {
    TO_LOAD,
    LOADED,
    ERROR,
}; // namespace Font

class FontFace {
  public:
    FontFace() = default;
    ~FontFace() = default;

    int LoadFont(const char *fontFilePath);
    const stbtt_fontinfo *GetFontInfo();

    FontFaceState GetState();

  private:
    std::unique_ptr<unsigned char[]> fontBuffer = nullptr;
    stbtt_fontinfo fontInfo;

    FontFaceState state = TO_LOAD;

    void setState(FontFaceState newState);
};

#endif /* FONT_H */
