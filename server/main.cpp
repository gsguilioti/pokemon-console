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

    srv.bind("get_players_actions", [&]()
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        return (!(game.players[0]->get_duel_action()) && !(game.players[1]->get_duel_action()));
    });

    srv.bind("get_duel_message", [&]()
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        std::string message = game.duel->message;

        return message;
    });

    srv.bind("start_pokebag", [&](int id)
    {
        std::lock_guard<std::mutex> lock(game_mutex);

        auto player = std::find_if(game.players.begin(), game.players.end(),
                           [id](std::shared_ptr<Player>& player) {
                               return player->get_id() == id;
                           });
        if(player != game.players.end())
            return (*player)->start_pokebag(); 
        
        return std::vector<Pokemon>();
    });

    srv.bind("get_active_pokemon", [&](int id)
    {
        std::lock_guard<std::mutex> lock(game_mutex);

        auto player = std::find_if(game.players.begin(), game.players.end(),
                           [id](std::shared_ptr<Player>& player) {
                               return player->get_id() == id;
                           });
        if(player != game.players.end())
        {
            auto pokemon = (*player)->get_active_pokemon();
            if(pokemon == nullptr)
                std::cout << "invalid pokemon\n";

            return *pokemon;
        }
        
        return Pokemon();
    });

    srv.bind("get_enemy_pokemon", [&](int id)
    {
        std::lock_guard<std::mutex> lock(game_mutex);

        auto player = std::find_if(game.players.begin(), game.players.end(),
                           [id](std::shared_ptr<Player>& player) {
                               return player->get_id() != id;
                           });
        if(player != game.players.end())
        {
            auto pokemon = (*player)->get_active_pokemon();
            if(pokemon == nullptr)
                std::cout << "invalid pokemon\n";

            return *pokemon;
        }
        
        return Pokemon();
    });

    srv.bind("choose_starter", [&](int id, int starterPos)
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        auto player = std::find_if(game.players.begin(), game.players.end(),
                           [id](std::shared_ptr<Player>& player) {
                               return player->get_id() == id;
                           });
        if(player == game.players.end())
            return 0;

        return (*player)->choose_starter(starterPos);
    });

    srv.bind("action", [&](int id, int action, int option)
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        auto player = std::find_if(game.players.begin(), game.players.end(),
                           [id](std::shared_ptr<Player>& player) {
                               return player->get_id() == id;
                           });
        if(player == game.players.end())
            return 0;

        (*player)->set_action({action, option});
        (*player)->set_duel_action(true);
        return 1;
    });

    srv.bind("pokemon_faint", [&](int id, int index)
    {
        std::lock_guard<std::mutex> lock(game_mutex);
        auto player = std::find_if(game.players.begin(), game.players.end(),
                           [id](std::shared_ptr<Player>& player) {
                               return player->get_id() == id;
                           });
        if(player == game.players.end())
            return 0;

        int changed = (*player)->pokemon_faint(index);
        return changed;
    });

    srv.run();

    return 0;
}