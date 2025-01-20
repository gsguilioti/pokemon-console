#pragma once

#include <string>

#include "enums.h"
#include "player.h"

class Duel {
public:
    enum player currentTurn;
    std::shared_ptr<Player> playerOne;
    std::shared_ptr<Player> playerTwo;

    Duel(Player one, Player two) : 
        currentTurn(PLAYER_ONE), 
        playerOne(std::make_shared<Player>(one)),
        playerTwo(std::make_shared<Player>(two)) 
    {}

    enum player get_current_turn() const 
    {
        return currentTurn;
    };

    void end_turn() 
    {
        currentTurn = (currentTurn == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    };

    void start();

    std::string perform_action(int player, const std::string& action);
};
