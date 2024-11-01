#include "game.h"
#include <mutex>
#include <thread>
#include <chrono>

void Game::start()
{
    this->duel = std::make_shared<Duel>();
    std::cout << "the game started\n";
    this->state = GAME_CHOOSE_STARTER;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}