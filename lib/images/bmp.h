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
#include <cstdio>
#include <memory>

constexpr int16_t BMP_FILE_HEADER = 0x4D42;
constexpr int32_t BMP_FILE_INFO_SIZE = 40;

class UnsupportedBMPImage : public std::exception {
  public:
    UnsupportedBMPImage(const char *info) : std::exception(), info(info){};

    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        snprintf(error_msg, 80, "Unsupported BMP image: %s.", info);
        return error_msg;
    };

  private:
    static char error_msg[80];
    const char *info;
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
     * @throw std::system_error
     */
    void Save(const char *imageFile);

    /**
     * @brief Load image from file.
     *
     * @param imageFile image filename
     *
     * @throw std::system_error
     * @throw std::runtime_error
     */
    void Load(const char *imageFile);

    const uint8_t *GetFrontImage() const;
    const uint8_t *GetBackImage() const;

    void DeleteFrontImage();
    void DeleteBackImage();

    int32_t GetWidth() const;
    int32_t GetHeight() const;

  private:
    constexpr int32_t getOffset() const;
    constexpr int32_t getImageSize() const;
    constexpr int32_t getFileSize() const;

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
