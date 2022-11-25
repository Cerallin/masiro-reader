#include <CppUTest/TestHarness.h>

#include "domnode.h"


extern DOM::DOMNode *dom;

TEST_GROUP(TestTextAlign) {
    void setup() { dom = new DOM::DOMNode(16, 32); }
    void teardown() { delete dom; }
};

TEST(TestTextAlign, TestSetTextAlign) {
    Style::TextAlign expected = Style::AlignCenter;
    dom->styles.SetTextAlign(expected);
    CHECK_EQUAL(expected, dom->styles.GetTextAlign());
}
