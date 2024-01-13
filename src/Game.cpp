#pragma once
#define SDL_MAIN_HANDLED
#define FNL_IMPL
#include "depedencies.h"
#include "glad/glad.h"
#include "SDL2/SDL.h"
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "window.h"
#include "texture.h"
#include "shader.h"
#include "block_raycast_handler.h"
#include "world.h"
#include "camera.h"
#include "entity.h"
#include "entity_mesh.h"
#include "block_model.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void InputHandler(Window* gameWindow, const u8* keystate); 

glm::mat4 view;
glm::mat4 perspective;
glm::mat4 transform = glm::mat4(1.0);
glm::mat4 transform2 = glm::mat4(1.0);
bool centeredMouse = false;

int main(int argc, char* args[]){
    std::vector<f32> vBuffer;
    std::vector<u32> iBuffer;

    vBuffer.insert(vBuffer.begin(), VerticesList::verticesCube[0], VerticesList::verticesCube[0]+72);

    for(int j = 0; j < 6; j++){
        if(j % 2 == 0){
            for(u32 i : VerticesList::indicesCube[0]){
                iBuffer.push_back(i + (j*4));
            }
        }
        else{
            for(u32 i : VerticesList::indicesCube[1]){
                iBuffer.push_back(i + (j*4));
            }
        }
        
    }
    Window gameWindow = Window(SCREEN_WIDTH, SCREEN_HEIGHT, "World");
    
    //opengl stuff
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BlockModel blockModel;
    BlockManager blockManager;
    Biome biome(&blockManager);
    Camera camera = Camera(CAMERA_THIRDPERSON, glm::vec3(0,0,0));
    Texture texture = Texture("tilemap.png");
    Texture texture2 = Texture("selection.png");
    Texture entityTexture  = Texture("guy.png");
    Shader shader = Shader("chunk_vertex.glsl", "chunk_fragment.glsl");
    Shader shaderBlock = Shader("vertex_selection.glsl", "fragment_selection.glsl");
    Shader shaderEntity = Shader("entity_vertex.glsl", "entity_fragment.glsl");
    World world(blockManager.GetBlocks(), blockModel.GetBlockModels(), biome);

    //don't remove this code or the world will break :)    
    std::vector<fnl_state> noiseVec; 

    Chunk c; 
    Chunk* n[4];
    c.CreateChunkData(blockManager.GetBlocks(), blockModel.GetBlockModels(), biome.GetBiome("desert"), noiseVec, 0,0);
    //c.CreateChunkMesh(n);
    //


    BlockRaycastHandler blockHandler;
    Entity player = Entity(glm::vec2(2,8), &world);
    player.flags.playerControlled = 1;
    EntityMesh entityMesh = EntityMesh();
    std::cout << "world created" << std::endl;

    Mesh selectionMesh;

    selectionMesh.InitializeMesh(GL_STATIC_DRAW, vBuffer.data(), vBuffer.size(), iBuffer.data(), iBuffer.size());

    perspective = camera.GetProjectMatrix();
    
    const double FRAME_TIME = 1.0 / 60.0; // delta time for 60 FPS

    double lastTime = SDL_GetTicks64();
    double frameCounter = 0;

    while (!gameWindow.WindowShouldClose())
    {
        gameWindow.PollEvents();

        double startTime = SDL_GetTicks64();
        double passedTime = startTime - lastTime;
        lastTime = startTime;
        frameCounter++;
        
        const u8* keystate = SDL_GetKeyboardState(NULL);

        InputHandler(&gameWindow, keystate);

        //wrong math 
        //if(passedTime >= 1.0/60.0){
            //lastTime = startTime;
            //frameCounter = 0;
            
            gameWindow.Update(centeredMouse);
            
            camera.Update(keystate, player.GetPosition());
            blockHandler.Update(&camera, &world, keystate);
            player.Update(&camera, &world);
            world.Update(player.GetPosition());
            transform2 = glm::translate(glm::vec3(blockHandler.GetBlock())+0.5f);
            transform2 = glm::scale(transform2, glm::vec3(1.02f,1.02f,1.02f));
            //entityMesh.Update(glm::vec3(0));
            view = camera.GetViewMatrix();
        //}   

        glm::vec3 skyColor = world.GetSkyColor();
        glClearColor(skyColor.x, skyColor.y, skyColor.z, 0.0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        texture.ActivateTexture();
        shader.UseProgram();
        glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
        glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
        world.Draw(player.GetPosition(), camera.GetPosition());
        //keep this here otherwise chunk generation crashes :)
        //chunk.Draw();
        
        if(blockHandler.IsSolid()){
            texture2.ActivateTexture();
            shaderBlock.UseProgram();
            glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
            glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
            selectionMesh.DrawMesh(iBuffer.size(), transform2);
        }
        entityTexture.ActivateTexture();
        shaderEntity.UseProgram();
            glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
            glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
        entityMesh.Draw(player.GetPosition(), &camera);
        gameWindow.SwapBuffers();
    }

    return 0;
}

void InputHandler(Window* gameWindow, const u8* keystate){
    SDL_Event e = gameWindow->GetEvents();
    if(e.type  == SDL_QUIT){
        gameWindow->Quit();
    }
    if(keystate[SDL_SCANCODE_ESCAPE]){
        gameWindow->Quit();    
    }
    if(keystate[SDL_SCANCODE_X]){
        centeredMouse = !centeredMouse;
    }
}