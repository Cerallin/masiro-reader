#include <CppUTest/TestHarness.h>

#include "imagelayer.h"

ImageLayer *imageLayer;

TEST_GROUP(TestImageLayer) {
    void setup() {
        imageLayer = new ImageLayer;
    }
};

TEST(TestImageLayer, TestGetCodepointHMetrics) {
    int advanceWidth = -1, leftSideBearing = -1;
    font->GetCodepointHMetrics('H', &advanceWidth, &leftSideBearing);
    CHECK_FALSE(-1 == advanceWidth);
    CHECK_FALSE(-1 == leftSideBearing);
}
