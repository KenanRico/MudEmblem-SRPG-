#include "entities.h"
#include "map.h"
#include "eventhandler.h"
#include "game.h"
#include "gridbox.h"
#include "level.h"

#include <vector>
#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>



Entities::Entities(SDL_Renderer* renderer): 
current_level(nullptr), 
gridbox(renderer){
	//do nothing
}

Entities::~Entities(){
	//do nothing
	for(std::vector<Level*>::iterator iter=levels.begin(); iter!=levels.end(); ++iter){
		delete *iter;
	}
}


void Entities::addMap(SDL_Renderer* renderer, const char* map_source){
	//insert level
	levels.push_back(new Level(renderer, map_source));
	//set current_level to first level on first insert, otherwise don't touch
	if(levels.size()==1){
		current_level = *(levels.begin());
	}else{
		//do not change current_map
	}

	/*
	maps.push_back(new Map(renderer, map_source));
	if(maps.size()==1){
		current_map = *(maps.begin());
	}else{
		//do not change current_map
	}
	*/
}

void Entities::update(){
	///update level
	current_level->update(gridbox);


	/*
	current_map->update();
	*/

	///update grid box
	gridbox.update(current_level->getRenderedGrids());
}

void Entities::render() const{
	//render level
	current_level->render();
	
	/*
	current_map->render(renderer);
	*/
	//render gridbox
	gridbox.render();
}
