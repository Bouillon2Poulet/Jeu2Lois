#pragma once
//#include <corecrt_math.h>
#include <cmath>
#include <string>
#include <utility>
#include "Button.h"
#include "Console.h"
#include "globalVariable.h"
#include "internal.h"
#include "loi2proba.h"
#include "p6/p6.h"

enum class lawType {
    uniforme,
    poisson,
    normale,
    bernoulli,
    student,
    geometrique,
    booleenne,
    discrete,

    SIZE,
};

inline std::string lawTypeToString(lawType type)
{
    switch (type)
    {
    case lawType::uniforme:
        return "uniforme";
    case lawType::poisson:
        return "poisson";
    case lawType::normale:
        return "normale";
    case lawType::bernoulli:
        return "bernoulli";
    case lawType::student:
        return "student";
    case lawType::geometrique:
        return "geometrique";
    case lawType::booleenne:
        return "booleenne";
    case lawType::discrete:
        return "discrete";
    default:
        return "unknown";
    }
}

inline lawType randomLawType()
{
    return static_cast<lawType>((int)p6::random::number((float)8));
}

// LAWS
inline int X1(p6::Context& ctx, Button& de, std::vector<int>& diceRolls)
{
    Console::addMessage(std::pair<std::string, p6::Color>("Lancez le de svp", p6::NamedColor::White));
    de.draw(ctx, "de");
    if (!de.isClicked(ctx))
    {
        return 0;
    }
    diceRolls.push_back(p6::random::number(1, 7));
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat :" + std::to_string(diceRolls.back()), p6::NamedColor::White));
    return diceRolls.back();
}

inline int X2(const float lambda) // AUTO
{
    // return exp(-lambda) * (pow(lambda, k) / fact(k)); //Première version un peu nulle
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("(POISSON : AUTOMATIQUE)", p6::NamedColor::White));

    unsigned int count = 0;
    float        sum   = 0.f;
    do
    {
        sum += (-1.f / lambda) * (std::log(p6::random::number()) / std::log(std::exp(1)));
        count++;
    } while (sum < 1.f);

    std::cout << count << std::endl;
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat :" + std::to_string(count), p6::NamedColor::White));
    return count;
}

inline int X3(p6::Context& ctx, Button& de, std::vector<int>& diceRolls) // TODO : 2 lancés de dé
{
    /*
    Console::addMessage(std::pair<std::string, p6::Color>("Veuillez lance 2 fois le de", p6::NamedColor::White));

    de.draw(ctx, "2 de");
    if (!de.isClicked(ctx))
    {
        return 0;
    }
    Console::needToBeUpdated(true);


    int u1 = (p6::random::number(1, 7));
    int u2 = (p6::random::number(1, 7));
    diceRolls.push_back(u1);
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat 1 :" + std::to_string(diceRolls.back()), p6::NamedColor::White));
    diceRolls.push_back(u2);
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat 2 :" + std::to_string(diceRolls.back()), p6::NamedColor::White));
    Console::needToBeUpdated(false);
    //return diceRolls.back();
*/
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("(NORMALE : AUTOMATIQUE)", p6::NamedColor::White));


    // Génération de deux nombres aléatoires indépendants suivant une loi uniforme sur ]0,1[
    float u1 = p6::random::number() + std::numeric_limits<float>::epsilon();
    float u2 = p6::random::number();
    Console::needToBeUpdated(false);

    float mu    = 0.f;
    float sigma = 3.f;

    // Transformation de Box-Muller
    float z = std::sqrt(-2.f * std::log(u1)) * std::cos(2.f * p6::PI * u2);

    // Ajout de mu et multiplication par sigma pour obtenir une variable aléatoire normale centrée en mu et de variance sigma^2
    return static_cast<int>(mu + sigma * z + 0.5f); // On arrondit à l'entier le plus proche
}

inline int X4(p6::Context& ctx, Button& de) // PIECE
{
    Console::addMessage(std::pair<std::string, p6::Color>("Veuillez lance la piece", p6::NamedColor::White));

    de.draw(ctx, "piece");
    if (!de.isClicked(ctx))
    {
        return 0;
    }
    Console::needToBeUpdated(true);
    float       p         = 0.5; // Probabilité réussite ou échec
    float       result    = p6::random::number();
    std::string resultStr = (result < p) ? "pile" : "face";
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat: " + resultStr, p6::NamedColor::White));

    int move = p6::random::integer(1, 6);
    if (result <= p) // l'action est réussie
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Reussite : + " + std::to_string(move) + "cases", p6::NamedColor::White));
        return move;
    }    // le joueur avance;
    else // l'action est ratée
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Echec : - " + std::to_string(move) + "cases", p6::NamedColor::White));
        return -move;
    } // le joueur recule;
}

inline int X5(const float df, const float mu, const float sigma) // AUTO
{
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("(STUDENT : AUTOMATIQUE)", p6::NamedColor::White));

    // On calcule la variable aléatoire Z suivant une loi normale centrée réduite
    float Z = mu + sigma * std::sqrt(df) * (std::pow(p6::random::number(), -2.f / df) - 1.f) / std::sqrt(2.f / df);

    // On calcule la variable aléatoire T suivant une loi de Student
    float T = Z * std::sqrt(df / (p6::random::number() * (df - 2.f)));

    // On retourne le nombre de cases que le joueur avance
    return static_cast<int>(T);
}

inline int X6(p6::Context& ctx, Button& de, int& X6LawFailsCount) // PIECE
{
    float result = 0;

    Console::addMessage(std::pair<std::string, p6::Color>("Veuillez lance la piece", p6::NamedColor::White));

    float p = 0.5;
    de.draw(ctx, "piece");
    if (!de.isClicked(ctx))
    {
        return 0;
    }
    result = p6::random::number();
    X6LawFailsCount++;
    std::string resultStr = (result < p) ? "pile" : "face";
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat: " + resultStr, p6::NamedColor::White));

    if (result <= p) // l'action est réussie
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Reussite, le joueur avance du nombre de lance !", p6::NamedColor::White));
        Console::addMessage(std::pair<std::string, p6::Color>("nombre de lance : " + std::to_string(X6LawFailsCount), p6::NamedColor::White));

        int count       = X6LawFailsCount;
        X6LawFailsCount = 0;
        return count;
    }    // le joueur avance;
    else // l'action est ratée
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Echec, rdv au prochain tour !", p6::NamedColor::White));
        Console::addMessage(std::pair<std::string, p6::Color>("nombre de lance : " + std::to_string(X6LawFailsCount), p6::NamedColor::White));
        Turn::_endOfTurn = true; // A enlever si on veut que le joueur lance plusieurs fois au même tour
        return 0;
    } // le joueur recule;
}

inline int X7(const std::vector<int>& diceRolls) // AUTO
{
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("(BOOLENNE : AUTOMATIQUE)", p6::NamedColor::White));
    Console::addMessage(std::pair<std::string, p6::Color>("Nombre de lance de de :" + std::to_string(diceRolls.size()), p6::NamedColor::White));

    int numRolls   = diceRolls.size();
    int nbLanceMin = 10;
    if (numRolls > nbLanceMin)
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Réussite : " + std::to_string(diceRolls.size()) + ">" + std::to_string(nbLanceMin), p6::NamedColor::White));
        return nbLanceMin / 2; // Le joueur peut avancer -> position + lancer de dé
    }
    else
    {
        Console::addMessage(std::pair<std::string, p6::Color>("Echec : " + std::to_string(diceRolls.size()) + "<" + std::to_string(nbLanceMin), p6::NamedColor::White));
        std::cout << "BOOLENNE RESULT" << -1 * (nbLanceMin / 2);
        return -1 * (nbLanceMin / 2); // Le joueur recule de 5 cases -> position  - 5
    }
}

inline int X8(p6::Context& ctx, Button& de, int indexCurrentCase)
{
    std::vector<std::vector<double>> transitionMatrix = {
        {0.2, 0.2, 0.2, 0.2, 0.2, 0, 0, 0, 0, 0},
        {0.2, 0.2, 0.2, 0.2, 0, 0.2, 0, 0, 0, 0},
        {0.2, 0.2, 0.2, 0, 0, 0, 0, 0, 0.2, 0.2},
        {0.2, 0.2, 0, 0, 0, 0, 0, 0, 0.2, 0.4},
        {0.2, 0, 0, 0, 0, 0, 0, 0, 0.4, 0.4},
        {0, 0, 0.2, 0, 0, 0, 0, 0, 0.4, 0.4},
        {0, 0, 0, 0.2, 0, 0, 0.2, 0, 0, 0.6},
        {0, 0, 0, 0.2, 0, 0.2, 0.2, 0, 0, 0.4}};

    // On récupère la probabilité de chaque événement possible

    std::vector<double> probs = transitionMatrix[indexCurrentCase % transitionMatrix.size()];

    // On utilise la chaîne de Markov pour calculer la probabilité cumulée d'arriver à chaque état
    std::vector<double> cumProbs(probs.size());
    cumProbs[0] = probs[0];
    for (int i = 1; i < probs.size(); i++)
    {
        cumProbs[i] = cumProbs[i - 1] + probs[i];
    }

    Console::addMessage(std::pair<std::string, p6::Color>("Veuillez lance la piece", p6::NamedColor::White));
    de.draw(ctx, "piece");
    if (!de.isClicked(ctx))
    {
        return 0;
    }
    float       result    = p6::random::number();
    std::string resultStr = std::to_string(result);
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat: " + resultStr, p6::NamedColor::White));

    // On tire un nombre aléatoire entre 0 et 1 pour déterminer l'événement qui se produit
    double r = p6::random::number();
    for (int i = 0; i < cumProbs.size(); i++)
    {
        if (r <= cumProbs[i])
        {
            // Si le nombre aléatoire est inférieur à la probabilité cumulée d'arriver à cet état, on retourne cet état
            Console::addMessage(std::pair<std::string, p6::Color>("Reussite: " + resultStr + "<" + std::to_string(cumProbs[i]), p6::NamedColor::White));
            return i;
        }
    }

    // Si on ne trouve pas d'état, on recule de 5 cases
    return -5;
}

int lawTypeToFunction(p6::Context& ctx, Button& de, lawType type, std::vector<int>& diceRolls, int indexCurrentCase, int& X6LawFailsCount)
{
    // Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("Loi : " + lawTypeToString(type), p6::NamedColor::White));

    switch (type)
    {
    case lawType::uniforme: // DÉ
        return X1(ctx, de, diceRolls);
        break;
    case lawType::poisson: // AUTOMATIQUE
        return X2(3.f);
        break;
    case lawType::normale: // 2 lancés de dés pour la moyenne
        return X3(ctx, de, diceRolls);
        break;
    case lawType::bernoulli:
        return X4(ctx, de);
        break;
    case lawType::student:
        return X5(10.f, 0, 3.f);
        break;
    case lawType::geometrique:
        return X6(ctx, de, X6LawFailsCount);
        break;
    case lawType::booleenne:
        return X7(diceRolls);
        break;
    case lawType::discrete:
        return X8(ctx, de, indexCurrentCase);
        break;
        // default:
        //     return "unknown";
    }
}

bool lawTypeToEndOfTurn(lawType type)
{
    switch (type)
    {
    case lawType::uniforme:
        return true;
    case lawType::poisson:
        return false;
    case lawType::normale:
        return true;
    case lawType::bernoulli:
        return true;
    case lawType::student:
        return true;
    case lawType::geometrique:
        return true;
    case lawType::booleenne:
        return false;
    case lawType::discrete:
        return true;
    }
}