#include <mutex>
#include <thread>
#include <chrono>

#include "game.h"

void Game::start(rpc::client& client)
{
    std::cout << "the game started\n";

    std::cout << "choose your starter: \n";
    int count = 1;
    for(auto pokemon: player->pokemons)
        std::cout << count++ << "." << pokemon->get_name() << "\n";

    int starter;
    std::cin >> starter;

    client.call("choose_starter", this->player->getId(), starter);
}