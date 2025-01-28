#include "player.h"
#include <nlohmann/json.hpp>

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>

using json = nlohmann::json;

std::vector<std::shared_ptr<Pokemon>> Player::start_pokebag()
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
        pokemons.push_back(std::make_shared<Pokemon>(allPokemons[i]));

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

std::string Player::execute_shift(int option)
{
    int index = 0;
    auto active = std::find(pokemons.begin(), pokemons.end(), *this->get_active_pokemon());

    if (active != pokemons.end())
        index = std::distance(pokemons.begin(), active);

    if((option -1) == index)
        return "";

    std::cout << "player: " << this->get_id() << " changed " << active->get()->get_name() << " to " << this->pokemons[option-1]->get_name() << "\n";
    std::string message = "Player " + std::to_string(this->get_id()) + " shifted " +  active->get()->get_name() + " to " + this->pokemons[option - 1]->get_name() + "\n";
    this->set_active_pokemon(this->pokemons[option-1]);

    this->set_duel_action(false);

    return message;
}

std::string Player::execute_battle(int option, std::shared_ptr<Player> enemy)
{
    if(!this->get_duel_action())
        return "";
        
    auto active = this->get_active_pokemon();
    auto move = active->get_moves()[option -1];

    int damage = std::max(1, static_cast<int>(std::floor((active->get_atk() * (move.get_power()) / enemy->get_active_pokemon()->get_def()))));
    std::cout << "player: " << this->get_id() << " dealt " << damage << " damage to enemy\n";
    enemy->get_active_pokemon()->set_health(enemy->get_active_pokemon()->get_health() - damage);

    std::string message = "Player " + std::to_string(this->get_id()) + " dealt " + std::to_string(damage) + " damage to " + enemy->get_active_pokemon()->get_name() + "\n";
    if(enemy->get_active_pokemon()->get_health() <= 0)
    {
        enemy->get_active_pokemon()->set_fainted();
        message += enemy->get_active_pokemon()->get_name() + " fainted.\n";
        enemy->set_duel_action(false);
    }

    this->set_duel_action(false);

    return message;
}

int Player::pokemon_faint(int index)
{
    auto active = std::find(pokemons.begin(), pokemons.end(), *this->get_active_pokemon());

    int faint = 0;
    if (active != pokemons.end())
        faint = std::distance(pokemons.begin(), active);

    if((index -1) == faint)
        return 0;

    std::cout << "player: " << this->get_id() << " sent " << this->pokemons[index-1]->get_name() << "\n";
    this->set_active_pokemon(this->pokemons[index-1]);

    return 1;
}

bool Player::all_pokemons_fainted() const 
{
    for (const auto& pokemon : pokemons) {
        if (pokemon && !pokemon->is_fainted()) {
            return false;
        }
    }
    return true; 
}