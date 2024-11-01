#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

#include "rpc/client.h"
#include "rpc/rpc_error.h"
#include "enums.h"

rpc::client client("localhost", 8080);

void cleanup(int signum)
{
    std::string response = client.call("disconnect").as<std::string>();
    std::cout << response << std::endl;
    exit(signum);
}

int main() 
{
    std::signal(SIGINT, cleanup);

    int player = 0;
    std::tuple<int, std::string> response = client.call("connect").as<std::tuple<int, std::string>>();
    std::cout << std::get<1>(response) << std::endl;
    player = std::get<0>(response);

    while (true) 
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        int state = client.call("get_state").as<int>();

        switch (state)
        {
            case GAME_WAITING:
                player = PLAYER_ONE;
                std::cout << "waiting for oponent... \n";
                continue;
            case GAME_CHOOSE_STARTER:
                std::cout << "game is starting... \n";
                break;
            default:
                break;
        }
    }

    cleanup(SIGINT);

    return 0;
}