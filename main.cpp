#include "CrowGame.hpp"
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    #include <windows.h>
#endif

template <typename T> bool isIn(T thing, std::vector<T> * source) {
    if (std::find(source->begin(), source->end(), thing) != source->end()) {
        return true;
    } else {
        return false;
    }
}

bool doInput(SDL_Event e, int x, int y, glm::vec3 trans, Anima *hover, Manipulator *manipulator) {
    // inital resources
    std::vector<std::string> * commands = controller.getInput(e);

    // input
    GLuint button = e.button.button;
    const unsigned char  *keystate = SDL_GetKeyboardState(NULL);

    if (isIn<std::string>("rotate camera horizontally", commands)) {
        if (e.motion.xrel > 0) {
#if defined(_WIN32)
            cf->getMainCamera()->rotateHorizontal(0.045f);
#else
            cf->getMainCamera()->rotateHorizontal(0.025f);
#endif

        } else if (e.motion.xrel < 0) {

#if defined(_WIN32)
            cf->getMainCamera()->rotateHorizontal(-0.045f);
#else
            cf->getMainCamera()->rotateHorizontal(-0.025f);
#endif
        }
    }

    if (isIn<std::string>("rotate camera vertically", commands)) {
        if (e.motion.yrel > 0) {
#if defined(_WIN32)
            cf->getMainCamera()->rotateVertical(-0.045f);
#else
            cf->getMainCamera()->rotateVertical(-0.025f);
#endif
        } else if (e.motion.yrel < 0) {
#if defined(_WIN32)
            cf->getMainCamera()->rotateVertical(0.045f);
#else
            cf->getMainCamera()->rotateVertical(0.025f);
#endif
        }
    }

    if (isIn<std::string>("select", commands)) {
        // select the currently hovered Anima or Interface
        Anima *sel = manipulator->doSelection( hover );

        if ( sel ) {
            interfaceGen.showButtonSet( sel->getName() );

        } else {
            interfaceGen.hideAllButtons();
        }
    }

    if (isIn<std::string>("action", commands)) {
        // currently selected
        Anima *selected = manipulator->getSelected();
        Interface *inter = manipulator->findInterface( x, y );

        if ( inter ) {
            if ( inter->getName() == "oneCrow" ) {
                interfaceGen.activateButton( 0, selected );
            } else if ( inter->getName() == "threeCrow" ) {
                interfaceGen.activateButton( 1, selected );
            }
        } else if ( selected ) {
            // possible target selection
            Anima *foundAnima = manipulator->findAnima(trans.x, trans.y, trans.z);
            manipulator->doNullClick( trans.x, trans.y, trans.z );

            if ( foundAnima && foundAnima != selected ) {
                glm::vec3 dest = foundAnima->getPosition();
                Habitat *selectedHabitat = dynamic_cast<Habitat *>( selected );
                Habitat *targetHabitat = dynamic_cast<Habitat *>( foundAnima );

                if ( selectedHabitat && targetHabitat ) {
                    if ( selectedHabitat->getNumCrows() > 0 &&
                         targetHabitat->canFitCrows() ) {
                            manipulator->doConfirmClick( dest.x, dest.y, dest.z );
                            selectedHabitat->sendCrow( selectedHabitat, targetHabitat );
                    }
                }
            }
        }
    }

    // hot keys
    if (isIn<std::string>("button one", commands)) {
        interfaceGen.activateButton( 0, manipulator->getSelected() );
    }
    if (isIn<std::string>("button two", commands)) {
        interfaceGen.activateButton( 1, manipulator->getSelected() );
    }
    if (isIn<std::string>("button three", commands)) {
        interfaceGen.activateButton( 2, manipulator->getSelected() );
    }
    if (isIn<std::string>("button four", commands)) {
        interfaceGen.activateButton( 3, manipulator->getSelected() );
    }

    // camera zoom
    if (isIn<std::string>("zoom", commands)) {
        if (e.wheel.y < 0) {
            cf->getMainCamera()->zoomOut();
        } else if (e.wheel.y > 0) {
            cf->getMainCamera()->zoomIn();
        }
    }

    // draw style
    if (isIn<std::string>("draw light", commands)) {
        opulence.setClearColour( 1.0, 1.0, 1.0, 1.0 );
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    } else if (isIn<std::string>("draw normal", commands)) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    } else if (isIn<std::string>("draw dark", commands)) {
        opulence.setClearColour( 0.0, 0.0, 0.0, 1.0 );
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // terraforming
    if (isIn<std::string>("make tree", commands) && !makeTreeDebounce) {
        Tree *tree = new Tree(trans.x, trans.y, trans.z, &spriteGen, &interfaceGen);
        tree->setOrientation(
            glm::angleAxis(
                glm::radians(static_cast<float>(rand() % 360)),
                glm::vec3(0, 1, 0)
            )
        );
        animas.push_back(tree);

        makeTreeDebounce = true;
    } else if (!isIn<std::string>("make tree", commands) && makeTreeDebounce) {
        makeTreeDebounce = false;
    }
    if (isIn<std::string>("make light", commands) && !makeLightDebounce) {
        spriteGen.addPointLight( trans.x, trans.y + 1.0f, trans.z, 1 );

        makeLightDebounce = true;
    } else if (!isIn<std::string>("make light", commands) && makeLightDebounce) {
        makeLightDebounce = false;
    }
    if (isIn<std::string>("make garbage", commands) && !makeGarbageDebounce) {
        Garbage *garbage = new Garbage( trans.x, trans.y + 0.05f, trans.z, &spriteGen );
        garbage->setOrientation(
            glm::angleAxis(
                glm::radians(static_cast<float>(rand() % 360)),
                glm::vec3(0, 1, 0)
            )
        );
        animas.push_back( garbage );
        makeGarbageDebounce = true;

    } else if (!isIn<std::string>("make garbage", commands) && makeGarbageDebounce) {
        makeGarbageDebounce = false;

    }
    if (isIn<std::string>("make car", commands) && !makeCarDebounce) {
        if (std::abs( trans.x ) > 100.0f || std::abs( trans.z ) > 100.0f) {
            return false;
        }

        std::vector< std::string > cars = {
            "car_green",
            "car_purple",
            "car_red",
            "car_yellow"
        };

        Sprite *car = spriteGen.createSpriteModel(
            trans.x,
            0.05f,
            trans.z,
            cars.at( rand() % cars.size() )
        );
        car->setScale(0.275f);
        scenery.push_back(car);
        makeCarDebounce = true;
    } else if (!isIn<std::string>("make car", commands) && makeCarDebounce) {
        makeCarDebounce = false;
    }

    // memories
    if (isIn<std::string>("save game", commands)) {
        LevelMaker lvl( &spriteGen, &interfaceGen );
        if ( lvl.saveLevel( &scenery, &animas, lf ) ) {
            std::cout << "Game Saved!" << std::endl;
        } else {
            std::cout << "ERROR: saving game command failed!" << std::endl;
        }
    }

    return isIn<std::string>("quit", commands);
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
#else
    int main (int argc, char* argv[]) {
#endif
    opulence.start();

    screenWidth = opulence.getScreenWidth();
    screenHeight = opulence.getScreenHeight();

    // enable text input
    SDL_StartTextInput();

    RtsCamera *camera;
    cf->setMainCamera( cf->makeCamera(camera) );
    cf->getMainCamera()->setEye(new glm::vec3(10.0f, 15.0f, 0.0f));
    cf->getMainCamera()->setFocus(new glm::vec3(10.0f, 1.0f, -75.0f));

    CharacterSet *title = new CharacterSet(0.1, 0, 8, &spriteGen, "crows");
    title->setContent("CROWGAME", 0.07f);
    animas.push_back(title);

    dayMan = new DayManager( &spriteGen, lf );

    // create Level
    LevelMaker levelMaker( &spriteGen, &interfaceGen );
    levelMaker.loadLevel( OsType::osConvert( "../res/level/testlevel" ),
                          &scenery, &animas, lf );

    // anima finder and mouse interaction
    Manipulator manipulator( &animas, &spriteGen, screenWidth, screenHeight );

    // interface init
    interfaceGen.createKeyboardButtons( &spriteGen );
    interfaceGen.createCrowInterface( &spriteGen );

    /*** MAIN LOOP ***/
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        // mouse position in 3-space
        glm::vec3 trans = opulence.getClickTranslation(e.button.x, e.button.y);

        // move the title text
        if (title) {
            glm::vec3 pos = title->getPosition();
            title->setPosition(pos.x * 1.01f, pos.y);

            if (pos.x < -2 || pos.x > 2) {
                title = nullptr;
            }
        }

        // camera constants
        int x = screenWidth / 2,
            y = screenHeight / 2,
            camVelocity = 1;
        SDL_GetMouseState(&x, &y);

        // Anima currently being hovered (if any)
        Anima *hover = manipulator.findAnima(trans.x, trans.y, trans.z);
        manipulator.doHover( hover );

        // event loop
        while (SDL_PollEvent(&e)) {
            quit = doInput(e, x, y, trans, hover, &manipulator);

            if (quit) {
                break;
            }
        }

        // get milliseconds passed since last frame
        int time = opulence.getTimeManager()->getLap();

        // the squirrel army's actions
        ai.update(time, &animas);

        // update the stuff and things
        cf->getMainCamera()->move(x, y, screenWidth, screenHeight);
        for (auto &anima : animas) {
            // update each anima with the delta time (in seconds)
            anima->update( time / 1000.0f );
        }

        // move the Sprites
        spriteGen.moveAll();
        spriteGen.sanitation();

        // day/night cycle
        float rotation = dayMan->rotateSky();
        opulence.setClearColour( rotation / 3.5f,
                                 rotation / 3.5f,
                                 rotation,
                                 1 );

        // draw it all
        opulence.render();
    }

    /*** clean up crowgame resources ***/
    // Sprites
    spriteGen.killAll();

    // scene
    delete dayMan;

    // opulence
    opulence.shutDown();
    opulence.getStats();
    /***********************************/

    return 0;
}
