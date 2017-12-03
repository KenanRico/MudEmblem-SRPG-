#include "map.h"
#include "xmlparse.h"
#include "gamesystem.h"
#include "eventhandler.h"
#include "game.h"

#include <vector>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/extensions/SDL_image.h>



Map::Map(SDL_Renderer* renderer, const char* tilemap_src): tilemap((struct Tilemap){0, 0, 0.0f, 0.0f}), camera((struct Camera){0.0f, 0.0f, 0.0f, 0.0f}){

	///set camera to some portion of the tilemap
	camera = (struct Camera){0.4f, 0.6f, 0.4f, 0.6f};

	///parse tilemap into tilesets and mapping
	XMLParse::parseToTilemap(tilemap_src, tilemap, tilesets, mapping);
	//load tilesets' textures 
	std::string path(tilemap_src);
	path = path.substr(0, path.find_last_of('/'));
	for(std::vector<struct Tileset>::iterator iter=tilesets.begin(); iter!=tilesets.end(); ++iter){
		SDL_Surface* temp = IMG_Load(std::string(path+"/"+iter->source).c_str());
		if(temp!=nullptr){
			iter->texture = SDL_CreateTextureFromSurface(renderer, temp);
			SDL_FreeSurface(temp);
		}else{
			throw std::runtime_error("ERROR: Failed to load texture");
		}
	}

	//print mappings in console
	for(std::vector<int**>::const_iterator grid=mapping.begin(); grid!=mapping.end(); ++grid){
		for(int i=0; i<tilemap.height; ++i){
			for(int j=0; j<tilemap.width; ++j){
				std::cout<<(*grid)[i][j]<<" ";
			}
			std::cout<<"\n";
		}
		std::cout<<"\n";
	}
}

Map::~Map(){
	//free textures
	for(std::vector<struct Tileset>::iterator iter=tilesets.begin(); iter!=tilesets.end(); ++iter){
		if(iter->texture!=nullptr){
			SDL_DestroyTexture(iter->texture);
		}else{
			throw std::runtime_error("ERROR: texture is NULL");
		}
	}
	//free mapping
	for(std::vector<int**>::iterator iter=mapping.begin(); iter!=mapping.end(); ++iter){
		for(int i=0; i<tilemap.height; ++i){
			delete[] (*iter)[i];
		}
		delete[] *iter;
	}
}

void Map::update(){
	updateCamera();
}

void Map::updateCamera(){
	//zoom in/out (temporary functionality)
	float new_left = camera.left + EventHandler::getScroll(Game::Event::Y) * 0.005f;
	float new_right = camera.right - EventHandler::getScroll(Game::Event::Y) * 0.005f;
	float new_top = camera.top + EventHandler::getScroll(Game::Event::Y) * 0.005f;
	float new_bottom = camera.bottom - EventHandler::getScroll(Game::Event::Y) * 0.005f;

	//make sure camera is not zoomed too close/far (0.0 <= left < right <= 1.0 and 0.0 <= top < bottom <= 1.0)
	if(0.0f<=new_left && new_left<new_right && new_right<=1.0f && 0.0f<=new_top && new_top<new_bottom && new_bottom<=1.0f){
		camera.left = new_left;
		camera.right = new_right;
		camera.top = new_top;
		camera.bottom = new_bottom;
	}else{
		//do not update camera
	}
}

void Map::render(SDL_Renderer* renderer) const{
	//clear currently-renderer grids vector
	grids.clear();

	//init render rectangles	
	SDL_Rect src = {0, 0, 0, 0};
	SDL_Rect dest = {0, 0, 0, 0};

	//render grids
	for(int i=0; i<tilemap.height; ++i){
		for(int j=0; j<tilemap.width; ++j){
			if(isInFrame(i, j)){
				//render all layers on this grid
				for(std::vector<int**>::const_iterator grid=mapping.begin(); grid!=mapping.end(); ++grid){
					if((*grid)[i][j]>0){
						//draw in this grid with appropriate ID
						drawGrid(renderer, i, j, (*grid)[i][j], src, dest);
					}else;
				}
				//push currently-renderer grid into vector
				grids.push_back((struct Grid){i, j, dest});
			}else{
				//since this grid is not in camera, skip its rendering to improve performance
			}
		}
	}
}

/*test if this grid is visible from camera*/
bool Map::isInFrame(int row_num, int col_num) const{

	float cam_left = floor((float)tilemap.width * camera.left);
	float cam_right = floor((float)tilemap.width * camera.right);
	float cam_top = floor((float)tilemap.height * camera.top);
	float cam_bottom = floor((float)tilemap.height * camera.bottom);

	//if this grid tested to be visible(fully or partially), return true
	return((cam_left<=col_num && col_num<=cam_right) && (cam_top<=row_num && row_num<=cam_bottom));
}


void Map::drawGrid(SDL_Renderer* renderer, int i, int j, int tileID, SDL_Rect& src, SDL_Rect& dest) const{
	//get texture && update src rectangle
	SDL_Texture* tex = nullptr;
	for(std::vector<struct Tileset>::const_iterator tileset=tilesets.begin(); tileset!=tilesets.end(); ++tileset){
		if(tileset->begin_ID<=tileID && tileID<=tileset->end_ID){
			//found correct tileset
			tex = tileset->texture;
			int ID = tileID - tileset->begin_ID;
			int row = ID / tileset->column_count;
			int col = ID % tileset->column_count;
			src.x = ((float)col/(float)tileset->column_count) * tileset->width;
			src.y = ((float)row/(float)tileset->row_count) * tileset->height;
			src.w = tileset->width / tileset->column_count;
			src.h = tileset->height / tileset->row_count;
			break;
		}else{
			//keep iterating until correct tileset is found
		}
	}
	if(tex==nullptr){
		throw std::runtime_error("ERROR: could not fetch texture from tilesets");
	}else;

	//update dest rectangle
	float screen_pos_y = (i * tilemap.tile_height - camera.left) / (camera.right - camera.left); 
	float screen_pos_x = (j * tilemap.tile_width - camera.top) / (camera.bottom - camera.top);
	dest.x = ceil(GameSystem::integerX(screen_pos_x));
	dest.y = ceil(GameSystem::integerY(screen_pos_y));
	dest.w = ceil((GameSystem::integerX(tilemap.tile_width)+1) / (camera.right - camera.left));
	dest.h = ceil((GameSystem::integerY(tilemap.tile_height)+1) / (camera.bottom - camera.top));
	
	//render
	SDL_RenderCopy(renderer, tex, &src, &dest);
}

const std::vector<struct Map::Grid>& Map::getRenderedGrids() const{
	return grids;
}
