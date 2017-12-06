#include "cursor.h"
#include "eventhandler.h"
#include "game.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/extensions/SDL_image.h>

struct Cursor::CursorImage Cursor::normal = (struct Cursor::CursorImage){nullptr, (SDL_Rect){0,0,0,0}};
struct Cursor::CursorSize Cursor::size = (struct Cursor::CursorSize){40, 40};
SDL_Rect Cursor::dest_rect = (SDL_Rect){0, 0, Cursor::size.width, Cursor::size.height};


void Cursor::init(SDL_Renderer* renderer){
	///load cursor image for "normal"
	SDL_Surface* temp = IMG_Load("cursor/sword.png");
	if(temp!=nullptr){
		normal.texture = SDL_CreateTextureFromSurface(renderer, temp);
		SDL_FreeSurface(temp);
		if(normal.texture!=nullptr){
			SDL_QueryTexture(normal.texture, 0, 0, &(normal.rect.w), &(normal.rect.h));
		}else{
			throw std::runtime_error("ERROR: Failed to retrieve cursor image dimensions");
		}
	}else{
		throw std::runtime_error("ERROR: Failed to load cursor texture");
	}	
}

void Cursor::update(){
	float cursor_x = EventHandler::getThisCyclePosition(Game::Event::X);
	float cursor_y = EventHandler::getThisCyclePosition(Game::Event::Y);
	dest_rect.x = (int)(cursor_x);
	dest_rect.y = (int)(cursor_y);
}

void Cursor::render(SDL_Renderer* renderer){
	SDL_RenderCopy(renderer, normal.texture, &(normal.rect), &(dest_rect));
}

void Cursor::destroy(){
	SDL_DestroyTexture(normal.texture);
}
