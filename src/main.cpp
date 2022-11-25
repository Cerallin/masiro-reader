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

#include "libmasiro/display.h"
#include "libmasiro/graphics.h"

int main(void) {
    BufferPool::Init();

    Layer layer(EPD_WIDTH, EPD_HEIGHT);
    BufferPool::AssignBuffer(layer);

    ImageLayer imageLayer = layer;
    imageLayer.Init().LoadFrom(SRC_DIR "/assets/lain.bmp");

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

    Display display;
    display.Init();

    Frame frame(layer);
    display.Forward(frame);

    BufferPool::ReleaseLayer(layer);

    return 0;
}
