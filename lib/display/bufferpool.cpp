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

#include "bufferpool.h"

uint8_t BufferPool::bufferPool[EPD_BUFFER_SIZE * LAYER_BUFFER_POOL_SIZE];
std::queue<uint8_t *> BufferPool::bufferQueue;

void BufferPool::Init() {
    uint8_t *buff = BufferPool::bufferPool;
    for (size_t i = 0; i < LAYER_BUFFER_POOL_SIZE; i++) {
        buff += EPD_BUFFER_SIZE;
        BufferPool::bufferQueue.push(buff);
    }
}

void BufferPool::AssignBufferTo(Layer &layer) {
    uint8_t *buff;

    if (bufferQueue.size() < 2) {
        throw OutOfBufferException();
    }

    buff = bufferQueue.front();
    layer.SetBackImage(buff);
    bufferQueue.pop();

    buff = bufferQueue.front();
    layer.SetFrontImage(buff);
    bufferQueue.pop();
}

void BufferPool::RecycleBufferFrom(Layer &layer) {
    bufferQueue.push(layer.GetOldImage());
    bufferQueue.push(layer.GetNewImage());
}
