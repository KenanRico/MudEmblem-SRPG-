#include "map.h"
#include "gridbox.h"

#include <SDL2/SDL.h>


#ifndef SPRITE_H
#define SPRITE_H

class Sprite{
	private:
		enum Movement { STILL=0, LEFT=1, UP=2, RIGHT=3, DOWN=4 };
	protected:
		struct Position{
			float x;
			float y;
		} position; //position relative to the whole map
		int movement;
		SDL_Texture* texture;
		SDL_Rect src;
		SDL_Rect dest;

	public:
		Sprite(Map const *, SDL_Renderer* renderer, const char*, float, float);
		virtual ~Sprite();
	private:
		Sprite() = delete;
		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

	public:
		virtual void update(Map const *, const GridBox&) = 0;
		virtual void render() const = 0;
};

#endif
