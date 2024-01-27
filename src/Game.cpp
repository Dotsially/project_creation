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
#include "texture_packer.h"
#include "block_model.h"
#include "texture.h"
#include "shader.h"
#include "block_raycast_handler.h"
#include "world.h"
#include "camera.h"
#include "entity.h"
#include "entity_mesh.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

void InputHandler(Window* gameWindow, const u8* keystate); 

glm::mat4 view;
glm::mat4 perspective;
glm::mat4 transform = glm::mat4(1.0);
glm::mat4 transform2 = glm::mat4(1.0);
bool centeredMouse = false;

typedef enum GameState {LOGO = 0, TITLE, SAVES, SERVERS, WORLD_CREATION, GAMEPLAY} GameState;

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
    GameState gameState = GAMEPLAY;
    //opengl stuff
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BlockModel blockModel;

    TexturePacker texturePacker;
    texturePacker.PackTextures();

    BlockManager blockManager(texturePacker.GetTextures());
    Biome biome(&blockManager);

    Texture texture; 
    texture.InitializeTextureFromAtlas(texturePacker.GetTextureAtlas());
    Texture texture2;
    texture2.InitializeTextureFromFile("selection.png");
    Texture entityTexture;
    entityTexture.InitializeTextureFromFile("player.png");

    Shader shader = Shader("chunk_vertex.glsl", "chunk_fragment.glsl");
    Shader shaderBlock = Shader("vertex_selection.glsl", "fragment_selection.glsl");
    Shader shaderEntity = Shader("entity_vertex.glsl", "entity_fragment.glsl");



    while (!gameWindow.WindowShouldClose())
    {
        switch(gameState){
            case TITLE:
                while (gameState == TITLE && !gameWindow.WindowShouldClose())
                {  
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

                    gameWindow.PollEvents();  
                    
                    const u8* keystate = SDL_GetKeyboardState(NULL);  
                     
                    InputHandler(&gameWindow, keystate);
                    
                    gameWindow.SwapBuffers();              
                }

                break;
            case SAVES:

                break;
            case WORLD_CREATION:
                
                break;    
            case GAMEPLAY:            
                Camera camera = Camera(CAMERA_THIRDPERSON, glm::vec3(0,0,0));
                World world(&camera, blockManager.GetBlocks(), blockModel.GetBlockModels(), biome);

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

                while (gameState == GAMEPLAY && !gameWindow.WindowShouldClose())
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
                        world.Update(&camera, player.GetPosition());
                        transform2 = glm::translate(glm::vec3(blockHandler.GetBlock())+0.5f);
                        transform2 = glm::scale(transform2, glm::vec3(1.02f,1.02f,1.02f));
                        //entityMesh.Update(glm::vec3(0));
                        view = camera.GetViewMatrix();
                    //}   

                    glm::vec3 skyColor = world.GetSkyColor();
                    glClearColor(skyColor.x, skyColor.y, skyColor.z, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                    glEnable(GL_CULL_FACE);

                    shader.UseProgram();
                    glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
                    glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
                    
                    texture.ActivateTexture();
                    world.Draw(player.GetPosition(), camera.GetPosition());
                    
                    if(blockHandler.IsSolid()){
                        shaderBlock.UseProgram();
                        texture2.ActivateTexture();
                        glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
                        glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
                        selectionMesh.DrawMesh(iBuffer.size(), transform2);
                    }

                    glDisable(GL_CULL_FACE);
                    shaderEntity.UseProgram();
                        entityTexture.ActivateTexture();
                        glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
                        glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
                    
                    entityMesh.Draw(&player, &camera);

                    gameWindow.SwapBuffers();
                
                }

                break;
        }

    
    
    
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