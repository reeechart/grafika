#ifndef POINT_CPP
#define POINT_CPP

#include <math.h>

class Point {
	private:
		short x;
		short y;

	public:
		// ctor
		Point(short x = 0, short y = 0) {
			this->x = x;
			this->y = y;
		}

		// cctor
		Point(const Point& point) {
			this->x = point.x;
			this->y = point.y;
		}

		// getter
		short getX() {
			return x;
		}

		short getY() {
			return y;
		}

		// setter
		void setX(short x) {
			this->x = x;
		}

		void setY(short y) {
			this->y = y;
		}

		void scale(Point point, float scaleX, float scaleY) {
			x = (x - point.x) * scaleX + point.x;
			y = (y - point.y) * scaleY + point.y;
		}

		void translate(short deltaX, short deltaY) {
			x += deltaX;
			y += deltaY;
		}

		// angle in degrees
		void rotate(Point point, double angle) {
			double angleInRadian = angle * M_PI / 180;
			double newX = point.x + cos(angleInRadian) *
				(this->x - point.x) - sin(angleInRadian) * (this->y - point.y);
			double newY = point.y + sin(angleInRadian) *
				(this->x - point.x) + cos(angleInRadian) * (this->y - point.y);
			this->x = (short)round(newX);
			this->y = (short)round(newY);
		}

		bool isEqual(Point point) {
			return x == point.x && y == point.y;
		}

		int distance(Point point) {
			return (int)(point.x - this->x) * (int)(point.x - this->x) + 
				(int)(point.y - this->y) * (int)(point.y - this->y);
		}

};

#endif