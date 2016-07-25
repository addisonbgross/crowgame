#ifndef CROWS_LOBBER_H
#define CROWS_LOBBER_H

// general
#include <vector>
#include <tuple>

// crowgame
#include "../generator/SpriteGenerator.hpp"

class Lobber {
private:
    float x, y, z;

    // time keeping
    float lobDelay;
    float delayCounter;

    // settings
    float lobHeight = 10.0f;
    float heightBuffer = 0.5f;
    float lobSpeed = 0.1f;

    // aesthetics
    std::vector< std::tuple<Sprite *, bool, glm::vec3> > objects;
    std::string type;
    SpriteGenerator *spriteGen;

    // object sets
    std::vector<std::string> garbageSet = {
        "applecore",
        "chickenleg",
        "fishbone",
        "milkcarton",
        "tunacan"
    };

public:
    Lobber(float x, float y, float z, std::string type, SpriteGenerator *spriteGen);
    ~Lobber();

    void lob();
    void update(float time, bool active);
};

#endif
