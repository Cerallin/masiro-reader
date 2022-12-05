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

#ifndef IMAGE_EXCEPTIONS_H
#define IMAGE_EXCEPTIONS_H

#include <cstring>
#include <exception>
#include <system_error>

class UnsupportedBMPImage : public std::exception {
  public:
    UnsupportedBMPImage(const char *info) : std::exception(), info(info){};

    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        snprintf(error_msg, 128, "Unsupported BMP image: %s.", info);
        return error_msg;
    };

  private:
    static char error_msg[128];
    const char *info;
};

class ImageIOException : public std::exception {
  public:
    ImageIOException(std::exception e, const char *path) : e(e), path(path){};

    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        snprintf(error_msg, 128, "%s: %s", e.what(), path);
        return error_msg;
    };

  protected:
    static char error_msg[128];

  private:
    std::exception e;
    const char *path;
};

class ImageLoadException : public ImageIOException {
  public:
    ImageLoadException(std::system_error e, const char *path)
        : ImageIOException(e, path){};

    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        char tmp[128];
        strncpy(tmp, ImageIOException::what(), 128);
        snprintf(error_msg, 128, "Cannot load image: %s", tmp);
        return error_msg;
    };
};

class ImageSaveException : public ImageIOException {
  public:
    ImageSaveException(std::system_error e, const char *path)
        : ImageIOException(e, path){};

    const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        char tmp[128];
        strncpy(tmp, ImageIOException::what(), 128);
        snprintf(error_msg, 128, "Cannot save image: %s", tmp);
        return error_msg;
    };
};

#endif /* IMAGE_EXCEPTIONS_H */
