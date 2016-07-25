#ifndef CROWS_OMNICONTROLLER_H
#define CROWS_OMNICONTROLLER_H

// general
#include <iostream>
#include <deque>

// crowgame
#include "../Controller.hpp"

/**
 * OmniController - Capable of creating/destroying objects in game
 */
class StandardController : public Controller {
private:
    int MAX_HISTORY = 10;
    std::deque<SDL_Event> *eventHistory;  // keep all input events ever

public:
    StandardController(ControlSet controlSet);
    ~StandardController();

    // get
    SDL_Event * getLastEvent();      // get last performed & read input event
    std::vector<std::string> * getInput(SDL_Event event);  // read input from keyboard and mouse

    // set
    void addCommand(std::string cmd);
};

#endif
