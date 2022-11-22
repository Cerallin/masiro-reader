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

#include "bmp.h"

#include <assert.h>
#include <cmath>
#include <cstring>
#include <new>
#include <stdio.h>

template <typename T> size_t fread__(T *num, FILE *fd) {
    return fread(num, sizeof(T), 1, fd);
}

template <typename T> size_t fwrite__(T num, FILE *fd) {
    return fwrite(&num, sizeof(T), 1, fd);
}

void BMPImage::SetPallette(uint8_t new_grayDegree[4]) {
    std::memcpy(grayDegree, new_grayDegree, 4);
    for (int i = 0; i < 4; i++) {
        grayDegreeSquared[i] = grayDegree[i] * grayDegree[i];
    }
}

BMPImage::BMPImage(int32_t width, int32_t height, unsigned char *front,
                   unsigned char *back)
    : width(width), height(height), front(front), back(back) {}

int BMPImage::Save(const char *imageFile) {
    FILE *fd = fopen(imageFile, "wb");
    if (fd == nullptr) {
        return -1;
    }

    /* Header */
    fwrite__<uint16_t>(BMP_FILE_HEADER, fd); // BMP file header
    fwrite__<int32_t>(getFileSize(), fd);    // file size

    fwrite__<int16_t>(0x0000, fd); // Reserved 2 bytes
    fwrite__<int16_t>(0x0000, fd); // Another reserved 2 bytes

    fwrite__<uint32_t>(getOffset(), fd); // Offset bits

    /* Information */
    fwrite__<int32_t>(BMP_FILE_INFO_SIZE, fd); // Info size

    fwrite__<int32_t>(width, fd); // Width (px)

    // upside-down
    fwrite__<int32_t>(-height, fd); // Height (px)

    fwrite__<int16_t>(0x01, fd); // Planes

    fwrite__<int16_t>(24, fd); // Bit count

    fwrite__<int32_t>(0x00, fd); // Compression type

    fwrite__<int32_t>(getImageSize(), fd); // Image size

    fwrite__<int32_t>(0x0EC4, fd); // XPelsPerMeter
    fwrite__<int32_t>(0x0EC4, fd); // YPelsPerMeter

    fwrite__<int32_t>(0x00, fd); // Use all colors
    fwrite__<int32_t>(0x00, fd); // Use all colors as important

    /* Skip grayDegree */

    /* Image */
    for (int32_t i = 0; i < width * height / 8; i++) {
        uint8_t f_data = front[i];
        uint8_t b_data = back[i];

        for (int k = 0; k < 8; k++) {
            uint8_t color = 0;
            if (b_data & 0x80)
                color += 2;
            if (f_data & 0x80)
                color += 1;

            color = grayDegree[color & 0x03];

            // Pretend to be a RGB Image
            fwrite__<uint8_t>(color, fd);
            fwrite__<uint8_t>(color, fd);
            fwrite__<uint8_t>(color, fd);

            f_data <<= 1;
            b_data <<= 1;
        }
    }

    fclose(fd);
    return 0;
}

int BMPImage::Load(const char *imageFile) {
    int32_t tmp = 0; // long enough
    int32_t fileSize, offsetToImage, infoSize, imageSize;

    int16_t bitCount;
    isUpsideDown = false;

    FILE *fd = fopen(imageFile, "rb");
    if (fd == nullptr) {
        perror("BMPIMage::Load");
        return -1;
    }

    fread__<int16_t>((int16_t *)&tmp, fd);
    assert(tmp == BMP_FILE_HEADER);

    fread__<int32_t>(&fileSize, fd);          // File size
    fseek(fd, sizeof(int16_t) * 2, SEEK_CUR); // Skip reserved bytes
    fread__<int32_t>(&offsetToImage, fd);
    fread__<int32_t>(&infoSize, fd);

    fread__<int32_t>(&width, fd);
    fread__<int32_t>(&height, fd);

    if (height < 0) {
        isUpsideDown = true;
        height = -height;
    }

    fseek(fd, sizeof(int16_t), SEEK_CUR); // Skip planes

    fread__<int16_t>(&bitCount, fd);

    if (bitCount < 8) {
        fprintf(stderr, "Unsupported BMP image!\n");
        return -1;
    }

    fread__<int32_t>(&tmp, fd); // Compression type

    if (tmp) {
        fprintf(stderr, "Compressed BMP images are not supported!\n");
        return -1;
    }

    fread__<int32_t>(&imageSize, fd);
    imageSize++;

    // Load image
    fseek(fd, offsetToImage, SEEK_SET);
    auto image = new (std::nothrow) unsigned char[imageSize];
    if (image == nullptr) {
        fprintf(stderr, "Out of memory.\n");
        return -1;
    }
    fread(image, 1, imageSize, fd);
    fclose(fd);

    int32_t len = imageSize / bitCount;

    // allocate buffer
    if (front == nullptr) {
        front = new (std::nothrow) unsigned char[len]();
        if (front == nullptr) {
            fprintf(stderr, "Out of memory.\n");
            return -1;
        }
    }

    if (back == nullptr) {
        back = new (std::nothrow) unsigned char[len]();
        if (back == nullptr) {
            fprintf(stderr, "Out of memory.\n");
            return -1;
        }
    }

    extractImage(image, imageSize, bitCount / 8);

    delete[] image;
    return 0;
}

template <typename TInt>
int BMPImage::getColorOf(const uint8_t *image, int byteCount) {
    TInt actual = 0;
    for (int i = 0; i < byteCount; i++) {
        actual += image[i] * image[i];
    }

    for (size_t i = 1; i < sizeof(grayDegreeSquared); i++) {
        TInt assumed = byteCount * grayDegreeSquared[i];
        if (actual < assumed)
            return ~(i - 1) & 0x03;
    }

    // Darker than black
    return ~3 & 0x03;
}

inline void paint(uint8_t *image, int offset, bool color) {
    if (color) {
        *image &= ~(0x80 >> (offset % 8));
    } else {
        *image |= 0x80 >> (offset % 8);
    }
}

void BMPImage::extractImage(const uint8_t *image, int32_t len, int byteCount) {
    int offset, wCount;
    int32_t size = (len / byteCount - 1) / 8;

    assert(len % byteCount == 0);

    for (int32_t i = 0; i < len; i += byteCount) {
        int color = getColorOf<uint32_t>(image + i, byteCount);

        offset = i / byteCount - 1;
        wCount = offset % width;
        offset /= 8;

        if (!isUpsideDown) {
            offset = size - offset - 1;
            wCount = width - wCount - 1;
        }

        paint(front + offset, wCount, color & 0x01);
        paint(back + offset, wCount, color & 0x02);
    }
}

constexpr int32_t BMPImage::getFileSize() const {
    return getOffset() + getImageSize();
}

constexpr int32_t BMPImage::getOffset() const { return 14 + 40; }

constexpr int32_t BMPImage::getImageSize() const {
    return 3 * width * height - 1;
}

const unsigned char *BMPImage::GetFrontImage() const { return front; }

const unsigned char *BMPImage::GetBackImage() const { return back; }

void BMPImage::DeleteFrontImage() {
    if (front != nullptr) {
        delete[] front;
    }
}

void BMPImage::DeleteBackImage() {
    if (back != nullptr) {
        delete[] back;
    }
}

int32_t BMPImage::GetWidth() const { return width; }

int32_t BMPImage::GetHeight() const { return height; }
