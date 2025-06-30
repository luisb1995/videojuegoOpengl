#pragma once
#include "GameObject.h"

class FallingObject : public GameObject {
public:
    float Speed;
    bool IsActive;

    FallingObject(glm::vec2 pos, glm::vec2 size, unsigned int texture, float speed);

    void Update(float dt);
};
