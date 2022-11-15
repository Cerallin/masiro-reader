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

#ifndef FONT_FONT_METRICS_H
#define FONT_FONT_METRICS_H

#include "fontface.h"

class FontMetrics {
  public:
    FontMetrics(float scale);
    FontMetrics(float scale, int ascent, int descent, int lineGap);

    void FontVMetrics(const stbtt_fontinfo *info);

    float GetLineHeight() const;

    float scale;
    int ascent = 0;
    int descent = 0;
    int lineGap = 0;
};

#endif /* FONT_FONT_METRICS_H */