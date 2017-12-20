#include "map.h"
#include "xmlparse.h"
#include "gamesystem.h"
#include "eventhandler.h"
#include "game.h"
#include "camera.h"
#include "renderengine.h"

#include <vector>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/extensions/SDL_image.h>



Map::Map(SDL_Renderer* renderer, const char* tilemap_src): 
src((SDL_Rect){0,0,0,0}), dest((SDL_Rect){0,0,0,0}),
tilemap((struct Tilemap){0, 0, 0.0f, 0.0f}), 
object_layer((struct ObjectLayer){nullptr, 0, 0}),
camera(0.2f, 0.4f, 0.4f, 0.6f){
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
	camera.update();
}


void Map::render() const{
	//clear currently-renderer grids vector
	grids.clear();

	//init render rectangles
	SDL_Rect src = {0, 0, 0, 0};
	SDL_Rect dest = {0, 0, 0, 0};

	//render grids
	for(int i=0; i<tilemap.height; ++i){
		for(int j=0; j<tilemap.width; ++j){
			if(isInFrame(i,j)){
				//render all layers on this grid
				for(std::vector<int**>::const_iterator grid=mapping.begin(); grid!=mapping.end(); ++grid){
					if((*grid)[i][j]>0){
						//draw in this grid with appropriate ID
						drawGrid(i, j, (*grid)[i][j], src, dest, grid-mapping.begin());
					}else;
				}
				//push currently-rendered grid into vector
				grids.push_back((struct Grid){i, j, dest});
			}else{
				//since this grid is not in camera, skip its rendering to improve performance
			}
		}
	}
}

/*test if this grid is visible from camera*/
bool Map::isInFrame(int row_num, int col_num) const{

	const struct Camera::Position& cam = camera.getPosition();
	float cam_left = floor((float)tilemap.width * cam.left);
	float cam_right = floor((float)tilemap.width * cam.right);
	float cam_top = floor((float)tilemap.height * cam.top);
	float cam_bottom = floor((float)tilemap.height * cam.bottom);
	
	//std::cout<<cam_left<<" "<<cam_right<<" "<<cam_top<<" "<<cam_bottom<<"\n";

	//if this grid tested to be visible(fully or partially), return true
	return ((cam_left<=col_num && col_num<=cam_right) && (cam_top<=row_num && row_num<=cam_bottom));
}


void Map::drawGrid(int i, int j, int tileID, SDL_Rect& src, SDL_Rect& dest, int layer) const{
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
	const struct Camera::Position& cam = camera.getPosition();
	float screen_pos_y = (i * tilemap.tile_height - cam.top) / cam.width;
	float screen_pos_x = (j * tilemap.tile_width - cam.left) / cam.height;
	dest.x = ceil(GameSystem::physicalX(screen_pos_x));
	dest.y = ceil(GameSystem::physicalY(screen_pos_y));
	dest.w = ceil((GameSystem::physicalX(tilemap.tile_width)+1) / cam.width);
	dest.h = ceil((GameSystem::physicalY(tilemap.tile_height)+1) / cam.height);

	//render
	if(layer==0){
		//go into ground layer
		RenderEngine::addToRenderer(Game::Render::GROUND, tex, src, dest);
	}else{
		//go into object layer
		RenderEngine::addToRenderer(Game::Render::OBJECT, tex, src, dest);
	}
}

const std::vector<struct Map::Grid>& Map::getRenderedGrids() const{
	return grids;
}

void Map::getGridDimension(int* w, int* h) const{
	*w = ceil((GameSystem::physicalX(tilemap.tile_width)+1) / camera.getPosition().width);
	*h = ceil((GameSystem::physicalY(tilemap.tile_height)+1) / camera.getPosition().height);
}

const struct Camera::Position& Map::getCameraInfo() const{
	return camera.getPosition();
}

const struct Map::ObjectLayer& Map::getObjectLayer() const{
	return object_layer;
}
