#include "PathFinder.hpp"

std::vector<glm::vec3> * PathFinder::flightPath(glm::vec3 start, glm::vec3 end)
{
    glm::vec3 diff = end - start;

    pathPoints.push_back(start);
    diff.y += 1.5 * FLIGHT_PATH_SCALE;
    pathPoints.push_back(start + diff * 0.1f);
    diff.y += 3.0 * FLIGHT_PATH_SCALE;
    pathPoints.push_back(start + diff * 0.2f);
    diff.y += 2.0 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff * 0.3f);
    diff.y += 1.0 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff * 0.4f);
    diff.y += 0.5 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff * 0.5f);
    diff.y -= 1.0 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff * 0.6f);
    diff.y -= 1.5 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff * 0.7f);
    diff.y -= 2.5 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff * 0.8f);
    diff.y -= 1.0 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff * 0.9f);
    diff.y -= 0.5 * FLIGHT_PATH_SCALE;   
    pathPoints.push_back(start + diff);

     //diff.y += diff.y * 1.1;
    //pathPoints.push_back(start + diff * 0.1f);
    //diff.y += 1.5f;
    //pathPoints.push_back(start + diff * 0.2f);
    //diff.y += 2.5f;
    //pathPoints.push_back(start + diff * 0.3f);
    //diff.y += 2.0f;
    //pathPoints.push_back(start + diff * 0.4f);
    //diff.y += 1.5f;
    //pathPoints.push_back(start + diff * 0.5f);
    //diff.y += 1.0f;
    //pathPoints.push_back(start + diff * 0.6f);
    //diff.y += 0.25f;
    //pathPoints.push_back(start + diff * 0.7f);
    //diff.y -= 0.75f;
    //pathPoints.push_back(start + diff * 0.8f);
    //diff.y -= 0.75f;
    //pathPoints.push_back(start + diff * 0.9f);

    //pathPoints.push_back(end);

    return &pathPoints;
}

std::vector<glm::vec3> * PathFinder::groundPath(glm::vec3 start, glm::vec3 end)
{
    // so shitty
    pathPoints.push_back(start);
    pathPoints.push_back(end);

    return &pathPoints;
}
