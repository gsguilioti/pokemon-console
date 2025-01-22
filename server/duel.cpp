#include <mutex>
#include "duel.h"

void Duel::start()
{
    std::cout << "duel started\n";
    this->playerOne->setActivePokemon(this->playerOne->pokemons[this->playerOne->get_starter_pos()]);
    this->playerTwo->setActivePokemon(this->playerTwo->pokemons[this->playerTwo->get_starter_pos()]);

    std::cout << "player one active: " << playerOne->getActivePokemon()->get_num() << "\n";
    std::cout << "player two active: " << playerTwo->getActivePokemon()->get_num() << "\n";

    while(!(this->playerOne->get_duel_action() && this->playerTwo->get_duel_action()))
    {

    }

    std::cout << "both players made an action\n";
}
