#ifndef CROWS_ADVANCEDSPRITE_H
#define CROWS_ADVANCEDSPRITE_H

// crowgame
#include "Sprite.hpp"
#include "../anima/Anima.hpp"
#include "../habitat/Habitat.hpp"

class AdvancedSprite : public Sprite {
private:
    bool hasGoods = false;

public:
    Habitat *destination = nullptr;
    Sprite *goodsSprite;
    Sprite *goodsMarker;
    float markerAngle;

    AdvancedSprite(Animation *anim, Sprite *goods);

    // AdvancedSprite
    void advancedMove();
    void arrivalAction();
    void setGoods( bool goods );
    void setDestination( Habitat *dest );
};

#endif
