#include "DayManager.hpp"

DayManager::DayManager(SpriteGenerator *gen, LightFactory *lf)
{
    // init sun/directional light
    lf->makeDirectionalLight( &sunColour, &sunStartDir );

    this->lf = lf;
    this->gen = gen;

    sun = gen->createSpriteModel("sun");
    sun->setScale(30);
    sun->setHighlight(true);

    moon = gen->createSpriteModel("moon");
    moon->setScale(15);
    moon->setHighlight(true);

    land = gen->createSpriteModel("land");
    land->setScale(40);
    land->setPosition(20, 0.5f, -65);

    // positions
    glm::mat4 translateDown = glm::translate(glm::mat4(1.0f), glm::vec3(0, -100.0f, 0));

    glm::vec3 dir = *lf->getDirectionalLight()->getDirection();
    dir = glm::vec3( translateDown * glm::vec4( dir, 1.0f ) );
    dir *= -SPACE_DISTANCE;
    sun->setPosition(dir.x, dir.y, dir.z);
    moon->setPosition(dir.x * -1, dir.y, dir.z);
}

DayManager::~DayManager()
{
    if (sun) {
        sun->alive = false;
    }
    if (moon) {
        moon->alive = false;
    }
    if (land) {
        land->alive = false;
    }
    for (auto & cloud : clouds) {
        if (cloud) {
            cloud->alive = false;
        }
    }
}

float DayManager::rotateSky()
{
    float DAYLIGHT_FLOOR = 0.2f;
    float DAYLIGHT_CEIL = 0.9f;
    float DAYLIGHT_FACTOR = 3.0f;

    glm::vec3 dir = *lf->getDirectionalLight()->getDirection();
    glm::vec3 ogDir = dir;
    glm::vec3 noon = glm::vec3(0, -1, 0);
    dir *= -SPACE_DISTANCE;
    sun->setPosition(dir.x, dir.y, dir.z);
    dir *= -1; // moon
    moon->setPosition(dir.x, dir.y, dir.z);

    float daylight = glm::clamp( DAYLIGHT_FACTOR * glm::dot( ogDir, noon ), DAYLIGHT_FLOOR, DAYLIGHT_CEIL );
    lf->getDirectionalLight()->setDirection( glm::normalize( ogDir * glm::angleAxis( ORBITAL_VELOCITY, glm::vec3(1, 0, 0) ) ) );
    lf->setDirectionalIntensity( daylight );
    lf->setAmbientIntensity( daylight / 2.25f );

    return daylight;
}
