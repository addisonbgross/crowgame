#ifndef CROWS_MANIPULATOR_H
#define CROWS_MANIPULATOR_H

// crowgame
#include "../anima/interface/Interface.hpp"
#include "../habitat/tree/Tree.hpp"

class Manipulator {
private:
    // currently selected Anima
    Anima *selected = nullptr;

    // currently hovered Anima
    Anima *hovered = nullptr;

    // Sprite that highlights the currently selcted Anima
    Sprite *selector = nullptr;
    // Sprite that highlights the currently hovered upon Anima
    Sprite *hoverer = nullptr;
    // Sprite that shows the location of the current right click
    Sprite *clicker = nullptr;

    Sprite *crowAction = nullptr;

    // the rotation of the hoverer
    float spin = 0.025f;
    glm::quat rotation = glm::angleAxis( spin, glm::vec3(0, 1, 0) );

    // set of Animas to search through
    std::vector<Anima *> *animas;

    // screen dimensions for translation from projection space to opulence 3-space
    float screenWidth;
    float screenHeight;

public:
    Manipulator(std::vector<Anima *> *ani);
    Manipulator(std::vector<Anima *> *ani, SpriteGenerator *gen, float screenWidth, float screenHeight);
    ~Manipulator();

    void addSelector(Sprite *sel);
    void addHoverer(Sprite *hov);
    void addNullClick(Sprite *click);
    void addConfirmClick(Sprite *crowAction);

    Interface * findInterface(int x, int y);
    Anima * findAnima(float x, float y, float z);
    void doHover(Anima *found);
    Anima * doSelection(Anima *found);
    void doNullClick(float x, float y, float z);
    void doConfirmClick(float x, float y, float z);

    // get
    Anima * getSelected();
};

#endif
