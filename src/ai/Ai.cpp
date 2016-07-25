#include "Ai.hpp"

Ai::Ai(InterfaceGenerator *interfaceGen)
{
    this->interfaceGen = interfaceGen;
    currentDifficulty = MEDIUM;
    currentState = INITIALIZING;
}

Ai::~Ai() {}

// public
void Ai::update(float time, std::vector<Anima *> *animas)
{
    // delay Ai actions based on set difficulty
    delayCounter += (time / 1000.0f);
    if (delayCounter < currentDifficulty) {
        return;
    } else {
        delayCounter = 0;
    }

    // perform action
    switch (currentState) {
        case IDLE:

            break;

        case INITIALIZING:
            if (!occupyEmptyTree(animas)) {
                // if no more trees can be occupied
                currentState = EXPANDING;
            }
            break;

        case EXPANDING:
            if (occupyResource(animas)) {
                // a resource will be successfully occupied
                currentState = CONSOLIDATING;
            } else {
                // no garbage to collect, time to expand
                occupyEmptyTree(animas);
            }

            launchAttack(animas);
            break;

        case CONSOLIDATING:
            occupyResource(animas);
            break;

        default:
            break;
    };

    // return any squirrels that have gathered resources
    harvestResources();

    // utilize squirrel food
    currentState = (reproduce()) ? currentState : EXPANDING;
}

// private
void Ai::addTree(Tree *tree)
{
    if (tree) {
        occupiedTrees.push_back(tree);
    }
}

void Ai::removeTree(Tree *tree)
{
    if (tree) {
        occupiedTrees.erase(
            std::remove(
                occupiedTrees.begin(),
                occupiedTrees.end(),
                tree
            ),
            occupiedTrees.end()
        );
    }
}

bool Ai::occupyEmptyTree(std::vector<Anima *> *animas)
{
    if (interfaceGen->getSquirrelFood() > 0) {
        for (auto anima : *animas) {
            Tree *tree = dynamic_cast<Tree *>(anima);
            if (tree && tree->getNumSquirrels() == 0) {
                bool result = interfaceGen->spawnSquirrel(tree);
                if (result) {
                    // continue consolidating resources
                    occupiedTrees.push_back(tree);
                    return true;
                } else {
                    // occupy a new empty tree
                    return false;
                }
            }
        }
    }

    return false;
}

bool Ai::occupyResource(std::vector<Anima *> *animas)
{
    if (interfaceGen->getSquirrelPopulation() > 0) {
        Garbage *garbage = dynamic_cast<Garbage *>(
                                animas->at(rand() % animas->size())
                           );
        if (garbage) {
            // send unit from random occupied tree
            if (occupiedTrees.size() > 0) {
                Tree *tree = occupiedTrees.at(rand() % occupiedTrees.size());
                if (tree &&
                    !tree->isInCombat() &&
                    tree->getNumSquirrels() > 0 &&
                    garbage->canFitSquirrels() &&
                    garbage->getIncomingCrows() == 0) {
                        tree->sendSquirrel(tree, garbage);
                        occupiedGarbages.push_back(garbage);
                }
            }

            return true;
        }
    }

    return false;
}

bool Ai::reproduce()
{
    if (interfaceGen->getSquirrelFood() > 0) {
        if (occupiedTrees.size() > 0) {
            Tree *tree = occupiedTrees.at(rand() % occupiedTrees.size());
            if (tree &&
                !tree->isInCombat() &&
                tree->canFitSquirrels()) {

                if (interfaceGen->getSquirrelPopulation() + 1 <=
                    occupiedTrees.size() * tree->TREE_COIN_LIMIT) {
                    interfaceGen->spawnSquirrel(tree);
                    return true;
                } else {
                    return false;
                }
            }
        }
    }

    return true;
}

void Ai::harvestResources()
{
    for (auto garbage : occupiedGarbages) {
        if (garbage->readyForPickup() && garbage->getNumSquirrels() > 0) {
            int randTree = rand() % occupiedTrees.size();
            Tree * tree = occupiedTrees.at(randTree);
            if (tree && tree->canFitSquirrels()) {
                garbage->sendSquirrel(garbage, tree);

                // remove garbage from occupied garbages
                for(std::vector<Garbage *>::iterator i = occupiedGarbages.begin();
                    i != occupiedGarbages.end();
                    ++i) {
                        if(*i == garbage) {
                            occupiedGarbages.erase(i);
                            break;
                        }
                }
                return;
            }
        }
    }
}

void Ai::launchAttack(std::vector<Anima *> *animas)
{
    for (auto anima : *animas) {
        if (anima->getName() == "tree") {
            Tree *enemy = dynamic_cast<Tree *>( anima );

            if (enemy->hasCrows()) {
                Tree *tree = occupiedTrees.at( rand() % occupiedTrees.size() );

                if (enemy->canFitSquirrels()) {
                    for (int i = 0; i < tree->numSquirrels; ++i) {
                        tree->sendSquirrel(tree, enemy);
                        addTree(tree);
                    }
                }
            }
        }
    }
}
