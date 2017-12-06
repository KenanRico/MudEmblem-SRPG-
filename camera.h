


#ifndef CAMERA_H
#define CAMERA_H

class Camera{
	public:
		struct Position{
			float left;
			float right;
			float top;
			float bottom;
			float width;
			float height;
		};
	private:
		struct Action{
			bool drag;
			bool zoom;
			bool slide;
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
		void updateActions(float, float);
		void handleActions(struct Position&, float, float, float, float);
		void updateView(const struct Position&);
};

#endif
