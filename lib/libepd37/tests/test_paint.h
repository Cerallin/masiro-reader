#include <CppUTest/TestHarness.h>

#include "epdpaint.h"
#include <stdlib.h>
#include <string.h>

#define TEST_WIDTH  16
#define TEST_HEIGHT 8

#define TEST_BUFFER_SIZE (TEST_WIDTH / 8 * TEST_HEIGHT * 2)

uint8_t buffer[TEST_BUFFER_SIZE * 2];
uint8_t* empty = buffer + TEST_BUFFER_SIZE;

Paint *paint;

TEST_GROUP(TestPaint) {
    void setup() {
        memset((void *)buffer, 0, TEST_BUFFER_SIZE);
        paint = new Paint(buffer, buffer + TEST_BUFFER_SIZE / 2,
                          TEST_WIDTH, TEST_HEIGHT);
    }

    void teardown() {
        delete paint;
    }
};

TEST(TestPaint, TestInit) {
    CHECK_EQUAL(false, paint->GetInvertColor());
    memset((void *)empty, 0xFF, TEST_BUFFER_SIZE);
    paint->Init();
    MEMCMP_EQUAL(empty, buffer, TEST_BUFFER_SIZE);
}

TEST(TestPaint, TestClear) {
    memset((void *)empty, 0xFF, TEST_BUFFER_SIZE / 2);
    memset((void *)(empty + TEST_BUFFER_SIZE / 2), 0x00, TEST_BUFFER_SIZE / 2);
    paint->Clear(COLOR_BW);
    MEMCMP_EQUAL(empty, buffer, TEST_BUFFER_SIZE);
}

TEST(TestPaint, TestGetWidth) {
    CHECK_EQUAL(TEST_WIDTH, paint->GetWidth());
}

TEST(TestPaint, TestGetRelativeWidth) {
    paint->SetRotate(ROTATE_90);
    CHECK_EQUAL(TEST_HEIGHT, paint->GetRelativeWidth());
}

TEST(TestPaint, TestSetHeight) {
    paint->SetHeight(4);
    CHECK_EQUAL(4, paint->GetHeight());
}

TEST(TestPaint, TestSetRotate) {
    paint->SetRotate(ROTATE_270);
    CHECK_EQUAL(ROTATE_270, paint->GetRotate());
}


TEST(TestPaint, TestInvertColor) {
    paint->SetInvertColor(true);
    CHECK_EQUAL(true, paint->GetInvertColor());
}

TEST(TestPaint, TestGetNewImage) {
    CHECK_EQUAL(buffer, paint->GetNewImage());
}

TEST(TestPaint, TestGetOldImage) {
    CHECK_EQUAL(buffer + TEST_BUFFER_SIZE / 2, paint->GetOldImage());
}

TEST(TestPaint, TestDrawAbsolutePixel) {
    paint->Init();
    paint->DrawAbsolutePixel(3, 0, COLOR_BB);
    BITS_EQUAL(0xEF, buffer[0], 0xFF);
}

TEST(TestPaint, TestDrawPixel) {
    paint->Init();
    paint->SetRotate(ROTATE_270);
    paint->DrawPixel(TEST_HEIGHT, 3, COLOR_BB);
    BITS_EQUAL(0xEF, buffer[0], 0xFF);
}
