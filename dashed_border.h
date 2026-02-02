#pragma once
#include "types.h"

bool Dash(int x, int y);
void PutIf(Image& img, int x, int y, char c);

void DrawRectBorderDashed(Image& img, Point o, Size s, char mark='R');
void DrawEllipseBorderDashed(Image& img, Point o, Size s, char mark='R');
