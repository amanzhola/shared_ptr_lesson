#pragma once

#include "types.h"
#include <string>
#include <utility>

class Texture {
public:
    Texture(std::string name, Image image);
    ~Texture();

    const std::string& Name() const noexcept { return name_; }
    Size GetSize() const { return GetImageSize(image_); }

    char GetPixelColor(Point p) const;

private:
    std::string name_;
    Image image_;
};
