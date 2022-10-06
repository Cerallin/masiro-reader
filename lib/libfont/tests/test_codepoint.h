#include <CppUTest/TestHarness.h>

#include "codepoint.h"
#include <new>

char str[] = "hello 你好";
CodePoint codepoints[] = {
    0xFEFF, 'h', 'e', 'l', 'l', 'o', 0x4f60, 0x597d, 0x0,
};

TEST_GROUP(TestCodePoint){};

TEST(TestCodePoint, TestStrToUnicode) {
    size_t len = strlen(str);
    CodePoint *p = new CodePoint[len + 2]();

    auto num = CodePoint::StrToUnicode(str, len, &p);

    CHECK_EQUAL(8, num);
    MEMCMP_EQUAL(codepoints, p, num);

    delete[] p;
}

TEST(TestCodePoint, TESTFindNextBreak) {
    CodePoint breaks[] = {'o', 'l', 0x0};
    auto res = CodePoint::FindNextBreak(codepoints + 1, breaks);
    CHECK_EQUAL(codepoints + 3, res);
}

TEST(TestCodePoint, TESTFindNextChar) {
    auto res = CodePoint::FindNextChar(codepoints + 1, 'o');
    CHECK_EQUAL(codepoints + 5, res);
}
