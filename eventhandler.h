
#include <SDL2/SDL.h>


#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

class EventHandler{
	private:
		static SDL_Event event;
		static struct Mouse{
			bool click[3];
			bool press[3];
			int scroll[2];
			float click_position[3][2];
			float current_position[2];
		} mouse;
		static bool close;
	private:
		EventHandler() = delete;
		EventHandler(const EventHandler&) = delete;
		EventHandler& operator=(const EventHandler&) = delete;
		~EventHandler() = delete;
	public:
		static void init();
		static void update();
		static bool getClick(int);
		static bool getPress(int);
		static int getScroll(int);
		static float getClickPosition(int, int);
		static float getCurrentPosition(int);
		static bool  closeWindow();
};

#endif
