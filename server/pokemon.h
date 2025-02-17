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

    bool operator==(const Pokemon& other) const {
        return this->get_num() == other.get_num();
    }

    std::string get_name() { return name; };
    std::string get_name() const { return name; };
    uint16_t get_num() const { return num; };
    uint16_t get_spd() { return spd; };
    uint16_t get_atk() { return atk; };
    uint16_t get_def() { return def; };
    int16_t get_health() { return health; };
    std::vector<Move> get_moves() const { return moves; };

    void set_health(int value) { health = value; };
    void set_fainted() { fainted = true;};
    bool is_fainted() { return fainted; };

    MSGPACK_DEFINE(num, name, health, atk, def, spd, moves, fainted);
private:
    uint16_t num;
    std::string name;
    int16_t health;
    uint16_t atk;
    uint16_t def;
    uint16_t spd;
    std::vector<Move> moves;

    bool fainted = false;
};

inline bool operator==(const std::shared_ptr<Pokemon>& lhs, const Pokemon& rhs) {
    return lhs->get_num() == rhs.get_num();
}

inline bool operator==(const Pokemon& lhs, const std::shared_ptr<Pokemon>& rhs) {
    return lhs.get_num() == rhs->get_num();
}