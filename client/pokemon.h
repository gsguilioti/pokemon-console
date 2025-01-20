#pragma once
#include <rpc/msgpack.hpp>

#include <string>
#include <cstdint>
#include <vector>

#include "move.h"

class Pokemon {
public:
    Pokemon(int num, const std::string& name, int health, int attack, int defense, int speed, const std::vector<Move>& moves)
        : num(num), name(name), health(health), atk(attack), def(defense), spd(speed), moves(moves) {}
    Pokemon() = default;
    uint16_t get_num(){ return num; };
    std::string get_name(){ return name; };

    MSGPACK_DEFINE(num, name, health, atk, def, spd, moves);
private:
    uint16_t num;
    std::string name;
    uint16_t health;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    std::vector<Move> moves;
};
