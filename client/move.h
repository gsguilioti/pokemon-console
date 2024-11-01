#pragma once
#include <rpc/msgpack.hpp>

#include <string>
#include <cstdint>

class Move {
public:
    Move(int id, const std::string& name, int power)
        : id(id), name(name), power(power) {}
    Move() = default;

    MSGPACK_DEFINE(id, name, power);
private:
    uint16_t id;
    std::string name;
    uint16_t power;
};
