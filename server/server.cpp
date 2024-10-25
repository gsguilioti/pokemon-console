#include "server.h"

uint16_t players = 0;

void on_player_connect()
{
    if(players >= 2)
    {
        std::cout << "cannot connect more players\n";
        return;
    }
    players++;
    std::cout << "player connected " <<  std::to_string(players)  <<"/2\n";
}

void on_player_disconnect()
{
    players--;
    std::cout << "player disconnected" << std::to_string(players)  <<"/2\n";
}