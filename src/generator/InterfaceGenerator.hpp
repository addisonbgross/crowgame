#ifndef CROWS_INTERFACEGENERATOR_H
#define CROWS_INTERFACEGENERATOR_H

// general
#include <string>

// crowgame
#include "SpriteGenerator.hpp"
#include "../habitat/Habitat.hpp"
#include "../anima/interface/button/Button.hpp"
#include "../anima/interface/character/population/Population.hpp"

class InterfaceGenerator {
private:
    // ptr to main anima vector in CrowGame.h
    std::vector<Anima *> *animas;

    // keyboard
    std::vector<Button *> buttons;

    // population
    Population *crowPop;
    Population *squirrelPop;
    Population *crowFood;
    Population *squirrelFood;

    // constants
    int CROW_COST = 2;
    int THREE_CROW_COST = 5;
    int SQUIRREL_COST = 2;
    int THREE_SQUIRREL_COST = 5;
    int CROW_STARTING_FOOD = 5;
    int SQUIRREL_STARTING_FOOD = 5;

public:
    InterfaceGenerator( std::vector<Anima *> *animas );

    // sets
    void createKeyboardButtons( SpriteGenerator *gen );
    void createCrowInterface( SpriteGenerator * gen );

    // gets
    int getCrowPopulation();
    int getSquirrelPopulation();
    int getCrowFood();
    int getSquirrelFood();

    // interactions
    void activateButton( int num, Anima *selected );
    void hideAllButtons();
    void showButtonSet( std::string name );
    void showButton( int num );
    void addFood( std::string team );
    void removeFood( std::string team );
    void addPopulation( std::string team );
    void removePopulation( std::string team );
    void removePopulation( std::string team, int amount );
    void setPopulation( std::string team, int amount );

    // squirrel actions
    bool spawnSquirrel( Habitat *habitat );
};

#endif
