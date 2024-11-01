#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

#include "rpc/client.h"
#include "rpc/rpc_error.h"

#include "enums.h"
#include "game.h"
#include "duel.h"
#include "player.h"

rpc::client client("localhost", 8080);
Game game;

void cleanup(int signum)
{
    std::string response = client.call("disconnect", game.player->getId()).as<std::string>();
    std::cout << response << std::endl;
    exit(signum);
}

int main() 
{
    std::signal(SIGINT, cleanup);

    std::tuple<int, std::string> response = client.call("connect").as<std::tuple<int, std::string>>();
    std::cout << std::get<1>(response) << std::endl;
    game.player = std::make_shared<Player>(std::get<0>(response));

    while (true) 
    {
        int state = client.call("get_state").as<int>();

        switch (state)
        {
            case GAME_WAITING:
                game.player->reset_id();
                std::cout << "waiting for oponent... \n";
                std::this_thread::sleep_for(std::chrono::seconds(3));
                continue;
            case GAME_CHOOSE_STARTER:
                break;
            default:
                std::cout<< "Game State Error. Exiting...\n";
                cleanup(SIGINT);
        }
    }

    game.start();

    return 0;
}