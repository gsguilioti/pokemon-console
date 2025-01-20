#include <mutex>
#include "duel.h"

std::string Duel::perform_action(int turn, const std::string& action)
{
    if (turn != currentTurn)
        return "Not your turn!";

    return "Unknown action!";
}

void Duel::start()
{
    std::cout << "duel started\n";
    while(1)
    {
        
    }
}
