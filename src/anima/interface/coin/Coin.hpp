#ifndef CROWS_COIN_H
#define CROWS_COIN_H

// crowgame
#include "../Interface.hpp"

class Coin : public virtual Interface {
private:
    Sprite *coin;

public:
    Coin( float x, float y, SpriteGenerator *gen, std::string team )
    {
        if ( team == "crow" ) {
            coin = gen->createSpriteModel( "crowCoin" );
        } else if ( team == "squirrel" ) {
            coin = gen->createSpriteModel( "squirrelCoin" );
        } else {
            coin = gen->createSpriteModel( "foodCoin" );
        }

        coin->setScale( 0.04f );
        coin->setPosition( x, y, -1 );
        coin->setHighlight( true );
        coin->setInterface( true );
    }

    ~Coin()
    {
        coin->alive = false;
    }

    // interface
    glm::vec3 getPosition()
    {
        return glm::vec3(
            coin->getPosition()->x,
            coin->getPosition()->y,
            coin->getPosition()->z
        );
    }

    glm::vec3 getHitBox()
    {
        return glm::vec3(
            coin->getWidth(),
            coin->getHeight(),
            coin->getDepth()
        );
    }

    void update( float time ) {};

    std::string getName()
    {
        return "coin";
    }

};

#endif
