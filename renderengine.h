

#include <vector>

#include <SDL2/SDL.h>

#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

class RenderEngine{
	public:
		struct RenderObject{
			SDL_Texture* texture;
			SDL_Rect src;
			SDL_Rect dest;
		};
	private:
		static std::vector<struct RenderObject> ground_renderer;
		static std::vector<struct RenderObject> object_renderer;
		static std::vector<struct RenderObject> effect_renderer;
		static std::vector<struct RenderObject> cursor_renderer;
	private:
		RenderEngine() = delete;
		~RenderEngine() = delete;
		RenderEngine(const RenderEngine&) = delete;
		RenderEngine& operator=(const RenderEngine&) = delete;
	public:
		static void addToRenderer(int, SDL_Texture*, const SDL_Rect&, const SDL_Rect&);
		static void renderAll(SDL_Renderer*);
};

#endif
