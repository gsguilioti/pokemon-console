#include "rpc/client.h"
#include "rpc/rpc_error.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

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
                {
                    auto pokemons = client.call("start_pokebag", game.player->getId()).as<std::vector<Pokemon>>();
                    if(!pokemons.empty())
                        game.player->pokemons = pokemons;

                    game.start(client);
                    while(state = client.call("get_state").as<int>() == GAME_CHOOSE_STARTER)
                    {
                        std::cout << "waiting for oponent... \n";
                        std::this_thread::sleep_for(std::chrono::seconds(3));
                    }
                }
                std::cout << "duel started. \n";
                break;
            case GAME_DUEL:
                {
                    std::cout << "what you want to do? \n 1. Battle \n 2. Pokemon \n";
                    int action;
                    std::cin >> action;

                    if(action == 1)
                    {
                        auto receivedPokemon = client.call("get_active_pokemon", game.player->getId()).as<Pokemon>();
                        std::shared_ptr<Pokemon> pokemon = std::make_shared<Pokemon>(receivedPokemon);
                        if(pokemon == nullptr)
                            std::cout << "pokemon nao chegou\n";
                        int count = 1;
                        for(auto move: pokemon->get_moves())
                        {
                            std::cout << count++ << "." << move.get_name() << "\n";
                        }
                    }
                    else
                    {
                        auto receivedPokemon = client.call("get_active_pokemon", game.player->getId()).as<Pokemon>();
                        std::shared_ptr<Pokemon> activePokemon = std::make_shared<Pokemon>(receivedPokemon);
                        if(activePokemon == nullptr)
                            std::cout << "pokemon nao chegou\n";
                        int count = 1;
                        for(auto pokemon: game.player->pokemons)
                        {
                            if(pokemon.get_num() == activePokemon->get_num())
                            {
                                std::cout << count++ << count++ << "." << pokemon.get_name() << "*\n";
                                continue;
                            }
                            std::cout << count++ << "." << pokemon.get_name() << "\n";
                        }
                    }

                    int option;
                    std::cin >> option;

                    client.call("action", game.player->getId(), action, option).as<int>();
                }
                continue;
            default:
                std::cout<< "game state error. exiting...\n";
                cleanup(SIGINT);
        }
    }

    return 0;
}
