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

#include "debug.h"

#include "display/display.h"

#include <cstdint>
#include <exception>

void Display::Refresh() {
    if (frames.empty()) {
        return;
    }

    frames.top().Display(Epd::GetInstance());
    debug("Display top frame, %" PRIdMAX " in stack", frames.size());
}

size_t Display::Forward(Frame &frame) {
    frame.Display(Epd::GetInstance());
    frames.push(std::move(frame));
    return this->Count();
}

size_t Display::Forward(Layer &layer) {
    Frame frame(layer);
    return this->Forward(frame);
}

size_t Display::Backward() {
    if (frames.empty()) {
        throw EmptyStackException();
    }

    auto top = frames.top();
    frames.pop();

    top.Display(Epd::GetInstance());

    return this->Count();
}

size_t Display::Clear() {
    size_t size = frames.size();
    frames = std::stack<Frame>();
    return size;
}

size_t Display::Count() const { return frames.size(); }

void Display::Init() {
    if (Epd::GetInstance().Init() != 0) {
        throw EpdInitFailedException();
    }
}

void Display::Sleep() { Epd::GetInstance().Sleep(); }

void Display::Awake() { Epd::GetInstance().PowerOn(); }
