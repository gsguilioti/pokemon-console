#pragma once

#include <string>
#include <memory>
#include "enums.h"
#include "player.h"

class Duel {
public:
    std::shared_ptr<Player> playerOne;
    std::shared_ptr<Player> playerTwo;

    Duel(std::shared_ptr<Player> one, std::shared_ptr<Player> two) : 
        playerOne(one),
        playerTwo(two) 
    {}

    void start();
};
