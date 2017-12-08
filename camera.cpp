#include "camera.h"
#include "eventhandler.h"
#include "gamesystem.h"
#include "game.h"

#include <iostream>


Camera::Camera(float l, float r, float t, float b): 
current((struct Position){l, r, t, b, r-l, b-t}),
origin(current),
action((struct Action){false, false, false}){
	//do nothing
}

Camera::~Camera(){
	//do nothing
}


void Camera::update(){
	struct Position new_camera = current;
	float this_cycle_cursor_x = EventHandler::getThisCyclePosition(Game::Event::X);
	float this_cycle_cursor_y = EventHandler::getThisCyclePosition(Game::Event::Y);
	float last_cycle_cursor_x = EventHandler::getLastCyclePosition(Game::Event::X);
	float last_cycle_cursor_y = EventHandler::getLastCyclePosition(Game::Event::Y);
	//update procedures
	updateActions(this_cycle_cursor_x, this_cycle_cursor_y);
	handleActions(new_camera, this_cycle_cursor_x, this_cycle_cursor_y, last_cycle_cursor_x, last_cycle_cursor_y);
	updateView(new_camera);
}

void Camera::updateActions(float cursor_x, float cursor_y){
	action.drag = EventHandler::getPress(Game::Event::MIDDLE);
	action.zoom = (EventHandler::getScroll(Game::Event::Y)!=0.0f);
	action.slide = (cursor_x<=5.0f || (int)cursor_x>=GameSystem::windowW()-5 || cursor_y<=5.0f || (int)cursor_y>=GameSystem::windowH()-5);
}

void Camera::handleActions(struct Position& new_camera, float this_cycle_cursor_x, float this_cycle_cursor_y, float last_cycle_cursor_x, float last_cycle_cursor_y){
	if(action.drag){
		float camera_offset_x = GameSystem::floatingX(this_cycle_cursor_x-last_cycle_cursor_x) * current.width;
		float camera_offset_y = GameSystem::floatingY(this_cycle_cursor_y-last_cycle_cursor_y) * current.height;
		new_camera.left -= camera_offset_x;
		new_camera.right -= camera_offset_x;
		new_camera.top -= camera_offset_y;
		new_camera.bottom -= camera_offset_y;
	}else if(action.zoom){
		new_camera.left = current.left + EventHandler::getScroll(Game::Event::Y) * 0.01f;
		new_camera.right = current.right - EventHandler::getScroll(Game::Event::Y) * 0.01f;
		new_camera.top = current.top + EventHandler::getScroll(Game::Event::Y) * 0.01f;
		new_camera.bottom = current.bottom - EventHandler::getScroll(Game::Event::Y) * 0.01f;
	}else if(action.slide){
		if(this_cycle_cursor_x<=5.0f){
			new_camera.left -= 0.01f * current.width;
			new_camera.right -= 0.01f * current.width;
		}else if((int)this_cycle_cursor_x>=GameSystem::windowW()-5){
			new_camera.left += 0.01f * current.width;
			new_camera.right += 0.01f * current.width;
		}else;
		if(this_cycle_cursor_y<=5.0f){
			new_camera.top -= 0.01f * current.height;
			new_camera.bottom -= 0.01f * current.height;
		}else if((int)this_cycle_cursor_y>=GameSystem::windowH()-5){
			new_camera.top += 0.01f * current.height;
			new_camera.bottom += 0.01f * current.height;
		}else;
	}else{
		//no action
	}
	new_camera.width = new_camera.right - new_camera.left;
	new_camera.height = new_camera.bottom - new_camera.top;
}

void Camera::updateView(const struct Position& new_camera){
	if(0.0f<=new_camera.left
	&& new_camera.left<new_camera.right
	&& new_camera.right<=1.0f
	&& 0.0f<=new_camera.top
	&& new_camera.top<new_camera.bottom
	&& new_camera.bottom<=1.0f){
		current = new_camera;
	}else{
		//do not update camera
	}
}



const struct Camera::Position& Camera::getPosition() const{
	return current;
}
