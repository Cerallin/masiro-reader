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

#include "display.h"
#include "graphics.h"

#include <iostream>

int main(void) {
    BufferPool::Init();

    Layer layer(EPD_WIDTH, EPD_HEIGHT);
    BufferPool::AssignBufferTo(layer);

    layer.Init();

    try {
        ImageLayer imageLayer = layer;
        imageLayer.SetInvertColor(true).LoadFrom(SRC_DIR "/assets/lain.bmp");

        FontFace XLWenKai;
        XLWenKai.LoadFont(SRC_DIR "/assets/LXGWWenKaiScreen.ttf");

        char str[] = "Serial Experiments Lain";
        TextLayer textLayer = layer;
        textLayer.SetTextAlign(Text::AlignCenter)
            .SetFont(new Font(&XLWenKai, 28.0f))
            .SetText(str)
            .SetTextPadding(0, 195, 0, 0)
            .TypeSetting()
            .Render();

        auto display = new Display();
        display->Init();
        display->Forward(*(new Frame(layer.InvertColor())));

        BufferPool::RecycleBufferFrom(layer);
    } catch (EpdInitFailedException &e) {
        std::cout << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
