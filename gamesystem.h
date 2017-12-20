#include <SDL2/SDL.h>

#ifndef GAME_SYSTEM_H
#define GAME_SYSTEM_H

class GameSystem{
	private:
		static int state;
		static int start_time;
		static int window_width;
		static int window_height;
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
		static float normalizedX(float);
		static float normalizedY(float);
		static int physicalX(float);
		static int physicalY(float);
};

#endif
