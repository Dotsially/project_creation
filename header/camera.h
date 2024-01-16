#pragma once
#include "depedencies.h"
#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vector_angle.hpp"


enum CameraType{
    CAMERA_THIRDPERSON,
    CAMERA_FIRSTPERSON,
    CAMERA_FREECAM
};

class Camera{
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 worldUp;
    glm::vec3 right;
    glm::vec3 forward;
    glm::vec3 up;
    glm::vec3 moveDirection;
    glm::vec2 lastMouse;
    glm::vec2 angle;
    glm::vec2 initialAngle;
    u8 firstMove = false;
    u8 isMoving = false;
    i8 lookDirection;
    i8 inputDirection;
    f32 percentMoved;
    f32 fov;
    f32 yaw;
    f32 pitch;
    CameraType cameraType;

public:
    Camera(CameraType type, glm::vec3 position);
    ~Camera();
    void Update(const u8* keystate, glm::vec3 targetPosition);
    void ProcessInput(const u8* keystate);
    void Move();
    glm::mat4 GetProjectMatrix();
    glm::mat4 GetViewMatrix();
    glm::vec3 GetPosition();
    glm::vec3 GetTarget();
    glm::vec3 GetRight();
    glm::vec3 GetForward();
    glm::vec3 GetUp();
    f32 GetFov();
    i8 GetLookdirection();
};