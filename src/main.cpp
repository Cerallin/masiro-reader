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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "frame.h"
#include "imagelayer.h"
#include "textlayer.h"

constexpr size_t BUFFER_SIZE = (EPD_WIDTH * EPD_HEIGHT / 8);

unsigned char layer_buffer[BUFFER_SIZE * 2];

constexpr auto front_buff = layer_buffer,
               back_buff = layer_buffer + BUFFER_SIZE;

int main(void) {
    Layer layer(EPD_WIDTH, EPD_HEIGHT);
    layer.SetFrontImage(front_buff).SetBackImage(back_buff);

    ImageLayer imageLayer = layer;
    imageLayer.Init();
    if (imageLayer.LoadFrom(SRC_DIR "/assets/lain.bmp")) {
        fprintf(stderr, "Cannot load image file %s\n",
                SRC_DIR "/assets/lain.bmp");
        return 1;
    }

    TextLayer textLayer(layer, Graphic::AlignCenter);

    FontFace XLWenKai;
    if (XLWenKai.LoadFont(SRC_DIR "/assets/LXGWWenKaiScreen.ttf")) {
        fprintf(stderr, "Load font failed.\n");
        return 1;
    }

    FontFamily fontFamily(&XLWenKai);

    Font TitleFont(&fontFamily, 28.0f);
    textLayer.SetFont(&TitleFont);

    char str[] = "Serial Experiments Lain";
    if (textLayer.SetText(str) == -1) {
        return 1;
    }
    textLayer.SetInvertColor(true)
        .SetTextPadding(0, 195, 0, 0)
        .CalcTypeSetting()
        .Render();

    Epd &epd = Epd::GetInstance();

    if (epd.Init() != 0) {
        printf("e-Paper init failed\n");
        return -1;
    }

    Frame(&layer).Display(&epd);

    epd.PowerOff();

    return 0;
}
