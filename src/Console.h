#pragma once

#include <utility>
#include <vector>
#include "Button.h"
#include "glm/fwd.hpp"
#include "internal.h"
#include "p6/p6.h"

class Console {
private:
    static std::vector<std::pair<std::string, p6::Color>> _messages;
    static bool                                           _needToBeUpdated;
    bool                                                  _isBig   = false;
    std::vector<glm::vec2>                                _size    = {{0.7, .4}, {0.7, .9}};
    Button                                                _makeBig = Button(p6::Radii(.05, .05), p6::NamedColor::Black);

public:
    inline Console(){};

    inline static void addMessage(const std::pair<std::string, p6::Color>& message)
    {
        if (!_needToBeUpdated)
            return;
        _messages.push_back(message);
    }

    inline static void addToLastMessage(std::string msg)
    {
        if (!_needToBeUpdated)
            return;
        _messages.back().first += msg;
    }

    inline static void deleteLastMessage()
    {
        if (!_needToBeUpdated)
            return;
        _messages.pop_back();
    }

    inline void draw(p6::Context& ctx)
    {
        drawWindow(ctx);
        drawMessages(ctx);
    }

    inline void drawWindow(p6::Context& ctx)
    {
        glm::vec2 buttonPos = (_isBig) ? glm::vec2((ctx.aspect_ratio() / 3.f)-0.08, 0.8) : glm::vec2((ctx.aspect_ratio() / 3.f)-0.08, -0.2);
        _makeBig.position(buttonPos);
        // Fenêtre
        ctx.text_size        = 0.03f;
        ctx.fill             = p6::NamedColor::Black;
        glm::vec2 actualSize = (_isBig) ? _size[1] : _size[0];
        ctx.rectangle(
            p6::BottomLeftCorner(glm::vec2(ctx.aspect_ratio() / 2.f + 0.2, -0.5) - _size[0]),
            p6::Radii(actualSize),
            p6::Rotation()
        );

        glm::vec2 titlePosition = (_isBig) ? glm::vec2(ctx.aspect_ratio() / 2.f + 0.2, 0.8) : glm::vec2(ctx.aspect_ratio() / 2.f + 0.2, -0.2);
        ctx.fill                = p6::NamedColor::White;
        ctx.text(
            to_u16string("Console"),
            p6::Center(titlePosition),
            p6::Rotation()
        );

        _makeBig.draw(ctx, "+");
        if (_makeBig.isClicked(ctx))
        {
            _isBig = !_isBig;
        }
    }

    inline void drawMessages(p6::Context& ctx)
    {
        // std::cout<<
        ctx.text_size   = 0.015f;
        int count       = 0;
        int maxMessages = (_isBig) ? 40 : 15;
        for (int i = _messages.size() - 1; i >= std::max(0, (int)_messages.size() - maxMessages); i--)
        {
            if (!isWhite(_messages[i].second))
                ctx.fill = _messages[i].second;
            ctx.text(
                to_u16string(_messages[i].first),
                p6::BottomLeftCorner(glm::vec2((ctx.aspect_ratio() / 2.f) - 0.45, -0.85) + (float)count * glm::vec2(0, 0.04)),
                p6::Rotation()
            );
            count++;
        }
    }

    bool needToBeUpdated()
    {
        return _needToBeUpdated;
    }

    static void needToBeUpdated(bool value)
    {
        _needToBeUpdated = value;
    }

    inline static std::string lastMessage()
    {
        return _messages.back().first;
    }

    inline bool isWhite(p6::Color color)
    {
        if (color.r() == 0)
        {
            if (color.g() == 0)
            {
                if (color.b() == 0)
                {
                    return true;
                }
            }
        }
        return false;
    }
};

std::vector<std::pair<std::string, p6::Color>> Console::_messages = {};

bool Console::_needToBeUpdated = true;
