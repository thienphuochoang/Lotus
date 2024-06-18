#pragma once
#include <glm.hpp>

struct KeyboardControlComponent
{
    glm::vec2 upVelocity;
    glm::vec2 downVelocity;
    glm::vec2 leftVelocity;
    glm::vec2 rightVelocity;
    KeyboardControlComponent(glm::vec2 upVelocity = glm::vec2(0,0), 
        glm::vec2 downVelocity = glm::vec2(0, 0),
        glm::vec2 leftVelocity = glm::vec2(0, 0),
        glm::vec2 rightVelocity = glm::vec2(0, 0)) :
        upVelocity(upVelocity), downVelocity(downVelocity),
        leftVelocity(leftVelocity), rightVelocity(rightVelocity)
    {}
};