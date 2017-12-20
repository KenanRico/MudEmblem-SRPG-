#include "map.h"

#include <SDL2/SDL.h>


#ifndef GRID_BOX_H
#define GRID_BOX_H

class GridBox{
	private:
		bool selecting;
		SDL_Rect src;
		SDL_Rect box;
		SDL_Texture* texture;
		int row_index;
		int column_index;

	public:
		GridBox(SDL_Renderer*);
		~GridBox();
	private:
		GridBox() = delete;
		GridBox(const GridBox&) = delete;
		GridBox& operator=(const GridBox&) = delete;
	
	public:
		void update(const std::vector<Map::Grid>&);
		void render() const;
		void getGridIndex(int*, int*) const;
};

#endif
