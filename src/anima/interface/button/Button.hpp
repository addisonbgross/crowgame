#ifndef CROWS_BUTTON_H
#define CROWS_BUTTON_H

// general
#include <vector>

// crowgame
#include "../Interface.hpp"
#include "../../../generator/SpriteGenerator.hpp"

class Button : public virtual Interface {
private:
    // the button is either inactive (not in use), active (the effect
    // of the button's host in occurence). A button will pass through
    //
    // INACTIVE -> RECEEDING -> ACTIVE -> PRECEEDING -> INACTIVE
    //
    // If the button's toggle=true, then the button will stay in the
    // ACTIVE state until it is pressed again.
    enum button_state {
        INACTIVE,
        ACTIVE,
        RECEEDING,
        PRECEEDING
    };
    button_state currentState = INACTIVE;
    float rotationTracker;
    float depthTracker;
    bool toggle = false;
    std::string name;

    Sprite *button;

public:
    Button( float x, float y, std::string type, SpriteGenerator *gen );
    ~Button();

    // button
    void activate();
    bool isReady();

    // set
    void setOrientation( glm::quat orientation );
    void setToggle( bool t );

    // get
    glm::quat * getOrientation();
    Sprite * getSprite();

    // interface
    glm::vec3 getPosition();
    glm::vec3 getHitBox();
    void update( float time );
    std::string getName();
};

#endif
