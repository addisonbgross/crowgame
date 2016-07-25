#ifndef CROWS_PATHFINDER_H
#define CROWS_PATHFINDER_H

// general
#include <vector>
#include <glm/detail/type_vec3.hpp>

// crowgame
#include "../sprite/Sprite.hpp"

class PathFinder {
private:
    std::vector<glm::vec3> pathPoints;
    const float FLIGHT_PATH_SCALE = 3;

public:
    std::vector<glm::vec3> * flightPath(glm::vec3 start, glm::vec3 end);
    std::vector<glm::vec3> * groundPath(glm::vec3 start, glm::vec3 end);
};

#endif
