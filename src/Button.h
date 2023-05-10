#pragma once
#include "internal.h"
#include "p6/p6.h"

class Button {
private:
    p6::Radii _size = p6::Radii(.2, .1);
    glm::vec2 _position{};
    p6::Color _color = p6::NamedColor::Black;
    // float       _lastTimeClicked;
    bool _canBeClicked = true;

public:
    inline bool mouseIsOnButton(const glm::vec2& mousePosition)
    {
        if (mousePosition.y < _position.y + _size.value.y / 2 && mousePosition.y > _position.y - _size.value.y / 2)
        {
            // std::cout << "!!";
            if (mousePosition.x < _position.x + _size.value.x / 2 && mousePosition.x > _position.x - _size.value.x / 2)
            {
                // std::cout << "??";
                return true;
            }
        }
        return false;
    }
    inline bool isClicked(p6::Context& ctx)
    {
        if (_canBeClicked)
        {
            if (ctx.mouse_button_is_pressed(p6::Button(0)) && mouseIsOnButton(ctx.mouse()))
            {
                // std::cout << "Button Clicked\n";
                _canBeClicked = false;
                return true;
            }
        }
        if (!_canBeClicked) // RELEASE
        {
            if (!ctx.mouse_button_is_pressed(p6::Button(0)))
            {
                // std::cout << "!!" << std::endl;
                _canBeClicked = true;
            }
        }
        return false;
    }

    inline Button() = default;

    inline Button(p6::Radii size, p6::Color _color)
        : _size(size), _color(_color)
    {
    }

    inline Button(glm::vec2 position)
        : _position(position){};

    inline void position(glm::vec2 position)
    {
        _position = position;
    }

    inline void draw(p6::Context& ctx, std::string text) const
    {
        ctx.fill       = _color;
        ctx.use_stroke = true;
        ctx.rectangle(
            p6::Center(_position),
            _size,
            p6::Rotation{}
        );

        ctx.fill = p6::NamedColor::White;
        ctx.text(
            to_u16string(text),
            p6::Center(_position),
            p6::Rotation()
        );
    }
};