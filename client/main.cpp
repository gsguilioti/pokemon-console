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

    std::string response = client.call("connect").as<std::string>();
    std::cout << response << std::endl;

    while (true) 
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        try
        {
            std::string action = "attack";
            std::string response = client.call("perform_action", static_cast<int>(PLAYER_ONE), action).as<std::string>();
            std::cout << "Server response: " << response << std::endl;

            if (response == "It's not your turn!") {
                std::cout << "Waiting for your turn..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
            }
        }
        catch(const rpc::rpc_error& e)
        {
            std::cerr << "RPC Error: " << e.what() << std::endl;
        } 
    }

    cleanup(SIGINT);

    return 0;
}