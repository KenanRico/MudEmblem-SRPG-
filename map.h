#include "camera.h"

#include <vector>
#include <string>

#include <SDL2/SDL.h>


#ifndef MAP_H
#define MAP_H

class Map{
	public:
		struct Tilemap{
			int width; // #columns
			int height; // #rows
			float tile_width;
			float tile_height;
		};
		struct Tileset{
			std::string source;
			unsigned int begin_ID;
			unsigned int end_ID;
			int column_count;
			int row_count;
			int width;
			int height;
			SDL_Texture* texture;
		};
		struct Grid{
			int row_index;
			int column_index;
			SDL_Rect rectangle;
		};
		struct ObjectLayer{
			int** objects;
			int width;
			int height;
		};

	private:
		//render rectangles
		SDL_Rect src;
		SDL_Rect dest;
		//tiles related attributes/data
		struct Tilemap tilemap;
		std::vector<struct Tileset> tilesets; 
		std::vector<int**> mapping;
		//object layer
		struct ObjectLayer object_layer;
		//camera
		Camera camera;
		//individual grids
		mutable std::vector<struct Grid> grids;

	public:
		Map(SDL_Renderer*, const char*);
		~Map();
	private:
		Map() = delete;
		Map(const Map&) = delete;
		Map& operator=(const Map&) = delete;

	public:
		void update();
		void render() const;
		const std::vector<struct Grid>& getRenderedGrids() const;
		void getGridDimension(int*, int*) const;
		const struct Camera::Position& getCameraInfo() const;
		const struct ObjectLayer& getObjectLayer() const;
	private:
		bool isInFrame(int, int) const;
		void drawGrid(int, int, int, SDL_Rect&, SDL_Rect&, int) const;
};

#endif
