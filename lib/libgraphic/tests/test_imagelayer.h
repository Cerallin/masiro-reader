#include <CppUTest/TestHarness.h>

#include "config.h"
#include "imagelayer.h"
#include <new>
#include <unistd.h>

#define TEST_BUFFER_SIZE (EPD_WIDTH * EPD_HEIGHT / 8)

const char image_file[] = SRC_DIR "/assets/lain.bmp";

ImageLayer *imageLayer;

unsigned char images[TEST_BUFFER_SIZE * 2];

unsigned char *front = images, *back = images + TEST_BUFFER_SIZE;

unsigned char layer_images[TEST_BUFFER_SIZE * 2];

unsigned char *layer_front = layer_images,
              *layer_back = layer_images + TEST_BUFFER_SIZE;

TEST_GROUP(TestImageLayer) {
    void setup() {
        imageLayer = new ImageLayer(EPD_WIDTH, EPD_HEIGHT, ROTATE_270);
    }

    void teardown() {
        delete imageLayer;
    }
};

TEST(TestImageLayer, TESTSetImages) {
    imageLayer->SetImages(layer_images);

    CHECK_EQUAL(layer_front, imageLayer->GetNewImage());
    CHECK_EQUAL(layer_back, imageLayer->GetOldImage());
}

TEST(TestImageLayer, TestBMPImageFileExistence) {
    CHECK_EQUAL(0, access(image_file, F_OK));
}

TEST(TestImageLayer, TestLoadFromImage) {
    BMPImage img(EPD_WIDTH, EPD_HEIGHT, front, back);
    CHECK_EQUAL(0, img.Load(image_file));

    imageLayer->SetFrontImage(layer_front);

    imageLayer->SetBackImage(layer_back);

    CHECK_EQUAL(0, imageLayer->LoadFrom(&img));
    CHECK_EQUAL(layer_front, imageLayer->GetNewImage());
    CHECK_EQUAL(layer_back, imageLayer->GetOldImage());

    MEMCMP_EQUAL(layer_front, imageLayer->GetNewImage(), TEST_BUFFER_SIZE);
    MEMCMP_EQUAL(layer_back, imageLayer->GetOldImage(), TEST_BUFFER_SIZE);
}
