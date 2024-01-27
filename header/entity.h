#pragma once
#include "depedencies.h"
#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "world.h"
#include "camera.h" 

typedef struct entityFlags{
    u8 modelType;
    u8 walkable;
    u8 playerControlled;
    u8 item;
    u8 pickup;
    u8 textureDirection = 0;
    u8 flipped = 0;
}entityFlags;

class Entity{
private:    
    glm::vec3 position; 
    glm::vec3 initialPosition;   
    glm::vec3 inputDirection;
    glm::vec3 forward;
    glm::vec3 right;
    f32 percentMoved;
    i8 lookDirection;

    u8 isMoving = false;
    void ProcessInput(const u8* keystate, Camera* camera, World* world);
    void Move();


public:
    Entity(glm::vec2 position, World* world);
    ~Entity();
    void Update(Camera* camera, World* world);
    void Draw();
    glm::vec3 GetPosition();
    entityFlags flags;
};