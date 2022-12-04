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

#ifndef DISPLAY_BUFFER_POOL_H
#define DISPLAY_BUFFER_POOL_H

#include "graphics/layer.h"

#include <queue>

constexpr size_t EPD_BUFFER_SIZE = EPD_WIDTH * EPD_HEIGHT / 8;
constexpr size_t LAYER_BUFFER_POOL_SIZE = 8;

class OutOfBufferException : public std::exception {
  public:
    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "No enough buffer left.";
    }
};

class BufferPool {
  public:
    static void Init();
    /**
     * @brief Assign buffer to layer
     *
     * @param layer
     *
     * @throw OutOfBufferException
    */
    static void AssignBufferTo(Layer &layer);
    static void RecycleBufferFrom(Layer &layer);

  private:
    static uint8_t bufferPool[EPD_BUFFER_SIZE * LAYER_BUFFER_POOL_SIZE];
    static std::queue<uint8_t *> bufferQueue;
};

#endif /* DISPLAY_BUFFER_POOL_H */
