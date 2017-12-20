#include "map.h"
#include "gridbox.h"
#include "level.h"

#include <vector>
#include <map>
#include <string>

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
		std::vector<Level*> levels;
		Level* current_level;
		/*
		std::vector<Map*> maps;
		Map* current_map;
		*/
		//grid box
		GridBox gridbox;
		//sprites
		std::vector<Sprite*> sprites;

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
		void render() const;
};

#endif
