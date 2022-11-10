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
