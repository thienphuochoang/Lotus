#pragma once
#include <glm.hpp>
struct TransformComponent
{
    glm::vec2 position;
    glm::vec2 scale;
    float rotation;

    TransformComponent(glm::vec2 inputPosition = glm::vec2(0,0),
        glm::vec2 inputScale = glm::vec2(1,1),
        float inputRotation = 0.0f)
    {
        this->position = inputPosition;
        this->scale = inputScale;
        this->rotation = inputRotation;
    }
};