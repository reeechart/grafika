#include <math.h>

class Point {
	private:
		short x;
		short y;
	
	public:
		// ctor
		Point(short x = 0, short y = 0) {
			this.x = x;
			this.y = y;
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
			this.x = x;
		}

		void setY(short y) {
			this.y = y;
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
		void rotate(Point point, float angle) {
			this.x = sin(angle) * point.x + cos(angle) * point.y;
			this.y = cos(angle) * point.x - sin(angle) * point.y;
		}
}