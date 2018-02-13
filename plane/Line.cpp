#ifndef LINE_CPP
#define LINE_CPP

#include <math.h>
#include <stdlib.h>
#include <vector>
#include "Point.cpp"

#define NOT_EXIST -1
#define EXIST_NORMAL 0
#define EXIST_ABOVE 1
#define EXIST_BELOW 2
#define EXIST_LEFT 1
#define EXIST_RIGHT 2

using namespace std;

class Line {
	private:
		Point p1;
		Point p2;

	public:
		// ctor
		Line(Point p1 = Point(), Point p2 = Point()) {
			this->p1 = p1;
			this->p2 = p2;
		}

		Line(const Line& line) {
			this->p1 = line.p1;
			this->p2 = line.p2;
		}

		// getter
		Point getP1() {
			return p1;
		}

		Point getP2() {
			return p2;
		}

		vector<Point> getAllPoints() {
			vector<Point> pointsVector;
			Point pointIterator = p1;
			int dx = abs(p2.getX() - p1.getX()), sx = p1.getX() < p2.getX() ? 1 : -1;
			int dy = abs(p2.getY() - p1.getY()), sy = p1.getY() < p2.getY() ? 1 : -1;
			int error = (dx > dy ? dx : -dy) / 2;
			int previousError;

			for (;;) {
				pointsVector.push_back(pointIterator);
				if (pointIterator.getX() == p2.getX() && pointIterator.getY() == p2.getY()) break;
				previousError = error;
				if (previousError > -dx) {
					error -= dy;
					pointIterator.translate(sx, 0);
				}
				if (previousError < dy) {
					error += dx;
					pointIterator.translate(0, sy);
				}
			}
			return pointsVector;
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

		bool isHorizontal() {
			return p1.getY() == p2.getY();
		}

		bool isVertical() {
			return p1.getX() == p2.getX();
		}

		// pair<short, short> getIntersectionStatusX(short x) {
		// 	if (isVertical())
		// 		return make_pair(NOT_EXIST, NOT_EXIST);
		// 	else {
		// 		if ((this->p1.getX() - x) * (this->p2.getX() - x) > 0)
		// 			return make_pair(NOT_EXIST, NOT_EXIST);
		// 		else {
		// 			float intersectionY;
		// 			short statusCode = EXIST_NORMAL;
		// 			intersectionY = ((p2.getY() - p1.getY()) * (x - p1.getX())) / (p2.getX() - p1.getX()) + p1.getY();
		// 			if ((short) round(intersectionY) == p1.getY()) {
		// 				if (x > p2.getX()) statusCode = EXIST_LEFT;
		// 				else statusCode = EXIST_RIGHT;
		// 			} else if ((short) round(intersectionY) == p2.getY()) {
		// 				if (x > p1.getX()) statusCode = EXIST_LEFT;
		// 				else statusCode = EXIST_RIGHT;
		// 			}
		// 			return make_pair((short) round(intersectionY), (short)statusCode);
		// 		}
		// 	}
		// }

		pair<short, short> getIntersectionStatusY(short y) {
			if (isHorizontal())
				return make_pair(NOT_EXIST, NOT_EXIST);
			else {
				if ((this->p1.getY() - y) * (this->p2.getY() - y) > 0)
					return make_pair(NOT_EXIST, NOT_EXIST);
				else {
					float intersectionX;
					short statusCode = EXIST_NORMAL;
					intersectionX = ((p2.getX() - p1.getX()) * (y - p1.getY())) / (p2.getY() - p1.getY()) + p1.getX();
					if ((short) round(intersectionX) == p1.getX()) {
						if (y > p2.getY()) statusCode = EXIST_ABOVE;
						else statusCode = EXIST_BELOW;
					} else if ((short) round(intersectionX) == p2.getX()) {
						if (y > p1.getY()) statusCode = EXIST_ABOVE;
						else statusCode = EXIST_BELOW;
					}
					return make_pair((short) round(intersectionX), (short)statusCode);
				}
			}
		}

		short getIntersectionPointX(short y) {
			if ((p1.getY() - y) * (p2.getY() - y) > 0) {
				return -1;
			} else {
				return ((p2.getX() - p1.getX()) * (y - p1.getY())) / (p2.getY() - p1.getY()) + p1.getX();
			}
		}

		short getIntersectionPointY(short x) {
			if ((p2.getX() - x) * (p2.getX() - x) > 0) {
				return -1;
			} else {
				return ((p2.getY() - p1.getY()) * (x - p1.getX())) / (p2.getX() - p1.getX()) + p1.getY();
			}
 		}

};

#endif
