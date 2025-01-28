#pragma once

#include "rpc/client.h"

#include <string>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <tuple>
#include <memory>

#include "enums.h"
#include "player.h"

class Game {
public:
    Game() = default;
    std::shared_ptr<Player> player;

    void start(rpc::client& client);
};
