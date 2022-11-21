#include <CppUTest/TestHarness.h>

#include "config.h"

#include "imagelayer.h"

#include <new>
#include <unistd.h>

const char image_file[] = SRC_DIR "/assets/lain.bmp";

ImageLayer *imageLayer;

constexpr size_t EPD_BUFFER_SIZE = EPD_WIDTH * EPD_HEIGHT / 8;

unsigned char images[EPD_BUFFER_SIZE * 2];

constexpr unsigned char *front = images, *back = images + EPD_BUFFER_SIZE;

TEST_GROUP(TestImageLayer) {
    void setup() {
        imageLayer = new ImageLayer(EPD_WIDTH, EPD_HEIGHT, Graphic::ROTATE_270);
    }

    void teardown() {
        delete imageLayer;
    }
};

TEST(TestImageLayer, TESTBMPImageFileExistence) {
    CHECK_EQUAL(0, access(image_file, F_OK));
}

TEST(TestImageLayer, TESTLoadFromImage) {
    unsigned char tmp_buff[EPD_BUFFER_SIZE * 2];
    BMPImage img(EPD_WIDTH, EPD_HEIGHT, tmp_buff, tmp_buff + EPD_BUFFER_SIZE);
    CHECK_EQUAL(0, img.Load(image_file));

    imageLayer->SetImages(images);

    CHECK_EQUAL(0, imageLayer->LoadFrom(&img));
    CHECK_EQUAL(front, imageLayer->GetNewImage());
    CHECK_EQUAL(back, imageLayer->GetOldImage());

    MEMCMP_EQUAL(front, imageLayer->GetNewImage(), TEST_BUFFER_SIZE);
    MEMCMP_EQUAL(back, imageLayer->GetOldImage(), TEST_BUFFER_SIZE);
}

TEST(TestImageLayer, TESTLoadFromImageDirectly) {
    imageLayer->SetImages(images);
    CHECK_EQUAL(0, imageLayer->LoadFrom(image_file));
}
