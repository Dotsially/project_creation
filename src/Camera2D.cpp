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
    if(zoomLevelIndex > 9){
        zoomLevelIndex = 9;
    }
}

glm::mat4 Camera2D::GetProjectMatrix(){
    f32 zoomLevel = zoomLevels[zoomLevelIndex];
    glm::vec2 finalSize = size * zoomLevel;

    float left = focusPoint.x - finalSize.x/2.0f;
    float right = focusPoint.x + finalSize.x/2.0f;
    float top = focusPoint.y - finalSize.y/2.0f;
    float bottom = focusPoint.y + finalSize.y/2.0f;

    glm::mat4 projection = glm::ortho(left, right, bottom, top);

    return projection;
}

i32 Camera2D::GetZoomLevel(){
    return zoomLevelIndex;
}


