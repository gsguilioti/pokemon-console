#pragma once

#include <string>
#include <memory>
#include "enums.h"
#include "player.h"

class Duel {
public:
    std::shared_ptr<Player> playerOne;
    std::shared_ptr<Player> playerTwo;

    Duel(Player one, Player two) : 
        playerOne(std::make_shared<Player>(one)),
        playerTwo(std::make_shared<Player>(two)) 
    {}

    void start();

    std::string perform_action(int player, const std::string& action);
};
