/**
 * Masiro Reader: a EPD book reader.
 * Copyright (C) 2022  Cerallin
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "graphic/paragraph.h"

char str[] = "LilyGo\n换行测试";
char str1[] = "他自己唱歌\n"
              "自己走路\n"
              "和两只\n"
              "温柔的黄狗说话";

char str_lain[] = "きおくにないことは、なかったこと。\n"
                  "きおくなんて、ただのきろく。\n"
                  "きろくなんて、かきかえて、しまえばいい。\n\n"
                  "Serial Experiments Lain";
char str_masiro[] =
    "鉴于原创心血来潮挖坑又不填的情况过多，标点误用严重影响部分"
    "审核的血压值，现决定提高原创申请门槛。\n"
    "1）申请字数要求6000字或以上。\n"
    "2）标点符号使用规范恰当，如避免用“。。。”等奇形怪状的用法"
    "表示省略号，避免全文只有感叹号没有句号结尾等。具体作品的标"
    "准，简体中文作品参见《中华人民共和国国家标准GB/"
    "T15834-2011标点符号用法》，繁体中文作品参见《重訂標點符號手冊》修"
    "訂版，最终审核结果以审核结果为准。";
char str2[] =
    "An individual human existence should be like a river-small at first, "
    "narrowly contained within its banks, and rushing passionately past "
    "boulders and over waterfalls. Gradually the river grows wider, the banks "
    "recede, the waters flow more quietly, and in the end, without any visible "
    "break, they become merged in the sea, and painlessly lose their "
    "individual being.";
char str3[] =
    "Youth means a temperamental predominance of courage over timidity, of the "
    "appetite for adventure over the love of ease. This often exists in a man "
    "of sixty more than a boy of twenty. Nobody grows old merely by a number "
    "of years. We grow old by deserting our ideals.";
char str4[] =
    "Whether sixty or sixteen, there is in every human being's heart the lure "
    "of wonders, the unfailing childlike appetite of what's next and the joy "
    "of the game of living. In the center of your heart and my heart there is "
    "a wireless station: so long as it receives messages of beauty, hope, "
    "cheer, courage and power from men and from the infinite, so long are you "
    "young.";

int main(void) {
    FontFace XLWenKai;
    if (XLWenKai.LoadFont("/home/julia/LXGWWenKaiScreen.ttf")) {
        fprintf(stderr, "Load font failed.\n");
        return 1;
    }

    Font TitleFont(&XLWenKai, 20.0f);

    Epd &epd = Epd::GetInstance();

    if (epd.Init() != 0) {
        printf("e-Paper init failed\n");
        return -1;
    }

    Paragraph paragraph(epd.u16MaxWidth, epd.u16MaxHeight, &TitleFont,
                        AlignLeft, ROTATE_270);
    if (paragraph.Init()) {
        fprintf(stderr, "Out of memory.\n");
        return 1;
    }
    if (paragraph.SetText(str_masiro) == -1) {
        fprintf(stderr, "Setting text wrong.\n");
    }
    paragraph.SetTextPadding(20);
    paragraph.Render();
    // paragraph.SetInvertColor(true);
    paragraph.Display(&epd);

    epd.PowerOff();

    paragraph.SetRotate(ROTATE_0);
    // (new BMPImage(paragraph.GetWidth(), paragraph.GetHeight(),
    //               paragraph.GetNewImage(), paragraph.GetOldImage()))
    //     ->save("/home/julia/tmp.bmp");

    return 0;
}

// // Display picture
// int main(void) {
//     BMPImage *testImage = new BMPImage();
//     testImage->load("/home/julia/lain.bmp");
//     // test->save("/home/julia/test_load.bmp");
//     Layer layer(testImage->GetWidth(), testImage->GetHeight());
//     layer.Init();
//     layer.LoadFrom(testImage);
//     delete testImage;
//     layer.SetInvertColor(true);

//     Epd &epd = Epd::GetInstance();

//     if (epd.Init() != 0) {
//         printf("e-Paper init failed\n");
//         return -1;
//     }
//     layer.Display(&epd);
//     return 0;
// }
