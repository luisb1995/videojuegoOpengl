#pragma once
#include "GameObject.h"
#include <glm/glm.hpp>

class Player : public GameObject {
public:
    Player(glm::vec2 pos, glm::vec2 size, unsigned int defaultTexture, unsigned int damagedTexture);
    
    // Removed override keyword and added new Draw method
    void Draw(Shader &shader, unsigned int VAO, bool useDamagedTexture);
    
    // Implement base class Draw method
    void Draw(Shader &shader, unsigned int VAO)  {
        Draw(shader, VAO, false); // Default to normal texture
    }
    
    void Move(float dx);
    bool CheckCollisionBody(const GameObject &other) const;
    bool CheckCollisionPlate(const GameObject &other) const;
    bool CheckCollision(const GameObject &other) ; // Polymorphic collision

private:
    unsigned int damagedTexture;
    bool facingLeft;
    
    // Helper for collision detection
    bool AABBOverlap(float minX, float maxX, float minY, float maxY, 
                     const GameObject &other) const;
};