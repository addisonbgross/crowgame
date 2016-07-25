#ifndef CROWS_CHARACTERSET_H
#define CROWS_CHARACTERSET_H

// general
#include <string>
#include <sstream>

// crowgame
#include "Character.hpp"
#include "../coin/Coin.hpp"

class CharacterSet : public virtual Interface {
private:
    float x, y;
    std::vector<Character *> characters;
    int value;

protected:
    Coin *coin;

public:
    CharacterSet( float x, float y, int size, SpriteGenerator *gen, std::string team );
    ~CharacterSet();

    // value
    bool increment();
    bool decrement();
    void setValue( int val );
    void setValue( std::string val );
    int getValue();
    void setPosition( float x, float y );
    void setContent( std::string content, float spacing );

    // interface
    glm::vec3 getPosition();
    glm::vec3 getHitBox();
    void update( float time );
    std::string getName();

    template < typename T > int string_to_int( const T& t )
    {
        std::istringstream ss(t);
        int result;
        return ss >> result ? result : 0;
    }
};

#endif
