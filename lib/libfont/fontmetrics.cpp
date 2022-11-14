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

float FontMetrics::GetLineHeight() const {
    return scale * (ascent - descent + lineGap);
}
