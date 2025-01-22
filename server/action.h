#pragma once

#include "enums.h"

class Action {
public:
    Action() : m_action(static_cast<enum action>(0)), m_option(0) {}
    Action(int action, int option)
        : m_action{static_cast<enum action>(action)}, m_option{option} {}

    int get_action(){return m_action;};
    int get_option(){return m_option;};
    
private:
    enum action m_action;
    int m_option;
};
