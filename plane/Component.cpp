#ifndef COMPONENT_CPP
#define COMPONENT_CPP

#include "Plane.cpp"
#include "Canvas.cpp"

class Component {
	private:
		Plane plane;
		Point topLeftPosition;
		Point bottomRightPosition;
		Color color;
		short velocityX;
		short velocityY;
		short accelerationX;
		short accelerationY;

	public:
		// ctor
		Component(Plane plane, Point topLeftPosition = Point(), Point bottomRightPosition = Point(),
			Color color = Color()) {
			this->plane = plane;
			this->topLeftPosition = topLeftPosition;
			this->bottomRightPosition = bottomRightPosition;
			this->color = color;
		}

		// cctor
		Component(const Component& component) {
			this->plane = component.plane;
			this->topLeftPosition = component.topLeftPosition;
			this->bottomRightPosition = component.bottomRightPosition;
			this->color = component.color;
		}

		// getter
		Plane getPlane() {
			return plane;
		}

		Point getTopLeftPosition() {
			return topLeftPosition;
		}

		Point getBottomRightPosition() {
			return bottomRightPosition;
		}

		Color getColor() {
			return color;
		}

		// setter
		void setPlane(Plane plane) {
			this->plane = plane;
		}

		void setTopLeftPosition(Point position) {
			this->topLeftPosition = position;
		}

		void setBottomRightPosition(Point position) {
			this->bottomRightPosition = position;
		}

		void setColor(Color color) {
			this->color = color;
		}

		void scale(Point point, float scaleX, float scaleY) {
			plane.scale(point, scaleX, scaleY);
		}

		void translate(short deltaX, short deltaY) {
			plane.translate(deltaX, deltaY);
		}

		// angle in degrees
		void rotate(Point point, float angle) {
			plane.rotate(point, angle);
		}
};

#endif