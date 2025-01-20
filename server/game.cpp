#include "game.h"
#include <mutex>
#include <thread>
#include <chrono>

void Game::on_player_connect()
{
    numPlayers++;
    players.emplace_back(numPlayers);
    std::cout << "player connected " <<  std::to_string(numPlayers)  <<"/2\n";

    if(numPlayers == 2)
    {
        std::thread game_thread(&Game::start, this);
        game_thread.detach();
    }
}

void Game::on_player_disconnect(int id)
{
    players.erase(std::remove_if(players.begin(), players.end(),
                                 [id](Player& player) {
                                     return player.get_id() == id;
                                 }),
                  players.end());
    numPlayers--;
    std::cout << "player disconnected " << std::to_string(numPlayers)  <<"/2\n";
}

void Game::start()
{
    this->duel = std::make_shared<Duel>(players[0], players[1]);
    std::cout << "the game started\n";
    this->state = GAME_CHOOSE_STARTER;
    while(!(this->players[0].get_starter_status()) && !(this->players[1].get_starter_status()))
    {
        std::cout << "waiting for both players to start the duel...\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    this->state = GAME_DUEL;
    this->duel->start();
}