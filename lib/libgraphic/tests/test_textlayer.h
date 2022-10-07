#include <CppUTest/TestHarness.h>

#include "textlayer.h"

const char font_file[] = "../../assets/LXGWWenKaiScreen.ttf";

TextLayer *textLayer;

FontFace XLWenKai;

TEST_GROUP(TestTextLayer) {
    void setup() {
        XLWenKai.LoadFont(font_file);
        Font font(&XLWenKai);

        textLayer = new TextLayer(EPD_WIDTH, EPD_HEIGHT, &font);
    }

    void teardown() {
        delete textLayer;
    }
};

TEST(TestTextLayer, TestSetText) {
    char str[] = "hello world";
    char *p = str;
    CHECK_EQUAL(0, textLayer->SetText(p));
}
