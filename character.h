#include "sprite.h"
#include "map.h"
#include "gridbox.h"


#include <SDL2/SDL.h>


#ifndef CHARACTER_H
#define CHARACTER_H

class Character: public Sprite{
	private:
		//some fields

	public:
		Character(Map const *, SDL_Renderer*, const char*, float, float);
		~Character();
	private:
		Character() = delete;
		Character(const Character&) = delete;
		Character& operator=(const Character&) = delete;

	public:
		void update(Map const *, const GridBox&) override;
		void render() const override;
	private:
		void updatePosition(const struct Map::ObjectLayer&, const GridBox&);
		void pathFind(int, int, int, int, int const **);
};

#endif
