#pragma once
#include <ostream>
#include <string>
#include "globalVariable.h"
#include "internal.h"
#include "p6/p6.h"

inline void displayVictoryScreen(p6::Context& ctx, int indexPlayer, const std::vector<int>& poissonLawResults)
{
    ctx.fill       = p6::NamedColor::Arsenic;
    ctx.use_stroke = true;
    ctx.rectangle(
        p6::Center(),
        p6::Radii(1, 0.5),
        p6::Rotation{}
    );

    ctx.text_size=0.04;
    int sum = 0;
    std::string sumAsString;
    for (auto result : poissonLawResults){
        sumAsString+=std::to_string(result)+"+";
        sum+=result;
    }

    std::string winMsg = " Le joueur " + std::to_string(indexPlayer) + " gagne !";
    ctx.fill           = p6::NamedColor::White;
    ctx.text(
        to_u16string(winMsg),
        p6::Center(0,0.2),
        p6::Rotation()
    );

    std::string msgNB = "Nb loi de poisson : " + std::to_string(poissonLawResults.size());
    ctx.fill           = p6::NamedColor::White;
    ctx.text(
        to_u16string(msgNB),
        p6::Center(0,0.1),
        p6::Rotation()
    );

    sumAsString.pop_back();
    std::string msgSum = "Somme = " + std::to_string(sum);
    ctx.fill           = p6::NamedColor::White;
    ctx.text(
        to_u16string(msgSum),
        p6::Center(0,0.0),
        p6::Rotation()
    );

    std::string msgVar = "Variance : " +std::to_string(poissonLawVariance());
    ctx.fill           = p6::NamedColor::White;
    ctx.text(
        to_u16string(msgVar),
        p6::Center(0,-0.1),
        p6::Rotation()
    );

    std::string msgMoy = "Moyenne : " +std::to_string((float)sum/(float)poissonLawResults.size());
    ctx.fill           = p6::NamedColor::White;
    ctx.text(
        to_u16string(msgMoy),
        p6::Center(0,-0.2),
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
