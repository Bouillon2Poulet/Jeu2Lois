#pragma once

#include <functional>
#include "loi2proba.h"
#include "p6/p6.h"

class Case {
private:
    glm::vec2 _position{};
    p6::Color _color{};
    lawType   _type;

public:
    inline Case(const glm::vec2& position, const lawType& type)
        : _position(position), _type(type)
    {
        switch (_type)
        {
        case lawType::poisson: _color = p6::NamedColor::Blue; break;
        case lawType::uniform: _color = p6::NamedColor::Yellow; break;
        case lawType::normale: _color = p6::NamedColor::GreenYellow; break;
        case lawType::bernoulli: _color = p6::NamedColor::PaleGreen; break;
        case lawType::student: _color = p6::NamedColor::BurntOrange; break;
        case lawType::geometrique: _color = p6::NamedColor::GreenMunsell; break;
        case lawType::booleenne: _color = p6::NamedColor::Cyan; break;
        case lawType::discrete: _color = p6::NamedColor::BrilliantLavender; break;
        default: _color = p6::NamedColor::Red; break;
        }
    }

    inline void draw(p6::Context& ctx, float width) const
    {
        ctx.fill       = _color;
        ctx.use_stroke = true;
        ctx.square(
            p6::Center(_position),
            p6::Radius(width / 2),
            p6::Rotation{}
        );
    }

    inline lawType lawType()
    {
        return _type;
    }
};