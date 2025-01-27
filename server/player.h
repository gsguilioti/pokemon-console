#pragma once

#include <iostream>
#include <cstdint>
#include <vector>

#include "enums.h"
#include "pokemon.h"
#include "action.h"

class Player {
public:
    Player(uint16_t id): m_id{id}
    {};

    std::vector<std::shared_ptr<Pokemon>> pokemons;

    uint16_t get_id() { return m_id; };
    std::vector<std::shared_ptr<Pokemon>> start_pokebag();

    int choose_starter(int starterPos);
    int get_starter_pos(){ return starterPos; };

    bool get_starter_status(){ return hasChoosenStarter; };
    void set_starter_choosen() { hasChoosenStarter = true; };

    void set_active_pokemon(std::shared_ptr<Pokemon> pokemon) { activePokemon = pokemon; };
    void set_active_pokemon(Pokemon* pokemon) { 
        activePokemon = pokemon ? std::make_shared<Pokemon>(*pokemon) : nullptr; 
    };
    std::shared_ptr<Pokemon> get_active_pokemon() { return activePokemon; };

    bool get_duel_action(){ return hasMadeDuelAction; };
    void set_duel_action(bool yn) { hasMadeDuelAction = yn; };

    Action get_action() { return action; };
    void set_action(Action act) { action = act; };

    std::string execute_shift(int option);
    std::string execute_battle(int option, std::shared_ptr<Player> enemy);
    int pokemon_faint(int index);
    
private:
    uint16_t m_id;
    bool hasChoosenStarter = false;
    uint16_t starterPos = 0;

    std::shared_ptr<Pokemon> activePokemon = nullptr;
    bool hasMadeDuelAction = false;
    Action action;
};
