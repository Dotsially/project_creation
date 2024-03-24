#pragma once
#include "depedencies.h"
#include "glm/glm.hpp"
#include "SDL2/SDL.h"
#include "glm/gtc/matrix_transform.hpp"


class Camera2D{
private:
    const f32 zoomLevels[10] = {0.125, 0.25, 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0};
    glm::vec2 focusPoint;
    glm::vec2 size;
    i32 zoomLevelIndex = 1;
public:    
    Camera2D(glm::vec2 focusPoint, glm::vec2 size, i32 zoomLevel);
    ~Camera2D();

    void Update();
    void UpdateZoom(i32 y);

    glm::mat4 GetProjectMatrix();
    i32 GetZoomLevel();
};