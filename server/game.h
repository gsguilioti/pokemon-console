#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <tuple>
#include <memory>
#include <vector>
#include <algorithm>

#include "enums.h"
#include "duel.h"
#include "player.h"

class Game {
public:
    uint16_t numPlayers = 0;
    std::shared_ptr<Duel> duel;
    std::vector<std::shared_ptr<Player>> players;

    Game() = default;

    void on_player_connect();
    void on_player_disconnect(int id);
    int get_state(){ return state; };

private:
    void start();
    int state = GAME_WAITING;
};
