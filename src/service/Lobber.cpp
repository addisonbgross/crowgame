#include "Lobber.hpp"

Lobber::Lobber(float x, float y, float z, std::string type, SpriteGenerator *spriteGen)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->spriteGen = spriteGen;
    this->type = type;

    if (type == "garbage") {
        lobDelay = 0.5f;
        this->delayCounter = lobDelay;
    }
}

Lobber::~Lobber()
{
    for (auto obj : objects) {
        std::get<0>(obj)->alive = false;
    }
}

void Lobber::lob()
{
    if (type == "garbage") {
        std::string objName = garbageSet.at( rand() % garbageSet.size() );
        Sprite *obj = spriteGen->createSpriteModel(objName);
        obj->setPosition(x, y, z);
        obj->setScale(0.2f);

        float angle = rand() % 360;
        glm::vec3 direction = glm::rotate( glm::vec3(1, 0, 0), glm::radians(angle), glm::vec3(0, 1, 0) );
        objects.push_back( std::make_tuple(obj, false, direction) );

    } else {
        std::cout << "some other type" << std::endl;
    }
}

void Lobber::update(float time, bool active)
{
    // lob a new object after a delay
    if (active) {
        delayCounter += time;

        if (delayCounter > lobDelay) {
            lob();

            // set delay to random point between 0 and lobDelay
            delayCounter = static_cast<float>( rand() ) / (static_cast <float>(RAND_MAX / lobDelay) );
        }
    } else {
        delayCounter = 0;
    }

    // update pre-lobbed objects
    for (int i = 0; i < objects.size(); ++i) {
        std::tuple<Sprite *, bool, glm::vec3> obj = objects.at(i);
        float diff = glm::abs( lobHeight - std::get<0>(obj)->getPosition()->y );
        glm::vec3 *position = std::get<0>(obj)->getPosition();

        if (!std::get<1>(obj) && position->y + heightBuffer < lobHeight) {
            position->y += diff * 0.1f;

        } else if (std::get<0>(obj)->getPosition()->y > 0) {
            std::get<1>(objects.at(i)) = true;
            position->y -= diff * 0.1f;

        } else {
            std::get<0>(objects.at(i))->alive = false;
            objects.erase(objects.begin() + i);
        }

        // translate object
        glm::vec3 direction = std::get<2>(obj);
        *position += direction * lobSpeed;

        // rotate object
        glm::quat *orientation = std::get<0>(obj)->getOrientation();
        *orientation *= glm::angleAxis( lobSpeed, glm::vec3(0, 1, 0) );
    }
}
