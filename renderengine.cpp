#include "renderengine.h"
#include "game.h"

#include <vector>
#include <iostream>
#include <algorithm>

#include <SDL2/SDL.h>


std::vector<struct RenderEngine::RenderObject> RenderEngine::ground_renderer;
std::vector<struct RenderEngine::RenderObject> RenderEngine::object_renderer;
std::vector<struct RenderEngine::RenderObject> RenderEngine::effect_renderer;
std::vector<struct RenderEngine::RenderObject> RenderEngine::cursor_renderer;
bool operator<(const struct RenderEngine::RenderObject& lhs, const struct RenderEngine::RenderObject& rhs){
	return (rhs.dest.y+rhs.dest.h)<(lhs.dest.y+lhs.dest.h);
}


void RenderEngine::addToRenderer(int type, SDL_Texture* texture, const SDL_Rect& src, const SDL_Rect& dest){
	switch(type){
		case Game::Render::GROUND:
			ground_renderer.push_back((struct RenderObject){texture, src, dest});
			break;
		case Game::Render::OBJECT:
			object_renderer.push_back((struct RenderObject){texture, src, dest});
			break;
		case Game::Render::EFFECT:
			effect_renderer.push_back((struct RenderObject){texture, src, dest});
			break;
		case Game::Render::CURSOR:
			cursor_renderer.push_back((struct RenderObject){texture, src, dest});
			break;
		default:
			break;
	}
}

void RenderEngine::renderAll(SDL_Renderer* renderer){
	//sort object_renderer based on objects' bottom y coord
	std::stable_sort(object_renderer.begin(), object_renderer.end());
	//render everything in particular order
	for(std::vector<struct RenderObject>::iterator iter=ground_renderer.begin(); iter!=ground_renderer.end(); ++iter){
		SDL_RenderCopy(renderer, iter->texture, &(iter->src), &(iter->dest));
	}
	for(std::vector<struct RenderObject>::iterator iter=object_renderer.begin(); iter!=object_renderer.end(); ++iter){
		SDL_RenderCopy(renderer, iter->texture, &(iter->src), &(iter->dest));
	}
	for(std::vector<struct RenderObject>::iterator iter=effect_renderer.begin(); iter!=effect_renderer.end(); ++iter){
		SDL_RenderCopy(renderer, iter->texture, &(iter->src), &(iter->dest));
	}
	for(std::vector<struct RenderObject>::iterator iter=cursor_renderer.begin(); iter!=cursor_renderer.end(); ++iter){
		SDL_RenderCopy(renderer, iter->texture, &(iter->src), &(iter->dest));
	}

	//clear all buffer
	ground_renderer.clear();
	object_renderer.clear();
	effect_renderer.clear();
	cursor_renderer.clear();
}
