#include <CppUTest/TestHarness.h>

#include "domnode.h"

extern DOM::DOMNode *dom;

TEST_GROUP(TestMargin) {
    void setup() { dom = new DOM::DOMNode(16, 32); }
    void teardown() { delete dom; }
};

TEST(TestMargin, TestSetMargin1Param) {
    Style::Margin expected = {10, 10, 10, 10};
    dom->styles.SetMargin(10);
    MEMCMP_EQUAL(&expected, dom->styles.GetMargin(), sizeof(Style::Margin));
}

TEST(TestMargin, TestSetMargin2Params) {
    Style::Margin expected = {
        .marginLeft = 1,
        .marginRight = 1,
        .marginTop = 2,
        .marginBottom = 2,
    };
    dom->styles.SetMargin(1, 2);
    MEMCMP_EQUAL(&expected, dom->styles.GetMargin(), sizeof(Style::Margin));
}

TEST(TestMargin, TestSetMargin4Params) {
    Style::Margin expected = {
        .marginLeft = 1,
        .marginRight = 3,
        .marginTop = 2,
        .marginBottom = 4,
    };
    dom->styles.SetMargin(1, 2, 3, 4);
    MEMCMP_EQUAL(&expected, dom->styles.GetMargin(), sizeof(Style::Margin));
}
