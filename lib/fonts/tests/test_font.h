#include <CppUTest/TestHarness.h>

#include "font.h"
#include <unistd.h>

extern const char font_file[];

const CodePoint CHAR_H('H'), CHAR_E('E');

extern FontFace *face;
Font *font;

TEST_GROUP(TestFont) {
    void setup() {
        face = new FontFace;
        face->LoadFont(font_file);
        font = new Font(face, 32.0f);
    }

    void teardown() {
        delete font;
        delete face;
    }
};

TEST(TestFont, TestGetCodepointHMetrics) {
    int advanceWidth = -1, leftSideBearing = -1;
    font->GetCodepointHMetrics(&CHAR_H, &advanceWidth, &leftSideBearing);
    CHECK_FALSE(-1 == advanceWidth);
    CHECK_FALSE(-1 == leftSideBearing);
}

TEST(TestFont, TestGetCodepointBitmapBox) {
    int ix0, iy0, ix1, iy1;
    ix0 = iy0 = ix1 = iy1 = 0xFF;
    font->GetCodepointBitmapBox(&CHAR_H, &ix0, &iy0, &ix1, &iy1);
    CHECK_FALSE(0xFF == ix0);
    CHECK_FALSE(0xFF == iy0);
    CHECK_FALSE(0xFF == ix1);
    CHECK_FALSE(0xFF == iy1);
}

TEST(TestFont, TestGetCodepointBitmap) {
    int width, height;
    width = height = 0xFF;
    delete[] font->GetCodepointBitmap(&CHAR_H, &width, &height, 0, 0);
    CHECK_FALSE(0xFF == width);
    CHECK_FALSE(0xFF == height);
}

TEST(TestFont, TestGetCodepointKernAdvance) {
    int kern = 0xFF;
    kern = font->GetCodepointKernAdvance(&CHAR_H, &CHAR_E);
    CHECK_FALSE(0xFF == kern);
}
