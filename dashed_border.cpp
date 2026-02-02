#include "dashed_border.h"

bool Dash(int x, int y) { return ((x + y) % 2) == 0; }

void PutIf(Image& img, int x, int y, char c) {
    Size s = GetImageSize(img);
    if (x < 0 || y < 0 || x >= s.width || y >= s.height) return;
    img[y][x] = c;
}

void DrawRectBorderDashed(Image& img, Point o, Size s, char mark) {
    int x0 = o.x, y0 = o.y;
    int x1 = o.x + s.width - 1;
    int y1 = o.y + s.height - 1;

    for (int x = x0; x <= x1; ++x) {
        if (Dash(x, y0)) PutIf(img, x, y0, mark);
        if (Dash(x, y1)) PutIf(img, x, y1, mark);
    }
    for (int y = y0; y <= y1; ++y) {
        if (Dash(x0, y)) PutIf(img, x0, y, mark);
        if (Dash(x1, y)) PutIf(img, x1, y, mark);
    }
}

void DrawEllipseBorderDashed(Image& img, Point o, Size s, char mark) {
    for (int dy = 0; dy < s.height; ++dy) {
        for (int dx = 0; dx < s.width; ++dx) {
            if (!IsPointInEllipse({dx, dy}, s)) continue;

            bool edge = false;
            const int nx[4] = {1,-1,0,0};
            const int ny[4] = {0,0,1,-1};

            for (int k = 0; k < 4; ++k) {
                int ddx = dx + nx[k], ddy = dy + ny[k];
                if (ddx < 0 || ddy < 0 || ddx >= s.width || ddy >= s.height) { edge = true; break; }
                if (!IsPointInEllipse({ddx, ddy}, s)) { edge = true; break; }
            }
            if (!edge) continue;

            int X = o.x + dx;
            int Y = o.y + dy;
            if (Dash(X, Y)) PutIf(img, X, Y, mark);
        }
    }
}
