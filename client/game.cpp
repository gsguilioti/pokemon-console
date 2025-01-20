#include "game.h"
#include <mutex>
#include <thread>
#include <chrono>

void Game::start()
{
    this->duel = std::make_shared<Duel>();
    std::cout << "the game started\n";
    this->state = GAME_CHOOSE_STARTER;

    std::cout << "choose your starter: \n";
    for(auto pokemon: player->pokemons)
        std::cout << pokemon.get_num() << "." << pokemon.get_name() << "\n";

    int starter;
    std::cin >> starter;
}