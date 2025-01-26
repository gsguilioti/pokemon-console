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

void draw_duel()
{
    system("clear");
    auto enemy = client.call("get_enemy_pokemon", game.player->getId()).as<Pokemon>();
    std::shared_ptr<Pokemon> enemyPokemon = std::make_shared<Pokemon>(enemy);
    auto your = client.call("get_active_pokemon", game.player->getId()).as<Pokemon>();
    std::shared_ptr<Pokemon> yourPokemon = std::make_shared<Pokemon>(your);
    
    if(yourPokemon->get_health() <= 0)
    {
        while(1)
        {
            auto receivedPokemon = client.call("get_active_pokemon", game.player->getId()).as<Pokemon>();
            std::shared_ptr<Pokemon> activePokemon = std::make_shared<Pokemon>(receivedPokemon);

            system("clear");
            std::cout << "Your pokemon died, choose the next: \n";

            int count = 1;
            for(auto pokemon: game.player->pokemons)
            {
                if(pokemon.get_num() == activePokemon->get_num())
                {
                    std::cout << count++ << "." << pokemon.get_name() << "*\n";
                    continue;
                }
                std::cout << count++ << "." << pokemon.get_name() << "\n";
            }

            int index;
            std:: cin >> index;
            int faint = client.call("pokemon_faint", game.player->getId(), index).as<int>();
            if(faint) break;
        }
    }

    std::cout << client.call("get_duel_message").as<std::string>() << "\n";

    std::cout << "\n----------------------------------------------- \n";

    std::cout << "Enemy: " << enemyPokemon->get_name() << "\n";
    std::cout << "Health: " << enemyPokemon->get_health() << "\n";

    std::cout << "\n      X \n\n";

    std::cout << "Your: " << yourPokemon->get_name() << "\n";
    std::cout << "Health: " << yourPokemon->get_health() << "\n";

    std::cout << "\n----------------------------------------------- \n";
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
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                }
                std::cout << "duel started. \n";
                break;
            case GAME_DUEL:
                {
                    draw_duel();
                    std::cout << "what you want to do? \n 1. Battle \n 2. Pokemon \n";
                    int action;
                    std::cin >> action;

                    if(action == 1)
                    {
                        auto receivedPokemon = client.call("get_active_pokemon", game.player->getId()).as<Pokemon>();
                        std::shared_ptr<Pokemon> pokemon = std::make_shared<Pokemon>(receivedPokemon);
                        if(pokemon == nullptr)
                            std::cout << "pokemon nao chegou\n";

                        std::cout << "\n0. Back\n";
                        int count = 1;
                        for(auto move: pokemon->get_moves())
                        {
                            std::cout << count++ << "." << move.get_name() << "\n";
                        }
                    }
                    else if(action == 2)
                    {
                        auto receivedPokemon = client.call("get_active_pokemon", game.player->getId()).as<Pokemon>();
                        std::shared_ptr<Pokemon> activePokemon = std::make_shared<Pokemon>(receivedPokemon);
                        if(activePokemon == nullptr)
                            std::cout << "pokemon nao chegou\n";

                        std::cout << "\n0. Back\n";
                        int count = 1;
                        for(auto pokemon: game.player->pokemons)
                        {
                            if(pokemon.get_num() == activePokemon->get_num())
                            {
                                std::cout << count++ << "." << pokemon.get_name() << "*\n";
                                continue;
                            }
                            std::cout << count++ << "." << pokemon.get_name() << "\n";
                        }
                    }

                    int option;
                    std::cin >> option;

                    if(option == 0)
                        continue;

                    client.call("action", game.player->getId(), action, option).as<int>();
                    system("clear");

                    std::cout << "waiting for enemy." << std::flush;
                    while(!client.call("get_players_actions").as<bool>());
                }
                continue;
            default:
                std::cout<< "game state error. exiting...\n";
                cleanup(SIGINT);
        }
    }

    return 0;
}
