#include "map.h"

#include <vector>

#include <SDL2/SDL.h>


#ifndef ENTITIES_H
#define ENTITIES_H

class Entities{
	/*
	private:
		struct Level{
			Map* map;
			std::vector<Charactor*> allies;
			std::vector<Character*> enemies;
		};
	private:
		std::map<unsigned int, struct Level> levels;
		std::map<unsigned int, struct Level>::iterator current_level;
	*/

	private:
		//map related members
		std::vector<Map*> maps;
		std::vector<Map*>::iterator current_map;
		//grid selection related members
		bool selecting_grid;
		SDL_Rect grid_src;
		SDL_Rect grid_box;
		SDL_Texture* grid_texture;

	public:
		Entities(SDL_Renderer*);
		~Entities();
	private:
		Entities() = delete;
		Entities(const Entities&) = delete;
		Entities& operator=(const Entities&) = delete;

	public:
		void addMap(SDL_Renderer*, const char*);
		void update();
		void render(SDL_Renderer*) const;
};

#endif
