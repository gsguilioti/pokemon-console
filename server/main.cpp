#include "rpc/server.h"

#include <iostream>
#include <thread>
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
        return std::make_tuple<int, std::string>(game.players, "connected: " + std::to_string(game.players) + "/2\n");
    });

    srv.bind("disconnect", [&]() 
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        game.on_player_disconnect();
        return "disconnected: " + std::to_string(game.players) + "/2\n";
    });

    srv.bind("get_state", [&]()
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        return game.get_state(); 
    });

    srv.bind("perform_action", [&](int player, const std::string& action)
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        return game.duel->perform_action(player, action); 
    });

    srv.run();

    return 0;
}