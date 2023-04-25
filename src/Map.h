#pragma once

// #include <unistd.h>
#include <string>
#include "Button.h"
#include "Case.h"
#include "Console.h"
#include "Player.h"
#include "UI.h"
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
    unsigned int        _indexCurrentPlayer;
    std::vector<Button> _des = {Button("De1", {0, 0}), Button("De2", {0.7, .7})};

    UI      _ui{};
    Console _console{};

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
                _cases.emplace_back(casePosition, lawType::uniform);
            else
                _cases.emplace_back(casePosition, randomLawType());

            casePosition += _caseWidth * indexToVector(i);
        }

        for (unsigned int i = 1; i <= 3; i++)
        {
            _players.emplace_back(startPosition, i);
        }
    }

    inline void draw(p6::Context& ctx)
    {
        for (const auto& currentCase : _cases)
        {
            currentCase.draw(ctx, _caseWidth);
        }

        bool         win         = false;
        unsigned int playerIndex = 1;
        for (const auto& currentPlayer : _players)
        {
            currentPlayer.draw(ctx);
            win = currentPlayer.hasWon();
            if (win)
            {
                displayVictoryScreen(ctx, playerIndex);
                continue;
            }
            playerIndex++;
        }

        _ui.draw(ctx, _players, _indexCurrentPlayer);

        _console.draw(ctx);
    }

    inline void update(p6::Context& ctx)
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Tour du joueur " + std::to_string(_indexCurrentPlayer + 1) + " :", p6::NamedColor::White));
        if (_players[_indexCurrentPlayer].update(ctx, _cases, _caseWidth, _des))
        {
            Console::needToBeUpdated(true);
            Console::addMessage(std::pair<std::string, p6::Color>("/////////////", p6::NamedColor::White));
            nextPlayer();
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
};