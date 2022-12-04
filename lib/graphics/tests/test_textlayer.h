#include <CppUTest/TestHarness.h>

#include "config.h"

#include "textlayer.h"

const char font_file[] = SRC_DIR "/assets/LXGWWenKaiScreen.ttf";
char testStr[] = "hello world\n你好，世界。";

extern unsigned char layer_images[];
extern unsigned char *layer_front, *layer_back;

TextLayer *textLayer;

FontFace *XLWenKai;

TEST_GROUP(TestTextLayer){
    void setup() {
        XLWenKai = new FontFace();
        XLWenKai->LoadFont(font_file);

        textLayer = new TextLayer(EPD_WIDTH, EPD_HEIGHT);
        textLayer->SetFont(new Font(XLWenKai));
        textLayer->SetFrontImage(layer_front);
        textLayer->SetBackImage(layer_back);
    }

    void teardown() {
        delete textLayer;
        delete XLWenKai;
    }
};

TEST(TestTextLayer, TestSetText) {
    char *p = testStr;
    textLayer->SetText(p);
}

TEST(TestTextLayer, TestRender) {
    char *p = testStr;
    textLayer->SetText(p);
    textLayer->TypeSetting();
    textLayer->Render();
    // Nothing but checks mem leak
}
