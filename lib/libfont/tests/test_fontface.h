#include <CppUTest/TestHarness.h>

#include "config.h"
#include "fontface.h"
#include <unistd.h>

const char font_file[] = SRC_DIR "/assets/LXGWWenKaiScreen.ttf";

TEST_GROUP(TestFontFace){};

TEST(TestFontFace, TestFontFileExistence) {
    CHECK_EQUAL(0, access(font_file, F_OK));
}

TEST(TestFontFace, TestLoadFont) {
    FontFace fontFace;
    int res = fontFace.LoadFont(font_file);
    CHECK_EQUAL(0, res);
}

TEST(TestFontFace, TestLoadFontFail) {
    FontFace fontFace;
    int res = fontFace.LoadFont("/nonexist.ttf");
    CHECK_EQUAL(-1, res);
}
