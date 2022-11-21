#include <CppUTest/TestHarness.h>

#include "config.h"

#include "textlayer.h"

const char font_file[] = SRC_DIR "/assets/LXGWWenKaiScreen.ttf";
char testStr[] = "hello world\n你好，世界。";

extern unsigned char layer_images[];
extern unsigned char *layer_front, *layer_back;

TextLayer *textLayer;

FontFace *XLWenKai;

FontFamily *fontFamily;

Font *font;

TEST_GROUP(TestTextLayer){void setup(){XLWenKai = new FontFace();
if (XLWenKai->LoadFont(font_file) == -1) {
    fprintf(stderr, "Font file not found.\n");
}

fontFamily = new FontFamily(XLWenKai);
font = new Font(fontFamily);

textLayer = new TextLayer(EPD_WIDTH, EPD_HEIGHT, font);
textLayer->SetFrontImage(layer_front);
textLayer->SetBackImage(layer_back);
}

void teardown() {
    delete textLayer;
    delete fontFamily;
    delete XLWenKai;
    delete font;
}
}
;

TEST(TestTextLayer, TestSetText) {
    char *p = testStr;
    CHECK_EQUAL(0, textLayer->SetText(p));
}

TEST(TestTextLayer, TestRender) {
    char *p = testStr;
    textLayer->SetText(p);
    textLayer->Render();
    // Nothing but checks mem leak
}
