#include "game.h"
#include <mutex>

std::string Game::perform_action(int player, const std::string& action)
{
    if (player != currentPlayer)
        return "Not your turn!";

    return "Unknown action!";
}

void Game::on_player_connect()
{
    if(players >= 2)
    {
        std::cout << "cannot connect more players\n";
        return;
    }
    players++;
    std::cout << "player connected " <<  std::to_string(players)  <<"/2\n";
}

void Game::on_player_disconnect()
{
    players--;
    std::cout << "player disconnected " << std::to_string(players)  <<"/2\n";
}