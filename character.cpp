#include "character.h"
#include "map.h"
#include "camera.h"
#include "gamesystem.h"
#include "renderengine.h"
#include "game.h"
#include "eventhandler.h"
#include "gridbox.h"

#include <math.h>

#include <SDL2/SDL.h>


Character::Character(Map const * map, SDL_Renderer* renderer, const char* sprite_source, float x, float y):
Sprite(map, renderer, sprite_source, x, y){
	//do nothing
}

Character::~Character(){
	//calls Sprite::~Sprite implicitly
}

void Character::update(Map const * map, const GridBox& grid){
	//update position
	updatePosition(map->getObjectLayer(), grid);
	//update dest rect
	const struct Camera::Position& camera = map->getCameraInfo();
	dest.x = GameSystem::physicalX( (position.x - camera.left) / camera.width );
	dest.y = GameSystem::physicalY( (position.y - camera.top) / camera.height );
	map->getGridDimension(&dest.w, &dest.h);
}

void Character::render() const{
	RenderEngine::addToRenderer(Game::Render::OBJECT, texture, src, dest);
}

void Character::updatePosition(const struct Map::ObjectLayer& object_layer, const GridBox& grid){
	//get src grid indices
	int src_row = floor(object_layer.height * position.y);
	int src_column = floor(object_layer.width * position.x);

	//get dest grid indices
	int dest_row = -1;
	int dest_column = -1;
	grid.getGridIndex(&dest_row, &dest_column);

	//apply path finding algorithm to determine what is the next grid sprite should land on
	pathFind(src_row, src_column, dest_row, dest_column, (int const **)object_layer.objects);

	/*
	//calculate normalized displacements
	float dis_x = GameSystem::normalizedX(EventHandler::getThisCyclePosition(Game::Event::X)) - GameSystem::normalizedX(dest.x);
	float dis_y = GameSystem::normalizedY(EventHandler::getThisCyclePosition(Game::Event::Y)) - GameSystem::normalizedY(dest.y);
	float distance = sqrtf(powf(dis_x, 2) + powf(dis_y, 2));
	if(distance!=0.0f){
		//update position
		float norm_diff_x = dis_x / distance;
		float norm_diff_y = dis_y / distance;
		position.x += norm_diff_x * 0.001f;
		position.y += norm_diff_y * 0.001f;
	}else{
		//Don't update position
	}
	//std::cout<<dis_x<<" "<<dis_y<<"\n";
	*/
}

void Character::pathFind(int src_row, int src_column, int dest_row, int dest_column, int const ** objects){
	//update movement
}
