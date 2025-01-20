#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <tuple>
#include <memory>

#include "enums.h"
#include "duel.h"
#include "player.h"

#include "rpc/client.h"

class Game {
public:
    Game() = default;
    std::shared_ptr<Duel> duel;
    std::shared_ptr<Player> player;

    void start(rpc::client& client);
private:
    int state = GAME_WAITING;
};
