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

#include "bmp.h"
#include "exceptions.hpp"

#include <cmath>
#include <cstdio>

char UnsupportedBMPImage::error_msg[128];
char ImageIOException::error_msg[128];

void BMPImage::SetPallette(uint8_t new_grayDegree[4]) {
    std::memcpy(grayDegree, new_grayDegree, 4);
    for (int i = 0; i < 4; i++) {
        grayDegreeSquared[i] = grayDegree[i] * grayDegree[i];
    }
}

auto doNothing = [](uint8_t *ptr) {};

BMPImage::BMPImage(int32_t width, int32_t height, uint8_t *front, uint8_t *back)
    : width(width), height(height), front(front), back(back) {}

void BMPImage::Save(const char *imageFile) {
    FILE *fd = fopen(imageFile, "wb");
    if (fd == nullptr) {
        throw ImageSaveException(
            std::system_error(errno, std::generic_category()), imageFile);
    }

    /* Header */
    BitmapFileHeader fileHeader = {
        .bfType = BMP_FILE_HEADER,
        .bfSize = getFileSize(),
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = getOffset(),
    };
    fwrite(&fileHeader, sizeof(BitmapFileHeader), 1, fd);

    /* Information */
    BitmapInfoHeader infoHeader = {
        .biSize = BMP_FILE_INFO_SIZE,
        .biWidth = width,
        .biHeight = isUpsideDown ? -height : height,
        .biPlanes = 0x01,
        .biBitCount = 24,
        .biSizeImage = getImageSize(),
        .biXPelsPerMeter = 0x0EC4, // FIXME: why?
        .biYPelsPerMeter = 0x0EC4,
        .biClrUsed = 0,      // Use all colors
        .biClrImportant = 0, // Use all colors as important
    };
    fwrite(&infoHeader, sizeof(BitmapInfoHeader), 1, fd);

    /* Skip pallette */

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
            fwrite(&color, sizeof(uint8_t), 1, fd);
            fwrite(&color, sizeof(uint8_t), 1, fd);
            fwrite(&color, sizeof(uint8_t), 1, fd);

            f_data <<= 1;
            b_data <<= 1;
        }
    }

    fclose(fd);
}

void BMPImage::Load(const char *imageFile) {
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    isUpsideDown = false;

    FILE *fd = fopen(imageFile, "rb");
    if (fd == nullptr) {
        throw ImageLoadException(
            std::system_error(errno, std::generic_category()), imageFile);
    }

    fread(&fileHeader, sizeof(BitmapFileHeader), 1, fd);
    fread(&infoHeader, sizeof(BitmapInfoHeader), 1, fd);

    width = infoHeader.biWidth;
    height = infoHeader.biHeight;

    if (height < 0) {
        isUpsideDown = true;
        height = -height;
    }

    if (infoHeader.biBitCount < 8) {
        throw UnsupportedBMPImage("bit size < 8");
    }

    if (infoHeader.biCompression != 0) {
        throw UnsupportedBMPImage("compressed");
    }

    auto imageSize = infoHeader.biSizeImage + 1;

    // Load image
    auto image = std::make_unique<uint8_t[]>(imageSize);
    fseek(fd, fileHeader.bfOffBits, SEEK_SET);
    fread(image.get(), 1, imageSize, fd);
    fclose(fd);

    extractImage(image.get(), imageSize, infoHeader.biBitCount / 8);
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

constexpr uint32_t BMPImage::getFileSize() const {
    return getOffset() + getImageSize();
}

constexpr uint32_t BMPImage::getOffset() const { return 14 + 40; }

constexpr uint32_t BMPImage::getImageSize() const {
    return 3 * width * height - 1;
}

const uint8_t *BMPImage::GetFrontImage() const { return front; }

const uint8_t *BMPImage::GetBackImage() const { return back; }

int32_t BMPImage::GetWidth() const { return width; }

int32_t BMPImage::GetHeight() const { return height; }
