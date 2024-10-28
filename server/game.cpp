#include "game.h"
#include <mutex>
#include <thread>
#include <chrono>

void Game::on_player_connect()
{
    players++;
    std::cout << "player connected " <<  std::to_string(players)  <<"/2\n";

    if(players == 2)
    {
        std::thread game_thread(&Game::start, this);
        game_thread.detach();
    }
}

void Game::on_player_disconnect()
{
    players--;
    std::cout << "player disconnected " << std::to_string(players)  <<"/2\n";
}

void Game::start()
{
    this->duel = std::make_shared<Duel>();
    std::cout << "the game started\n";
    this->state = GAME_CHOOSE_STARTER;
    while(1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "inside the game loop\n";
    }
}