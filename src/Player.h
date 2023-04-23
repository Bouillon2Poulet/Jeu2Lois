#pragma once
#include "internal.h"
#include "p6/p6.h"

class Player {
private:
    glm::vec2    _position;
    unsigned int _indexCurrentCase = 0;
    unsigned int _indexGoalCase    = 0;

public:
    inline Player(const glm::vec2& position)
        : _position(position) {}

    inline void draw(p6::Context& ctx) const
    {
        ctx.fill       = p6::NamedColor::Red;
        ctx.use_stroke = false;
        ctx.circle(
            p6::Center(_position),
            p6::Radius(.01f)
        );
    }

    inline void update()
    {
        if (_indexCurrentCase != _indexGoalCase)
        {
            _position += glm::vec2(indexToVector(_indexCurrentCase));
            _indexCurrentCase++;
        }
    }

    inline void indexGoalCaseIncr(const int movement)
    {
        _indexGoalCase += movement;
    }
};