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

#include <cstdint>
#include <memory>

constexpr uint16_t BMP_FILE_HEADER = 0x4D42;
constexpr uint32_t BMP_FILE_INFO_SIZE = 40;

#pragma pack(push, bmp, 1)
struct BitmapFileHeader {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};
#pragma pack(pop)

struct BitmapInfoHeader {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};

class BMPImage {
  public:
    BMPImage(int32_t width, int32_t height, uint8_t *front, uint8_t *back);
    ~BMPImage() = default;

    void SetPallette(uint8_t grayDegree[4]);

    /**
     * @brief Save image to file.
     *
     * @param imageFile image filename
     *
     * @throw ImageSaveException
     */
    void Save(const char *imageFile);

    /**
     * @brief Load image from file.
     *
     * @param imageFile image filename
     *
     * @throw ImageLoadException
     * @throw UnsupportedBMPImage
     */
    void Load(const char *imageFile);

    const uint8_t *GetFrontImage() const;
    const uint8_t *GetBackImage() const;

    void DeleteFrontImage();
    void DeleteBackImage();

    int32_t GetWidth() const;
    int32_t GetHeight() const;

  private:
    constexpr uint32_t getOffset() const;
    constexpr uint32_t getImageSize() const;
    constexpr uint32_t getFileSize() const;

    void extractImage(const uint8_t *image, int32_t len, int byteCount);

    template <typename TInt>
    int getColorOf(const uint8_t *image, int byteCount);

    int32_t width;
    int32_t height;

    bool isUpsideDown;

    uint8_t *front;
    uint8_t *back;

    uint8_t grayDegree[4] = {
        0x00, // White
        0x3F, // Light gray
        0x7F, // Dark gray
        0xFF, // Black
    };

    uint16_t grayDegreeSquared[4] = {
        0x0001, // 0x00 ^ 2
        0x0F81, // 0x3F ^ 2
        0x3F01, // 0x7F ^ 2
        0xFE01, // 0xFF ^ 2
    };
};

#endif /* IMAGE_BMP_H */
