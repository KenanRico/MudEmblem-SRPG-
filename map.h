
#include <vector>
#include <string>

#include <SDL2/SDL.h>


#ifndef MAP_H
#define MAP_H

class Map{
	public:
		struct Tilemap{
			int width;
			int height;
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
		struct Camera{
			float left;
			float right;
			float top;
			float bottom;
		};
		
		struct Grid{
			int row_index;
			int column_index;
			SDL_Rect rectangle;
		};

	private:
		Tilemap tilemap;
		std::vector<Tileset> tilesets; 
		std::vector<int**> mapping;
		Camera camera;
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
		void render(SDL_Renderer*) const;
		const std::vector<struct Grid>& getRenderedGrids() const;
	private:
		void updateCamera();
		bool isInFrame(int, int) const;
		void drawGrid(SDL_Renderer*, int, int, int, SDL_Rect&, SDL_Rect&) const;
};

#endif
