#include <CppUTest/TestHarness.h>

#include "treenode.h"

DOM::TreeNode *node;

TEST_GROUP(TestTreeNode) {
    void setup() {
        node = new DOM::TreeNode;
    }

    void teardown() {
        delete node;
    }
};

TEST(TestTreeNode, TestConstruct) {
    CHECK_EQUAL(NULLPTR, node->GetChildren());
    CHECK_EQUAL(NULLPTR, node->GetFirstChild());
    CHECK_EQUAL(NULLPTR, node->GetLastChild());
    CHECK_EQUAL((size_t)0, node->CountChildren());
}

TEST(TestTreeNode, TestPush1Child) {
    DOM::TreeNode tmp;

    node->PushChildren(&tmp);

    CHECK_EQUAL(&tmp, node->GetChildren());
    CHECK_EQUAL(&tmp, node->GetFirstChild());
    CHECK_EQUAL(&tmp, node->GetLastChild());
}

TEST(TestTreeNode, TestPush2Children) {
    DOM::TreeNode a, b;

    node->PushChildren(&a);
    node->PushChildren(&b);

    CHECK_EQUAL(&a, node->GetChildren());
    CHECK_EQUAL(&a, node->GetFirstChild());
    CHECK_EQUAL(&b, node->GetLastChild());
}

TEST(TestTreeNode, TestCountChildren) {
    DOM::TreeNode a, b;

    node->PushChildren(&a);
    node->PushChildren(&b);

    CHECK_EQUAL((size_t)2, node->CountChildren());
}

TEST(TestTreeNode, TestPopFromEmpty) {
    CHECK_EQUAL(NULLPTR, node->PopChildren());
}

TEST(TestTreeNode, TestPop1Child) {
    DOM::TreeNode tmp;
    node->PushChildren(&tmp);
    CHECK_EQUAL(&tmp, node->PopChildren());
}

TEST(TestTreeNode, TestPop2Children) {
    DOM::TreeNode a, b;
    node->PushChildren(&a);
    node->PushChildren(&b);
    CHECK_EQUAL(&b, node->PopChildren());
    CHECK_EQUAL(&a, node->PopChildren());
    CHECK_EQUAL(NULLPTR, node->PopChildren());
}

TEST(TestTreeNode, TestUnshift1Child) {
    DOM::TreeNode tmp;

    node->UnshiftChildren(&tmp);

    CHECK_EQUAL(&tmp, node->GetChildren());
    CHECK_EQUAL(&tmp, node->GetFirstChild());
    CHECK_EQUAL(&tmp, node->GetLastChild());
}

TEST(TestTreeNode, TestUnshift2Children) {
    DOM::TreeNode a, b;
    node->UnshiftChildren(&a);
    node->UnshiftChildren(&b);

    CHECK_EQUAL(&b, node->GetFirstChild());
    CHECK_EQUAL(&a, node->GetLastChild());
}

TEST(TestTreeNode, TestShiftEmpty) {
    CHECK_EQUAL(NULLPTR, node->ShiftChildren());
}

TEST(TestTreeNode, TestShift1Child) {
    DOM::TreeNode tmp;

    node->UnshiftChildren(&tmp);

    CHECK_EQUAL(&tmp, node->ShiftChildren());
}

TEST(TestTreeNode, TestShift2Children) {
    DOM::TreeNode a, b;

    node->UnshiftChildren(&a);
    node->UnshiftChildren(&b);

    CHECK_EQUAL(&b, node->ShiftChildren());
    CHECK_EQUAL(&a, node->ShiftChildren());
    CHECK_EQUAL(NULLPTR, node->ShiftChildren());
}

TEST(TestTreeNode, TestQueue) {
    DOM::TreeNode a, b;

    node->PushChildren(&a);
    node->PushChildren(&b);
    CHECK_EQUAL(&a, node->ShiftChildren());
    CHECK_EQUAL(&b, node->ShiftChildren());
    CHECK_EQUAL(NULLPTR, node->ShiftChildren());
}

TEST(TestTreeNode, TestParent) {
    DOM::TreeNode tmp;

    node->SetParent(&tmp);
    CHECK_EQUAL(&tmp, node->GetParent());
}
