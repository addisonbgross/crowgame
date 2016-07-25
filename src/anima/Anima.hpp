#ifndef CROWS_ANIMA_H
#define CROWS_ANIMA_H

// general
#include <sstream>

// glm
#include <glm/glm.hpp>

class Anima {
public:
    bool alive = true;

    virtual void update( float time ) = 0;
    virtual std::string getName() = 0;
    virtual glm::vec3 getHitBox() = 0;
    virtual glm::vec3 getPosition() = 0;

    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
};

#endif
