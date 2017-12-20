#include "sprite.h"
#include "map.h"
#include "camera.h"
#include "gamesystem.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/extensions/SDL_image.h>


Sprite::Sprite(Map const * map, SDL_Renderer* renderer, const char* sprite_source, float x, float y):
position((struct Position){0.0f, 0.0f}), movement(STILL), texture(nullptr), src((SDL_Rect){0,0,0,0}), dest((SDL_Rect){0,0,0,0}){
	///config in-map position
	position.x = (0.0f<=x && x<=1.0f) ? x : 0.0f;
	position.y = (0.0f<=y && y<=1.0f) ? y : 0.0f;

	///load texture
	SDL_Surface* temp = IMG_Load(sprite_source);
	if(temp!=nullptr){
		texture = SDL_CreateTextureFromSurface(renderer, temp);
		SDL_FreeSurface(temp);
		//config src rectangle
		if(texture!=nullptr){
			SDL_QueryTexture(texture, 0, 0, &src.w, &src.h);
		}else{
			throw std::runtime_error("ERROR: Failed to find sprite texture");
		}
	}else{
		throw std::runtime_error("ERROR: Failed to load sprite texture");
	}

	///config dest rectangle
	const struct Camera::Position& camera = map->getCameraInfo();
	dest.x = (position.x - camera.left) / camera.width * (float)GameSystem::windowW();
	dest.y = (position.y - camera.top) / camera.height * (float)GameSystem::windowH();
	map->getGridDimension(&dest.w, &dest.h);
}

Sprite::~Sprite(){
	SDL_DestroyTexture(texture);
}
