#ifndef CROWS_CHARACTER_H
#define CROWS_CHARACTER_H

// general
#include <string>

// crowgame
#include "../../../generator/SpriteGenerator.hpp"
#include "../Interface.hpp"

class Character : public virtual Interface {
private:
    int nums[ 10 ] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    std::string currentCharacter;
    std::vector<Sprite *> numbers;
    Sprite *content = nullptr;
    SpriteGenerator *spriteGen;

public:
    Character( float x, float y, SpriteGenerator *gen );
    ~Character();

    void setValue( std::string num );
    void setContent( char content );
    void setPosition( float x, float y );
    void hideAll();

    // interface
    glm::vec3 getPosition();
    glm::vec3 getHitBox();
    void update( float time );
    std::string getName();

    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
};

#endif
