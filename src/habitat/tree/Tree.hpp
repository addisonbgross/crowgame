#ifndef CROWS_TREE_H
#define CROWS_TREE_H

// general
#include <vector>

// crowgame
#include "../garbage/Garbage.hpp"
#include "../../generator/InterfaceGenerator.hpp"

class Tree : public virtual Habitat, public virtual Anima {
private:
    Sprite *tree;
    SpriteGenerator *spriteGen;
    InterfaceGenerator *interfaceBridge;
    Sprite *victorRing;
    std::vector<Sprite *> crowCoins;
    std::vector<Sprite *> squirrelCoins;
    bool showingCoins = false;

    int STARTING_CROWS = 0;
    int STARTING_SQUIRRELS = 0;

    // combat
    Sprite *combatMarker;
    Sprite *crowVictoryMarker;
    Sprite *squirrelVictoryMarker;
    bool inCombat = false;
    bool inVictory = false;
    float COMBAT_LENGTH = 10.0;
    float COMBAT_BUFFER = 0.1;
    float COMBAT_MARKER_SCALE = 3.0;
    float COMBAT_REFRESH = 1.0;
    float combatTime = 0;
    float victoryTime = 0;

public:
    Tree( float x, float y, float z, SpriteGenerator *gen, InterfaceGenerator *inter );
    ~Tree();

    // data
    void showCoins();
    void hideCoins();
    void toggleCoins();
    void updateCoins();
    bool isInCombat();

    // add
    void addCrowCoin( Sprite *coin );
    void addSquirrelCoin( Sprite *coin );

    // remove
    void popCrow();
    void popSquirrel();

    // Anima
    void update( float time );
    std::string getName();
    glm::vec3 getHitBox();
    glm::vec3 getPosition();

    // Habitat
    bool canFitCrows();
    bool canFitSquirrels();
    bool hasCrows();
    bool hasSquirrels();
    void arrival( std::string name );
    void departure( std::string name );
    void sendCrow( Habitat *origin, Habitat *destination );
    void sendSquirrel( Habitat *origin, Habitat *destination );
    bool pushCrow();
    bool pushSquirrel();
    void setOrientation(glm::quat rotation);
};

#endif
