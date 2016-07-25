#ifndef CROWS_GARBAGE_H
#define CROWS_GARBAGE_H

// crowgame
#include "../Habitat.hpp"
#include "../../anima/Anima.hpp"
#include "../../service/Lobber.hpp"
#include "../../generator/SpriteGenerator.hpp"

class Garbage : public virtual Habitat, public virtual Anima {
private:
    // timing (in seconds)
    float REFILL_TIME = 3.0f;
    float MINE_TIME = 3.0f;

    float timeCounter = 0;
    float refillCounter = 0;
    float mineCounter = 0;
    float fullPosition = 0;
    float emptyPosition = 0;

    bool isFull = false;
    bool isMining = false;
    bool hasGoodsToSend = false;

    SpriteGenerator *spriteGen;

    Sprite *full;
    Sprite *empty;
    Sprite *garbageBag;
    AdvancedSprite *crow;
    AdvancedSprite *squirrel;

    Lobber *lobber;

public:
    Garbage( float x, float y, float z, SpriteGenerator *gen );
    ~Garbage();

    // garbage
    bool readyForPickup();

    // Anima
    void update( float time );
    std::string getName();
    glm::vec3 getHitBox();
    glm::vec3 getPosition();

    // Habitat
    bool canFitCrows();
    bool canFitSquirrels();
    int getNumCrows();
    int getNumSquirrels();
    int getIncomingCrows();
    int getIncomingSquirrels();
    void arrival( std::string name );
    void departure( std::string name );
    void sendCrow( Habitat *origin, Habitat *destination );
    void sendSquirrel( Habitat *origin, Habitat *destination );
    bool pushCrow();
    bool pushSquirrel();
    void setOrientation(glm::quat rotation);
};

#endif
