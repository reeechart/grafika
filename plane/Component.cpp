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
		void updateEnvelope() {
			short minX = H_SIZE, maxX = 0, minY = V_SIZE, maxY = 0;
			for (auto& line : plane.getLines()) {
				if (line.getP1().getX() < minX) minX =line.getP1().getX();
				if (line.getP2().getX() < minX) minX =line.getP2().getX();
				if (line.getP1().getX() > maxX) maxX =line.getP1().getX();
				if (line.getP2().getX() > maxX) maxX =line.getP2().getX();
				if (line.getP1().getY() < minY) minY =line.getP1().getY();
				if (line.getP2().getY() < minY) minY =line.getP2().getY();
				if (line.getP1().getY() > maxY) maxY =line.getP1().getY();
				if (line.getP2().getY() > maxY) maxY =line.getP2().getY();
			}
			bottomRightPosition = Point(maxX, maxY);
			topLeftPosition = Point(minX, minY);
		}

	public:
		// ctor
		Component() {

		}

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
			updateEnvelope();
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
			updateEnvelope();
		}

		void translate(short deltaX, short deltaY) {
			plane.translate(deltaX, deltaY);
			updateEnvelope();
		}

		// angle in degrees
		void rotate(Point point, float angle) {
			plane.rotate(point, angle);
			updateEnvelope();
		}

		void rotateAgainstCenter(float angle) {
			short middleX, middleY;
			middleX = (bottomRightPosition.getX() + topLeftPosition.getX()) / 2;
			middleY = (bottomRightPosition.getY() + topLeftPosition.getY()) / 2;
			Point middlePoint = Point(middleX, middleY);
			rotate(middlePoint, angle);
		}
};

#endif
