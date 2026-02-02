#include "textures_factory.h"
#include <algorithm>

Image MakeHouseImage() {
    const int W = 40;
    const int H = 20;
    Image img(H, std::string(W, 'B')); // sky

    for (int y = 14; y < H; ++y)
        for (int x = 0; x < W; ++x)
            img[y][x] = 'G'; // grass

    const int wall_left = 6, wall_right = 22;
    const int wall_top = 9, wall_bottom = 17;
    for (int y = wall_top; y <= wall_bottom; ++y)
        for (int x = wall_left; x <= wall_right; ++x)
            img[y][x] = 'O'; // walls

    for (int y = 12; y <= 14; ++y)
        for (int x = 14; x <= 17; ++x)
            img[y][x] = 'C'; // window

    const int roof_base_y = wall_top;
    const int roof_top_y = 3;
    const int roof_center_x = (wall_left + wall_right) / 2;
    const int roof_half_base = (wall_right - wall_left) / 2 + 4;
    const int roof_height = roof_base_y - roof_top_y;

    for (int y = roof_top_y; y <= roof_base_y; ++y) {
        int t = y - roof_top_y;
        int half = (roof_half_base * t) / roof_height;
        int left = roof_center_x - half;
        int right = roof_center_x + half;
        left = std::max(left, 0);
        right = std::min(right, W - 1);
        for (int x = left; x <= right; ++x)
            img[y][x] = 'W'; // roof
    }

    const int sun_cx = 32, sun_cy = 5;
    const int r2 = 9;
    for (int y = 1; y <= 9; ++y) {
        for (int x = 27; x <= 37; ++x) {
            int dx = x - sun_cx;
            int dy = y - sun_cy;
            if (dx * dx + dy * dy <= r2)
                img[y][x] = 'Y'; // sun
        }
    }

    return img;
}

Image MakeSolidImage(int w, int h, char code) {
    return Image(h, std::string(w, code));
}
