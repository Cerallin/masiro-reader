#include <CppUTest/TestHarness.h>

#include "codepoint.h"
#include <string.h>
#include <new>

char str[] = "hello 你好";
CodePoint codepoints[] = {
    0xFEFF, 'h', 'e', 'l', 'l', 'o', 0x4f60, 0x597d, 0x0,
};

TEST_GROUP(TestCodePoint){};

TEST(TestCodePoint, TestStrToUnicode) {
    size_t srcLen = strlen(str), destLen = (srcLen + 2) * 2;
    CodePoint *p = new CodePoint[destLen/2]();

    auto num = CodePoint::StrToUnicode(str, srcLen, &p, destLen);

    CHECK_EQUAL(9, num);
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
