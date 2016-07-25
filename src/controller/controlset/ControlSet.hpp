#ifndef CROWS_CONTROLSET_H
#define CROWS_CONTROLSET_H

#include <map>

struct ControlSet {
    std::string name = "ControlSet";
    std::map<std::string, bool> rules = {
        // camera
        {"rotate camera horizontally", true},
        {"rotate camera vertically", true},
        {"zoom", true},

        // animas
        {"make crow", true},
        {"make squirrel", true},

        // scene
        {"select", true},

        // terraforming
        {"make tree", true},
        {"make light", true},
        {"make garbage", true},
        {"make car", true},

        // control
        {"action", true},
        {"delete all", true},
        {"delete", true},
        {"control all", true},
        {"control player", true},
        {"button one", true},
        {"button two", true},
        {"button three", true},
        {"button four", true},

        // info
        {"stats", true},
        {"start timer", true},
        {"stop timer", true},

        // drawing
        {"draw light", true},
        {"draw normal", true},
        {"draw dark", true},

        // memories
        {"save game", true},
        {"load game", true},

        // quit
        {"quit", true}
    };
};

#endif
