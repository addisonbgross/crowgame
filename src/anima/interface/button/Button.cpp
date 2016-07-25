#include "Button.hpp"

Button::Button( float x, float y, std::string type, SpriteGenerator *gen)
{
    if (type == "threeCrow") {
        button = gen->createSpriteModel( "threeCrowButton" );
    } else {
        button = gen->createSpriteModel( "crowButton" );
    }
    button->setScale( 0.5f );
    button->setPosition( x, y, -1 );
    button->setHighlight( true );
    button->setInterface( true );
    currentState = INACTIVE;
    name = type;
}

Button::~Button()
{
    button->alive = false;
}

// button
void Button::activate()
{
    if ( isReady() ) {
        currentState = RECEEDING;
        rotationTracker = 0.0f;
        depthTracker = button->getPosition()->z;

    } else if ( isReady() && toggle) {
        currentState = PRECEEDING;
    }
}

bool Button::isReady()
{
    return ( currentState == INACTIVE ) ? true : false;
}

// set
void Button::setOrientation( glm::quat orientation )
{
    button->setOrientation( orientation );
}

void Button::setToggle( bool t )
{
    toggle = t;
}

// get
glm::quat * Button::getOrientation()
{
    return button->getOrientation();
}

Sprite * Button::getSprite()
{
    return button;
}

// interface
glm::vec3 Button::getPosition()
{
    return glm::vec3(
        button->getPosition()->x,
        button->getPosition()->y,
        button->getPosition()->z
    );
}

glm::vec3 Button::getHitBox()
{
    return glm::vec3(
        button->getWidth(),
        button->getHeight(),
        button->getDepth()
    );
}

void Button::update( float time )
{
    float ROTATION_RATE = 0.55f;
    float MOVEMENT_RATE = 0.01f;

    if ( currentState != INACTIVE ) {
        if ( currentState == RECEEDING ) {
            button->getPosition()->z -= MOVEMENT_RATE;
            button->setOrientation(
                *button->getOrientation() *
                glm::angleAxis( ROTATION_RATE, glm::vec3( 0, 1, 0 ) ) );

            rotationTracker += ROTATION_RATE;

            if ( rotationTracker > glm::pi<float>() ) {
                if ( toggle ) {
                    currentState = ACTIVE;
                    button->setOrientation( glm::angleAxis( 0.0f, glm::vec3( 0, 1, 0 ) ) );
                } else {
                    currentState = PRECEEDING;
                }
            }
        } else if ( currentState == PRECEEDING ) {
            button->getPosition()->z += MOVEMENT_RATE;
            button->setOrientation( *button->getOrientation() * glm::angleAxis( ROTATION_RATE, glm::vec3( 0, 1, 0 ) ) );
            rotationTracker -= ROTATION_RATE;

            if ( button->getPosition()->z >= depthTracker ) {
                currentState = INACTIVE;
                button->getPosition()->z = depthTracker;
                button->setOrientation( glm::angleAxis( 0.0f, glm::vec3( 0, 1, 0 ) ) );
            }
        }

    } else {
        currentState = INACTIVE;
    }
}

std::string Button::getName()
{
    return name;
}
