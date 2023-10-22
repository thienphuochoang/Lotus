#include "Game.h"
#include "SDL_image.h"
#include <glm.hpp>
#include <iostream>
using namespace std;
Game* Game::instance = nullptr;
Game::Game()
{
    cout << "Game constructor called" << endl;
    lotusSDL = new Lotus_SDL();
}

Game::~Game()
{
    cout << "Game destructor called" << endl;
    delete lotusSDL;
}
void Game::Initialize()
{
    lotusSDL->Initialize();
}

void Game::Run()
{
    lotusSDL->Run();
}

void Game::Setup()
{
    lotusSDL->Setup();
}

void Game::ProcessInput()
{
    lotusSDL->ProcessInput();
}

void Game::Update()
{
    lotusSDL->Update();
}

void Game::Render()
{
    lotusSDL->Render();
}

void Game::OnDestroy()
{
    lotusSDL->OnDestroy();
}

Game* Game::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Game();
    }
    return instance;
}
