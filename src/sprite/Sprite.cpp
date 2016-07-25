#include "Sprite.hpp"

Sprite::Sprite(Model *model)
{
    scale = 1.0;
    rep = model;
    models.push_back(model);
    alive = true;
}

Sprite::Sprite(Animation *animation)
{
    scale = 1.0;
    rep = animation;
    animations.push_back(animation);
    alive = true;
}

Sprite::~Sprite() {}

void Sprite::move()
{
    // const
    float MOVE_LIMIT = 0.5f;
    float MOVE_RATE = 0.25f;
    float TURN_LIMIT = 5.5f;
    float TURN_RATE = 0.1f;

    glm::vec3 traj = path[0];
    glm::vec3 dir = *this->getPosition() - traj;

    if (glm::length(dir) < MOVE_LIMIT) {
        path.erase(path.begin());

    } else {
        // move along path
        setPosition( *getPosition() - glm::normalize(dir) * MOVE_RATE );

        // create direction vector from Sprite's orientation quat
        glm::quat orientation = glm::normalize( *this->rep->getOrientationQuat() );

        // if the Sprite needs to turn more than +/-180 degrees, turn the other way
        float yawDifference = getAngleBetweenXZ( dir );
        int turnMod = (yawDifference > 0) ? 1 : -1;

        if ( glm::abs( yawDifference ) >= TURN_LIMIT ) {
            orientation *= glm::angleAxis( turnMod * TURN_RATE, glm::vec3(0, 1, 0) );
        }

        //float pitchDifference = getAngleBetweenXY( dir );
        //turnMod = (getPosition()->y > traj.y) ? 1 : -1;
        //glm::mat4 rotationMatrix = glm::rotate(glm::mat5(1.0f), glm::radians(90.f), glm::vec3(0.0f, 1.0f, 0.0f));
        //glm::vec3 lateralAxis = glm::normalize( glm::vec4( rotationMatrix * glm::vec4(dir, 1.0f) ) );
       
        //orientation *= glm::angleAxis( -turnMod * TURN_RATE, glm::normalize( lateralAxis ) );
        //std::cout << pitchDifference << std::endl;
        this->setOrientation( glm::normalize( orientation ) );
    }
}

void Sprite::resetAnimations()
{
    for (auto const& animation : animations) {
        animation->restart();
    }
}

float Sprite::getAngleBetweenXZ( glm::vec3 v )
{
    // create direction vector from Sprite's orientation quat
    glm::quat ori = glm::normalize( *this->rep->getOrientationQuat() );
    glm::vec4 refined = ori * glm::vec4( glm::vec3(0, 0, 1), 1.0f );

    // calculate the Pitch difference projected against the XZ plane
    float oPitch = std::atan2( refined.x, -refined.z );
    float dPitch = std::atan2( glm::normalize( v ).x, -glm::normalize( v ).z );
    float result = glm::degrees( oPitch - dPitch );

    // for best results make any turns more than 180 degrees negative
    // in the other direction
    if (result > 180.0f) {
        result = (result - 180.0f) * -1;
    } else if (result < -180.0f) {
        result = (result + 180.0f) * -1;
    }

    return result;
}

float Sprite::getAngleBetweenXY( glm::vec3 v )
{
    // create direction vector from Sprite's orientation quat
    glm::quat ori = glm::normalize( *this->rep->getOrientationQuat() );
    glm::vec4 refined = ori * glm::vec4( glm::vec3(0, 0, 1), 1.0f );

    // calculate the pitch difference projected against the XY plane
    float oPitch = std::atan2( refined.x, -refined.y );
    float dPitch = std::atan2( glm::normalize( v ).x, -glm::normalize( v ).y );
    float result = glm::degrees( oPitch - dPitch );

    // for best results make any turns more than 180 degrees negative
    // in the other direction
    if (result > 180.0f) {
        result = (result - 180.0f) * -1;
    } else if (result < -180.0f) {
        result = (result + 180.0f) * -1;
    }

    return result;
}

bool Sprite::isDead()
{
    // dead from an external source
    if (!alive) {
        return true;
    }

    // hidden from being a one-time animation
    for (int i = 0; i < animations.size(); ++i) {
        if (!animations.at(i)->getIsRunning() && animations.at(i)->getIsOnce()) {
            this->hide();
        }
    }

    // I'm not dead yet
    return false;
}

void Sprite::kill(ModelFactory *mf)
{
    if (models.size() > 0) {
        for (int i = 0; i < models.size(); ++i) {
            mf->destroyModel(models.at(i));
            models.erase(models.begin() + i);
        }
    }

    if (animations.size() > 0) {
        for (int i = 0; i < animations.size(); ++i) {
            mf->destroyAnimation(animations.at(i));
            animations.erase(animations.begin() + i);
        }
    }
}

// visibility
Sprite * Sprite::hide()
{
    for (int i = 0; i < models.size(); ++i) {
        models.at(i)->hidden = true;
    }

    for (int i = 0; i < animations.size(); ++i) {
        for (int j = 0; j < animations.at(i)->getNumFrames(); ++j) {
            animations.at(i)->getFrame(j)->hidden = true;
        }
    }

    return this;
}

Sprite * Sprite::show()
{
    for (int i = 0; i < models.size(); ++i) {
        models.at(i)->hidden = false;
    }

    for (int i = 0; i < animations.size(); ++i) {
        for (int j = 0; j < animations.at(i)->getNumFrames(); ++j) {
            animations.at(i)->getFrame(j)->hidden = false;
        }
    }

    return this;
}

bool Sprite::isVisible()
{
    return ( models.at(0)->hidden == false ) ? true : false;
}

// get
std::string Sprite::getName()
{
    return rep->name;
}

glm::vec3 * Sprite::getPosition()
{
    return &rep->position;
}

glm::quat * Sprite::getOrientation()
{
    return rep->getOrientationQuat();
}

float Sprite::getOrientationAngle()
{
    return getAngleBetweenXZ( glm::vec3(0, 0, 1) );
}

float Sprite::getWidth()
{
    return rep->getWidth();
}

float Sprite::getHeight()
{
    return rep->getHeight();
}

float Sprite::getDepth()
{
    return rep->getDepth();
}

float Sprite::getScale()
{
    return scale;
}

// set
void Sprite::setPosition(float x, float y, float z)
{
    if (models.size() > 0) {
        for (int i = 0; i < models.size(); ++i) {
            models.at(i)->position.x = x;
            models.at(i)->position.y = y;
            models.at(i)->position.z = z;
        }
    }

    if (animations.size() > 0) {
        for (int i = 0; i < animations.size(); ++i) {
            animations.at(i)->position.x = x;
            animations.at(i)->position.y = y;
            animations.at(i)->position.z = z;
        }
    }
}

void Sprite::setPosition(glm::vec3 position)
{
    this->setPosition(position.x, position.y, position.z);
}

void Sprite::setOrientation(glm::quat rotation)
{
    for (int i = 0; i < models.size(); ++i) {
            models[i]->setOrientation( rotation );
    }

    for (int i = 0; i < animations.size(); ++i) {
            animations[i]->setOrientation( rotation );
    }
}

void Sprite::setScale(float s)
{
    scale = s;

    if (models.size() > 0) {
        for (int i = 0; i < models.size(); ++i) {
            models.at(i)->setScale(s);
        }
    }

    if (animations.size() > 0) {
        for (int i = 0; i < animations.size(); ++i) {
            animations.at(i)->setScale(s);
        }
    }
}

void Sprite::setRate(float r)
{
    rate = r;

    if (animations.size() > 0) {
        for (int i = 0; i < animations.size(); ++i) {
            animations.at(i)->setRate(r);
        }
    }
}

void Sprite::setPingPong(bool pong)
{
    if ( animations.size() > 0 ) {
        for ( auto const& animation : animations ) {
            animation->setPingPong( true );
        }
    }
}

glm::quat Sprite::rotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
    float m = glm::sqrt(2.f + 2.f * glm::dot(start, dest));
    glm::vec3 w = (1.f / m) * glm::cross(start, dest);
    return glm::normalize( glm::quat(0.5f * m, w.x, w.y, w.z) );
}

void Sprite::setHighlight(int high)
{
    if ( high == 1 ) {
        for (auto const& model : models) {
            model->setHighlight( 1 );
        }
        for (auto const& animation : animations) {
            animation->setHighlight( 1 );
        }
    } else {
        for (auto const& model : models) {
            model->setHighlight( 0 );
        }
        for (auto const& animation : animations) {
            animation->setHighlight( 0 );
        }
    }
}

void Sprite::setInterface(int inter)
{
    if ( inter == 1 ) {
        for (auto const& model : models) {
            model->setInterface( 1 );
        }
        for (auto const& animation : animations) {
            animation->setInterface( 1 );
        }
    } else {
        for (auto const& model : models) {
            model->setInterface( 0 );
        }
        for (auto const& animation : animations) {
            animation->setInterface( 0 );
        }
    }
}
