#pragma once
#include "depedencies.h"
#include "camera.h"
#include "chunk.h"


class Frustum{
private:
    f32 chunkSphereRadius = (CHUNK_SIZE/2.0) * glm::sqrt(3);
    f32 vFov;
    f32 hFov;
    f32 factorY;
    f32 tanY;
    f32 factorX;
    f32 tanX;

public:
    void IsOnFrustum(Camera* camera, Chunk* chunk);
    void Initialize(Camera* camera);

};