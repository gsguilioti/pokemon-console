#pragma once

#include <string>

#include "enums.h"

class Duel {
public:
    enum player currentTurn;

    Duel() : currentTurn(PLAYER_ONE) {}

    enum player get_current_turn() const 
    {
        return currentTurn;
    }

    void end_turn() 
    {
        currentTurn = (currentTurn == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    }

    std::string perform_action(int player, const std::string& action);
};
