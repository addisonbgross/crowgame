#include "Character.hpp"

Character::Character( float x, float y, SpriteGenerator *gen )
{
    for ( auto n : nums ) {
        numbers.push_back( gen->createSpriteModel( x, y, -1, to_string( n ) ) );
        Sprite *current = numbers.back();
        current->setScale( 0.06f );
        current->setHighlight( true );
        current->setInterface( true );
        current->hide();
    }

    spriteGen = gen;

    numbers.at( 0 )->show();
}

Character::~Character()
{
    for ( auto sprite : numbers ) {
        sprite->alive = false;
    }

    if (content) {
        content->alive = false;
    }
}

void Character::setValue( std::string num )
{
    currentCharacter = num;
    for ( auto number : numbers ) {
        if ( number->getName() == currentCharacter ) {
            number->show();
        } else {
            number->hide();
        }
    }
}

void Character::setContent( char content )
{
    for ( auto number : numbers ) {
        number->hide();
    }

    this->content = spriteGen->createSpriteModel( 0, 0, -1, std::string( 1, content ) );
    this->content->setScale( 0.08f );
    this->content->setHighlight( true );
    this->content->setInterface( true );
    this->content->show();
}

void Character::setPosition( float x, float y )
{
    for ( auto num : numbers ) {
        num->setPosition( x, y, -1 );
    }

    if ( content ) {
        content->setPosition( x, y, -1 );
    }
}

void Character::hideAll()
{
    for ( auto n : numbers ) {
        n->hide();
    }

    if ( content ) {
        content->hide();
    }
}

// interface
glm::vec3 Character::getPosition()
{
    return glm::vec3(
        numbers.at( 0 )->getPosition()->x,
        numbers.at( 0 )->getPosition()->y,
        numbers.at( 0 )->getPosition()->z
    );
}

glm::vec3 Character::getHitBox()
{
    return glm::vec3(
        numbers.at( 0 )->getWidth(),
        numbers.at( 0 )->getHeight(),
        numbers.at( 0 )->getDepth()
    );
}

void Character::update( float time ) {};

std::string Character::getName()
{
    return "character";
}
