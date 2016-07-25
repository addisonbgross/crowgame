#include "AdvancedSprite.hpp"

AdvancedSprite::AdvancedSprite(Animation *anim, Sprite *goods) : Sprite( anim )
{
    goodsSprite = goods;
    markerAngle = 0;
}

void AdvancedSprite::advancedMove()
{
    glm::vec3 pos = *getPosition();

    if ( goodsSprite ) {
        goodsSprite->setPosition( pos.x, pos.y + 1.5f, pos.z );
        goodsSprite->setOrientation( *getOrientation() );
    }

    if ( goodsMarker ) {
        goodsMarker->setPosition( pos.x, 1.5f, pos.z );
        goodsMarker->setOrientation(
            glm::angleAxis( markerAngle += 0.05f, glm::vec3(0, 1, 0) )
        );

        if ( markerAngle > 360 ) {
            markerAngle = 0;
        }
    }
}

void AdvancedSprite::arrivalAction()
{
    if ( destination ) {
        if ( hasGoods ) {
            destination->arrival( getName() + "_has_goods");
        } else {
            destination->arrival( getName() );
        }
        this->alive = false;
        goodsSprite->alive = false;
        goodsMarker->alive = false;
    }
}

void AdvancedSprite::setGoods(bool goods)
{
    hasGoods = goods;
}

// set
void AdvancedSprite::setDestination( Habitat *dest )
{
    destination = dest;
}
