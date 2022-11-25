#include <CppUTest/TestHarness.h>

#include "domnode.h"

DOM::DOMNode *dom;

TEST_GROUP(TestDOMNodeBase) {
    void setup() {
        dom = new DOM::DOMNode(16, 32);
    }

    void teardown() {
        delete dom;
    }
};

TEST(TestDOMNodeBase, TestConstruct) {
    DOM::DOMNode tmp(8, 8);
    CHECK_EQUAL((size_t)0, dom->CountChildren());
    dom->PushChildren(&tmp);
    CHECK_EQUAL(&tmp, dom->PopChildren());
}