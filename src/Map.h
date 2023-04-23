#pragma once

#include "Button.h"
#include "Case.h"
#include "Player.h"
#include "internal.h"
#include "loi2proba.h"

class Map {
private:
    std::vector<Case>   _cases;
    std::vector<Player> _players;
    Button              _test;
    float               _caseWidth = .1f;
    float               _marging   = 1.f;

public:
    inline Map(const float aspectRatio)
        : _test("Test", {0, 0})
    {
        glm::vec2 startPosition(-aspectRatio + _marging + _caseWidth / 2.f); // Start
        glm::vec2 casePosition = startPosition;
        for (unsigned int i = 0; i < 78; i++)
        {
            _cases.emplace_back(casePosition, randomLawType());
            casePosition += _caseWidth * indexToVector(i);
        }

        for (unsigned int i = 0; i < 1; i++)
        {
            _players.emplace_back(startPosition);
        }
    }

    inline void draw(p6::Context& ctx)
    {
        p6::Image IMGgoose = p6::load_image("img/goosegame.jpeg");
        ctx.image(IMGgoose, p6::FitY());

        for (const auto& currentCase : _cases)
        {
            currentCase.draw(ctx, _caseWidth);
        }

        for (const auto& currentPlayer : _players)
        {
            currentPlayer.draw(ctx);
        }

        _test.draw(ctx);
    }

    inline void update(p6::Context& ctx)
    {
        if (_test.isClicked(ctx))
        {
            _players.front().indexGoalCaseIncr(5);
        }
        _players.front().update();
    }
};