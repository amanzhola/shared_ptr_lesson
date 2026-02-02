#pragma once

#include <string>
#include <vector>
#include <cmath>

struct Size { int width = 0; int height = 0; };
struct Point { int x = 0; int y = 0; };

using Image = std::vector<std::string>;

inline Size GetImageSize(const Image& img) {
    int h = static_cast<int>(img.size());
    int w = (h == 0) ? 0 : static_cast<int>(img[0].size());
    return {w, h};
}

inline bool IsPointInEllipse(Point p, Size size) {
    if (size.width <= 0 || size.height <= 0) return false;
    double x = (p.x + 0.5) / (size.width  / 2.0) - 1.0;
    double y = (p.y + 0.5) / (size.height / 2.0) - 1.0;
    return x * x + y * y <= 1.0;
}
