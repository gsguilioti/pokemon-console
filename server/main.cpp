#include "rpc/server.h"

#include <iostream>
#include "server.h"

int main()
{
    rpc::server srv(8080);

    srv.bind("connect", [&]() 
    {
        on_player_connect();
        return "connected: " + std::to_string(players) + "/2\n";
    });

    srv.bind("disconnect", [&]() 
    {
        on_player_disconnect();
        return "disconnected: " + std::to_string(players) + "/2\n";
    });

    srv.run();

    return 0;
}