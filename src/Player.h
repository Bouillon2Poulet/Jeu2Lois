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
    glm::vec2    _position;
    unsigned int _indexCurrentCase = 0;
    // unsigned int     _indexCurrentCaseBeforeMovement = 0;
    unsigned int     _indexGoalCase = 0;
    p6::Color        _color;
    std::vector<int> _diceRolls;

public:
    inline Player(const glm::vec2& position, unsigned int index)
        : _position(position)
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

    inline bool update(p6::Context& ctx, const std::vector<Case>& cases, const float caseWidth, std::vector<Button>& des)
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Case : " + std::to_string(_indexCurrentCase), p6::NamedColor::White));
        Case currentCase = cases[_indexCurrentCase];
        _indexGoalCase += lawTypeToFunction(ctx, des, currentCase.lawType(), _diceRolls, _indexCurrentCase);
        int movement = _diceRolls.empty() ? 0 :  _diceRolls.back();
        return moveIfNecessary(caseWidth, movement);
    }

    inline void indexGoalCaseIncr(const int movement)
    {
        _indexGoalCase += movement;
    }

    inline bool moveIfNecessary(const float caseWidth, int movement)
    {
        if (_indexCurrentCase == MAPnbCases()) // Win
        {
            return false;
        }
        if (_indexCurrentCase != _indexGoalCase)
        {
            _position += glm::vec2(caseWidth * indexToVector(_indexCurrentCase));
            _indexCurrentCase++;
            if (_indexCurrentCase == _indexGoalCase)
            {
                Console::needToBeUpdated(true);
                Console::addMessage(std::pair<std::string, p6::Color>("Case " + std::to_string(_indexCurrentCase - movement) + " -> Case " + std::to_string(_indexGoalCase), p6::NamedColor::White));
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