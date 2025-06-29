#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Player::Player(glm::vec2 pos, glm::vec2 size, 
               unsigned int defaultTexture, unsigned int damagedTexture)
    : GameObject(pos, size, defaultTexture), 
      damagedTexture(damagedTexture), 
      facingLeft(false) {}

// The specialized Draw method with texture selection
void Player::Draw(Shader &shader, unsigned int VAO, bool useDamagedTexture) {
    shader.use();
    
    // Create transformation matrix with flipping for direction
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(Position, 0.0f));
    
    // Flip texture if facing left
    if (facingLeft) {
        transform = glm::scale(transform, glm::vec3(-Size.x, Size.y, 1.0f));
    } else {
        transform = glm::scale(transform, glm::vec3(Size, 1.0f));
    }
    
    unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    
    // Select texture based on boolean
    unsigned int texToUse = useDamagedTexture ? damagedTexture : Texture;
    glBindTexture(GL_TEXTURE_2D, texToUse);
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Player::Move(float dx) {
    Position.x += dx;
    // Update facing direction based on movement
    if (dx < 0) {
        facingLeft = true;
    } else if (dx > 0) {
        facingLeft = false;
    }
}

bool Player::CheckCollisionBody(const GameObject &other) const {
    // Full body collision (same as GameObject::CheckCollision)
    return GameObject::CheckCollision(other);
}

bool Player::CheckCollisionPlate(const GameObject &other) const {
    // Only check the top 15% of the player's head
    const float plateHeight = Size.y * 0.15f;
    const float plateY = Position.y + Size.y - plateHeight;
    
    return AABBOverlap(
        Position.x, Position.x + Size.x,    // Player X range
        plateY, plateY + plateHeight,        // Player plate area
        other
    );
}

bool Player::CheckCollision(const GameObject &other) {
    // Polymorphic version - uses the plate collision
    return CheckCollisionPlate(other);
}

// Helper function for axis-aligned bounding box collision
bool Player::AABBOverlap(float minX, float maxX, float minY, float maxY, 
                         const GameObject &other) const {
    return (minX < other.Position.x + other.Size.x) &&
           (maxX > other.Position.x) &&
           (minY < other.Position.y + other.Size.y) &&
           (maxY > other.Position.y);
}