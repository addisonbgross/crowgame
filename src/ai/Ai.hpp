#ifndef CROWS_AI_H
#define CROWS_AI_H

// general
#include <iostream>
#include <vector>
#include <algorithm>

// crowgame
#include "AiState.hpp"
#include "../habitat/tree/Tree.hpp"
#include "../habitat/garbage/Garbage.hpp"

class Ai {
private:
    InterfaceGenerator *interfaceGen;

    // state
    int currentDifficulty;
    AiState currentState;

    // cache items
    std::vector<Tree *> occupiedTrees;
    std::vector<Garbage *> occupiedGarbages;

    // diffculty delay (in seconds)
    float delayCounter = 0;
    const float EASY = 2.5f;
    const float MEDIUM = 1.25f;
    const float HARD = 0.75f;
    const float UNFAIR = 0;

    void addTree(Tree *tree);
    void removeTree(Tree *tree);
    bool occupyEmptyTree(std::vector<Anima *> *animas);
    bool occupyResource(std::vector<Anima *> *animas);
    bool reproduce();
    void harvestResources();
    void launchAttack(std::vector<Anima *> *animas);

public:
    Ai(InterfaceGenerator *interfaceGen);
    ~Ai();

    void update(float time, std::vector<Anima *> *animas);
};

#endif
