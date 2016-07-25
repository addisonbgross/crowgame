#include "Manipulator.hpp"

Manipulator::Manipulator( std::vector<Anima *> *ani )
{
    this->animas = ani;
}

Manipulator::Manipulator( std::vector<Anima *> *ani, SpriteGenerator *gen , float screenWidth, float screenHeight ) : Manipulator( ani )
{
    addSelector( gen->createSpriteAnimation( "selector", "pingpong" ) );
    addHoverer( gen->createSpriteModel( "hoverer" ) );
    addNullClick( gen->createSpriteAnimation( "click", "once" ) );
    addConfirmClick( gen->createSpriteAnimation( "crowAction", "once"));
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

Manipulator::~Manipulator()
{
    selector->alive = false;
    hoverer->alive = false;
    clicker->alive = false;
}

void Manipulator::addSelector( Sprite *sel )
{
    sel->setRate( 0.55f );
    sel->setScale( 2.0f );
    sel->hide();
    sel->setHighlight( 1 );
    this->selector = sel;
}

void Manipulator::addHoverer( Sprite *hov )
{
    hov->setRate( 0.3f );
    hov->setScale( 2.0f );
    hov->hide();
    hov->setHighlight( 1 );
    this->hoverer = hov;
}

void Manipulator::addNullClick( Sprite *click )
{
    click->setRate( 0.55f );
    click->setScale( 2.0f );
    click->hide();
    click->setHighlight( 1 );
    this->clicker = click;
}

void Manipulator::addConfirmClick( Sprite *crowAction )
{
    crowAction->setRate( 0.5f );
    crowAction->setScale( 3.0f );
    crowAction->hide();
    crowAction->setHighlight( 1 );
    this->crowAction = crowAction;
}

// findInterface - using the default OpenGL viewport [-1,-1] x [1,1], search for Interface
//                 by translating the mouse event through the projection matrix
Interface * Manipulator::findInterface( int x, int y )
{
    // gather Interafaces from Animas
    std::vector<Interface *> interfaces;
    for ( auto & anima : *animas ) {
        Interface *interface = dynamic_cast<Interface *>( anima );

        if ( interface ) {
            interfaces.push_back( interface );
        }
    }
    int numInterfaces = interfaces.size();

    // used to translate the Interface's position & mouse's position
    glm::mat4 proj = glm::perspective( 45.0f, screenWidth / screenHeight, 0.1f, 1000.0f );

    for ( int i = 0; i < numInterfaces; ++i ) {
        Interface *inter = interfaces.at( i );

        // the position, hit box, and mouse click all get translated through the current projection matrix
        glm::vec4 pos = proj * glm::vec4( inter->getPosition(), 1.0f );
        glm::vec4 box = proj * glm::vec4( inter->getHitBox(), 1.0f );
        glm::vec4 click;

        // translate 3d game coordinates to 2d interface weirdness [-1, 1] x [-1, 1]
        if ( x > screenWidth / 2 ) {
            click.x = ( x - ( screenWidth / 2 ) ) / ( screenWidth / 2 );
        } else {
            click.x =  (-1) * ( ( screenWidth / 2 ) - glm::abs(x) ) / ( screenWidth / 2 );
        }
        if ( y > screenHeight / 2 ) {
            click.y = (-1) * ( y - ( screenHeight / 2 ) ) / ( screenHeight / 2 );
        } else {
            click.y =  ( ( screenHeight / 2 ) - glm::abs(y) ) / ( screenHeight / 2 );
        }

        // search in upper right quadrant Q1
        if ( pos.x >= 0 && pos.y >= 0 ) {
            if ( click.x >= pos.x - (box.x / 2) && click.x <= pos.x + (box.x / 2) ) {
                if ( click.y >= pos.y && click.y <= pos.y + box.y ) {
                    return interfaces.at( i );
                }
            }
        }
        // search in lower right quadrant Q2
        if ( pos.x >= 0 && pos.y < 0 ) {
            if ( click.x >= pos.x - (box.x / 2) && click.x <= pos.x + (box.x / 2) ) {
                if ( click.y <= pos.y + box.y && click.y >= pos.y ) {
                    return interfaces.at( i );
                }
            }
        }
        // search in lower left quadrant Q3
        if ( pos.x < 0 && pos.y < 0 ) {
            if ( click.x >= pos.x - (box.x / 2) && click.x <= pos.x + (box.x / 2) ) {
                if ( click.y <= pos.y + box.y && click.y >= pos.y ) {
                    return interfaces.at( i );
                }
            }
        }
        // search in upper left quadrant Q4
        if ( pos.x < 0 && pos.y >= 0 ) {
            if ( click.x >= pos.x - (box.x / 2) && click.x <= pos.x + (box.x / 2) ) {
                if ( click.y >= pos.y && click.y <= pos.y + box.y ) {
                    return interfaces.at( i );
                }
            }
        }
    }

    return nullptr;
}

Anima * Manipulator::findAnima(float x, float y, float z)
{
    std::vector<Anima *> found;
    int numAnima = animas->size();

    for (int i = 0; i < numAnima; ++i) {
        Anima *ani = animas->at(i);

        // only selecting Tree Sprites currently
        glm::vec3 pos = ani->getPosition();
        float sX = pos.x;
        float sY = pos.y;
        float sZ = pos.z;

        glm::vec3 box = ani->getHitBox();
        float width = box.x;
        float height = box.y;
        float depth = box.z;

        // from the centre of the Sprite: half way to the top and
        // bottom, both sides, front and back. A bounding cube
        if (x >= sX - (width / 2) && x <= sX + (width / 2)) {
            if (y >= sY && y <= sY + height) {
                if (z >= sZ - (depth / 2) && z <= sZ + (depth / 2)) {
                    found.push_back( animas->at(i) );
                }
            }
        }
    }

    // default selection
    if (found.size() > 0) {
        return found.at(0);
    }

    return nullptr;
}

void Manipulator::doHover(Anima *found)
{
    // a new anima will be hovered
    if (found && found != selected) {
        if (hovered && hovered != selected) {
            // hide the coins of the tree that we previously hovered
            Tree *preTree = dynamic_cast<Tree *>( hovered );
            if ( preTree ) {
                preTree->hideCoins();
            }
        }

        hovered = found;
        Tree *tree = dynamic_cast<Tree *>( hovered );

        if ( tree ) {
            tree->showCoins();
        }

        if ( hoverer ) {
            glm::vec3 pos = found->getPosition();
            hoverer->setPosition( pos.x, pos.y + 0.5f, pos.z );
            hoverer->show();
        }

    } else {
        if ( hovered && hovered != selected ) {
            Tree *tree = dynamic_cast<Tree *>( hovered );

            if ( tree ) {
                tree->hideCoins();
            }

            if ( hoverer ) {
                hoverer->hide();
            }

            hovered = nullptr;
        }
    }

    if ( hoverer && found ) {
        hoverer->setOrientation( *hoverer->getOrientation() * rotation );
    }
}

Anima * Manipulator::doSelection( Anima *found )
{
    if ( found ) {
        if (selected) {
            selector->hide();

            Tree *preTree = dynamic_cast<Tree *>( selected );
            if ( preTree ) {
                preTree->hideCoins();
            }
        }

        selected = found;

        if ( selector ) {
            selector->setPosition( selected->getPosition().x,
                                   selected->getPosition().y + 0.5f,
                                   selected->getPosition().z );
            selector->show();
        }

        Tree *tree = dynamic_cast<Tree *>( selected );
        if ( tree ) {
            tree->showCoins();
        }

        if ( hoverer ) {
            hoverer->hide();
        }

        return selected;

    } else {
        if (selected) {
            Tree *tree = dynamic_cast<Tree *>( selected );

            if ( tree ) {
                tree->hideCoins();
            }

            selector->hide();
            selected = nullptr;
        }

        return nullptr;
    }
}

void Manipulator::doNullClick(float x, float y, float z)
{
    if ( clicker ) {
        clicker->setPosition( x, y + 0.5f, z );
        clicker->resetAnimations();
        clicker->show();
    }
}

void Manipulator::doConfirmClick(float x, float y, float z)
{
    if ( crowAction ) {
        crowAction->setPosition( x, y + 0.5f, z );
        crowAction->resetAnimations();
        crowAction->show();
    }
}

// get
Anima * Manipulator::getSelected()
{
    return selected;
}
