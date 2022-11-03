#include "frame.h"

Frame::Frame(const uint8_t *old_image, const uint8_t *new_image) {}

Frame::Frame(const Layer *layer)
    : old_image(layer->GetOldImage()), new_image(layer->GetNewImage()) {}

void Frame::Display(const Epd *epd) {
    epd->DisplayFrame(old_image, new_image);
    this->displayed = true;
}

bool Frame::isDisplayed() const { return this->displayed; }
