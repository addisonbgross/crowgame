#include "Tree.hpp"

Tree::Tree( float x, float y, float z, SpriteGenerator *gen, InterfaceGenerator *inter )
{
    tree = gen->createSpriteModel( x, y, z, "tree" );

    // used for displaying the victor of a tree battle
    victorRing = nullptr;

    // combat duration marker
    combatMarker = gen->createSpriteModel(
                            x,
                            y + tree->getHeight() / 2,
                            z, "combat_marker" );
    combatMarker->setScale( COMBAT_MARKER_SCALE );
    combatMarker->setHighlight( true );
    combatMarker->hide();

    // crow victory explosion
    crowVictoryMarker = gen->createSpriteModel(
                            x,
                            y + tree->getHeight() / 2,
                            z, "crowVictory" );
    crowVictoryMarker->setScale( COMBAT_MARKER_SCALE );
    crowVictoryMarker->setHighlight( true );
    crowVictoryMarker->hide();

    // squirrel victory explosion
    squirrelVictoryMarker = gen->createSpriteModel(
                            x,
                            y + tree->getHeight() / 2,
                            z, "squirrelVictory" );
    squirrelVictoryMarker->setScale( COMBAT_MARKER_SCALE );
    squirrelVictoryMarker->setHighlight( true );
    squirrelVictoryMarker->hide();

    interfaceBridge = inter;
    spriteGen = gen;
    numCrows = STARTING_CROWS;
    numSquirrels = STARTING_SQUIRRELS;

    for ( int i = 0; i < TREE_COIN_LIMIT; ++i ) {
        addCrowCoin( gen->createSpriteModel( "crowCoin" )->hide() );
        addSquirrelCoin( gen->createSpriteModel( "squirrelCoin" )->hide() );
    }
}

Tree::~Tree() {
    if ( tree ) {
        tree->alive = false;
    }

    if ( combatMarker ) {
        combatMarker->alive = false;
    }

    for ( auto const coin : crowCoins ) {
        coin->alive = false;
    }

    for ( auto const coin : squirrelCoins ) {
        coin->alive = false;
    }
}

void Tree::sendCrow( Habitat *origin, Habitat *destination )
{
    Garbage *garb = dynamic_cast<Garbage *>( destination );
    if ( garb && garb->incomingSquirrels > 0) {
        return;
    }

    destination->incomingCrows++;

    AdvancedSprite *newCrow = spriteGen->createCrow(
                                    tree->getPosition()->x,
                                    tree->getPosition()->y,
                                    tree->getPosition()->z );

    newCrow->setDestination( destination );
    PathFinder path;
    glm::vec3 destPosition = destination->getPosition();
    destPosition.y += getPosition().y;
    newCrow->path = *path.flightPath(origin->getPosition(), destPosition);
    newCrow->isMoving = true;

    departure( "crow" );
}

void Tree::sendSquirrel( Habitat *origin, Habitat *destination )
{
    Garbage *garb = dynamic_cast<Garbage *>( destination );
    if ( garb && garb->incomingCrows > 0) {
        return;
    }

    destination->incomingSquirrels++;

    AdvancedSprite *newSquirrel = spriteGen->createSquirrel(
                                        tree->getPosition()->x,
                                        tree->getPosition()->y,
                                        tree->getPosition()->z );

    newSquirrel->setDestination( destination );
    PathFinder path;
    glm::vec3 destPosition = destination->getPosition();
    newSquirrel->path = *path.groundPath(origin->getPosition(), destPosition);
    newSquirrel->isMoving = true;

    departure( "squirrel" );
}

// data
void Tree::showCoins()
{
    showingCoins = true;
}

void Tree::hideCoins()
{
    showingCoins = false;
}

void Tree::toggleCoins()
{
    if ( showingCoins ) {
        for ( auto const& crow : crowCoins ) {
            crow->hide();
        }
        for ( auto const& squirrel : squirrelCoins) {
            squirrel->hide();
        }
        showingCoins = false;

    } else {
        showingCoins = true;
    }
}

void Tree::updateCoins()
{
    if ( !showingCoins ) {
        for ( int i = 0; i < TREE_COIN_LIMIT; ++i ) {
            crowCoins.at( i )->hide();
            squirrelCoins.at( i )->hide();
        }
    } else {
        for ( int i = 0; i < TREE_COIN_LIMIT; ++i ) {
            if ( i < numCrows ) {
                crowCoins.at( i )->show();
            } else {
                crowCoins.at( i )->hide();
            }

            if ( i < numSquirrels ) {
                squirrelCoins.at( i )->show();
            } else {
                squirrelCoins.at( i )->hide();
            }
        }
    }
}

bool Tree::isInCombat()
{
    return inCombat;
}

// add
void Tree::addCrowCoin( Sprite *coin )
{
    float stackHeight = coin->rep->getHeight() * crowCoins.size();

    // orient coins in two stacks of 5
    float zDiff = 0;
    if ( crowCoins.size() >= TREE_COIN_LIMIT / 2 ) {
        zDiff = coin->rep->getWidth() * 1.1f;
        stackHeight = (crowCoins.size() - TREE_COIN_LIMIT / 2) * coin->rep->getHeight();
    }

    coin->setPosition( tree->rep->position.x,
                       tree->rep->position.y + tree->rep->getHeight() + ( stackHeight * 1.15f ),
                       tree->rep->position.z - coin->rep->getWidth() * 0.6f - zDiff );
    coin->setHighlight( 1 );
    crowCoins.push_back( coin );
}

void Tree::addSquirrelCoin( Sprite *coin )
{
    float stackHeight = coin->rep->getHeight() * squirrelCoins.size();

    // orient coins in two stacks of 5
    float zDiff = 0;
    if ( squirrelCoins.size() >= TREE_COIN_LIMIT / 2 ) {
        zDiff = coin->rep->getWidth() * 1.1f;
        stackHeight = (squirrelCoins.size() - TREE_COIN_LIMIT / 2) * coin->rep->getHeight();
    }

    coin->setPosition( tree->rep->position.x,
                       tree->rep->position.y + tree->rep->getHeight() + ( stackHeight * 1.15f ),
                       tree->rep->position.z + coin->rep->getWidth() * 0.6f + zDiff );
    coin->setHighlight( 1 );
    squirrelCoins.push_back( coin );
}

bool Tree::pushCrow()
{
    if ( numCrows + 1 < TREE_COIN_LIMIT ) {
        ++numCrows;

        if ( inCombat ) {
            combatTime -= COMBAT_REFRESH;
        }

        return true;
    }

    return false;
}

bool Tree::pushSquirrel()
{
    if ( numSquirrels + 1 < TREE_COIN_LIMIT ) {
        ++numSquirrels;

        if ( inCombat ) {
            combatTime -= COMBAT_REFRESH;
        }

        return true;
    }

    return false;
}

// remove
void Tree::popCrow()
{
    crowCoins.back()->alive = false;
    crowCoins.pop_back();
}

void Tree::popSquirrel()
{
    squirrelCoins.back()->alive = false;
    squirrelCoins.pop_back();
}

void Tree::update( float time )
{
    // combat
    if ( numCrows > 0 && numSquirrels > 0 ) {
        inCombat = true;
        showCoins();
        combatMarker->show();

        float spin = 0.02f;
        combatMarker->setOrientation( *combatMarker->getOrientation() * glm::angleAxis( spin, glm::vec3(0, 1, 0) ) );

        combatTime += time;
        float combatRatio = combatTime / COMBAT_LENGTH;
        float scaling = (1 - combatRatio) * COMBAT_MARKER_SCALE;
        combatMarker->setScale( scaling );

        if ( combatTime >= COMBAT_LENGTH - COMBAT_BUFFER ) {
            if ( numCrows > numSquirrels ) {
                interfaceBridge->removePopulation("squirrel", numSquirrels);
                numSquirrels = 0;
                victorRing = crowVictoryMarker;
            } else if ( numCrows < numSquirrels ) {
                interfaceBridge->removePopulation("crow", numCrows);
                numCrows = 0;
                victorRing = squirrelVictoryMarker;
            } else {
                interfaceBridge->removePopulation("squirrel", numSquirrels);
                interfaceBridge->removePopulation("crow", numCrows);
                numSquirrels = 0;
                numCrows = 0;
                victorRing = combatMarker;
            }

            inVictory = true;
            inCombat = false;
            combatTime = 0;
            hideCoins();
            victorRing->show();
        }
    } else {
        inCombat = false;
        combatMarker->hide();
    }

    // update coins
    if ( showingCoins ) {
        float spin = 0.05f;
        glm::quat rotation = glm::angleAxis( spin, glm::vec3(0, 1, 0) );

        for ( auto &coin : crowCoins ) {
            coin->setOrientation( glm::normalize( *coin->getOrientation() * rotation ) );
        }

        for ( auto &coin : squirrelCoins ) {
            coin->setOrientation( glm::normalize( *coin->getOrientation() * glm::inverse( rotation ) ) );
        }
    }

    updateCoins();

    if ( inVictory && victorRing ) {
        victoryTime += 7.5f * time;
        float combatRatio = victoryTime / COMBAT_LENGTH;
        float scaling = (combatRatio) * COMBAT_MARKER_SCALE;
        victorRing->setScale( 2 * scaling );

        float spin = 0.15f;
        victorRing->setOrientation( *victorRing->getOrientation() * glm::angleAxis( spin, glm::vec3(0, 1, 0) ) );

        if ( victoryTime >= COMBAT_LENGTH - COMBAT_BUFFER ) {
            victorRing->hide();
            victorRing = nullptr;
            inVictory = false;
            victoryTime = 0;
        }
    }
}

bool Tree::canFitCrows()
{
    if ( numCrows + incomingCrows < TREE_COIN_LIMIT ) {
        return true;
    } else {
        return false;
    }
}

bool Tree::canFitSquirrels()
{
    if ( numSquirrels + incomingSquirrels < TREE_COIN_LIMIT ) {
        return true;
    } else {
        return false;
    }
}

bool Tree::hasCrows()
{
    return (numCrows > 0) ? true : false;
}

bool Tree::hasSquirrels()
{
    return (numSquirrels > 0) ? true : false;
}

glm::vec3 Tree::getHitBox()
{
    return glm::vec3(
        tree->rep->getWidth(),
        tree->rep->getHeight() * 3.0f,
        tree->rep->getDepth()
    );
}

glm::vec3 Tree::getPosition()
{
    return glm::vec3(
        tree->rep->position.x,
        tree->rep->position.y,
        tree->rep->position.z
    );
}

void Tree::arrival( std::string name )
{
    glm::vec3 pos = getPosition();
    Sprite *leaves = spriteGen->createSpriteAnimation("leaves", "once");
    leaves->setPosition(pos.x, pos.y + tree->getHeight(), pos.z);
    leaves->setRate(0.45f);
    leaves->setScale(0.35f);
    leaves->setHighlight(true);

    if ( name == "crow" ) {
        ++numCrows;
        --incomingCrows;

    } else if ( name == "crow_has_goods" ) {
        ++numCrows;
        --incomingCrows;
        interfaceBridge->addFood( "crow" );

    } else if ( name == "squirrel" ) {
        ++numSquirrels;
        --incomingSquirrels;

    } else if ( name == "squirrel_has_goods" ) {
        ++numSquirrels;
        --incomingSquirrels;
        interfaceBridge->addFood( "squirrel" );

    } else {
        std::cout << "<Error> Tree.cpp-> arrival() has weird arg : " << name << std::endl;
    }

    if ( inCombat ) {
       combatTime -= COMBAT_REFRESH;
    }
}

void Tree::departure( std::string name )
{
    if ( name == "crow" && numCrows > 0 ) {
        --numCrows;
    } else if ( name == "squirrel" && numSquirrels > 0 ) {
        --numSquirrels;
    } else {
        std::cout << "<Error> Tree.cpp-> departure() has weird arg : " << name << std::endl;
    }
}

void Tree::setOrientation(glm::quat rotation)
{
    tree->setOrientation( rotation );
}

std::string Tree::getName()
{
    return "tree";
}
