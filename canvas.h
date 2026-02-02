#pragma once

#include "types.h"
#include "shape.h"

#include <memory>
#include <vector>

class Canvas {
public:
    explicit Canvas(Size size) : size_(size) {}

    void AddShape(std::unique_ptr<Shape> s) { shapes_.push_back(std::move(s)); }

    Image Render() const;
    void PrintColored() const;

private:
    Size size_;
    std::vector<std::unique_ptr<Shape>> shapes_;
};
