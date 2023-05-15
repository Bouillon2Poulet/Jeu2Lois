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

    inline bool update(p6::Context& ctx, const std::vector<Case>& cases, const float caseWidth, Button& de, std::vector<int>& poissonLawResults)
    {
        Console::addToLastMessage("Case : " + std::to_string(_indexCurrentCase));
        Case currentCase = cases[_indexCurrentCase];
        if (_movementEnded && Turn::_endOfTurn == false) //Si il a fini de bouger et que ce n'est pas encore la fin de son tour
        {
            //Le mouvement est égal à ce que nous retourne la loi de probabilité
            _movement = lawTypeToFunction(ctx, de, currentCase.lawType(), _diceRolls, _indexCurrentCase, _X6LawFailsCount, poissonLawResults);
            _indexGoalCase += _movement; //La case cible s'incrémente du résultat de la loi
        }
        Console::needToBeUpdated(false);
        return moveIfNecessary(caseWidth, _movement, cases); //Si il y a eu un changement dans la case cible, on bouge
    }

    inline void indexGoalCaseIncr(const int movement)
    {
        _indexGoalCase += movement;
    }

    inline bool moveIfNecessary(const float caseWidth, int movement, std::vector<Case> cases)
    {
        if (_indexGoalCase < 0) //Si le joueur dépasse le début de la carte
        {
            _indexGoalCase = 0;
        }
        if (movement == 0) //Si on ne bouge pas
        {
            return false;
        }
        if (_indexGoalCase >= MAPnbCases()) //Si le joueur dépasse la fin de la carte
        {
            _indexGoalCase = MAPnbCases() - 1;
        }
        if (_indexCurrentCase == MAPnbCases() - 1) // Si le joueur gagne
        {
            Win::_hasWin = std::pair<bool, int>(true, _indexPlayer);
            return false;
        }
        if (_indexCurrentCase != _indexGoalCase) //Si il y a une différence entre la case actuelle du joueur et sa case cible, on le déplace
        {
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
            if (_indexCurrentCase == _indexGoalCase) //La case sur laquelle finit le joueur est-elle automatique ?
            {
                Console::needToBeUpdated(true);
                Console::addMessage(std::pair<std::string, p6::Color>("Case " + std::to_string(_indexCurrentCase - movement) + " -> Case " + std::to_string(_indexGoalCase), p6::NamedColor::White));
                Turn::_endOfTurn = lawTypeToEndOfTurn(cases[_indexCurrentCase].lawType());
                if (Turn::_endOfTurn == false)
                {

                    Console::needToBeUpdated(false);
                }
                _movementEnded = true;
                _movement      = 0;
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