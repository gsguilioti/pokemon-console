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

    return pokemons;
}

int Player::choose_starter(int starterPos)
{
    this->starterPos = starterPos -1;
    this->set_starter_choosen();
    return 1;
}