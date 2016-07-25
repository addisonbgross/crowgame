#ifndef CROWS_POPULATION_H
#define CROWS_POPULATION_H

// crowgame
#include "../CharacterSet.hpp"

class Population : public CharacterSet {
public:
    Population( float x, float y, SpriteGenerator *gen, std::string type ) :
    CharacterSet( x, y, 2, gen, type )
    {
        this->coin = new Coin( x - 0.1f, y - 0.007f, gen, type );
    }
};

#endif
