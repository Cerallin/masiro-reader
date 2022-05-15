#include <CppUTest/TestHarness.h>

#include "domnode.h"

extern DOM::DOMNode *dom;

TEST_GROUP(TestPadding) {
    void setup() { dom = new DOM::DOMNode(16, 32); }
    void teardown() { delete dom; }
};

TEST(TestPadding, TestSetPadding1Param) {
    Style::Padding expected = {10, 10, 10, 10};
    dom->SetPadding(10);
    MEMCMP_EQUAL(&expected, dom->GetPadding(), sizeof(Style::Padding));
}

TEST(TestPadding, TestSetPadding2Params) {
    Style::Padding expected = {
        .paddingLeft = 1,
        .paddingRight = 1,
        .paddingTop = 2,
        .paddingBottom = 2,
    };
    dom->SetPadding(1, 2);
    MEMCMP_EQUAL(&expected, dom->GetPadding(), sizeof(Style::Padding));
}

TEST(TestPadding, TestSetPadding4Params) {
    Style::Padding expected = {
        .paddingLeft = 1,
        .paddingRight = 3,
        .paddingTop = 2,
        .paddingBottom = 4,
    };
    dom->SetPadding(1, 2, 3, 4);
    MEMCMP_EQUAL(&expected, dom->GetPadding(), sizeof(Style::Padding));
}
