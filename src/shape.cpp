#include "shape.h"
#include <iostream>
#include <utility>

Shape::Shape(std::string label, ShapeType type)
    : label_(std::move(label)), type_(type) {
    std::cout << "🔷/🟣✅ Shape '" << label_ << "' created (this=" << this << ")\n";
}

Shape::~Shape() {
    std::cout << "🔷/🟣💀 Shape '" << label_ << "' destroyed (this=" << this << ")\n";
}

void Shape::PrintTexturePtr(const char* label, const std::shared_ptr<Texture>& p) {
    std::cout << "   🔎 \n" << label << ": ";
    if (!p) {
        std::cout << "nullptr (use_count=0)\n";
        return;
    }
    std::cout << "get()=" << p.get()
              << "  name=\"" << p->Name() << "\""
              << "  use_count=" << p.use_count()
              << "\n";
}
/*
void Shape::SetTexture(std::shared_ptr<Texture> tex) {
    std::cout << label_ << " 🔁 SetTexture() called\n";

    std::cout << "   🧠 ВАЖНО: сюда shared_ptr пришёл ПО ЗНАЧЕНИЮ => это КОПИЯ => use_count уже увеличился.\n";
    PrintTexturePtr("   входной параметр tex (КОПИЯ)", tex);

    PrintTexturePtr("   ДО: поле texture_", texture_);

    std::cout << "   🔁 Делаем MOVE: texture_ = std::move(tex)\n";
    texture_ = std::move(tex);

    PrintTexturePtr("   ПОСЛЕ: поле texture_", texture_);
    std::cout << "   🔻 После move параметр tex внутри функции станет nullptr (обычно)\n";
    PrintTexturePtr("   tex после move (внутри функции)", tex);

    std::cout << "   ✅ владение обновлено\n";
}
*/

void Shape::SetTexture(std::shared_ptr<Texture> tex) {
    // ОДНА строка внутри: параметр tex - это копия, поэтому use_count "временно" больше.
    std::cout << "   [ВНУТРИ] " << label_ << " ";
    if (!tex) {
        std::cout << "tex=nullptr\n";
    } else {
        std::cout << "tex.get()=" << tex.get()
                  << " use_count=" << tex.use_count()
                  << " name=\"" << tex->Name() << "\"\n";
    }

    // Реальная работа
    texture_ = std::move(tex);
}

void Shape::ResetTexture() {
    std::cout << label_ << " 🧹 ResetTexture() -> reset()\n";
    PrintTexturePtr("   ДО reset поле texture_", texture_);
    texture_.reset();
    PrintTexturePtr("   ПОСЛЕ reset поле texture_", texture_);
}

void Shape::Draw(Image& canvas) const {
    Size canvas_sz = GetImageSize(canvas);
    if (size_.width <= 0 || size_.height <= 0) return;

    Size tex_sz{0, 0};
    if (texture_) tex_sz = texture_->GetSize();

    for (int dy = 0; dy < size_.height; ++dy) {
        for (int dx = 0; dx < size_.width; ++dx) {
            bool inside = (type_ == ShapeType::RECTANGLE) ? true : IsPointInEllipse({dx, dy}, size_);
            if (!inside) continue;

            int x = pos_.x + dx;
            int y = pos_.y + dy;
            if (x < 0 || y < 0 || x >= canvas_sz.width || y >= canvas_sz.height) continue;

            if (texture_) {
                Point tp{tex_origin_.x + dx, tex_origin_.y + dy};
                if (tp.x < 0 || tp.y < 0 || tp.x >= tex_sz.width || tp.y >= tex_sz.height) {
                    canvas[y][x] = '.';
                } else {
                    canvas[y][x] = texture_->GetPixelColor(tp);
                }
            } else {
                canvas[y][x] = '.';
            }
        }
    }
}
