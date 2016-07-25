#include "CharacterSet.hpp"

CharacterSet::CharacterSet( float x, float y, int size, SpriteGenerator *gen, std::string team )
{
    for ( int i = 0; i < size; i++ ) {
        characters.push_back( new Character( x, y, gen ) );
        Character *current = characters.back();
        current->setPosition( x - ( i * current->getHitBox().x * 1.25f ), y );
    }

    this->x = x;
    this->y = y;
    this->setValue ( 0 );
}

CharacterSet::~CharacterSet()
{
    for ( auto ch : characters ) {
        delete ch;
    }

    if ( coin ) {
        delete coin;
    }
}

// value
bool CharacterSet::increment()
{
    if ( value < 99 ) {
        this->setValue( to_string( ++value ) );
        return true;
    } else {
        return false;
    }
}

bool CharacterSet::decrement()
{
    if ( value > 0 ) {
        this->setValue( to_string( --value ) );
        return true;
    } else {
        return false;
    }
}

void CharacterSet::setValue( int val )
{
    setValue( to_string( val ) );
}

void CharacterSet::setValue( std::string val )
{
    value = string_to_int( val );
    int numValues = (value / 10) + 1;
    int numAllowed = characters.size();

    for ( int i = 0; i < numAllowed; ++i ) {
        if ( i < numValues ) {
            int digit;

            if ( i == 0 ) {
                characters.at( i )->setValue( to_string( value % 10 ) );
            } else {
                characters.at( i )->setValue( to_string( value % (10 * (i * 10)) / (i * 10) ) );
            }
        }
    }
}

int CharacterSet::getValue() {
    return value;
}

void CharacterSet::setPosition( float x, float y ) {
    for ( int i = 0; i < characters.size(); i++ ) {
        Character *current = characters.at( i );
        current->setPosition( x - ( i * 0.04f ), y );
    }
}

void CharacterSet::setContent( std::string content, float spacing )
{
    for ( int i = 0; i < characters.size(); ++i ) {
        Character *current = characters.at( i );

        if ( i < content.size() ) {
            current->setContent( content.at( characters.size() - 1 - i ) );
            current->setPosition( x - ( i * spacing ), y );
        } else {
            current->hideAll();
        }
    }
}

// interface
glm::vec3 CharacterSet::getPosition()
{
    if ( characters.size() > 0 ) {
        return glm::vec3(
            characters.at( 0 )->getPosition().x,
            characters.at( 0 )->getPosition().y,
            characters.at( 0 )->getPosition().z
        );

    } else {
        return glm::vec3( 0, 0, 0 );
    }
}

glm::vec3 CharacterSet::getHitBox()
{
    if ( characters.size() > 0 ) {
        return glm::vec3(
            characters.at( 0 )->getHitBox().x,
            characters.at( 0 )->getHitBox().y,
            characters.at( 0 )->getHitBox().z
        );

    } else {
        return glm::vec3( 0, 0, 0 );
    }
}

void CharacterSet::update( float time ) {};

std::string CharacterSet::getName()
{
    return "characterset";
}
