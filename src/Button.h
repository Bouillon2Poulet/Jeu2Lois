#pragma once
#include "internal.h"
#include "p6/p6.h"

class Button {
private:
    p6::Radii   _size = p6::Radii(.2, .1);
    std::string _text;
    glm::vec2   _position;

public:
    inline bool mouseIsOnButton(const glm::vec2& mousePosition)
    {
        if (mousePosition.y < _position.y + _size.value.y / 2 && mousePosition.y > _position.y - _size.value.y / 2)
        {
            if (mousePosition.x < _position.y + _size.value.x / 2 && mousePosition.x > _position.y - _size.value.x / 2)
            {
                return true;
            }
        }
        return false;
    }
    inline bool isClicked(p6::Context& ctx)
    {
        if (ctx.mouse_button_is_pressed(p6::Button(0)) && mouseIsOnButton(ctx.mouse()))
        {
            std::cout << "Button Clicked\n";
            return true;
        }
        return false;
    }

    inline Button(const std::string text, const glm::vec2& position)
        : _text(text), _position(position) {}

    inline void draw(p6::Context& ctx) const
    {
        ctx.fill       = p6::NamedColor::Arsenic;
        ctx.use_stroke = true;
        ctx.rectangle(
            p6::Center(_position),
            _size,
            p6::Rotation{}
        );

        ctx.fill = p6::NamedColor::White;
        ctx.text(
            to_u16string(_text),
            p6::Center(_position),
            p6::Rotation()
        );
    }
};