#include "gridbox.h"
#include "map.h"
#include "eventhandler.h"
#include "game.h"
#include "renderengine.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/extensions/SDL_image.h>


GridBox::GridBox(SDL_Renderer* renderer): selecting(false), src((SDL_Rect){0,0,0,0}), box((SDL_Rect){0,0,0,0}), texture(nullptr), row_index(-1), column_index(-1){
	//load grid box texture
	SDL_Surface* temp = IMG_Load("assets/grid1.png");
	if(temp!=nullptr){
		texture = SDL_CreateTextureFromSurface(renderer, temp);
		SDL_FreeSurface(temp);
		//config grid src rectangle
		if(texture!=nullptr){
			SDL_QueryTexture(texture, 0, 0, &src.w, &src.h);
		}else{
			throw std::runtime_error("ERROR: Failed to find grid box texture");
		}
	}else{
		throw std::runtime_error("ERROR: Failed to load grid box texture");
	}
}

GridBox::~GridBox(){
	SDL_DestroyTexture(texture);
}

void GridBox::update(const std::vector<Map::Grid>& grids){
	///enable/disable grid selecting (click XOR selecting_grid)
	selecting = (EventHandler::getClick(Game::Event::RIGHT) != selecting);

	///update grid_box if grid selecting enabled
	if(selecting){
		int mouse_x = EventHandler::getThisCyclePosition(Game::Event::X);
		int mouse_y = EventHandler::getThisCyclePosition(Game::Event::Y);
		for(std::vector<struct Map::Grid>::const_iterator iter=grids.begin(); iter!=grids.end(); ++iter){
			const SDL_Rect& _box = iter->rectangle;
			if(_box.x<=mouse_x && mouse_x<=_box.x+_box.w && _box.y<=mouse_y && mouse_y<=_box.y+_box.h){
				box = _box;
				row_index = iter->row_index;
				column_index = iter->column_index;
				break;
			}else{
				//keep iterating
			}
		}
	}else{
		//grid selecting disabled
	}	
}

void GridBox::render() const{
	if(selecting){
		//SDL_RenderCopy(renderer, texture, &src, &box);
		RenderEngine::addToRenderer(Game::Render::EFFECT, texture, src, box);
	}else;
}

void GridBox::getGridIndex(int* row, int* col) const{
	*row = row_index;
	*col = column_index;
}
