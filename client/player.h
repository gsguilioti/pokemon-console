#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#include "enums.h"
#include "pokemon.h"

class Player {
public:
    Player(uint16_t id): m_id{id}
    {};

    std::vector<std::shared_ptr<Pokemon>> pokemons;
    uint16_t getId() { return m_id; };
    void reset_id();

private:
    uint16_t m_id;
};
