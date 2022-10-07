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
#include <new>
#include <stdio.h>

#define fwrite__(type, num)                                                    \
    {                                                                          \
        type tmp = (num);                                                      \
        fwrite(&tmp, sizeof(type), 1, fd);                                     \
    }

#define fread__(type, num) fread(num, sizeof(type), 1, fd);

#define fread_int8(num)    fread__(int8_t, num)
#define fread_int16(num)   fread__(int16_t, num)
#define fread_int32(num)   fread__(int32_t, num)
#define fread_uint8(num)   fread__(uint8_t, num)
#define fread_uint16(num)  fread__(uint16_t, num)
#define fread_uint32(num)  fread__(uint32_t, num)
#define fwrite_int8(num)   fwrite__(int8_t, num)
#define fwrite_int16(num)  fwrite__(int16_t, num)
#define fwrite_int32(num)  fwrite__(int32_t, num)
#define fwrite_uint8(num)  fwrite__(uint8_t, num)
#define fwrite_uint16(num) fwrite__(uint16_t, num)
#define fwrite_uint32(num) fwrite__(uint32_t, num)

#define square(x) (uint16_t)((uint16_t)(x) * (uint16_t)(x))

const uint8_t palette[] = {
    0x00, // White
    0x3F, // Light gray
    0x7F, // Dark gray
    0xFF, // Black
};

const uint16_t palette_squared[] = {
    0x0000,
    0x0F80,
    0x3F00,
    0xFE00,
};

BMPImage::BMPImage(int32_t width, int32_t height, unsigned char *front,
                   unsigned char *back)
    : width(width), height(height), front(front), back(back) {}

int BMPImage::Save(const char *imageFile) {
    FILE *fd = fopen(imageFile, "wb");
    if (fd == nullptr) {
        return -1;
    }

    /* Header */
    fwrite_uint16(BMP_FILE_HEADER); // BMP file header
    fwrite_int32(GetFileSize());    // file size

    fwrite_int16(0x0000); // Reserved 2 bytes
    fwrite_int16(0x0000); // Another reserved 2 bytes

    fwrite_uint32(GetOffset()); // Offset bits

    /* Information */
    fwrite_int32(BMP_FILE_INFO_SIZE); // Info size

    fwrite_int32(width); // Width (px)

    // upside-down
    fwrite_int32(-height); // Height (px)

    fwrite_int16(0x01); // Planes

    fwrite_int16(24); // Bit count

    fwrite_int32(0x00); // Compression type

    fwrite_int32(GetImageSize()); // Image size

    fwrite_int32(0x0EC4); // XPelsPerMeter
    fwrite_int32(0x0EC4); // YPelsPerMeter

    fwrite_int32(0x00); // Use all colors
    fwrite_int32(0x00); // Use all colors as important

    /* Skip palette */

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

            color = palette[color & 0x03];

            // Pretend to be a RGB Image
            fwrite_uint8(color);
            fwrite_uint8(color);
            fwrite_uint8(color);

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
        return -1;
    }

    fread_int16(&tmp);
    assert(tmp == BMP_FILE_HEADER);

    fread_int32(&fileSize);                   // File size
    fseek(fd, sizeof(int16_t) * 2, SEEK_CUR); // Skip reserved bytes
    fread_int32(&offsetToImage);
    fread_int32(&infoSize);

    fread_int32(&width);
    fread_int32(&height);

    if (height < 0) {
        isUpsideDown = true;
        height = -height;
    }

    fseek(fd, sizeof(int16_t), SEEK_CUR); // Skip planes

    fread_int16(&bitCount);

    if (bitCount < 8) {
        fprintf(stderr, "Unsupported BMP image!\n");
        return -1;
    }

    fread_int32(&tmp); // Compression type

    if (tmp) {
        fprintf(stderr, "Compressed BMP images are not supported!\n");
        return -1;
    }

    fread_int32(&imageSize);

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

    ExtractImage(image, imageSize, bitCount / 8);

    delete[] image;
    return 0;
}

inline int GetPaletteOf(uint32_t mean) {
    for (int i = 1; i < sizeof(palette); i++) {
        if (mean <= palette_squared[i])
            return ~(i - 1) & 0x03;
    }

    // Darker than black
    return ~3 & 0x03;
}

void BMPImage::ExtractImage(const uint8_t *image, int32_t len, int byteCount) {
    for (int32_t i = 0; i < len;) {
        uint32_t sum = 0;
        for (int n = 0; n < byteCount; n++, i++) {
            sum += image[i] * image[i];
        }

        auto color = GetPaletteOf(sum / byteCount);
        auto l = i / byteCount - 1;
        auto x = l % width;

        if (color & 0x01) {
            front[l / 8] &= ~(0x80 >> (x % 8));
        } else {
            front[l / 8] |= 0x80 >> (x % 8);
        }

        if (color & 0x02) {
            back[l / 8] &= ~(0x80 >> (x % 8));
        } else {
            back[l / 8] |= 0x80 >> (x % 8);
        }
    }

    if (!isUpsideDown) {
        // TODO
    }
}

int32_t BMPImage::GetFileSize() { return GetOffset() + GetImageSize(); }

int32_t BMPImage::GetOffset() { return 14 + 40; }

int32_t BMPImage::GetImageSize() { return 3 * width * height; }

const unsigned char *BMPImage::GetFrontImage() { return front; }

const unsigned char *BMPImage::GetBackImage() { return back; }

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

int32_t BMPImage::GetWidth() { return width; }

int32_t BMPImage::GetHeight() { return height; }
