#include "rpc/server.h"

#include <iostream>
#include "game.h"

Game game;
std::mutex game_mutex;

int main()
{
    rpc::server srv(8080);

    srv.bind("connect", [&]() 
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        game.on_player_connect();
        return "connected: " + std::to_string(game.players) + "/2\n";
    });

    srv.bind("disconnect", [&]() 
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        game.on_player_disconnect();
        return "disconnected: " + std::to_string(game.players) + "/2\n";
    });

    srv.bind("perform_action", [&game](int player, const std::string& action){ return game.perform_action(player, action); });

    srv.run();

    return 0;
}