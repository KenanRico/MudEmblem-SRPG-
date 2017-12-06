#include "gamesystem.h"
#include "game.h"
#include "eventhandler.h"
#include "entities.h"
#include "cursor.h"

#include <stdexcept>
#include <iostream>

#include <SDL2/SDL.h>

#ifdef main
#undef main
#endif


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Entities* entities = nullptr;

void InitSystem();
void InitGame();
void GameLoop();
void updateGame();
void renderGame();
void DestroyGame();
void DestroySystem();


int main()
{
	try{
		InitSystem();
		InitGame();
		GameLoop();
		DestroyGame();
		DestroySystem();
	}catch(const std::exception& Err){
		std::cout<<Err.what()<<"\n";
	}
    return 0;
}

void InitSystem(){
	//prevent gdb from crashing (no idea why)
	SDL_SetHint(SDL_HINT_WINDOWS_DISABLE_THREAD_NAMING, "1");
	//init SDL
	if(SDL_Init(SDL_INIT_EVERYTHING)==0){
		if((window = SDL_CreateWindow("J Cole the blues guitar god", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GameSystem::windowW(), GameSystem::windowH(), SDL_WINDOW_RESIZABLE)) != nullptr){
			if((renderer = SDL_CreateRenderer(window, -1, 0)) != nullptr){
				SDL_SetRenderDrawColor(renderer, 10,47,90, 255);
			}else{
				throw std::runtime_error("ERROR: Failed to create renderer");
			}
		}else{
			throw std::runtime_error("ERROR: Failed to create window");
		}
	}else{
		throw std::runtime_error("Failed to init SDL");
	}
	//hide default cursor
	SDL_ShowCursor(0);
}

void InitGame(){
	GameSystem::init();
	EventHandler::init();
	Cursor::init(renderer);
	entities = new Entities(renderer);
	entities->addMap(renderer, "maps/map1/map1.tmx");
}

void GameLoop(){
	while(!GameSystem::isState(Game::State::END)){
		updateGame();
		renderGame();
	}
}

void updateGame(){
	GameSystem::update(window);
	EventHandler::update();
	Cursor::update();
	entities->update();
}

void renderGame(){
	SDL_RenderClear(renderer);

	entities->render(renderer);
	Cursor::render(renderer);

	SDL_RenderPresent(renderer);
}

void DestroyGame(){
	Cursor::destroy();
}

void DestroySystem(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
