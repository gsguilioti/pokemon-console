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

    enum player player;
    std::tuple<int, std::string> response = client.call("connect").as<std::tuple<int, std::string>>();
    std::cout << std::get<1>(response) << std::endl;
    player = std::get<0>(response);

    while (true) 
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        bool started = client.call("game_started").as<bool>();
        if(!started)
        {
            player = PLAYER_ONE;
            std::cout << "waiting for oponent... \n";
            continue;
        }

        std::cout << "game is starting... \n";
    }

    cleanup(SIGINT);

    return 0;
}