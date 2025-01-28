#include "rpc/client.h"
#include "rpc/rpc_error.h"

#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>

#include "enums.h"
#include "game.h"
#include "player.h"

rpc::client* client = nullptr;
Game game;

void cleanup(int signum)
{
    std::string response = client->call("disconnect", game.player->getId()).as<std::string>();
    std::cout << response << std::endl;
    exit(signum);
}

void display_pokemon_info(const std::shared_ptr<Pokemon>& yourPokemon, const std::shared_ptr<Pokemon>& enemyPokemon)
{
    system("clear");
    std::cout << client->call("get_duel_message").as<std::string>() << "\n";
    std::cout << "----------------------------------------------- \n";
    std::cout << "Enemy: " << enemyPokemon->get_name() << "\n";
    std::cout << "Health: " << enemyPokemon->get_health() << "\n";
    std::cout << "\n      X \n\n";
    std::cout << "Your: " << yourPokemon->get_name() << "\n";
    std::cout << "Health: " << yourPokemon->get_health() << "\n";
    std::cout << "\n----------------------------------------------- \n";
}

void wait_for_enemy(std::shared_ptr<Pokemon>& enemyPokemon)
{
    std::cout << "waiting for enemy." << std::flush;
    while (enemyPokemon->get_health() <= 0)
    {
        enemyPokemon = std::make_shared<Pokemon>(client->call("get_enemy_pokemon", game.player->getId()).as<Pokemon>());
    }
}

void handle_pokemon_selection(const std::shared_ptr<Pokemon>& activePokemon)
{
    system("clear");
    std::cout << "Your pokemon died, choose the next: \n";

    auto pokemons = client->call("get_pokemons", game.player->getId()).as<std::vector<std::shared_ptr<Pokemon>>>();
    int count = 1;
    for (auto pokemon : pokemons)
    {
        if (pokemon->is_fainted())
        {
            count++;
            continue;
        }

        if (pokemon->get_num() == activePokemon->get_num())
        {
            count++;
            continue;
        }

        std::cout << count++ << "." << pokemon->get_name() << "\n";
    }
}

bool choose_pokemon()
{
    int index;
    std::cin >> index;

    int faint = client->call("pokemon_faint", game.player->getId(), index).as<int>();
    return faint;
}

void draw_duel()
{
    auto enemy = client->call("get_enemy_pokemon", game.player->getId()).as<Pokemon>();
    std::shared_ptr<Pokemon> enemyPokemon = std::make_shared<Pokemon>(enemy);
    auto your = client->call("get_active_pokemon", game.player->getId()).as<Pokemon>();
    std::shared_ptr<Pokemon> yourPokemon = std::make_shared<Pokemon>(your);

    wait_for_enemy(enemyPokemon);
    display_pokemon_info(yourPokemon, enemyPokemon);

    if (yourPokemon->get_health() <= 0)
    {
        while (true)
        {
            auto receivedPokemon = client->call("get_active_pokemon", game.player->getId()).as<Pokemon>();
            std::shared_ptr<Pokemon> activePokemon = std::make_shared<Pokemon>(receivedPokemon);

            handle_pokemon_selection(activePokemon);
            if (choose_pokemon())
            {
                draw_duel();
                break;
            }
        }
    }
}

int main()
{
    std::string ip;
    std::cout << "Em qual ip o cliente deve se conectar?\n"; 
    std::cin >> ip;

    int port;
    std::cout << "Em qual porta o cliente deve se conectar?\n"; 
    std::cin >> port;

    rpc::client local_client("localhost", port);
    client = &local_client;

    std::signal(SIGINT, cleanup);

    std::tuple<int, std::string> response = client->call("connect").as<std::tuple<int, std::string>>();
    std::cout << std::get<1>(response) << std::endl;
    game.player = std::make_shared<Player>(std::get<0>(response));

    while (true)
    {
        int state = client->call("get_state").as<int>();

        switch (state)
        {
        case GAME_WAITING:
            game.player->reset_id();
            std::cout << "waiting for oponent... \n";
            std::this_thread::sleep_for(std::chrono::seconds(3));
            continue;

        case GAME_CHOOSE_STARTER:
        {
            auto pokemons = client->call("start_pokebag", game.player->getId()).as<std::vector<std::shared_ptr<Pokemon>>>();
            if (!pokemons.empty())
                game.player->pokemons = pokemons;

            game.start(*client);
            while (state = client->call("get_state").as<int>() == GAME_CHOOSE_STARTER)
            {
                std::cout << "waiting for oponent... \n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        std::cout << "duel started. \n";
        break;

        case GAME_DUEL:
        {
            if (client->call("duel_over").as<bool>())
            {
                auto message = client->call("end_duel_message", game.player->getId()).as<std::string>();
                system("clear");
                std::cout << message << "\n" << std::flush;
                exit(1);
            }

            draw_duel();
            std::cout << "what you want to do? \n 1. Battle \n 2. Pokemon \n";
            int action;
            std::cin >> action;

            if (action == 1)
            {
                auto receivedPokemon = client->call("get_active_pokemon", game.player->getId()).as<Pokemon>();
                std::shared_ptr<Pokemon> pokemon = std::make_shared<Pokemon>(receivedPokemon);
                if (pokemon == nullptr)
                    std::cout << "pokemon nao chegou\n";

                std::cout << "\n0. Back\n";
                int count = 1;
                for (auto move : pokemon->get_moves())
                {
                    std::cout << count++ << "." << move.get_name() << "\n";
                }
            }
            else if (action == 2)
            {
                auto receivedPokemon = client->call("get_active_pokemon", game.player->getId()).as<Pokemon>();
                std::shared_ptr<Pokemon> activePokemon = std::make_shared<Pokemon>(receivedPokemon);
                if (activePokemon == nullptr)
                    std::cout << "pokemon nao chegou\n";

                auto pokemons = client->call("get_pokemons", game.player->getId()).as<std::vector<std::shared_ptr<Pokemon>>>();

                std::cout << "\n0. Back\n";
                int count = 1;
                for (auto pokemon : pokemons)
                {
                    if (pokemon->get_health() <= 0)
                    {
                        count++;
                        continue;
                    }

                    if (pokemon->get_num() == activePokemon->get_num())
                    {
                        count++;
                        continue;
                    }
                    std::cout << count++ << "." << pokemon->get_name() << "\n";
                }
            }

            int option;
            std::cin >> option;

            if (option == 0)
                continue;

            client->call("action", game.player->getId(), action, option).as<int>();
            system("clear");

            std::cout << "waiting for enemy." << std::flush;
            while (!client->call("get_players_actions").as<bool>());
        }
        continue;

        default:
            std::cout << "game state error. exiting...\n";
            cleanup(SIGINT);
        }
    }

    return 0;
}
