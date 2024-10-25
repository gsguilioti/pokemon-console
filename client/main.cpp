#include <iostream>
#include <csignal>

#include "rpc/client.h"

rpc::client c("localhost", 8080);

void cleanup(int signum)
{
    std::string response = c.call("disconnect").as<std::string>();
    std::cout << response << std::endl;
    exit(signum);
}

int main() 
{
    std::signal(SIGINT, cleanup);

    std::string response = c.call("connect").as<std::string>();
    std::cout << response << std::endl;

    while(1);

    return 0;
}