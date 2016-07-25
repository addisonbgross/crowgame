#ifndef CROWS_DAYMANAGER_H
#define CROWS_DAYMANAGER_H

// general
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <vector>

// crowgame
#include "../generator/SpriteGenerator.hpp"

// opulence
#include "../../opulence/src/factory/LightFactory.h"

class DayManager {
private:
    SpriteGenerator *gen;

    Sprite *sun = nullptr;
    Sprite *moon = nullptr;
    Sprite *land = nullptr;
    std::vector<Sprite *> clouds;

    LightFactory *lf;

    float SPACE_DISTANCE = 500; // vertical offset from horizon
    float ORBITAL_VELOCITY = -0.0005f; // speed at which the earth turns

    glm::vec4 sunColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
    glm::vec4 moonColour = glm::vec4( 0.5f, 0.5f, 1.0f, 1.0f );
    glm::vec3 sunStartDir = glm::vec3( 0.0f, 0.0f, 1.0f );

public:

    DayManager(SpriteGenerator *gen, LightFactory *lf);
    ~DayManager();

    float rotateSky();
};

#endif
