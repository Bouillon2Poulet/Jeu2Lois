#pragma once

#include "p6/p6.h"

class Case {
private:
    float     _width = 1.f;
    glm::vec2 _position{};
    p6::Color _color{};

public:
    void draw(p6::Context& ctx)
    {
        ctx.fill = _color;
        ctx.square(
            p6::Center(_position),
            p6::Radius(_width / 2),
            p6::Rotation{}
        )
    }
}