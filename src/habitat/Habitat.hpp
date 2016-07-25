#ifndef CROWS_HABITAT_H
#define CROWS_HABITAT_H

// crowgame
#include "../sprite/Sprite.hpp"

class Habitat {
public:
    int TREE_COIN_LIMIT = 10;
    int GARBAGE_COIN_LIMIT = 1;

    int numCrows = 0;
    int numSquirrels = 0;
    int incomingCrows = 0;
    int incomingSquirrels = 0;

    virtual bool canFitCrows() = 0;
    virtual bool canFitSquirrels() = 0;
    virtual glm::vec3 getHitBox() = 0;
    virtual glm::vec3 getPosition() = 0;
    virtual void arrival( std::string name ) = 0;
    virtual void departure( std::string name ) = 0;
    virtual void sendCrow( Habitat *origin, Habitat *destination ) = 0;
    virtual void sendSquirrel( Habitat *origin, Habitat *destination ) = 0;
    virtual bool pushCrow() = 0;
    virtual bool pushSquirrel() = 0;
    virtual void setOrientation(glm::quat rotation) = 0;

    int getNumCrows()
    {
        return numCrows;
    }

    int getNumSquirrels()
    {
        return numSquirrels;
    }
};

#endif
