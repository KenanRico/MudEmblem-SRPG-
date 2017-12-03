#include "eventhandler.h"
#include "game.h"

#include <iostream>

#include <SDL2/SDL.h>


struct EventHandler::Mouse EventHandler::mouse = (EventHandler::Mouse){
	{false, false, false}, {false, false, false}, {0, 0},
	{{0.0f,0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}}, {0.0f, 0.0f}
};
SDL_Event EventHandler::event;
bool EventHandler::close = false;


void EventHandler::init(){
	//nothing to init
}

void EventHandler::update(){
	mouse.click[Game::Event::LEFT] = false;
	mouse.click[Game::Event::MIDDLE] = false;
	mouse.click[Game::Event::RIGHT] = false;
	mouse.scroll[Game::Event::X] = 0;
	mouse.scroll[Game::Event::Y] = 0;
	
	if(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_QUIT:
				close = true;
				break;
			case SDL_MOUSEMOTION:
				mouse.current_position[Game::Event::X] = event.motion.x;
				mouse.current_position[Game::Event::Y] = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(!mouse.press[event.button.button-1]){
					mouse.press[event.button.button-1] = true;
					mouse.click[event.button.button-1] = true;
				}else;
				mouse.click_position[event.button.button-1][Game::Event::X] = event.button.x;
				mouse.click_position[event.button.button-1][Game::Event::Y] = event.button.y;
				break;
			case SDL_MOUSEBUTTONUP:
				mouse.press[event.button.button-1] = false;
				break;
			case SDL_MOUSEWHEEL:
				mouse.scroll[Game::Event::Y] = event.wheel.y;
				break;
			default:
				break;
		}
	}else{
		//no events
	}
}

bool EventHandler::getClick(int button){
	return mouse.click[button];
}

bool EventHandler::getPress(int button){
	return mouse.press[button];
}

int EventHandler::getScroll(int button){
	return mouse.scroll[button];
}

float EventHandler::getClickPosition(int button, int axis){
	return mouse.click_position[button][axis];
}

float EventHandler::getCurrentPosition(int button){
	return mouse.current_position[button];
}

bool EventHandler::closeWindow(){
	return close;
}
