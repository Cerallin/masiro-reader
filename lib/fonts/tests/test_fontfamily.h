#include <CppUTest/TestHarness.h>

#include "fontfamily.h"

extern const char font_file[];

FontFace *fontFace;
FontFamily *fontfamily;

TEST_GROUP(TestFontFamily) {
    void setup() {
        fontFace = new FontFace;
        CHECK_EQUAL(0, fontFace->LoadFont(font_file));

        fontfamily = new FontFamily(fontFace);
    }

    void teardown() {
        delete fontfamily;
        delete fontFace;
    }
};

TEST(TestFontFamily, TestGetFontFace) {
    auto ffList = fontfamily->GetFontFace();
    CHECK_EQUAL(1, ffList.size());
    CHECK_EQUAL(fontFace, ffList[0]);
}

TEST(TestFontFamily, TestGetFontFaceIndex) {
    CodePoint CHAR_H('H'), CHAR_UNDEF(0xFFFF);
    CHECK_EQUAL(0, fontfamily->GetFontFaceIndex(&CHAR_H));
    CHECK_EQUAL(-1, fontfamily->GetFontFaceIndex(&CHAR_UNDEF));
}
