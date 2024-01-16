#include "camera.h"
#include <iostream>

Camera::Camera(CameraType type, glm::vec3 position){
    this->cameraType = type;
    switch(cameraType){
        case CAMERA_FREECAM:
        case CAMERA_FIRSTPERSON:
            this->position = position;
            this->target = glm::vec3{0.0f,0.0f,-1.0f};
            this->worldUp = glm::vec3{0.0f,1.0f,0.0f};
            this->right = glm::normalize(glm::cross(this->target, this->worldUp));
            this->up = glm::normalize(glm::cross(this->right, this->target));
            this->fov = glm::radians(70.0);
            this->yaw = -90.0f;
            this->moveDirection = glm::normalize(
                glm::vec3{glm::cos(glm::radians(this->yaw)), 0.0, glm::sin(glm::radians(this->yaw))});
            break;
        case CAMERA_THIRDPERSON:
            this->position = position;
            this->target = glm::vec3{5.0f,0.0f,5.0f};
            this->up = glm::vec3{0.0f,1.0f,0.0f};;
            this->fov = glm::radians(60.0);
            this->angle = glm::vec2(0,0);
            this->lookDirection = 1;
            break; 
    }
}

Camera::~Camera(){
    
}


void Camera::Update(const u8* keystate, glm::vec3 targetPosition){    
    if(cameraType == CAMERA_FREECAM){
        if (keystate[SDL_SCANCODE_W]){
            position += 0.2f * moveDirection;
        }
        if (keystate[SDL_SCANCODE_S]){
            position -= 0.2f * moveDirection;
        }
        if (keystate[SDL_SCANCODE_A]){
            position -= 0.2f * right;
        }
        if (keystate[SDL_SCANCODE_D]){
            position += 0.2f * right;
        }
        if(keystate[SDL_SCANCODE_SPACE]){
            position.y += 0.2f;
        }
        if(keystate[SDL_SCANCODE_LSHIFT]){
            position.y -= 0.2f;
        }
    }

    int x;
    int y;
    SDL_GetRelativeMouseState(&x,&y);

    if(cameraType == CAMERA_FREECAM || cameraType == CAMERA_FIRSTPERSON){
        yaw += (f32)x * 0.2f;
        pitch -= (f32)y* 0.2f;     
        
        if (pitch > 89.0f){
            pitch = 89.0f;
        }
        if (pitch < -89.0f){
            pitch = -89.0f;
        }
    
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        
        target = glm::normalize(direction);
    
        right = glm::normalize(glm::cross(target, glm::vec3(0,1,0)));
    
        up =  glm::normalize(glm::cross(right, target));
    
        moveDirection =  glm::normalize(glm::vec3(cos(glm::radians(yaw)), 0, sin(glm::radians(yaw))));
    } 

    if(cameraType == CAMERA_THIRDPERSON){
        if(!isMoving){
            ProcessInput(keystate);
        }
        else if (inputDirection != 0){
            Move();
        }
        else{
            isMoving = false;
        }

        position = targetPosition + glm::vec3(glm::cos(angle.x)*6.0, 4.0 ,sin(angle.y)*6.0) + 0.5f;
        target = targetPosition + 0.5f;

        forward = -glm::normalize(position - target);
        right = glm::normalize(glm::cross(forward, glm::vec3(0,1,0)));
        up =  glm::normalize(glm::cross(right, forward));
    }
}    

void Camera::ProcessInput(const u8* keystate){
    inputDirection = 0;
    inputDirection = keystate[SDL_SCANCODE_E] - keystate[SDL_SCANCODE_Q];        
    
    lookDirection += inputDirection;

    if(lookDirection > 3){
        lookDirection = 0;
    }
    else if(lookDirection < 0){
        lookDirection = 3;
    }
    
    if(inputDirection != 0){
        initialAngle = angle;
        isMoving = true;
    }
}

void Camera::Move(){
    percentMoved += .01f;
    if (percentMoved >= 1.0)
        {
            switch(lookDirection){
            case 0:
                angle.x = 4.712;
                angle.y = 4.712;
                break;
            case 1:
                angle.x = 0;
                angle.y = 0;
                break;
            case 2:
                angle.x = 1.571; 
                angle.y = 1.571;
                break;
            case 3:
                angle.x = 3.142; 
                angle.y = 3.142;    
                break;
            }
            percentMoved = 0.0f;
            isMoving = false;
        }
        else
        {
            angle.x = initialAngle.x + (1.571 * inputDirection * percentMoved);
            angle.y = initialAngle.y + (1.571 * inputDirection * percentMoved);
        }
}


glm::mat4 Camera::GetViewMatrix(){
    glm::mat4 view;
    switch (cameraType){
        case CAMERA_FREECAM:
        case CAMERA_FIRSTPERSON:
            view  = glm::lookAt(this->position, this->position + this->target, this->up);
            break;
        case CAMERA_THIRDPERSON:
            view = glm::lookAt(this->position, this->target, this->up);   
            break; 
    }
    return view;
}

glm::mat4 Camera::GetProjectMatrix(){
    return glm::perspective(this->fov, 1280.0f/720.0f, 0.1f, 1000.0f);
}


glm::vec3 Camera::GetPosition(){
    return position;
}

glm::vec3 Camera::GetForward(){
    return forward;
}

glm::vec3 Camera::GetTarget(){
    return target;
}       

glm::vec3 Camera::GetRight(){
    return right;
}

glm::vec3 Camera::GetUp(){
    return up;
}

f32 Camera::GetFov(){
    return fov;
}

i8 Camera::GetLookdirection(){
    return lookDirection;
}