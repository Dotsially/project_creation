#include "frustum.h"


void Frustum::Initialize(Camera* camera){
    vFov = camera->GetFov();
    hFov = 2 * glm::atan(glm::tan(vFov * 0.5) * (1280.0f/720.0f));

    f32 halfY = vFov * 0.5f;
    factorY = 1.0f / glm::cos(halfY);
    tanY = glm::tan(halfY); 

    f32 halfX = hFov * 0.5f;
    factorX = 1.0f / glm::cos(halfX);
    tanX = glm::tan(halfX);
}

void Frustum::IsOnFrustum(Camera* camera, Chunk* chunk){
    glm::vec3 sphereVec = chunk->GetChunkCenter() - camera->GetPosition();
    
    f32 sz = glm::dot(sphereVec, camera->GetForward());
    if(!(0.1f - chunkSphereRadius <= sz <= 1000.0f + chunkSphereRadius)){
        chunk->isOnFrustum = 0;
        return;
    }

    f32 distance = 0;
    
    f32 sy = glm::dot(sphereVec, camera->GetUp());
    distance = factorY * chunkSphereRadius + sz * tanY;
    if(!(-distance <= sy <= distance)){
        chunk->isOnFrustum = 0;
        return;
    }

    f32 sx = glm::dot(sphereVec, camera->GetRight());
    distance = factorX * chunkSphereRadius + sz * tanX;
    if(!(-distance <= sx <= distance)){
        chunk->isOnFrustum = 0;
        return;
    }

    chunk->isOnFrustum = 1;
}
