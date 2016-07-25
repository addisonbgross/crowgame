#ifndef CROWS_SPRITE_H
#define CROWS_SPRITE_H

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/optimum_pow.hpp>

// general
#include <iostream>
#include <vector>
#include <typeinfo>
#include <math.h>

// opulence
#include "../../opulence/src/entity/light/Light.h"
#include "../../opulence/src/entity/animation/Animation.h"
#include "../../opulence/src/entity/camera/Camera.h"
#include "../../opulence/src/factory/ModelFactory.h"

// crowgame
#include "../service/PathFinder.hpp"

class Sprite {
private:
    float scale;
    float rate;
    glm::vec3 position;
    std::vector<Model*> models;
    std::vector<Animation*> animations;
    Camera *camera = nullptr;

    glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest);

public:
    Sprite(Model *model);
    Sprite(Animation *animation);
    virtual ~Sprite();

    // the representative Entity of the Sprite
    Entity *rep;

    // movement
    std::vector<glm::vec3> path;
    bool isMoving = false;
    void move();
    void resetAnimations();
    float getAngleBetweenXZ( glm::vec3 v );
    float getAngleBetweenXY( glm::vec3 v );

    // dead Sprites will be "recycled"
    bool alive;
    bool isDead();
    void kill(ModelFactory *mf);

    // visibility
    Sprite * hide();
    Sprite * show();
    bool isVisible();

    // get
    std::string getName();
    glm::vec3 * getPosition();
    glm::quat * getOrientation();
    float getOrientationAngle();
    float getWidth();
    float getHeight();
    float getDepth();
    float getRate();
    float getScale();

    // set
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 position);
    void setOrientation(glm::quat rotation);
    void setScale(float s);
    void setRate(float r);
    void setPingPong(bool pong);
    void setHighlight(int high);
    void setInterface(int inter);
};

#endif
