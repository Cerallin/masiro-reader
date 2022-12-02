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

#include "fontmetrics.h"

FontMetrics::FontMetrics(float scale) : scale(scale) {}

FontMetrics::FontMetrics(float scale, int ascent, int descent, int lineGap)
    : scale(scale), ascent(ascent), descent(descent), lineGap(lineGap) {}

void FontMetrics::FontVMetrics(const stbtt_fontinfo *info) {
    /**
     * 获取垂直方向上的度量
     * ascent：字体从基线到顶部的高度；
     * descent：基线到底部的高度，通常为负值；
     * lineGap：两个字体之间的间距；
     */
    stbtt_GetFontVMetrics(info, &ascent, &descent, &lineGap);
}

void FontMetrics::FontHMetrics(const stbtt_fontinfo *info) {
    /**
     * 获取垂直方向上的度量
     * ascent：字体从基线到顶部的高度；
     * descent：基线到底部的高度，通常为负值；
     * lineGap：两个字体之间的间距；
     */
    stbtt_GetFontHMetrics(info, &vertTypoAscender, &vertTypoDescender, &vertTypoLineGap);
}

float FontMetrics::GetLineHeight() const {
    return scale * (ascent - descent + lineGap);
}

float FontMetrics::GetLineWidth() const {
    return scale * (vertTypoAscender - vertTypoDescender + vertTypoLineGap);
}
