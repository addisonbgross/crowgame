#ifndef CROWS_LEVELMAKER_H
#define CROWS_LEVELMAKER_H

// general
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>

// CrowGame
#include "../habitat/tree/Tree.hpp"
#include "../habitat/garbage/Garbage.hpp"
#include "LevelObject.hpp"
#include "../../opulence/src/service/OsType.h"
#include "../../opulence/src/factory/LightFactory.h"

class LevelMaker {
private:
    SpriteGenerator *spriteGen;
    InterfaceGenerator *interfaceGen;
    std::string currentLevel;
    std::vector<LevelObject> scenery;
    std::vector<LevelObject> trees;

public:
    LevelMaker(SpriteGenerator *s, InterfaceGenerator *i);
    std::vector<LevelObject> readInLevel(std::string level);
    bool saveLevel(std::vector<Sprite *> *scenery, std::vector<Anima *> *animas, LightFactory *lf);
    bool loadLevel(std::string name, std::vector<Sprite *> *scenery, std::vector<Anima *> *animas, LightFactory *lf);

    template < typename T > float string_to_float( const T& t )
    {
        std::istringstream ss(t);
        float result;
        return ss >> result ? result : 0;
    }
};

#endif
