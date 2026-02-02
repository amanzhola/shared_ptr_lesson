#pragma once

#include "types.h"
#include "texture.h"

#include <memory>
#include <string>

enum class ShapeType { RECTANGLE, ELLIPSE };

class Shape {
public:
    Shape(std::string label, ShapeType type);
    ~Shape();

    void SetPosition(Point p) { pos_ = p; }
    void SetSize(Size s) { size_ = s; }
    void SetTextureOrigin(Point o) { tex_origin_ = o; }

    void SetTexture(std::shared_ptr<Texture> tex);
    void ResetTexture();

    const std::shared_ptr<Texture>& GetTexture() const { return texture_; }

    void Draw(Image& canvas) const;

private:
    static void PrintTexturePtr(const char* label, const std::shared_ptr<Texture>& p);

    std::string label_;
    ShapeType type_;
    Point pos_{0, 0};
    Size  size_{0, 0};
    std::shared_ptr<Texture> texture_;
    Point tex_origin_{0, 0};
};
