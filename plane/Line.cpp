#include <math.h>
#include "Point.cpp"

class Line {
	private:
		Point p1;
		Point p2;

	public:
		// ctor
		Line(Point p1, Point p2) {
			this->p1 = p1;
			this->p2 = p2;
		}

		Line(const Point &p) {
			this->p1 = p1;
			this->p2 = p2;
		}

		// getter
		Point getP1() {
			return p1;
		}

		Point getP2() {
			return p2;
		}

		// setter
		void setP1(Point p) {
			this->p1 = p;
		}

		void setP2(Point p) {
			this->p2 = p;
		}

		void scale(Point point, float scaleX, float scaleY) {
			this->p1.scale(point, scaleX, scaleY);
			this->p2.scale(point, scaleX, scaleY);
		}

		void translate(short deltaX, short deltaY) {
			this->p1.translate(deltaX, deltaY);
			this->p2.translate(deltaX, deltaY);
		}

		// angle in degrees
		void rotate(Point point, float angle) {
			this->p1.rotate(point, angle);
			this->p2.rotate(point, angle);
		}
};
