#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#include "enums.h"

class Player {
public:
    Player(uint16_t id): m_id{id}
    {};

    std::vector<int> pokemons;

    uint16_t getId() { return m_id; };
    
private:
    uint16_t m_id;
};
