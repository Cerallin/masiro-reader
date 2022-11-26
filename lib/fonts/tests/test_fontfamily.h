#include <CppUTest/TestHarness.h>

#include "fontfamily.h"

extern const char font_file[];

FontFace *face;
FontFamily *family;

TEST_GROUP(TestFontFamily) {
    void setup() {
        face = new FontFace;
        face->LoadFont(font_file);

        family = new FontFamily(face);
    }

    void teardown() {
        delete family;
        delete face;
    }
};

TEST(TestFontFamily, TestGetFontFace) {
    auto ffList = family->GetFontFace();
    CHECK_EQUAL(1, ffList.size());
    CHECK_EQUAL(face, ffList[0]);
}

TEST(TestFontFamily, TestGetFontFaceIndex) {
    CodePoint CHAR_H('H'), CHAR_UNDEF(0xFFFF);
    CHECK_EQUAL(0, family->GetFontFaceIndex(&CHAR_H));
    CHECK_EQUAL(-1, family->GetFontFaceIndex(&CHAR_UNDEF));
}
