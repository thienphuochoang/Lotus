#include "Game.h"

#include <SDL_image.h>
#include <glm.hpp>
#include <iostream>
using namespace std;
Game::Game()
{
    cout << "Game constructor called" << endl;
}

Game::~Game()
{
    cout << "Game destructor called" << endl;
}

void Game::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cerr << "Error initializing SDL!!!" << endl;
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    window = SDL_CreateWindow("Lotus",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        cerr << "Error creating SDL window!!!" << endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        cerr << "Error creating SDL renderer!!!" << endl;
        return;
    }
    //SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
}

void Game::Run()
{
    Setup();
    while (!quit)
    {
        ProcessInput();
        Update();
        Render();
    }
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup()
{
    playerPosition = glm::vec2(10, 20);
    playerVelocity = glm::vec2(0.1, 0);
}

void Game::ProcessInput()
{
    SDL_Event currentEvents;
    // Check for events
    while (SDL_PollEvent(&currentEvents))
    {
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
                        break;
                    }
                }
            }
        }
    }
}

void Game::Update()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks64(), millisecsPreviousFrame + MILLISECS_PER_FRAME));
    millisecsPreviousFrame = SDL_GetTicks64();
    playerPosition.x += playerVelocity.x;
    playerPosition.y += playerVelocity.y;
}

void Game::Render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Surface* surface = IMG_Load("./art/Character/Enemy-Carrot/Sprite-Sheets/carrot@idle-sheet.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect dstRect = { playerPosition.x, playerPosition.y, 640, 128 };
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);


    SDL_RenderPresent(renderer);
}

void Game::OnDestroy()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
