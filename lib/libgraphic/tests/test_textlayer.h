#include <CppUTest/TestHarness.h>

#include "config.h"
#include "textlayer.h"

const char font_file[] = SRC_DIR "/assets/LXGWWenKaiScreen.ttf";
char testStr[] = "hello world\n";

extern unsigned char images[];
extern unsigned char *front, *back;

TextLayer *textLayer;

FontFace *XLWenKai;

Font *font;

TEST_GROUP(TestTextLayer) {
    void setup() {
        XLWenKai = new FontFace();
        if (XLWenKai->LoadFont(font_file) == -1) {
            fprintf(stderr, "Font file not found.\n");
        }
        font = new Font(XLWenKai);

        textLayer = new TextLayer(EPD_WIDTH, EPD_HEIGHT, font);
        textLayer->SetFrontImage(images);
        textLayer->SetBackImage(images + TEST_BUFFER_SIZE);
    }

    void teardown() {
        delete textLayer;
        delete XLWenKai;
        delete font;
    }
};

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
