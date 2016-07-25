#ifndef CROWS_PLAYERSET_H
#define CROWS_PLAYERSET_H

#include "ControlSet.hpp"

struct PlayerSet : public ControlSet {
    std::string name = "PlayerSet";

    PlayerSet()
    {
        rules.find("rotate camera vertically")->second = false;
        rules.find("control all")->second = false;
        rules.find("delete all")->second = false;
        rules.find("delete")->second = false;
    }
};

#endif
