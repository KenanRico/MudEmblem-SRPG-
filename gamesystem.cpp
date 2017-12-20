#include "gamesystem.h"
#include "game.h"
#include "eventhandler.h"


#include <SDL2/SDL.h>


int GameSystem::state = Game::State::END;
int GameSystem::start_time = 0;
int GameSystem::window_width = 800;
int GameSystem::window_height = 600;

void GameSystem::init(){
	state = Game::State::RUN;
	start_time = SDL_GetTicks();
}

void GameSystem::update(SDL_Window* window){
	//called in gameLoop's update every render cycle

	//update game state
	if(EventHandler::closeWindow()){
		state = Game::State::END;
	}else;

	//update window demension
	SDL_GetWindowSize(window, &window_width, &window_height);

	//limit FPS by forcing each render cycle to last 1000/60 miliseconds
	int delta_time = SDL_GetTicks() - start_time;
	if(delta_time < 1000.0f/90.0f){
		SDL_Delay((int)(1000.0/90.0f)-delta_time);
	}else;
	start_time = SDL_GetTicks();
}

bool GameSystem::isState(int s){
	return state==s;
}

int GameSystem::windowW(){
	return window_width;
}

int GameSystem::windowH(){
	return window_height;
}

float GameSystem::normalizedX(float pos){
	return (float)pos/(float)window_width;
}

float GameSystem::normalizedY(float pos){
	return (float)pos/(float)window_height;
}

int GameSystem::physicalX(float pos){
	return pos * (float)window_width;
}

int GameSystem::physicalY(float pos){
	return pos * (float)window_height;
}
