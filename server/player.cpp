#include "player.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

using json = nlohmann::json;

std::vector<Pokemon> Player::start_pokebag()
{
    std::ifstream pokemonFile("../data/pokemon.json");
    json pokemonData;
    pokemonFile >> pokemonData;

    std::ifstream moveFile("../data/move.json");
    json moveData;
    moveFile >> moveData;

    std::unordered_map<int, Move> move_map;
    for (const auto& move_json : moveData["moves"]) 
    {
        Move move(move_json["id"], move_json["name"], move_json["power"]);
        move_map[move_json["id"]] = move;
    }

    std::vector<Pokemon> allPokemons;
    for (const auto& poke : pokemonData["pokemon"]) 
    {
        auto moves = poke["moves"].get<std::vector<int>>();
        std::vector<Move> pokemonMoves;
        for(auto id : moves)
            pokemonMoves.emplace_back(move_map[id]);

        allPokemons.emplace_back(
            poke["num"],
            poke["name"],
            poke["health"],
            poke["attack"],
            poke["defense"],
            poke["speed"],
            pokemonMoves
        );
    }

    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(allPokemons.begin(), allPokemons.end(), g);

    for (size_t i = 0; i < 3 && i < allPokemons.size(); ++i)
        pokemons.push_back(allPokemons[i]);

    std::cout << "pokebag filled\n";

    return pokemons;
}

int Player::choose_starter(int starterPos)
{
    this->starterPos = starterPos -1;
    this->set_starter_choosen();
    std::cout << "starter choosed\n";
    return 1;
}

void Player::execute_shift(int option)
{
    int index = 0;
    auto active = std::find(pokemons.begin(), pokemons.end(), *this->get_active_pokemon());

    if (active != pokemons.end())
        index = std::distance(pokemons.begin(), active);

    if((option -1) == index)
        return;

    std::cout << "player: " << this->get_id() << " changed " << active->get_name() << " to" << this->pokemons[option-1].get_name() << "\n";
    this->set_active_pokemon(this->pokemons[option-1]);

    this->set_duel_action(false);
}

void Player::execute_battle(int option, std::shared_ptr<Pokemon> enemy)
{
    if(!this->get_duel_action())
        return;
        
    auto active = this->get_active_pokemon();
    auto move = active->get_moves()[option -1];

    int damage = (active->get_atk() + (move.get_power() * 2) - enemy->get_def());
    std::cout << "player: " << this->get_id() << " deals " << damage << " damage to enemy\n";
    enemy->set_health(enemy->get_health() - damage);

    this->set_duel_action(false);
}

int Player::pokemon_faint(int index)
{
    auto active = std::find(pokemons.begin(), pokemons.end(), *this->get_active_pokemon());

    int faint = 0;
    if (active != pokemons.end())
        faint = std::distance(pokemons.begin(), active);

    if((index -1) == faint)
        return 0;

    std::cout << "player: " << this->get_id() << " send " << this->pokemons[index-1].get_name() << "\n";
    this->set_active_pokemon(this->pokemons[index-1]);

    return 1;
}