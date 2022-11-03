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
#ifndef IMAGE_BMP_H
#define IMAGE_BMP_H

#include <stdint.h>

#define BMP_FILE_HEADER    0x4D42
#define BMP_FILE_INFO_SIZE 40

class BMPImage {
  public:
    BMPImage(int32_t width, int32_t height, unsigned char *front,
             unsigned char *back);

    int Save(const char *imageFile);

    int Load(const char *imageFile);

    const unsigned char *GetFrontImage() const;
    const unsigned char *GetBackImage() const;

    void DeleteFrontImage();
    void DeleteBackImage();

    int32_t GetWidth() const;
    int32_t GetHeight() const;

  private:
    int32_t getOffset() const;
    int32_t getImageSize() const;
    int32_t getFileSize() const;

    void ExtractImage(const uint8_t *image, int32_t len, int byteCount);

    int32_t width;
    int32_t height;
    bool isUpsideDown;
    unsigned char *front;
    unsigned char *back;
};

#endif /* IMAGE_BMP_H */
