#pragma once
#include <string>
#include "Player.h"
#include "glm/fwd.hpp"
#include "internal.h"
#include "p6/p6.h"
class UI {
private:
    glm::vec2 _size = {0.7, .4};
    glm::vec2 _position{};

public:
    inline UI() = default;
    inline UI(const float aspectRatio)
        : _position({aspectRatio / 2.f + 0.2, 0.4})
    {
    }

    inline void draw(p6::Context& ctx, const std::vector<Player> players, int indexCurrentPlayer)
    {
        drawWindow(ctx);
        int count = 0;
        for (const auto currentPlayer : players)
        {
            drawPlayerWindow(ctx, currentPlayer, count, indexCurrentPlayer);
            count++;
        }
    }

    inline void drawWindow(p6::Context& ctx)
    {
        // Fenêtre
        ctx.text_size = 0.03f;
        ctx.fill      = p6::Color(39.f / 255.f, 110.f / 255.f, 255.F / 255.f);
        ctx.rectangle(
            p6::Center(_position),
            p6::Radii(_size),
            p6::Rotation()
        );

        ctx.fill = p6::NamedColor::White;
        ctx.text(
            to_u16string("Joueurs"),
            p6::Center(_position + glm::vec2(0, _size.y - 0.1f)),
            p6::Rotation()
        );
    }

    inline void drawPlayerWindow(p6::Context& ctx, Player player, int count, int indexCurrentPlayer)
    {
        // Fenêtre
        glm::vec2 position(_position + glm::vec2(-0.43, -0.05) + (float)count * glm::vec2(0.43, 0));
        glm::vec2 size(0.2, .27);
        ctx.fill = p6::NamedColor::BlueBell;
        ctx.rectangle(
            p6::Center(position),
            p6::Radii(size),
            p6::Rotation()
        );

        ctx.text_size = 0.03f;
        ctx.fill      = p6::NamedColor::White;
        ctx.text( // NUMERO
            to_u16string(std::to_string(count + 1)),
            p6::TopLeftCorner(position + (glm::vec2(-1.f, 1.f) * 4.f * size / 5.f)),
            p6::Rotation()
        );

        if (indexCurrentPlayer == count)
        {
            ctx.use_fill   = false;
            ctx.use_stroke = true;
            ctx.stroke     = player.color();
            ctx.circle(
                p6::Center(position + (glm::vec2(-1.f, 1.f) * 4.f * size / 5.f) + glm::vec2(0.03, -0.03)),
                p6::Radius(0.05)
            );
            ctx.use_fill   = true;
        }

        ctx.text_size = 0.025f;
        ctx.text( // CASE
            to_u16string("Case:" + std::to_string(player.indexCurrentCase())),
            p6::TopLeftCorner(position + (glm::vec2(-1.f, 1.f) * 4.f * size / 5.f) + glm::vec2(0, -0.1)),
            p6::Rotation()
        );

        ctx.text( // DICEROLLS
            to_u16string("Res:"),
            p6::TopLeftCorner(position + (glm::vec2(-1.f, 1.f) * 4.f * size / 5.f) + glm::vec2(0, -0.2)),
            p6::Rotation()
        );

        int countRoll = 0;
        for (const auto roll : player.diceRolls())
        {
            ctx.text(
                to_u16string(std::to_string(roll) + ","),
                p6::TopLeftCorner(position + (glm::vec2(-1.f, 1.f) * 4.f * size / 5.f) + glm::vec2((float)(countRoll % 4) * 0.1, ((float)(countRoll / 4)) * (-0.05f) - 0.25)),
                p6::Rotation()
            );
            countRoll++;
        }
    }
};