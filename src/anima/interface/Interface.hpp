#ifndef CROWS_INTERFACE_H
#define CROWS_INTERFACE_H

// crowgame
#include "../Anima.hpp"

class Interface : public virtual Anima {
private:
    // in order for mouse events to occur, we must translate the crazy
    // non-correct 3-space that Interface elements live in
    float xPos;
    float yPos;
    float width;
    float height;

public:
    // anima
    virtual void update( float time ) = 0;
    virtual std::string getName() = 0;
    virtual glm::vec3 getPosition() = 0;
    virtual glm::vec3 getHitBox() = 0;
};

#endif
