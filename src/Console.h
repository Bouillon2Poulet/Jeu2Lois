#pragma once

#include <utility>
#include <vector>
#include "glm/fwd.hpp"
#include "internal.h"
#include "p6/p6.h"

class Console {
private:
    static std::vector<std::pair<std::string, p6::Color>> _messages;
    static bool                                           _needToBeUpdated;

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
        // Fenêtre
        ctx.text_size = 0.03f;
        ctx.fill      = p6::NamedColor::Black;
        ctx.rectangle(
            p6::Center(ctx.aspect_ratio() / 2.f + 0.2, -0.5),
            p6::Radii(0.7, .4),
            p6::Rotation()
        );

        ctx.fill = p6::NamedColor::White;
        ctx.text(
            to_u16string("Console"),
            p6::Center(ctx.aspect_ratio() / 2.f + 0.2, -0.2),
            p6::Rotation()
        );
    }

    inline void drawMessages(p6::Context& ctx)
    {
        // std::cout<<
        ctx.text_size = 0.015f;
        int count     = 0;
        for (int i = _messages.size() - 1; i >= std::max(0, (int)_messages.size() - 30); i--)
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
