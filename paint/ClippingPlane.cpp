#ifndef CLIPPING_PLANE_CPP
#define CLIPPING_PLANE_CPP

#include "Point.cpp"
#include "Color.cpp"

using namespace std;

class ClippingPlane {
	private:
		short topLine, leftLine, rightLine, bottomLine;
		Color overlayColor;

	public:
		ClippingPlane(short topLine, short bottomLine, short leftLine, short rightLine) {
			this->topLine = topLine;
			this->bottomLine = bottomLine;
			this->leftLine = leftLine;
			this->rightLine = rightLine;
		}

		ClippingPlane(const ClippingPlane& clippingPlane) {
			this->topLine = clippingPlane.topLine;
			this->bottomLine = clippingPlane.bottomLine;
			this->leftLine = clippingPlane.leftLine;
			this->rightLine = clippingPlane.rightLine;
		}

		short getCode(Point point) {
			short code = 0;
			if (point.getY() <= topLine) {
				code += 8;
			}
			if (point.getY() >= bottomLine) {
				code += 4;
			}
			if (point.getX() <= leftLine) {
				code += 2;
			}
			if (point.getX() >= rightLine) {
				code += 1;
			}
			return code;
		}

		short getTopLine() {
			return topLine;
		}

		short getBottomLine() {
			return bottomLine;
		}

		short getLeftLine() {
			return leftLine;
		}

		short getRightLine() {
			return rightLine;
		}

		void setOverlayColor(Color color) {
			this->overlayColor = color;
		}

		Color getOverlayColor() {
			return overlayColor;
		}
};


#endif