#include <CppUTest/TestHarness.h>

#include "font.h"
#include <unistd.h>

extern const char font_file[];

FontFace *fontFace;
Font *font;

TEST_GROUP(TestFont) {
    void setup() {
        fontFace = new FontFace;
        CHECK_EQUAL(0, fontFace->LoadFont(font_file));
        font = new Font(fontFace, 32.0f);
    }

    void teardown() {
        delete fontFace;
        delete font;
    }
};

TEST(TestFont, TestGetCodepointHMetrics) {
    int advanceWidth = -1, leftSideBearing = -1;
    font->GetCodepointHMetrics('H', &advanceWidth, &leftSideBearing);
    CHECK_FALSE(-1 == advanceWidth);
    CHECK_FALSE(-1 == leftSideBearing);
}

TEST(TestFont, TestGetCodepointBitmapBox) {
    int ix0, iy0, ix1, iy1;
    ix0 = iy0 = ix1 = iy1 = 0xFF;
    font->GetCodepointBitmapBox('H', &ix0, &iy0, &ix1, &iy1);
    CHECK_FALSE(0xFF == ix0);
    CHECK_FALSE(0xFF == iy0);
    CHECK_FALSE(0xFF == ix1);
    CHECK_FALSE(0xFF == iy1);
}

TEST(TestFont, TestGetCodepointBitmap) {
    int width, height;
    width = height = 0xFF;
    auto bitmap = font->GetCodepointBitmap('H', &width, &height, 0, 0);
    CHECK_FALSE(0xFF == width);
    CHECK_FALSE(0xFF == height);
    font->FreeBitmap(bitmap);
}

TEST(TestFont, TestGetCodepointKernAdvance) {
    int kern = 0xFF;
    kern = font->GetCodepointKernAdvance('H', 'E');
    CHECK_FALSE(0xFF == kern);
}

TEST(TestFont, TestGetFontFace) { CHECK_EQUAL(fontFace, font->GetFontFace()); }
