#include "Plane.cpp"

class Component {
	protected:
		Plane plane;
		Point topLeftPosition;
		Point bottomRightPosition;
		short redColor;
		short greenColor;
		short blueColor;

	public:
		// ctor
		Component(Plane plane, Point topLeftPosition = Point(), Point bottomRightPosition = Point(),
			short redColor = 0, short greenColor = 0, short blueColor = 0) {
			this->plane = plane;
			this->topLeftPosition = topLeftPosition;
			this->bottomRightPosition = bottomRightPosition;
			this->redColor = redColor;
			this->greenColor = greenColor;
			this->blueColor = blueColor;
		}

		// cctor
		Component(const Component& component) {
			this->plane = component.plane;
			this->topLeftPosition = topLeftPosition;
			this->bottomRightPosition = bottomRightPosition;
			this->redColor = component.redColor;
			this->greenColor = component.greenColor;
			this->blueColor = component.blueColor;
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

		short getRedColor() {
			return redColor;
		}

		short getGreenColor() {
			return greenColor;
		}

		short getBlueColor() {
			return blueColor;
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

		void setRedColor(short redColor) {
			this->redColor = redColor;
		}

		void setGreenColor(short greenColor) {
			this->greenColor = greenColor;
		}

		void setBlueColor(short blueColor) {
			this->blueColor = blueColor;
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