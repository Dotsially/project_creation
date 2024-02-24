#pragma once
#define SDL_MAIN_HANDLED
#define FNL_IMPL
#include "depedencies.h"
#include "glad/glad.h"
#include "SDL2/SDL.h"
#include "server/server.h"
#include "client.h"
#include <thread>
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
#include "overworld.h"
#include "overworld_mesh.h"
#include "block_raycast_handler.h"
#include "world.h"
#include "camera.h"
#include "camera_2d.h"
#include "entity_manager.h"
#include "item_manager.h"
#include "entity_mesh.h"
#include "binary_tree.h"
#include <cctype>

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
    GameState gameState = WORLD_CREATION;
    //opengl stuff
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);  
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BlockModel blockModel;

    TexturePacker blockTexturePacker;
    blockTexturePacker.PackTextures("resources/textures/blocks");

    TexturePacker itemTexturePacker;
    itemTexturePacker.PackTextures("resources/textures/items");

    TexturePacker overworldTexturePacker;
    overworldTexturePacker.PackTextures("resources/textures/world_tiles");

    BlockManager blockManager(blockTexturePacker.GetTextures());
    ItemManager itemManager(itemTexturePacker.GetTextures());
    Biome biome(&blockManager);

    Texture texture; 
    texture.InitializeTextureFromAtlas(blockTexturePacker.GetTextureAtlas());
    Texture itemTexture; 
    itemTexture.InitializeTextureFromAtlas(itemTexturePacker.GetTextureAtlas());
    Texture overworldTexture;
    overworldTexture.InitializeTextureFromAtlas(overworldTexturePacker.GetTextureAtlas());
    Texture texture2;
    texture2.InitializeTextureFromFile("selection.png");
    Texture entityTexture;
    entityTexture.InitializeTextureFromFile("player.png");

    Shader shader = Shader("chunk_vertex.glsl", "chunk_fragment.glsl");
    Shader shaderBlock = Shader("vertex_selection.glsl", "fragment_selection.glsl");
    Shader shaderEntity = Shader("entity_vertex.glsl", "entity_fragment.glsl");
    Shader shaderItem = Shader("item_vertex.glsl", "item_fragment.glsl");
    Shader shaderOverworld = Shader("overworld_vertex.glsl", "overworld_fragment.glsl");

    u8 isHost = 0;
    char c = 0;
    std::string ip;
    
    if(gameState == GAMEPLAY){
        std::cout << "(S)erver or (C)lient?" << std::endl;
        if(std::cin >> c){
            if(std::tolower(c) == 'c'){
                std::cout << "Type in the ip: ";
                std::cin >> ip;
            }
            else if(std::tolower(c) == 's'){
                isHost = 1;
            }
        }
    }

    while (!gameWindow.WindowShouldClose())
    {
        switch(gameState){
            case TITLE:
                {


                while (gameState == TITLE && !gameWindow.WindowShouldClose())
                { 
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

                    gameWindow.PollEvents();  
                    
                    const u8* keystate = SDL_GetKeyboardState(NULL);  

                    InputHandler(&gameWindow, keystate);
                    
                    gameWindow.SwapBuffers();              
                }

                }
                break;
            case SAVES:

                break;
            case WORLD_CREATION:
                {
                    Camera2D camera(glm::vec2{100,100}, glm::vec2{80,45}, 1);
                    Overworld overworld;
                    overworld.GenerateOverworld(200,200);        
                    OverworldMesh overworldMesh;   
                    overworldMesh.InitializeMesh(&overworldTexturePacker, overworld.GetWorldTiles(), overworld.GetWidth(), overworld.GetHeight());         

                while (gameState == WORLD_CREATION && !gameWindow.WindowShouldClose())
                { 
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

                    gameWindow.PollEvents();  
                    
                    const u8* keystate = SDL_GetKeyboardState(NULL);  

                    InputHandlerWC(&gameWindow, &camera, keystate);
                    camera.Update();

                    perspective = camera.GetProjectMatrix();

                    
                    shaderOverworld.UseProgram();
                        overworldTexture.ActivateTexture();
                        glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
                    overworldMesh.Draw();
                    
                    gameWindow.SwapBuffers();              
                }

                overworld.DestroyOverworld();

                }
                break;    
            case GAMEPLAY:  
                {          
                Camera camera = Camera(CAMERA_THIRDPERSON, glm::vec3(0,0,0));
                Dungeon dungeon;
                dungeon.CreateDungeonFloor(&blockManager);
                World world(&camera, &dungeon, blockManager.GetBlocks(), blockModel.GetBlockModels(), biome);
                EntityManager entityManager;
                //itemManager.SetupItems(64, dungeon.GetRooms());
                BlockRaycastHandler blockHandler;
            
                //itemManager.CreateItemMesh();

                EntityMesh entityMesh;
                entityMesh.InitializeEntityMesh();
                std::cout << "world created" << std::endl;

                Mesh selectionMesh;
                
                selectionMesh.InitializeMesh(GL_STATIC_DRAW, vBuffer.data(), vBuffer.size(), iBuffer.data(), iBuffer.size());
                selectionMesh.AddAttribute(3, 3, 0);
                perspective = camera.GetProjectMatrix();
                
                const double FRAME_TIME = 1.0 / 60.0; // delta time for 60 FPS

                double lastTime = SDL_GetTicks64();
                double frameCounter = 0;

                if(enet_initialize() != 0){
                    std::cout << "Error initializing enet..." << std::endl;
                }
                else{
                    std::cout << "Enet initialized..." << std::endl; 
                }

                
                Server server;
                Client client;
                std::thread serverThread;

                if(isHost){
                    server.InitializeServer();
                    serverThread = std::thread(&Server::Run, &server); 

                    client.InitializeClient();
                    client.Connect("localhost", 2001);
                    client.ReceiveHandshake(&world, &entityManager);
                }
                else{
                    client.InitializeClient();
                    client.Connect(ip, 2001);
                    client.ReceiveHandshake(&world, &entityManager);
                }

                while (gameState == GAMEPLAY && !gameWindow.WindowShouldClose())
                {
                    client.Update(&world, &entityManager); 
                    gameWindow.PollEvents();

                    double startTime = 0;
                    
                    const u8* keystate = SDL_GetKeyboardState(NULL);

                    InputHandler(&gameWindow, keystate);

                    //wrong math 
                    //if(passedTime >= 1.0/60.0){
                        //lastTime = startTime;
                        //frameCounter = 0;  
                        gameWindow.Update(centeredMouse);
                        
                        blockHandler.Update(&camera, &world, keystate);
                        entityManager.Update(&camera, &world, client.GetServer());
                        camera.Update(keystate, entityManager.GetPlayerPosition());
                        entityMesh.Update(entityManager.GetEntities());
                        
                        world.Update(&camera, entityManager.GetPlayerPosition());
                        transform2 = glm::translate(glm::vec3(blockHandler.GetBlock())+0.5f);
                        transform2 = glm::scale(transform2, glm::vec3(1.02f,1.02f,1.02f));
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
                    world.Draw(entityManager.GetPlayerPosition(), camera.GetPosition());
                    
                    if(blockHandler.IsSolid()){
                        shaderBlock.UseProgram();
                        texture2.ActivateTexture();
                        glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
                        glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
                        selectionMesh.DrawMesh(iBuffer.size(), transform2);
                    }

                    glDisable(GL_CULL_FACE);

                    // shaderItem.UseProgram();
                    //     itemTexture.ActivateTexture();
                    //     glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
                    //     glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
                    // itemManager.DrawItems(&camera);

                    shaderEntity.UseProgram();
                        entityTexture.ActivateTexture();
                        glUniformMatrix4fv(1,1, false, glm::value_ptr(perspective));
                        glUniformMatrix4fv(2,1, false, glm::value_ptr(view));
                    entityMesh.Draw(&camera);

                    
                    gameWindow.SwapBuffers();

                }

                dungeon.DestroyDungeonFloor();
                
                client.DestroyClient();
                if(isHost){
                    server.Stop();
                    server.DestroyServer();
                    serverThread.join();
                }
                }

                break;
        }

    
    
    
    }
    
    
    return 0;
}


void InputHandlerWC(Window* gameWindow, Camera2D* camera, const u8* keystate){
    SDL_Event* e = gameWindow->GetEvent();
    if(e->type == SDL_QUIT){
        gameWindow->Quit();
    }
    if(e->type == SDL_MOUSEWHEEL){
        std::cout << e->wheel.x << " " << e->wheel.y << std::endl;
        i32 y = e->wheel.y;
        camera->UpdateZoom(y);
    }
    if(keystate[SDL_SCANCODE_ESCAPE]){
        gameWindow->Quit();    
    }
    if(keystate[SDL_SCANCODE_X]){
        centeredMouse = !centeredMouse;
    }
}

void InputHandler(Window* gameWindow, const u8* keystate){
    SDL_Event* e = gameWindow->GetEvent();
    if(e->type == SDL_QUIT){
        gameWindow->Quit();
    }
    if(keystate[SDL_SCANCODE_ESCAPE]){
        gameWindow->Quit();    
    }
    if(keystate[SDL_SCANCODE_X]){
            centeredMouse = !centeredMouse;
    }
}