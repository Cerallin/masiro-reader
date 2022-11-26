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

#ifndef DISPLAY_H
#define DISPLAY_H

#include "display/frame.h"

#include <stack>

class EpdInitFailedException : public std::exception {
  public:
    const char *what() { return "e-Paper init failed"; }
};

class EmptyStackException : public std::exception {
  public:
    const char *what() { return "Trying to pop an empty stack"; }
};

class Display {
  public:
    Display() = default;
    ~Display() = default;

    /**
     * @brief Init epd.
     *
     * @throw EpdInitFailedException
     */
    void Init();

    /**
     * @brief Display the frame last time displayed.
     */
    void Refresh();

    /**
     * @brief Sleep, not power off.
     */
    void Sleep();

    /**
     * @brief Power on.
     */
    void Awake();

    /**
     * @brief Diaplay a frame.
     *
     * @param frame to show
     */
    size_t Forward(Frame &frame);

    /**
     * @brief Display a layer.
     *
     * @param layer to show
     *
     * @throw EmptyStackException
     */
    size_t Forward(Layer &layer);

    /**
     * @brief Go back to last layer displayed.
     */
    size_t Backward();

    /**
     * @brief Clear all frames stored.
     */
    size_t Clear();

    /**
     * @brief Count frames.
     */
    size_t Count() const;

  private:
    std::stack<Frame> frames;
};

#endif /* DISPLAY_H */
