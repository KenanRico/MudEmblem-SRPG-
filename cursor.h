

#include <SDL2/SDL.h>


#ifndef CURSOR_H
#define CURSOR_H

class Cursor{
	private:
		struct CursorImage{
			SDL_Texture* texture;
			SDL_Rect rect;
		};
		struct CursorSize{
			int width;
			int height;
		};
	private:
		static struct CursorImage normal;
		static struct CursorSize size;
		static SDL_Rect dest_rect;
	private:
		Cursor() = delete;
		~Cursor() = delete;
		Cursor(const Cursor&) = delete;
		Cursor& operator=(const Cursor&) = delete;

	public:
		static void init(SDL_Renderer*);
		static void update();
		static void render(SDL_Renderer*);
		static void destroy();
};

#endif
