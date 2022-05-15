#include <CppUTest/TestHarness.h>

#include "domnode.h"


extern DOM::DOMNode *dom;

TEST_GROUP(TestPosition) {
    void setup() { dom = new DOM::DOMNode(16, 32); }
    void teardown() { delete dom; }
};

TEST(TestPosition, TestSetPosition) {
    Style::Position expected = {
        .left = 1,
        .right = 3,
        .top = 2,
        .bottom = 4,
    };
    dom->SetPosition(1, 2, 3, 4);
    MEMCMP_EQUAL(&expected, dom->GetPosition(), sizeof(Style::Position));
}
