#include <CppUTest/TestHarness.h>

#include "config.h"
#include "layer.h"
#include <new>
#include <unistd.h>
#include <string.h>

#define TEST_WIDTH  16
#define TEST_HEIGHT 8

#define TEST_BUFFER_SIZE (TEST_WIDTH / 8 * TEST_HEIGHT)

Layer *layer;

unsigned char layer_images[TEST_BUFFER_SIZE * 2];

unsigned char *layer_front = layer_images,
              *layer_back = layer_images + TEST_BUFFER_SIZE;

TEST_GROUP(TestLayer) {
    void setup() {
        layer = new Layer(TEST_WIDTH, TEST_HEIGHT, ROTATE_270);
        layer->SetFrontImage(layer_front);
        layer->SetBackImage(layer_back);
    }

    void teardown() {
        delete layer;
    }
};

TEST(TestLayer, TestInit) {
    CHECK_EQUAL(false, layer->GetInvertColor());
    memset((void *)layer_back, 0xFF, TEST_BUFFER_SIZE);
    CHECK_FALSE(layer->GetInvertColor());
    layer->Init();
    MEMCMP_EQUAL(layer_back, layer_front, TEST_BUFFER_SIZE);
}

TEST(TestLayer, TestClear) {
    memset((void *)layer_back, 0xFF, TEST_BUFFER_SIZE / 2);
    memset((void *)(layer_back + TEST_BUFFER_SIZE / 2), 0x00, TEST_BUFFER_SIZE / 2);
    layer->Clear(Color::BB);
    MEMCMP_EQUAL(layer_back, layer_front, TEST_BUFFER_SIZE);
}

TEST(TestLayer, TestGetWidth) {
    CHECK_EQUAL(TEST_WIDTH, layer->GetWidth());
}

TEST(TestLayer, TestGetRelativeWidth) {
    layer->SetRotate(ROTATE_90);
    CHECK_EQUAL(TEST_HEIGHT, layer->GetRelativeWidth());
}

TEST(TestLayer, TestSetRelativeHeight) {
    layer->SetRelativeHeight(666);
    CHECK_EQUAL(666, layer->GetRelativeHeight());
    CHECK_EQUAL(666, layer->GetWidth());

    CHECK_EQUAL(TEST_HEIGHT, layer->GetHeight());
    CHECK_EQUAL(TEST_HEIGHT, layer->GetRelativeWidth());
}

TEST(TestLayer, TestSetRotate) {
    layer->SetRotate(ROTATE_270);
    CHECK_EQUAL(ROTATE_270, layer->GetRotate());
}


TEST(TestLayer, TestInvertColor) {
    layer->SetInvertColor(true);
    CHECK_EQUAL(true, layer->GetInvertColor());
}

TEST(TestLayer, TestGetNewImage) {
    CHECK_EQUAL(layer_front, layer->GetNewImage());
}

TEST(TestLayer, TestGetOldImage) {
    CHECK_EQUAL(layer_back, layer->GetOldImage());
}

TEST(TestLayer, TestDrawAbsolutePixel) {
    layer->Init();
    layer->DrawAbsolutePixel(3, 0, Color::BB);
    BITS_EQUAL(0xEF, layer_front[0], 0xFF);
}

TEST(TestLayer, TestDrawPixel) {
    layer->Init();
    layer->SetRotate(ROTATE_270);
    layer->DrawPixel(TEST_HEIGHT, 3, Color::BB);
    BITS_EQUAL(0xEF, layer_front[0], 0xFF);
}


TEST(TestLayer, TESTSetImagesSeparately) {
    layer->SetFrontImage(layer_front);
    layer->SetBackImage(layer_back);

    CHECK_EQUAL(layer_front, layer->GetNewImage());
    CHECK_EQUAL(layer_back, layer->GetOldImage());
}

TEST(TestLayer, TESTSetImages) {
    layer->SetImages(layer_images);

    CHECK_EQUAL(layer_front, layer->GetNewImage());
    CHECK_EQUAL(layer_back, layer->GetOldImage());
}


TEST(TestLayer, TESTGetMemSize) {
    CHECK_EQUAL(TEST_BUFFER_SIZE, layer->GetMemSize());
}

TEST(TestLayer, TESTSetHeight) {
    layer->SetRelativeHeight(123);
    CHECK_EQUAL(123, layer->GetRelativeHeight());
}

TEST(TestLayer, TESTSetRotate) {
    layer->SetRotate(ROTATE_180);
    CHECK_EQUAL(ROTATE_180, layer->GetRotate());

    layer->SetRotate(ROTATE_0);
    CHECK_EQUAL(ROTATE_0, layer->GetRotate());

    layer->SetRotate(ROTATE_270);
    CHECK_EQUAL(ROTATE_270, layer->GetRotate());

    layer->SetRotate(ROTATE_90);
    CHECK_EQUAL(ROTATE_90, layer->GetRotate());
}

TEST(TestLayer, TESTSetInvertColor) {
    layer->SetInvertColor(true);
    CHECK_EQUAL(true, layer->GetInvertColor());

    layer->SetInvertColor(false);
    CHECK_EQUAL(false, layer->GetInvertColor());
}
