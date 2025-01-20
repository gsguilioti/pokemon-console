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

    int choose_starter(int starterPos);

    bool get_starter_status(){ return hasChoosenStarter; };
    void set_starter_choosen() { hasChoosenStarter = true; };
    
private:
    uint16_t m_id;
    bool hasChoosenStarter = false;
    uint16_t starterPos = 0;
};
