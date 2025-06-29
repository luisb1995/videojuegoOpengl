#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, unsigned int texture)
    : Position(pos), Size(size), Texture(texture) {}

void GameObject::Draw(Shader &shader, unsigned int VAO) {
    shader.use();
    
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(Position, 0.0f));
    transform = glm::scale(transform, glm::vec3(Size, 1.0f));
    
    unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    
    glBindTexture(GL_TEXTURE_2D, Texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool GameObject::CheckCollision(const GameObject &other) const {
    bool collisionX = Position.x + Size.x >= other.Position.x &&
                      other.Position.x + other.Size.x >= Position.x;
                      
    bool collisionY = Position.y + Size.y >= other.Position.y &&
                      other.Position.y + other.Size.y >= Position.y;
                      
    return collisionX && collisionY;
}