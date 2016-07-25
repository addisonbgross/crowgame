#include "StandardController.hpp"

StandardController::StandardController(ControlSet controlSet)
{
    this->controlSet = controlSet;
    eventHistory = new std::deque<SDL_Event>();
}

StandardController::~StandardController()
{
    delete eventHistory;
}

SDL_Event * StandardController::getLastEvent()
{
    return &eventHistory->back();
}

std::vector<std::string> * StandardController::getInput(SDL_Event e)
{
    commands.clear();

    // currently pressed button
    GLuint buttonState = e.button.state;
    GLuint button = e.button.button;
    const Uint8 *key = SDL_GetKeyboardState(NULL);

    // keep record of inputs
    if (eventHistory->size() == MAX_HISTORY) {
        eventHistory->pop_front();
    }
    eventHistory->push_back(e);

    // middle
    if (button == SDL_BUTTON_MIDDLE) addCommand("rotate camera horizontally");
    if (button == SDL_BUTTON_MIDDLE) addCommand("rotate camera vertically");

    // mouse wheel
    if (e.type == SDL_MOUSEWHEEL) addCommand("zoom");

    // left
    if (button == SDL_BUTTON_LEFT && key[SDL_SCANCODE_0] && e.button.state == SDL_PRESSED) addCommand("make crow");
    else if (button == SDL_BUTTON_LEFT && key[SDL_SCANCODE_9] && e.button.state == SDL_PRESSED) addCommand("make squirrel");
    else if (button == SDL_BUTTON_LEFT && key[SDL_SCANCODE_1] && e.button.state == SDL_PRESSED) addCommand("make tree");
    else if (button == SDL_BUTTON_LEFT && e.button.state == SDL_PRESSED) addCommand("select");

    // right
    if (button == SDL_BUTTON_RIGHT && e.button.state == SDL_PRESSED) addCommand("action");

    // delete
    if (key[SDL_SCANCODE_BACKSPACE] && key[SDL_SCANCODE_F]) addCommand("delete all");
    else if (key[SDL_SCANCODE_BACKSPACE]) addCommand("delete");

    // controller
    if (key[SDL_SCANCODE_TAB] && key[SDL_SCANCODE_LSHIFT]) addCommand("control player");
    else if (key[SDL_SCANCODE_TAB]) addCommand("control all");

    // hot keys
    if (key[SDL_SCANCODE_A]) addCommand("button one");
    if (key[SDL_SCANCODE_S]) addCommand("button two");
    if (key[SDL_SCANCODE_D]) addCommand("button three");
    if (key[SDL_SCANCODE_F]) addCommand("button four");

    // stats
    if (key[SDL_SCANCODE_PERIOD]) addCommand("stats");

    // timer
    if (key[SDL_SCANCODE_N]) addCommand("start timer");
    else if (key[SDL_SCANCODE_M]) addCommand("stop timer");

    // draw style
    if (key[SDL_SCANCODE_T]) addCommand("draw light");
    else if (key[SDL_SCANCODE_Y]) addCommand("draw normal");
    else if (key[SDL_SCANCODE_U]) addCommand("draw dark");

    // terraforming
    if (button == SDL_BUTTON_LEFT && key[SDL_SCANCODE_LCTRL]) addCommand("make tree");
    if (button == SDL_BUTTON_LEFT && key[SDL_SCANCODE_LSHIFT]) addCommand("make light");
    if (button == SDL_BUTTON_LEFT && key[SDL_SCANCODE_RCTRL]) addCommand("make garbage");
    if (button == SDL_BUTTON_LEFT && key[SDL_SCANCODE_4]) addCommand("make car");

    // memories
    if (key[SDL_SCANCODE_0]) addCommand("save game");;
    if (key[SDL_SCANCODE_8]) addCommand("load game");

    // quit
    if (key[SDL_SCANCODE_ESCAPE] || e.type == SDL_QUIT) addCommand("quit");

    return &commands;
}

void StandardController::addCommand(std::string cmd)
{
    if (controlSet.rules.find( cmd )->second) {
        commands.push_back(controlSet.rules.find( cmd )->first);
    }
}
