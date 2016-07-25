#include "InterfaceGenerator.hpp"

InterfaceGenerator::InterfaceGenerator( std::vector<Anima *> *animas )
{
    this->animas = animas;
}

void InterfaceGenerator::createKeyboardButtons( SpriteGenerator *gen )
{
    Button *one = new Button( 0.3f, -0.5f, "oneCrow", gen );
    Button *two = new Button( 0.4f, -0.5f, "threeCrow", gen );
    two->getSprite()->setScale(0.215f);

    buttons.push_back( one );
    buttons.push_back( two );
    hideAllButtons();

    animas->push_back( one );
    animas->push_back( two );
}

void InterfaceGenerator::createCrowInterface( SpriteGenerator *gen )
{
    // crows
    crowPop = new Population( 0.3f, 0.45f, gen, "crow" );
    animas->push_back( crowPop );
    crowFood = new Population( 0.3f, 0.39f, gen, "food" );
    crowFood->setValue( CROW_STARTING_FOOD );
    animas->push_back( crowFood );

    // squirrels
    squirrelPop = new Population( -0.3f, 0.45f, gen, "squirrel" );
    animas->push_back( squirrelPop );
    squirrelFood = new Population( -0.3f, 0.39f, gen, "food" );
    squirrelFood->setValue( SQUIRREL_STARTING_FOOD );
    animas->push_back( squirrelFood );
}

// gets
int InterfaceGenerator::getCrowPopulation()
{
    return crowPop->getValue();
}

int InterfaceGenerator::getSquirrelPopulation()
{
    return squirrelPop->getValue();
}

int InterfaceGenerator::getCrowFood()
{
    return crowFood->getValue();
}

int InterfaceGenerator::getSquirrelFood()
{
    return squirrelFood->getValue();
}

// interactions
void InterfaceGenerator::activateButton( int num, Anima *selected )
{
    Button *button = buttons.at( num );

    switch ( num ) {
        // produce 1 crow unit
        case 0:
            if ( button && selected && button->isReady() ) {
                if ( selected->getName() == "tree" ) {
                    if (crowFood->getValue() >= CROW_COST) {
                        Habitat *hab = dynamic_cast<Habitat *>( selected );
                        if ( hab ) {
                            hab->pushCrow();
                            button->activate();
                            crowPop->increment();

                            for (int i = 0; i < CROW_COST; ++i) {
                                crowFood->decrement();
                            }
                        }
                    }
                }
            }
            break;

        // produce 3 crow units
        case 1:
            if ( button && selected && button->isReady() ) {
                if ( selected->getName() == "tree" ) {
                    if (crowFood->getValue() >= THREE_CROW_COST) {
                        Habitat *hab = dynamic_cast<Habitat *>( selected );
                        if ( hab ) {
                            hab->pushCrow();
                            hab->pushCrow();
                            hab->pushCrow();
                            button->activate();
                            crowPop->increment();
                            crowPop->increment();
                            crowPop->increment();

                            for (int i = 0; i < THREE_CROW_COST; ++i) {
                                crowFood->decrement();
                            }
                        }
                    }
                }
            }
            break;
    }
}

void InterfaceGenerator::hideAllButtons()
{
    for ( auto b : buttons ) {
        b->getSprite()->hide();
    }
}

void InterfaceGenerator::showButtonSet( std::string name )
{
    if ( name == "tree" ) {
        showButton( 0 );
        showButton( 1 );
    }
}

void InterfaceGenerator::showButton( int num )
{
    Button *b = buttons.at( num );

    if ( b ) {
        b->getSprite()->show();
    }
}
void InterfaceGenerator::addFood( std::string team )
{
    if ( team == "crow" ) {
        if ( crowFood ) {
            crowFood->increment();
        }
    } else if ( team == "squirrel" ) {
        if ( squirrelFood ) {
            squirrelFood->increment();
        }
    }
}

void InterfaceGenerator::removeFood( std::string team )
{
    if ( team == "crow" ) {
        if ( crowFood ) {
            crowFood->decrement();
        }
    } else if ( team == "squirrel" ) {
        if ( squirrelFood ) {
            squirrelFood->decrement();
        }
    }
}

void InterfaceGenerator::addPopulation( std::string team )
{
    if ( team == "crow" ) {
        if ( crowPop ) {
            crowPop->increment();
        }
    } else if ( team == "squirrel" ) {
        if ( squirrelPop ) {
            squirrelPop->increment();
        }
    }
}

void InterfaceGenerator::removePopulation( std::string team )
{
    if ( team == "crow" ) {
        if ( crowPop ) {
            crowPop->decrement();
        }
    } else if ( team == "squirrel" ) {
        if ( squirrelPop ) {
            squirrelPop->decrement();
        }
    }
}

void InterfaceGenerator::removePopulation( std::string team, int amount )
{
    if ( team == "crow" ) {
        if ( crowPop ) {
            crowPop->setValue( crowPop->getValue() - amount );
        }
    } else if ( team == "squirrel" ) {
        if ( squirrelPop ) {
            squirrelPop->setValue( squirrelPop->getValue() - amount );
        }
    }
}

void InterfaceGenerator::setPopulation( std::string team, int amount )
{
    if ( team == "crow" ) {
        if ( crowPop ) {
            if ( amount < 0 ) {
                amount *= -1;
            }
            crowPop->setValue( amount );
        }
    } else if ( team == "squirrel" ) {
        if ( squirrelPop ) {
            if ( amount > 0 ) {
                amount *= -1;
            }
            squirrelPop->setValue( amount );
        }
    }
}

// squirrel actions
bool InterfaceGenerator::spawnSquirrel( Habitat *habitat )
{
    if ( getSquirrelFood() >= SQUIRREL_COST ) {
        if ( habitat ) {
            habitat->pushSquirrel();
            addPopulation("squirrel");
            removeFood("squirrel");
            return true;
        }
    }

    return false;
}
