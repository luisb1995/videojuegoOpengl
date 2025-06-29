#pragma once
#include <glm/glm.hpp>
#include "Shader.h"
class GameObject {
public:
    glm::vec2 Position;
    glm::vec2 Size;
    unsigned int Texture;
    
    GameObject(glm::vec2 pos, glm::vec2 size, unsigned int texture);
    
    void Draw(Shader &shader, unsigned int VAO);
    bool CheckCollision(const GameObject &other) const;
};