//own include
#include "map.h"

//sys include

//lib include
#include <SDL2/SDL.h>


#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

class RenderEngine{
	private:
		static SDL_Renderer* renderer;

	private:
		RenderEngine() = delete;
		RenderEngine(const RenderEngine&) = delete;
		RenderEngine& operator=(const RenderEngine&) = delete;

	public:
		static void init(SDL_Renderer*);
		static void renderMap(const Map&);
		//and more
};

#endif
