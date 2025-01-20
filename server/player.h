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

    std::vector<Pokemon> pokemons;

    uint16_t get_id() { return m_id; };
    std::vector<Pokemon> start_pokebag();
    
private:
    uint16_t m_id;
};
