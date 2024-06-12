#pragma once
#include <glm.hpp>
struct BoxColliderComponent
{
    int width;
    int height;
    glm::vec2 offset;

    BoxColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0, 0)) :
        width(width), height(height), offset(offset)
    {};
};