#include "camera.h"
#include "eventhandler.h"
#include "gamesystem.h"
#include "game.h"



Camera::Camera(float l, float r, float t, float b): 
current((struct Position){l, r, t, b}),
origin(current),
action((struct Action){false, false}){
	//do nothing
}

Camera::~Camera(){
	//do nothing
}


void Camera::update(){
	struct Position new_camera = current;
	updateActions();
	handleActions(new_camera);
	updateView(new_camera);
}

void Camera::updateActions(){
	action.drag = EventHandler::getPress(Game::Event::MIDDLE);
	action.zoom = (EventHandler::getScroll(Game::Event::Y)!=0.0f);
}

void Camera::handleActions(struct Position& new_camera){
	if(action.drag){
		float this_cycle_cursor_x = EventHandler::getThisCyclePosition(Game::Event::X);
		float this_cycle_cursor_y = EventHandler::getThisCyclePosition(Game::Event::Y);
		float last_cycle_cursor_x = EventHandler::getLastCyclePosition(Game::Event::X);
		float last_cycle_cursor_y = EventHandler::getLastCyclePosition(Game::Event::Y);
		float camera_offset_x = GameSystem::floatingX(this_cycle_cursor_x - last_cycle_cursor_x) * (current.right-current.left);
		float camera_offset_y = GameSystem::floatingX(this_cycle_cursor_y - last_cycle_cursor_y) * (current.bottom-current.top);
		new_camera.left -= camera_offset_x;
		new_camera.right -= camera_offset_x;
		new_camera.top -= camera_offset_y;
		new_camera.bottom -= camera_offset_y;
	}else if(action.zoom){
		new_camera.left = current.left + EventHandler::getScroll(Game::Event::Y) * 0.01f;
		new_camera.right = current.right - EventHandler::getScroll(Game::Event::Y) * 0.01f;
		new_camera.top = current.top + EventHandler::getScroll(Game::Event::Y) * 0.01f;
		new_camera.bottom = current.bottom - EventHandler::getScroll(Game::Event::Y) * 0.01f;
	}else{
		//no action
	}
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
