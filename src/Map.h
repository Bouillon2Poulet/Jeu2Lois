#pragma once

// #include <unistd.h>
#include <string>
#include "Button.h"
#include "Case.h"
#include "Console.h"
#include "Player.h"
#include "UI.h"
#include "drawing.h"
#include "globalVariable.h"
#include "internal.h"
#include "loi2proba.h"

class Map {
private:
    std::vector<Case>   _cases;
    std::vector<Player> _players;
    // Button              _test;
    float _caseWidth = .17f;
    // float               _marging   = 1.f;
    unsigned int         _indexCurrentPlayer;
    Button               _de{};
    std::pair<bool, int> _win = {false, -1};

    UI               _ui{};
    Console          _console{};
    std::vector<int> _poissonLawResults;

public:
    inline Map(const float aspectRatio)
        : _indexCurrentPlayer(p6::random::number(3)), _ui(aspectRatio)
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Le joueur " + std::to_string(_indexCurrentPlayer + 1) + " commence a jouer", p6::NamedColor::White));
        glm::vec2 startPosition(-aspectRatio + _caseWidth * 2.f, -0.8); // Start
        glm::vec2 casePosition = startPosition;
        for (unsigned int i = 0; i < MAPnbCases(); i++)
        {
            if (i == 0)
                _cases.emplace_back(casePosition, lawType::uniforme);
            else
                _cases.emplace_back(casePosition, randomLawType());

            casePosition += _caseWidth * indexToVector(i);
        }

        for (unsigned int i = 1; i <= 3; i++)
        {
            _players.emplace_back(startPosition, i);
        }
    }

    inline bool draw(p6::Context& ctx)
    {
        if (_win.first)
        {
            displayVictoryScreen(ctx, _win.second, _poissonLawResults);
            return false;
        }

        else
        {
            for (const auto& currentCase : _cases)
            {
                currentCase.draw(ctx, _caseWidth);
            }

            unsigned int playerIndex = 1;
            for (const auto& currentPlayer : _players)
            {
                currentPlayer.draw(ctx);
                _win = std::pair<bool, int>(currentPlayer.hasWon(), playerIndex);
                playerIndex++;
            }

            drawBorders(ctx);

            _ui.draw(ctx, _players, _indexCurrentPlayer);

            _console.draw(ctx);
            
            return true;
        }
    }

    inline void update(p6::Context& ctx)
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Tour du joueur " + std::to_string(_indexCurrentPlayer + 1) + " - ", _players[_indexCurrentPlayer].color()));
        _players[_indexCurrentPlayer].update(ctx, _cases, _caseWidth, _de, _poissonLawResults);
        if (Win::_hasWin.first)
        {
            displayVictoryScreen(ctx, Win::_hasWin.second, _poissonLawResults);
        }
        if (Turn::_endOfTurn)
        {
            Console::needToBeUpdated(true);
            Console::addMessage(std::pair<std::string, p6::Color>("", p6::NamedColor::White));
            nextPlayer();
            Turn::_endOfTurn = false;
        }
    }

    inline void nextPlayer()
    {
        _indexCurrentPlayer++;
        if (_indexCurrentPlayer >= 3)
        {
            _indexCurrentPlayer = 0;
        }
    }

    static inline void drawBorders(p6::Context& ctx)
    {
        glm::vec2              A(-1.67, -.715);
        glm::vec2              B(0.02, -.715);
        std::vector<glm::vec2> direction  = {{1., 0}, {0, 1}, {-1, 0}, {0, -1}};
        float                  baseLength = glm::length(B - A);
        std::vector<float>     length     = {baseLength * 0.805f, baseLength * 0.805f, baseLength * 0.705f, baseLength * 0.705f, baseLength * 0.605f, baseLength * 0.605f, baseLength * 0.502f, baseLength * 0.502f, baseLength * 0.4f, baseLength * 0.2f, 0};
        ctx.use_stroke                    = true;
        ctx.stroke                        = p6::NamedColor::Black;
        for (int i = 0; i < 11; i++)
        {
            ctx.line(A, B);
            // float length = glm::length(B - A);
            A = B;
            // float factor = (i % 2 == 0) ? 0.82f : 1.f;
            B = A + length[i] * direction[(i + 1) % 4];
        }

        A          = glm::vec2(-1.67, -.88);
        B          = glm::vec2(0.18, -.88);
        baseLength = glm::length(B - A);
        length     = {baseLength * 0.915f, baseLength * 0.915f, baseLength * 0.825f, 0};
        for (int i = 0; i < 4; i++)
        {
            ctx.line(A, B);
            // float length = glm::length(B - A);
            A = B;
            // float factor = (i % 2 == 0) ? 0.82f : 1.f;
            B = A + length[i] * direction[(i + 1) % 4];
        }
    }

    // static inline void endOfTurn(bool value)
    // {
    //     _endOfTurn = value;
    // }
};
