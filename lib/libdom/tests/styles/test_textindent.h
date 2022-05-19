#include <CppUTest/TestHarness.h>

#include "domnode.h"

extern DOM::DOMNode *dom;

TEST_GROUP(TestTextIndent) {
    void setup() { dom = new DOM::DOMNode(16, 32); }
    void teardown() { delete dom; }
};

TEST(TestTextIndent, TestSetTextIndent) {
    Style::TextIndent expected = 233;
    dom->styles.SetTextIndent(expected);
    CHECK_EQUAL(expected, dom->styles.GetTextIndent());
}
