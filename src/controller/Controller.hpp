#ifndef CROWS_CONTROLLER_H
#define CROWS_CONTROLLER_H

#include "SDL.h"

// crowgame
#include "../sprite/Sprite.hpp"
#include "./controlset/ControlSet.hpp"

/******************************************
 * Controller - Interface for user inputs *
 ******************************************/
class Controller {
private:
    Sprite *selected;
    float x;
    float y;

public:
    ControlSet controlSet;
    std::vector<std::string> commands;

    /* virtual */
    virtual SDL_Event * getLastEvent() = 0;
    virtual std::vector<std::string> * getInput(SDL_Event e) = 0;

    // get
    Sprite * getSelected()
    {
        return selected;
    }

    // set
    void setControlSet(ControlSet set)
    {
        controlSet = set;
    }
    void setSelected(Sprite *sprite)
    {
        selected = sprite;
    }
};

#endif
