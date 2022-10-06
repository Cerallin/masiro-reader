#include <CppUTest/TestHarness.h>

#include "imagelayer.h"
#include <new>

ImageLayer *imageLayer;
unsigned char images[EPD_WIDTH * EPD_HEIGHT * 2];
unsigned char *front = images, *back = images + EPD_WIDTH * EPD_HEIGHT;

TEST_GROUP(TestImageLayer) {
    void setup() {
        imageLayer = new ImageLayer(EPD_WIDTH, EPD_HEIGHT, ROTATE_270);
    }
};

TEST(TestImageLayer, TestLoadFromImage) {
    BMPImage img(EPD_WIDTH, EPD_HEIGHT, front, back);
    CHECK_EQUAL(0, imageLayer->LoadFrom(&img));
    CHECK_EQUAL(front, imageLayer->GetNewImage());
    CHECK_EQUAL(back, imageLayer->GetOldImage());
}
