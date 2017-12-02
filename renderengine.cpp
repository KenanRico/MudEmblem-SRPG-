#include "renderEngine.h"
#include "map.h"

#include <stdexcept>

#include <SDL2/SDL.h>


SDL_Renderer* RenderEngine::renderer = nullptr;


void RenderEngine::init(SDL_Renderer* r){
	if(r!=nullptr){
		renderer = r;
	}else{
		throw std::runtime_error("ERROR: passing NULL pointer as renderer into render engine");
	}
}

void RenderEngine::renderMap(const Map&){

}
