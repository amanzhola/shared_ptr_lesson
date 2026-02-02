#include "canvas.h"
#include "console_color.h"

Image Canvas::Render() const {
    Image img(size_.height, std::string(size_.width, ' '));
    for (const auto& s : shapes_) s->Draw(img);
    return img;
}

void Canvas::PrintColored() const {
    PrintColoredImage(Render());
}
