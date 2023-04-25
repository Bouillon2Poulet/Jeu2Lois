#pragma once
#include <corecrt_math.h>
#include <cmath>
#include <string>
#include "Button.h"
#include "Console.h"
#include "internal.h"
#include "loi2proba.h"
#include "p6/p6.h"

enum class lawType {
    uniform,
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
    case lawType::uniform:
        return "uniform";
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
    return static_cast<lawType>((int)p6::random::number((float)1));
}

// LAWS
inline int X1(p6::Context& ctx, Button& de, std::vector<int>& diceRolls)
{
    Console::addMessage(std::pair<std::string, p6::Color>("Lancez le de svp", p6::NamedColor::White));
    de.draw(ctx);
    if (!de.isClicked(ctx))
    {
        return 0;
    }
    diceRolls.push_back(p6::random::number(1, 7));
    Console::needToBeUpdated(true);
    Console::addMessage(std::pair<std::string, p6::Color>("Resultat :" + std::to_string(diceRolls.back()), p6::NamedColor::White));
    return diceRolls.back();
}

inline int X2(const float lambda)
{
    // return exp(-lambda) * (pow(lambda, k) / fact(k)); //Première version un peu nulle
    unsigned int count = 0;
    float        sum   = 0.f;
    do
    {
        sum += (-1.f / lambda) * (std::log(p6::random::number()) / std::log(std::exp(1)));
        count++;
    } while (sum < 1.f);

    return count;
}

inline int X3(const float u1, const float u2) // TODO : lancer de dé ou automatique ?
{
    float mu    = 0.f;
    float sigma = 3.f;

    // Transformation de Box-Muller
    float z = std::sqrt(-2.f * std::log(u1)) * std::cos(2.f * p6::PI * u2);

    // Ajout de mu et multiplication par sigma pour obtenir une variable aléatoire normale centrée en mu et de variance sigma^2
    return static_cast<int>(mu + sigma * z + 0.5f); // On arrondit à l'entier le plus proche
}

inline int X4(const float p) // TODO lancé de dé -> p
{
    if (p6::random::number() <= p)         // l'action est réussie
        return p6::random::integer(1, 6);  // le joueur avance;
    else                                   // l'action est ratée
        return -p6::random::integer(1, 6); // le joueur recule;
}

inline int X5(const float df, const float mu, const float sigma) // TODO lancé de dé ou automatique ?
{
    // On calcule la variable aléatoire Z suivant une loi normale centrée réduite
    float Z = mu + sigma * std::sqrt(df) * (std::pow(p6::random::number(), -2.f / df) - 1.f) / std::sqrt(2.f / df);

    // On calcule la variable aléatoire T suivant une loi de Student
    float T = Z * std::sqrt(df / (p6::random::number() * (df - 2.f)));

    // On retourne le nombre de cases que le joueur avance
    return static_cast<int>(T);
}

inline int X6(const float p) // TODO lancé de dé
{
    int count = 0;
    while (true)
    {
        if (p6::random::number() <= p)
        { // seuil de réussite
            // Le joueur a réussi, on retourne le nombre d'essais
            return count;
        }
        else
        {
            // Le joueur n'a pas réussi, on incrémente le nombre d'essais et on continue
            count++;
        }
    }
}

inline bool X7(const std::vector<int>& diceRolls)
{
    int numRolls = diceRolls.size();
    if (numRolls < 10)
    {
        return 5; // Le joueur peut avancer -> position + lancer de dé
    }
    else
    {
        return -5; // Le joueur recule de 5 cases -> position  - 5
    }
}

inline int X8(int indexCurrentCase)
{
    std::vector<std::vector<double>> transitionMatrix = {
        {0.2, 0.2, 0.2, 0.2, 0.2, 0, 0, 0},
        {0.2, 0.2, 0.2, 0.2, 0, 0.2, 0, 0},
        {0.2, 0.2, 0.2, 0, 0, 0, 0.2, 0.2},
        {0.2, 0.2, 0, 0, 0, 0, 0.2, 0.4},
        {0.2, 0, 0, 0, 0, 0, 0.4, 0.4},
        {0, 0.2, 0, 0, 0, 0, 0.4, 0.4},
        {0, 0, 0.2, 0, 0, 0.2, 0, 0.6},
        {0, 0, 0.2, 0, 0.2, 0.2, 0, 0.4}};

    // On récupère la probabilité de chaque événement possible

    std::vector<double> probs = transitionMatrix[indexCurrentCase % transitionMatrix.size()];

    // On utilise la chaîne de Markov pour calculer la probabilité cumulée d'arriver à chaque état
    std::vector<double> cumProbs(probs.size());
    cumProbs[0] = probs[0];
    for (int i = 1; i < probs.size(); i++)
    {
        cumProbs[i] = cumProbs[i - 1] + probs[i];
    }

    // On tire un nombre aléatoire entre 0 et 1 pour déterminer l'événement qui se produit
    double r = p6::random::number();
    for (int i = 0; i < cumProbs.size(); i++)
    {
        if (r <= cumProbs[i])
        {
            // Si le nombre aléatoire est inférieur à la probabilité cumulée d'arriver à cet état, on retourne cet état
            return i;
        }
    }

    // Si on ne trouve pas d'état, on retourne -1
    return -1;
}

int lawTypeToFunction(p6::Context& ctx, std::vector<Button>& des, lawType type, std::vector<int>& diceRolls, int indexCurrentCase)
{
    Console::addMessage(std::pair<std::string, p6::Color>("Loi : " + lawTypeToString(type), p6::NamedColor::White));

    switch (type)
    {
    case lawType::uniform: // DÉ
        return X1(ctx, des[0], diceRolls);
        break;
    case lawType::poisson: // AUTOMATIQUE
        return X2(3.f);
        break;
    case lawType::normale: // 2 lancés de dés pour la moyenne
        return X3(X1(ctx, des[0], diceRolls), X1(ctx, des[1], diceRolls));
        break;
    case lawType::bernoulli:
        return X4(X1(ctx, des[0], diceRolls));
        break;
    case lawType::student:
        return X5(5.f, 0, 3.f);
        break;
    case lawType::geometrique:
        return X6(X1(ctx, des[0], diceRolls));
        break;
    case lawType::booleenne:
        return X7(diceRolls);
        break;
    case lawType::discrete:
        return X8(indexCurrentCase);
        break;
        // default:
        //     return "unknown";
    }
}