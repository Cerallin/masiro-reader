#include <CppUTest/TestHarness.h>

#include "u16string.h"

using namespace u16String;

char str[] = "hello 你好";
CodePoint codepoints[] = {
    0xFEFF, 'h', 'e', 'l', 'l', 'o', 0x4f60, 0x597d, 0x0,
};

TEST_GROUP(TestU16String){};

TEST(TestU16String, TestStrToUnicode) {
    CodePoint *p;

    auto num = StrToUnicode(str, &p);

    CHECK_EQUAL(8, num);
    MEMCMP_EQUAL(codepoints, p, num);

    delete[] p;
}

TEST(TestU16String, TESTFindNextBreak) {
    CodePoint breaks[] = {'o', 'l', 0x0};
    auto res = FindNextBreak(codepoints + 1, breaks);
    CHECK_EQUAL(codepoints + 3, res);
}

TEST(TestU16String, TESTFindNextChar) {
    auto res = FindNextChar(codepoints + 1, 'o');
    CHECK_EQUAL(codepoints + 5, res);
}