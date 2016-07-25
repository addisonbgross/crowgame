#include "Garbage.hpp"

Garbage::Garbage( float x, float y, float z, SpriteGenerator *gen )
{
    full = gen->createSpriteModel( "garbage_full" );
    full->setScale( 0.5f );
    full->setPosition( x, y, z );
    full->setHighlight( true );
    full->hide();

    empty = gen->createSpriteModel( "garbage_empty" );
    empty->setScale( 0.5f );
    empty->setPosition( x, y, z);
    empty->setHighlight( true );

    garbageBag = gen->createSpriteModel( "garbage_bag" );
    garbageBag->setPosition( x, y + 3.5, z );
    garbageBag->setHighlight( true );
    garbageBag->hide();

    crow = gen->createDefensiveCrow( x, y, z );
    crow->setPosition( x, y, z );
    crow->hide();

    squirrel = gen->createSquirrel( x, y, z );
    squirrel->setPosition( x, y, z );
    squirrel->hide();

    float angle = rand() % 360;
    squirrel->setOrientation( *squirrel->getOrientation() * glm::angleAxis( glm::radians(angle), glm::vec3( 0, 1, 0 ) ) );

    fullPosition = full->getPosition()->y + full->getHeight() - 0.1f;
    emptyPosition = empty->getPosition()->y + empty->getHeight() - 0.1f;

    lobber = new Lobber(x, y, z, getName(), gen);

    isFull = false;

    numCrows = 0;
    numSquirrels = 0;

    this->spriteGen = gen;
}

Garbage::~Garbage()
{
    if ( full ) {
        full->alive = false;
    }
    if ( empty ) {
        empty->alive = false;
    }
    if ( garbageBag ) {
        garbageBag->alive = false;
    }
    if ( crow ) {
        crow->alive = false;
    }
    if ( squirrel ) {
        squirrel->alive = false;
    }
}

// garbage
bool Garbage::readyForPickup()
{
    return (garbageBag->isVisible()) ? true : false;
}

// anima
void Garbage::update( float time )
{
    if ( !isFull && !isMining ) {
        refillCounter += time;

        if ( refillCounter >= REFILL_TIME ) {
            refillCounter = 0;
            isFull = true;
            full->show();
            empty->hide();
            garbageBag->hide();
        }
    }

    if ( isMining ) {
        mineCounter += time;

        if ( mineCounter >= MINE_TIME ) {
            mineCounter = 0;
            isFull = false;
            hasGoodsToSend = true;
            full->hide();
            empty->show();
            garbageBag->show();
            refillCounter = time;
        }

        if (isFull) {
            // update any flinging garbage and add new garbage
            lobber->update( time, true );
        } else {
            // update pre-existing, flung garbage
            lobber->update( time, false );
        }

    } else {
        // update any flinging garbage
        lobber->update( time, false );
    }

    if ( numCrows > 0 ) {
        garbageBag->setOrientation( *garbageBag->getOrientation() * glm::angleAxis( -0.05f, glm::vec3( 0, 1, 0 ) ) );

        if ( hasGoodsToSend ) {
            crow->goodsMarker->show();
            crow->goodsMarker->setOrientation( *crow->goodsMarker->getOrientation() * glm::angleAxis( -0.05f, glm::vec3( 0, 1, 0 ) ) );
        }

        if ( isMining && !isFull ) {
            crow->getPosition()->y = emptyPosition;
        } else {
            crow->getPosition()->y = fullPosition;
        }

    } else if ( numSquirrels > 0 ) {
        garbageBag->setOrientation( *garbageBag->getOrientation() * glm::angleAxis( -0.05f, glm::vec3( 0, 1, 0 ) ) );

        if ( hasGoodsToSend ) {
            squirrel->goodsMarker->show();
        }

        if ( isMining && !isFull ) {
            squirrel->getPosition()->y = emptyPosition;
        } else {
            squirrel->getPosition()->y = fullPosition;
        }
    }

}


bool Garbage::canFitCrows()
{
    if ( numCrows + incomingCrows < GARBAGE_COIN_LIMIT &&
         full->isVisible() &&
         isMining == false ) {
        return true;
    } else {
        return false;
    }
}

bool Garbage::canFitSquirrels()
{
    if ( numSquirrels + incomingSquirrels < GARBAGE_COIN_LIMIT &&
         full->isVisible() &&
         isMining == false ) {
        return true;
    } else {
        return false;
    }
}

int Garbage::getNumCrows()
{
    return numCrows;
}

int Garbage::getNumSquirrels()
{
    return numSquirrels;
}

int Garbage::getIncomingCrows()
{
    return incomingCrows;
}

int Garbage::getIncomingSquirrels()
{
    return incomingSquirrels;
}

glm::vec3 Garbage::getHitBox()
{
    return glm::vec3(
        full->getWidth() * 3.0f,
        full->getHeight() * 3.0f,
        full->getDepth() * 3.0f
    );
}

glm::vec3 Garbage::getPosition()
{
    return glm::vec3(
        empty->getPosition()->x,
        empty->getPosition()->y,
        empty->getPosition()->z
    );
}

void Garbage::arrival( std::string name )
{
    if ( name == "crow" ) {
        ++numCrows;
        --incomingCrows;
        crow->show();

    } else if ( name == "squirrel" ) {
        ++numSquirrels;
        --incomingSquirrels;
        squirrel->show();
    }

    isMining = true;
    mineCounter = 0;
}

void Garbage::departure( std::string name )
{
    if ( name == "crow" && numCrows > 0 ) {
        --numCrows;
        isMining = false;
    } else if ( name == "squirrel" && numSquirrels > 0 ) {
        --numSquirrels;
        isMining = false;
    }
}

void Garbage::sendCrow( Habitat *origin, Habitat *destination ) {
    crow->hide();
    crow->goodsMarker->hide();

    destination->incomingCrows++;

    glm::vec3 pos = *crow->getPosition();
    AdvancedSprite *newCrow = spriteGen->createCrow( pos.x, pos.y, pos.z );
    newCrow->setDestination( destination );

    if ( hasGoodsToSend ) {
        newCrow->setGoods( true );
        newCrow->goodsSprite->show();
        hasGoodsToSend = false;
        garbageBag->hide();
    }

    PathFinder path;
    glm::vec3 destPosition = destination->getPosition();
    destPosition.y += destination->getHitBox().y / 2.0f;
    newCrow->path = *path.flightPath(origin->getPosition(), destPosition);
    newCrow->isMoving = true;
    newCrow->show();
    newCrow->goodsMarker->show();

    departure( "crow" );
}

void Garbage::sendSquirrel( Habitat *origin, Habitat *destination ) {
    squirrel->hide();
    squirrel->goodsMarker->hide();

    destination->incomingSquirrels++;

    glm::vec3 pos = *squirrel->getPosition();
    AdvancedSprite *newSquirrel = spriteGen->createSquirrel( pos.x, pos.y, pos.z );
    newSquirrel->setDestination( destination );

    if ( hasGoodsToSend ) {
        newSquirrel->setGoods( true );
        hasGoodsToSend = false;
        garbageBag->hide();
    }

    PathFinder path;
    glm::vec3 destPosition = destination->getPosition();
    newSquirrel->path = *path.groundPath(origin->getPosition(), destPosition);
    newSquirrel->isMoving = true;
    newSquirrel->show();
    newSquirrel->goodsSprite->show();
    newSquirrel->goodsMarker->show();

    departure( "squirrel" );
}

bool Garbage::pushCrow() { return false; }
bool Garbage::pushSquirrel() { return false; }

void Garbage::setOrientation(glm::quat rotation)
{
    full->setOrientation( rotation );
    empty->setOrientation( rotation );
    garbageBag->setOrientation( rotation );
}

std::string Garbage::getName()
{
    return "garbage";
}
