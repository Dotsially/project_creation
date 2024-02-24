#include "camera_2d.h"


Camera2D::Camera2D(glm::vec2 focusPoint, glm::vec2 size, i32 zoomLevel){
    this->focusPoint = focusPoint;
    this->size = size;
    UpdateZoom(zoomLevel);
}


Camera2D::~Camera2D(){

}

void Camera2D::Update(){
    int x;
    int y;
    u32 buttonPressed = SDL_GetRelativeMouseState(&x,&y);

    if(buttonPressed & SDL_BUTTON(1)){
        focusPoint += -glm::vec2{x,y};
    }
}

void Camera2D::UpdateZoom(i32 y){
    zoomLevelIndex += y;
    if(zoomLevelIndex < 0){
        zoomLevelIndex = 0;
    }
    if(zoomLevelIndex > 5){
        zoomLevelIndex = 5;
    }
}

glm::mat4 Camera2D::GetProjectMatrix(){
    float left = focusPoint.x - size.x/2.0f;
    float right = focusPoint.x + size.x/2.0f;
    float top = focusPoint.y - size.y/2.0f;
    float bottom = focusPoint.y + size.y/2.0f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top);
    f32 zoomLevel = zoomLevels[zoomLevelIndex];
    glm::mat4 zoom = glm::scale(glm::mat4{}, glm::vec3{zoomLevel, zoomLevel, 1.0});

    return projection * zoom;
}