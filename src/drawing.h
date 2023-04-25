#pragma once
#include <string>
#include "internal.h"
#include "p6/p6.h"

inline void displayVictoryScreen(p6::Context& ctx, int indexPlayer)
{
    ctx.fill       = p6::NamedColor::Arsenic;
    ctx.use_stroke = true;
    ctx.rectangle(
        p6::Center(),
        p6::Radii(1, 0.5),
        p6::Rotation{}
    );

    std::string winMsg = " Le joueur " + std::to_string(indexPlayer) + " gagne !";
    ctx.fill           = p6::NamedColor::White;
    ctx.text(
        to_u16string(winMsg),
        p6::Center(),
        p6::Rotation()
    );
}

inline void displayPlayerTurn(p6::Context& ctx, unsigned int indexPlayer)
{
    std::string winMsg = " Au tour du joueur " + std::to_string(indexPlayer + 1);
    ctx.fill           = p6::NamedColor::White;
    ctx.text(
        to_u16string(winMsg),
        p6::Center(ctx.aspect_ratio() / 2.f + 0.2, -0.1),
        p6::Rotation()
    );
}
