#include "texture.h"
#include <iostream>

Texture::Texture(std::string name, Image image)
    : name_(std::move(name))
    , image_(std::move(image)) {
    Size s = GetImageSize(image_);
    std::cout << "🧱✅ Texture " << name_ << " created"
              << "  (this=" << this << ", size=" << s.width << "x" << s.height << ")\n";
}

Texture::~Texture() {
    std::cout << "🧱💥 Texture " << name_ << " destroyed"
              << "  (this=" << this << ")\n";
}

char Texture::GetPixelColor(Point p) const {
    Size sz = GetSize();
    if (p.x < 0 || p.y < 0 || p.x >= sz.width || p.y >= sz.height) return ' ';
    return image_[p.y][p.x];
}
