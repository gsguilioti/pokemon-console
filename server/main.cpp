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
        return std::make_tuple<int, std::string>(game.numPlayers, "connected: " + std::to_string(game.numPlayers) + "/2\n");
    });

    srv.bind("disconnect", [&](int id) 
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        game.on_player_disconnect(id);
        return "disconnected: " + std::to_string(game.numPlayers) + "/2\n";
    });

    srv.bind("get_state", [&]()
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        return game.get_state(); 
    });

    srv.bind("start_pokebag", [&](int id)
    {
        std::lock_guard<std::mutex> lock(game_mutex);

        auto player = std::find_if(game.players.begin(), game.players.end(),
                           [id](Player& player) {
                               return player.get_id() == id;
                           });
        if(player != game.players.end())
            return player->start_pokebag(); 
        
        return std::vector<Pokemon>();
    });

    srv.bind("perform_action", [&](int player, const std::string& action)
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        return game.duel->perform_action(player, action); 
    });

    srv.run();

    return 0;
}