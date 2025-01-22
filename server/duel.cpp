#include <mutex>
#include "duel.h"

void Duel::start()
{
    std::cout << "duel started\n";
    this->playerOne->set_active_pokemon(this->playerOne->pokemons[this->playerOne->get_starter_pos()]);
    this->playerTwo->set_active_pokemon(this->playerTwo->pokemons[this->playerTwo->get_starter_pos()]);

    std::cout << "player one active: " << playerOne->get_active_pokemon()->get_num() << "\n";
    std::cout << "player two active: " << playerTwo->get_active_pokemon()->get_num() << "\n";

    while(true)
    {
        if(!(this->playerOne->get_duel_action() && this->playerTwo->get_duel_action()))
            continue;

        if(this->playerOne->get_action().get_action() == SHIFT)
            this->playerOne->execute_shift(this->playerOne->get_action().get_option());

        if(this->playerTwo->get_action().get_action() == SHIFT)
            this->playerTwo->execute_shift(this->playerTwo->get_action().get_option());

        if(this->playerOne->get_duel_action() || this->playerTwo->get_duel_action())
            this->execute_battle();
    }
}

void Duel::execute_battle()
{
    std::vector<std::shared_ptr<Pokemon>> pokemons;
    pokemons.push_back(playerOne->get_active_pokemon());
    pokemons.push_back(playerTwo->get_active_pokemon());

    std::sort(pokemons.begin(), pokemons.end(), [](const std::shared_ptr<Pokemon>& a, const std::shared_ptr<Pokemon>& b) 
    {
        return a->get_spd() > b->get_spd();
    });

    if (pokemons[0] == playerOne->get_active_pokemon()) 
    {
        this->playerOne->execute_battle(this->playerOne->get_action().get_option(), this->playerTwo->get_active_pokemon());
        this->playerTwo->execute_battle(this->playerTwo->get_action().get_option(), this->playerOne->get_active_pokemon());
    }
    else
    {
        this->playerTwo->execute_battle(this->playerTwo->get_action().get_option(), this->playerOne->get_active_pokemon());
        this->playerOne->execute_battle(this->playerOne->get_action().get_option(), this->playerTwo->get_active_pokemon());
    }
}
