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
    fontFace.LoadFont(font_file);
}

TEST(TestFontFace, TestLoadFontFail) {
    FontFace fontFace;
    CHECK_THROWS(FontLoadException, fontFace.LoadFont("/nonexist.ttf"));
}
