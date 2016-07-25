#ifndef CROWS_SPRITEGENERATOR_H
#define CROWS_SPRITEGENERATOR_H

// general
#include <vector>
#include <algorithm>

// crowgame
#include "../sprite/AdvancedSprite.hpp"
#include "../../opulence/src/service/OsType.h"
#include "../../opulence/src/factory/LightFactory.h"

class SpriteGenerator {
private:
    std::vector<Model*> models;
    std::vector<Animation*> animations;
    std::vector<Sprite *> sprites;

    ModelFactory *mf;
    LightFactory *lf;

public:
    SpriteGenerator(ModelFactory *modelFactory, LightFactory *lightFactory);

    // memory management
    bool recycleSprite(int position);
    void sanitation();
    void killAll();

    // Model
    Sprite * createSpriteModel(std::string name);
    Sprite * createSpriteModel(float x, float y, float z, std::string name);

    // Animation
    Sprite * createSpriteAnimation(std::string name);
    Sprite * createSpriteAnimation(std::string name, std::string mod);
    Sprite * createSpriteAnimation(float x, float y, float z, std::string name);
    Sprite * createSpriteAnimation(float x, float y, float z, std::string name, std::string mod);

    // Sprite
    AdvancedSprite * createCrow(float x, float y, float z);
    AdvancedSprite * createDefensiveCrow(float x, float y, float z);
    AdvancedSprite * createSquirrel(float x, float y, float z);
    void moveAll();
    void addPointLight(float x, float y, float z, bool isOn);
    void addPointLight(Sprite *sp, bool isOn);

    // get
    ModelFactory * getFactory();
    std::vector<Sprite *> * getSprites();

};

#endif
