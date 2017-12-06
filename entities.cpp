#include "entities.h"
#include "map.h"
#include "eventhandler.h"
#include "game.h"

#include <vector>
#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/extensions/SDL_image.h>



Entities::Entities(SDL_Renderer* renderer): 
current_map(maps.begin()), 
selecting_grid(false), grid_src((SDL_Rect){0, 0, 0, 0}), grid_box((SDL_Rect){0, 0, 0, 0}), grid_texture(nullptr){
	//load grid box texture
	SDL_Surface* temp = IMG_Load("assets/grid1.png");
	if(temp!=nullptr){
		grid_texture = SDL_CreateTextureFromSurface(renderer, temp);
		SDL_FreeSurface(temp);
		//config grid src rectangle
		if(grid_texture!=nullptr){
			SDL_QueryTexture(grid_texture, 0, 0, &grid_src.w, &grid_src.h);
		}else{
			throw std::runtime_error("ERROR: Failed to find grid box texture");
		}
	}else{
		throw std::runtime_error("ERROR: Failed to load grid box texture");
	}
}

Entities::~Entities(){
	//free grid box texture
	SDL_DestroyTexture(grid_texture);
}


void Entities::addMap(SDL_Renderer* renderer, const char* map_source){
	maps.push_back(new Map(renderer, map_source));
	if(maps.size()==1){
		current_map = maps.begin();
	}else{
		//do not change current_map
	}
}

void Entities::update(){
	///update map
	(*current_map)->update();

	///enable/disable grid selecting (click XOR selecting_grid)
	selecting_grid = (EventHandler::getClick(Game::Event::RIGHT) != selecting_grid);

	///update grid_box if grid selecting enabled
	if(selecting_grid){
		const std::vector<struct Map::Grid>& grids = (*current_map)->getRenderedGrids();
		int mouse_x = EventHandler::getThisCyclePosition(Game::Event::X);
		int mouse_y = EventHandler::getThisCyclePosition(Game::Event::Y);
		for(std::vector<struct Map::Grid>::const_iterator iter=grids.begin(); iter!=grids.end(); ++iter){
			const SDL_Rect& box = iter->rectangle;
			if(box.x<=mouse_x && mouse_x<=box.x+box.w && box.y<=mouse_y && mouse_y<=box.y+box.h){
				grid_box = box;
				break;
			}else{
				//keep iterating
			}
		}
	}else{
		//grid selecting disabled
	}
}

void Entities::render(SDL_Renderer* renderer) const{
	//render map
	(*current_map)->render(renderer);
	//render grid box if selecting grid
	if(selecting_grid){
		SDL_RenderCopy(renderer, grid_texture, &grid_src, &grid_box);
	}else;
}
