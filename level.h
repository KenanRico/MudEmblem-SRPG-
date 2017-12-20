#include "map.h"
#include "sprite.h"
#include "gridbox.h"

#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>


#ifndef LEVEL_H
#define LEVEL_H

class Level{
	private:
		Map* map;
		std::map<std::string, Sprite*> sprites;

	public:
		Level(SDL_Renderer*, const char*);
		~Level();
	private:
		Level() = delete;
		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;

	public:
		void update(const GridBox&);
		void render() const;
		const std::vector<struct Map::Grid>& getRenderedGrids() const;
};

#endif
