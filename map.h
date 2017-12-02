
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


	private:
		Tilemap tilemap;
		std::vector<Tileset> tilesets; 
		std::vector<int**> mapping;
		Camera camera;

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
	private:
		void updateCamera();
		bool isInFrame(int, int) const;
		void drawGrid(SDL_Renderer*, int, int, int, SDL_Rect&, SDL_Rect&) const;

	//friend class RenderEngine::renderMap(const Map&);
};

#endif
