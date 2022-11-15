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

#ifndef _Class
#error "Please specify class name for layer traits"
#endif

/**
 * @brief Set front image
 *
 * @param front image pointer
 * @returns Self
 */
_Class &SetFrontImage(uint8_t *front);

/**
 * @brief Set back image
 *
 * @param back image pointer
 * @returns Self
 */
_Class &SetBackImage(uint8_t *back);

/**
 * @brief Set both front and back image
 *
 * @param image image pointer, should be 2 * size long array.
 * @returns Self
 */
_Class &SetImages(uint8_t *image);

/**
 * @brief Refresh image buffer
 *
 * @return Self
 */
_Class &Init();

/**
 * @brief Set relative width, affected by rotate
 */
_Class &SetRelativeWidth(int32_t width);

/**
 * @brief Set relative height, affected by rotate
 */
_Class &SetRelativeHeight(int32_t height);

/**
 * @brief
 *
 * @param[in] rotate
 *
 * @return self
 */
_Class &SetRotate(int32_t rotate);

_Class &SetInvertColor(bool flag);
