#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <tuple>
#include <memory>

#include "enums.h"
#include "duel.h"

class Game {
public:
    std::shared_ptr<Duel> duel;
    uint16_t players;

    Game() = default;

    void on_player_connect();
    void on_player_disconnect();
    int get_state(){ return state; };

private:
    void start();
    int state = GAME_WAITING;
};
