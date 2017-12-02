#include <SDL2/SDL.h>

#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

class GameSystem{
	private:
		static int state;
		static int start_time;
		static int window_width;
		static int window_height;
		static int checker_board_size;
	private:
		GameSystem() = delete;
		GameSystem(const GameSystem&) = delete;
		GameSystem& operator=(const GameSystem&) = delete;
		~GameSystem() = delete;
	public:
		static void init();
		static void update(SDL_Window*);
		static bool isState(int);
		static int windowW();
		static int windowH();
		static float floatingX(float);
		static float floatingY(float);
		static int integerX(float);
		static int integerY(float);
		static int checkerBoardSize();
};

#endif
