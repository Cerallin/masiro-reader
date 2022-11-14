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