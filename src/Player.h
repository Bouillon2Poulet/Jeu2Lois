#pragma once
#include <string>
#include "Case.h"
#include "Console.h"
#include "drawing.h"
#include "globalVariable.h"
#include "internal.h"
#include "p6/p6.h"

class Player {
private:
    glm::vec2 _position;
    int       _indexCurrentCase = 0;
    // unsigned int     _indexCurrentCaseBeforeMovement = 0;
    int              _indexGoalCase = 0;
    int              _indexPlayer;
    p6::Color        _color;
    std::vector<int> _diceRolls;
    bool             _movementEnded   = true;
    int              _movement        = 0;
    int              _X6LawFailsCount = 0;

public:
    inline Player(const glm::vec2& position, unsigned int index)
        : _position(position), _indexPlayer(index)
    {
        switch (index)
        {
        case 1:
            _position += glm::vec2(.0, .03);
            _color = p6::NamedColor::BlueSapphire;
            break;
        case 2:
            _position += glm::vec2(-.03, -.03);
            _color = p6::NamedColor::PurpleHeart;
            break;
        case 3:
            _position += glm::vec2(.03, -.03);
            _color = p6::NamedColor::OrangeRed;
            break;
        default: _position += glm::vec2(); break;
        }
    }

    inline void draw(p6::Context& ctx) const
    {
        ctx.fill       = _color;
        ctx.use_stroke = false;
        ctx.circle(
            p6::Center(_position),
            p6::Radius(.01f)
        );
    }

    inline bool update(p6::Context& ctx, const std::vector<Case>& cases, const float caseWidth, Button& de)
    {
        Console::addToLastMessage("Case : " + std::to_string(_indexCurrentCase));
        Case currentCase = cases[_indexCurrentCase];
        if (_movementEnded && Turn::_endOfTurn == false)
        {
            _movement = lawTypeToFunction(ctx, de, currentCase.lawType(), _diceRolls, _indexCurrentCase, _X6LawFailsCount);
            std::cout << "\n??mov:" << _movement;
            _indexGoalCase += _movement;
        }
        Console::needToBeUpdated(false);
        // int movement = _diceRolls.empty() ? 0 : _diceRolls.back();
        return moveIfNecessary(caseWidth, _movement, cases);
    }

    inline void indexGoalCaseIncr(const int movement)
    {
        _indexGoalCase += movement;
    }

    inline bool moveIfNecessary(const float caseWidth, int movement, std::vector<Case> cases)
    {
        std::cout << "\n mov" << movement << std::endl;
        if (_indexGoalCase < 0)
        {
            std::cout << "?!?!?<<\n";
            _indexGoalCase = 0;
        }

        if (movement == 0)
        {
            return false;
        }
        if (_indexGoalCase >= MAPnbCases())
        {
            _indexGoalCase = MAPnbCases() - 1;
        }
        if (_indexCurrentCase == MAPnbCases() - 1) // Win
        {
            Win::_hasWin = std::pair<bool, int>(true, _indexPlayer);
            return false;
        }
        if (_indexCurrentCase != _indexGoalCase)
        {
            std::cout << movement << "//" << _indexGoalCase << "//" << _indexCurrentCase << std::endl;
            _movementEnded = false;
            if (sign(movement) > 0)
            {
                _position += glm::vec2(caseWidth * indexToVector(_indexCurrentCase));
                _indexCurrentCase++;
            }
            else
            {
                int index = (_indexCurrentCase == 0) ? 0 : _indexCurrentCase - 1;
                _position -= glm::vec2(caseWidth * indexToVector(index));
                _indexCurrentCase--;
            }
            if (_indexCurrentCase == _indexGoalCase)
            {
                Console::needToBeUpdated(true);
                Console::addMessage(std::pair<std::string, p6::Color>("Case " + std::to_string(_indexCurrentCase - movement) + " -> Case " + std::to_string(_indexGoalCase), p6::NamedColor::White));
                Turn::_endOfTurn = lawTypeToEndOfTurn(cases[_indexCurrentCase].lawType());
                if (Turn::_endOfTurn == false)
                {
                    // Console::deleteLastMessage();
                    // Console::addToLastMessage("AUTOMATIQUE");
                    Console::needToBeUpdated(false);
                }
                _movementEnded = true;
                _movement      = 0;
                std::cout << "?:?" << Turn::_endOfTurn;
                return true;
            }
        }

        Console::needToBeUpdated(false);
        return false;
    }

    inline bool hasWon() const
    {
        if (_indexCurrentCase >= MAPnbCases())
        {
            return true;
        }
        return false;
    }

    unsigned int indexCurrentCase()
    {
        return _indexCurrentCase;
    }

    std::vector<int> diceRolls()
    {
        return _diceRolls;
    }

    p6::Color color()
    {
        return _color;
    }
};