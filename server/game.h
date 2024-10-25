#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <mutex>

#include "enums.h"

class Game {
public:
    uint16_t players;
    enum player currentPlayer;

    Game() : currentPlayer(PLAYER_ONE) {}

    enum player get_current_player() const 
    {
        return currentPlayer;
    }

    void end_turn() 
    {
        currentPlayer = (currentPlayer == PLAYER_ONE) ? PLAYER_TWO : PLAYER_ONE;
    }

    void on_player_connect();
    void on_player_disconnect();
    std::string perform_action(int player, const std::string& action);
};
