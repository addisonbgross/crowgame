#include "LevelMaker.hpp"

LevelMaker::LevelMaker(SpriteGenerator *s, InterfaceGenerator *i)
{
    spriteGen = s;
    interfaceGen = i;
}

std::vector<LevelObject> LevelMaker::readInLevel(std::string level)
{
    // append crow file type
    std::string levelFile = level + ".crow";

    // attempt to open file
    std::ifstream fileStream( OsType::osConvert(levelFile), std::ios::in);
    if(!fileStream.is_open()) {
        std::cerr << "Could not read file [" << levelFile << "] Does not exist." << std::endl;
        return scenery;
    } else {
        std::cout << "[Loading Level: " << levelFile << "]" << std::endl;
    }

    // current line in file
    std::string line = "";

    // read in file
    while(!fileStream.eof()) {
        std::getline(fileStream, line);

        if (line.find("#") == std::string::npos && // section heading
            line.find("//") == std::string::npos && // commented out
            line != "" && line != " ") {
            std::istringstream strStream(line);
            std::string sub;

            strStream >> sub;
            std::string name = sub;

            strStream >> sub;
            float x = string_to_float(sub);

            strStream >> sub;
            float y = string_to_float(sub);

            strStream >> sub;
            float z = string_to_float(sub);

            strStream >> sub;
            float rotation = string_to_float(sub);

            scenery.push_back( LevelObject(name, x, y, z, rotation) );
        }
    }

    return scenery;
}

bool LevelMaker::saveLevel(std::vector<Sprite *> *scenery,
                           std::vector<Anima *> *animas,
                           LightFactory *lf)
{
    std::ofstream level;
    level.open( OsType::osConvert( "../res/level/testlevel.crow" ) );

    if ( !level ) {
        return false;
    }

    level << "# scenery" << std::endl;
    for (auto sprite : *scenery) {
        level << sprite->getName() << " "
              << sprite->getPosition()->x << " "
              << sprite->getPosition()->y << " "
              << sprite->getPosition()->z << " "
              << sprite->getOrientationAngle() << std::endl;
    }

    level << std::endl;

    level << "# animas" << std::endl;
    for (auto anima : *animas) {
        if ( !dynamic_cast<Interface *>( anima ) ) {
            level << anima->getName() << " "
                  << anima->getPosition().x << " "
                  << anima->getPosition().y << " "
                  << anima->getPosition().z << " "
                  << "0" << std::endl;
        }
    }

    level << std::endl;

    level << "# point lights" << std::endl;
    for ( auto point : *lf->getPointLights() ) {
        level << "lightball" << " "
              << point->position.x << " "
              << point->position.y << " "
              << point->position.z << " "
              << point->isOn << std::endl;
    }

    level.close();
    return true;
}

bool LevelMaker::loadLevel(std::string name,
                           std::vector<Sprite *> *scenery,
                           std::vector<Anima *> *animas,
                           LightFactory *lf)
{
    std::vector<LevelObject> level = readInLevel( name );
    if (level.size() == 0) {
        return false;
    }

    for (auto obj : level) {
        if (obj.name == "tree") {
            Tree *tree = new Tree( obj.x, obj.y, obj.z, spriteGen, interfaceGen );
            tree->setOrientation(
                glm::angleAxis(
                    glm::radians( obj.rotation ),
                    glm::vec3(0, 1, 0)
                )
             );
            animas->push_back( tree );
        } else if (obj.name == "garbage") {
            Garbage *garbage = new Garbage( obj.x, obj.y, obj.z, spriteGen );
            garbage->setOrientation(
                glm::angleAxis(
                    glm::radians( obj.rotation ),
                    glm::vec3(0, 1, 0)
                )
            );
            animas->push_back( garbage );
        } else if (obj.name == "lightball") {
            spriteGen->addPointLight( obj.x, obj.y, obj.z, obj.rotation );
        } else if (obj.name == "car_yellow" ||
                   obj.name == "car_purple" ||
                   obj.name == "car_red" ||
                   obj.name == "car_green") {
            Sprite *sprite = spriteGen->createSpriteModel( obj.x, obj.y + 0.6f, obj.z, obj.name );
            sprite->setScale(0.275f);
            sprite->setOrientation(
                glm::angleAxis(
                    glm::radians( obj.rotation ),
                    glm::vec3(0, 1, 0)
                )
            );
            scenery->push_back( sprite );

        } else if (obj.name == "") {
            // do nothing

        } else {
            Sprite *sprite = spriteGen->createSpriteModel( obj.x, obj.y, obj.z, obj.name );
            sprite->setOrientation(
                glm::angleAxis(
                    glm::radians( obj.rotation ),
                    glm::vec3(0, 1, 0)
                )
            );
            scenery->push_back( sprite );
        }
    };

    return true;
}
