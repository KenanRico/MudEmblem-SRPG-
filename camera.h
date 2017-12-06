


#ifndef CAMERA_H
#define CAMERA_H

class Camera{
	public:
		struct Position{
			float left;
			float right;
			float top;
			float bottom;
		};
	private:
		struct Action{
			bool drag;
			bool zoom;
		};

	private:
		struct Position current;
		struct Position origin;
		struct Action action;
		
	public:
		Camera(float, float, float, float);
		~Camera();
	private:
		Camera() = delete;
		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;

	public:
		void update();
		const struct Position& getPosition() const;
	private:
		void updateActions();
		void handleActions(struct Position&);
		void updateView(const struct Position&);
};

#endif
