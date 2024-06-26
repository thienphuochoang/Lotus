#include "Lotus_SDL.h"
#include "../log/Lotus_Log.h"
#include "../ecs/ECS.h"
#include "../level/Level.h"
#include "../systems/MovementSystem.h"
#include "../systems/RenderSystem.h"
#include "../systems/AnimationSystem.h"
#include "../systems/CollisionSystem.h"
#include "../systems/RenderColliderSystem.h"
#include "../systems/DamageSystem.h"
#include "../systems/KeyboardControlSystem.h"
#include "../systems/CameraMovementSystem.h"
#include "../systems/ProjectileEmitSystem.h"
#include "../systems/ProjectileSystem.h"
#include "../systems/RenderTextSystem.h"
#include "../systems/RenderHealthBarSystem.h"
#include "../systems/RenderGUISystem.h"
#include <SDL.h>
#include <SDL_image.h>
#include <glm.hpp>
#include <iostream>
#include <imgui.h>
#include <../imgui/backends/imgui_impl_sdl2.h>
#include <../imgui/backends/imgui_impl_sdlrenderer2.h>
using namespace std;

int Lotus_SDL::MAP_WIDTH;
int Lotus_SDL::MAP_HEIGHT;
int Lotus_SDL::WINDOW_WIDTH;
int Lotus_SDL::WINDOW_HEIGHT;
Lotus_SDL::Lotus_SDL()
{
    registry = std::make_unique<EntityManager>();
    assets = std::make_unique<Asset>();
    eventManager = std::make_unique<EventManager>();
    Lotus_Log::Info("LotusSDL constructor called");
}

Lotus_SDL::~Lotus_SDL()
{
    Lotus_Log::Info("LotusSDL destructor called");
}

void Lotus_SDL::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Lotus_Log::Error("Error initializing SDL!!!");
        return;
    }

    if (TTF_Init() != 0)
    {
        Lotus_Log::Error("Error initializing SDL TTF!!!");
        return;
    }

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    WINDOW_WIDTH = 1280;
    WINDOW_HEIGHT = 720;
    //WINDOW_WIDTH = displayMode.w;
    //WINDOW_HEIGHT = displayMode.h;
    window = SDL_CreateWindow("Lotus",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        Lotus_Log::Error("Error creating SDL window!!!");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        Lotus_Log::Error("Error creating SDL renderer!!!");
        return;
    }

    ImGui::CreateContext();
    ImGui::StyleColorsLight();
    // Initialize ImGui for SDL and SDL_Renderer
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    // Initialize the camera view
    camera.x = 0;
    camera.y = 0;
    camera.w = WINDOW_WIDTH;
    camera.h = WINDOW_HEIGHT;
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void Lotus_SDL::Run()
{
    Setup();
    while (!quit)
    {
        ProcessInput();
        Update();
        Render();
    }
}

void Lotus_SDL::Setup()
{
    // Add the systems that need to be processed in our game
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<RenderColliderSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<KeyboardControlSystem>();
    registry->AddSystem<CameraMovementSystem>();
    registry->AddSystem<ProjectileEmitSystem>();
    registry->AddSystem<ProjectileSystem>();
    registry->AddSystem<RenderTextSystem>();
    registry->AddSystem<RenderHealthBarSystem>();
    registry->AddSystem<RenderGUISystem>();
    Level levelManager;
    levelManager.Load(registry, assets, renderer, 1);
}

void Lotus_SDL::ProcessInput()
{
    SDL_Event currentEvents;
    // Check for events
    while (SDL_PollEvent(&currentEvents))
    {
        ImGui_ImplSDL2_ProcessEvent(&currentEvents);
        ImGuiIO& io = ImGui::GetIO();

        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

        io.MousePos = ImVec2(mouseX, mouseY);
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

        switch (currentEvents.type)
        {
            case SDL_QUIT:
            {
                quit = true;
                break;
            }
            case SDL_KEYDOWN:
            {
                switch (currentEvents.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                    {
                        quit = true;
                    }
                }
                eventManager->EmitEvent<KeyPressedEvent>(currentEvents.key.keysym.sym);
                break;
            }
        }
    }
}

void Lotus_SDL::Update()
{
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME)
    {
        SDL_Delay(timeToWait);
    }
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    millisecsPreviousFrame = SDL_GetTicks();
    
    // Reset all event handlers for the current frame
    eventManager->Reset();
    
    // Execute the subscriptions of the events for all systems
    registry->GetSystem<MovementSystem>().SubscribeToEvents(eventManager);
    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventManager);
    registry->GetSystem<KeyboardControlSystem>().SubscribeToEvents(eventManager);
    registry->GetSystem<ProjectileEmitSystem>().SubscribeToEvents(eventManager);

    // Update the registry to process the entities that are waiting to be created or removed
    registry->Update();

    // Update the systems
    registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventManager);
    registry->GetSystem<CameraMovementSystem>().Update(camera);
    registry->GetSystem<ProjectileEmitSystem>().Update(registry);
    registry->GetSystem<ProjectileSystem>().Update();
}

void Lotus_SDL::Render()
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->GetSystem<RenderSystem>().Update(renderer, assets, camera);
    registry->GetSystem<RenderTextSystem>().Update(renderer, assets, camera);
    registry->GetSystem<RenderHealthBarSystem>().Update(renderer, assets, camera);
    registry->GetSystem<RenderColliderSystem>().Update(renderer, camera);

    // Start ImGui frame
    //ImGui_ImplSDLRenderer2_NewFrame();
    //ImGui_ImplSDL2_NewFrame();
    //ImGui::NewFrame();
    //ImGui::Render();
    //ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    registry->GetSystem<RenderGUISystem>().Update(renderer, registry);

    SDL_RenderPresent(renderer);
}

void Lotus_SDL::OnDestroy()
{
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}