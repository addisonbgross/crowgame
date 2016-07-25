#ifndef CROWS_CROWGAME_H
#define CROWS_CROWGAME_H

// general
#include <array>

// opulence
#include "opulence/opulence.cpp"

// crowgame
#include "src/ai/Ai.hpp"
#include "src/controller/standardcontroller/StandardController.hpp"
#include "src/service/Manipulator.hpp"
#include "src/utility/LevelMaker.hpp"
#include "src/utility/DayManager.hpp"
#include "src/habitat/garbage/Garbage.hpp"

// init default opulence instance
Opulence opulence( "<<< crowgame >>>" );

// screen dimensions
float screenWidth;
float screenHeight;

// opulence entity creators
ModelFactory *mf = opulence.getModelFactory();
CameraFactory *cf = opulence.getCameraFactory();
LightFactory *lf = opulence.getLightFactory();
TimeManager *tm = opulence.getTimeManager();

// scenery: non-interactive models and animations
std::vector<Sprite *> scenery;

// animas: interactive elements
std::vector<Anima *> animas;

// generators
SpriteGenerator spriteGen( mf, lf );
InterfaceGenerator interfaceGen( &animas );

// controllers
ControlSet cSet;
StandardController controller( cSet );

// artificial intelligence
Ai ai( &interfaceGen );

// day manager
DayManager *dayMan;

// terraforming
bool makeTreeDebounce = false;
bool makeLightDebounce = false;
bool makeGarbageDebounce = false;
bool makeCarDebounce = false;

#endif
