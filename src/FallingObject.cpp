#include "FallingObject.h"

FallingObject::FallingObject(glm::vec2 pos, glm::vec2 size, unsigned int texture, float speed)
    : GameObject(pos, size, texture), Speed(speed), IsActive(true) {}

void FallingObject::Update(float dt) {
    Position.y -= Speed * dt;

    if (Position.y + Size.y < -1.0f) {
        IsActive = false;
    }
}
