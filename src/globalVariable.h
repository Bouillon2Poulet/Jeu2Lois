#pragma once

#include <utility>

unsigned int MAPnbCases()
{
    return 78;
}

static struct Turn {
    static bool _endOfTurn;
};

bool Turn::_endOfTurn = false;

static struct Win {
    static std::pair<bool, int> _hasWin;
};

std::pair<bool, int> Win::_hasWin = {false, -1};