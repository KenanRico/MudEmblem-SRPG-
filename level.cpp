#include "level.h"
#include "map.h"
#include "sprite.h"
#include "character.h"
#include "gridbox.h"

#include <map>

#include <SDL2/SDL.h>


Level::Level(SDL_Renderer* renderer, const char* map_source){
	//init map
	map = new Map(renderer, map_source);
	//initialize sprites and insert into level's new sprites
	//sprites.insert(std::make_pair("square#1", new Character(map, renderer, "assets/square.png", 0.0f, 0.0f)));
	sprites.insert(std::make_pair("square#2", new Character(map, renderer, "assets/square.png", 0.5f, 0.5f)));
}

Level::~Level(){
	delete map;
	for(std::map<std::string, Sprite*>::iterator iter=sprites.begin(); iter!=sprites.end(); ++iter){
		delete iter->second;
	}
}

void Level::update(const GridBox& grid){
	map->update();
	for(std::map<std::string, Sprite*>::iterator iter=sprites.begin(); iter!=sprites.end(); ++iter){
		iter->second->update(map, grid);
	}
}

void Level::render() const{
	map->render();
	for(std::map<std::string, Sprite*>::const_iterator iter=sprites.begin(); iter!=sprites.end(); ++iter){
		iter->second->render();
	}
}

const std::vector<struct Map::Grid>& Level::getRenderedGrids() const{
	return map->getRenderedGrids();
}
