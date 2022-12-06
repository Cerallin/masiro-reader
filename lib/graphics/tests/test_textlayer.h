#include <CppUTest/TestHarness.h>

const char font_file[] = SRC_DIR "/assets/LXGWWenKaiScreen.ttf";
char testStr[] = "hello world\n你好，世界。";

extern unsigned char layer_images[];
extern unsigned char *layer_front, *layer_back;

TextLayer *textLayer;

FontFace *XLWenKai;

Font *font;

TEST_GROUP(TestTextLayer){
    void setup() {
        XLWenKai = new FontFace();
        XLWenKai->LoadFont(font_file);

        font = new Font(XLWenKai);

        textLayer = new TextLayer(EPD_WIDTH, EPD_HEIGHT);
        textLayer->SetFont(font);
        textLayer->SetFrontImage(layer_front);
        textLayer->SetBackImage(layer_back);
    }

    void teardown() {
        delete textLayer;
        delete font;
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
