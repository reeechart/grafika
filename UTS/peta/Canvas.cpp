#ifndef CANVAS_CPP
#define CANVAS_CPP

#include "Color.cpp"
#include "Point.cpp"
#include "ClippingPlane.cpp"

#include <iostream>

class Canvas {
	private:
		Color **colorMap;
		Point topLeftPosition;
		Point bottomRightPosition;

	public:
		// ctor
		Canvas() {
			this->topLeftPosition = Point(0, 0);
			this->bottomRightPosition = Point(1360, 760);
			colorMap = new Color*[getHeight()];
			for (int i = 0; i < getHeight(); ++i) {
				colorMap[i] = new Color[getWidth()];
				for (int j = 0; j < getWidth(); ++j) {
					colorMap[i][j] = Color();
				}
			}
		}

		Canvas(Point topLeftPosition, Point bottomRightPosition, Color color = Color()) {
			this->topLeftPosition = topLeftPosition;
			this->bottomRightPosition = bottomRightPosition;
			colorMap = new Color*[getHeight()];
			for (int i = 0; i < getHeight(); ++i) {
				colorMap[i] = new Color[getWidth()];
				for (int j = 0; j < getWidth(); ++j) {
					colorMap[i][j] = color;
				}
			}
		}

		// cctor
		Canvas(const Canvas& canvas) {
			this->topLeftPosition = canvas.topLeftPosition;
			this->bottomRightPosition = canvas.bottomRightPosition;
			this->colorMap = new Color*[getHeight()];
			for (int i = 0; i < getHeight(); ++i) {
				colorMap[i] = new Color[getWidth()];
				for (int j = 0; j < getWidth(); ++j) {
					this->colorMap[i][j] = canvas.colorMap[i][j];
				}
			}
		}

		Canvas& operator=(const Canvas& canvas) {
			this->topLeftPosition = canvas.topLeftPosition;
			this->bottomRightPosition = canvas.bottomRightPosition;
			this->colorMap = new Color*[getHeight()];
			for (int i = 0; i < getHeight(); ++i) {
				colorMap[i] = new Color[getWidth()];
				for (int j = 0; j < getWidth(); ++j) {
					this->colorMap[i][j] = canvas.colorMap[i][j];
				}
			}
		}
		
		// dtor
		~Canvas() {
			for (int i = 0; i < getHeight(); ++i) {
				delete [] colorMap[i];
			}
			delete [] colorMap;
		}

		void clear() {
			for (int y = 0; y < getHeight(); y++) {
				for (int x = 0; x < getWidth(); x++) {
					colorMap[y][x] = Color();
				}
			}
		}

		void overlayClippingPlane(ClippingPlane& clippingPlane) {
			for (int y = clippingPlane.getTopLine(); y <= clippingPlane.getBottomLine(); y++) {
				for (int x = clippingPlane.getLeftLine(); x <= clippingPlane.getRightLine(); x++) {
					if (y == clippingPlane.getTopLine() || y == clippingPlane.getBottomLine() ||
						x == clippingPlane.getLeftLine() || x == clippingPlane.getRightLine())
					colorMap[y][x] = clippingPlane.getOverlayColor();
				}
			}
		}

		// getter
		Color getColor(int posY, int posX) {
			if (!(posY >= getHeight() || posY < 0 || posX >= getWidth() || posX < 0)) {
				return colorMap[posY][posX];
			} else {
				return Color();
			}
		}

		Color getOffsetColor(int posY, int posX) {
			return getColor(posY - getTop(), posX - getLeft());
		}

		short getHeight() {
			return bottomRightPosition.getY() - topLeftPosition.getY() + 1;
		}

		short getWidth() {
			return bottomRightPosition.getX() - topLeftPosition.getX() + 1;
		}

		short getTop() {
			return topLeftPosition.getY();
		}

		short getBottom() {
			return bottomRightPosition.getY();
		}

		short getLeft() {
			return topLeftPosition.getX();
		}

		short getRight() {
			return bottomRightPosition.getX();
		}

		// setter
		void setColor(int posY, int posX, Color color) {
			if (!(posY >= getHeight() || posY < 0 || posX >= getWidth() || posX < 0)) {
				colorMap[posY][posX] = color;
			}
		}

		void setOffsetColor(int posY, int posX, Color color) {
			setColor(posY - getTop(), posX - getLeft(), color);
		}

		void colorBorder(Color color, int top, int bottom, int left, int right) {
			for (int i = left; i <= right; i++) {
				setColor(top, i, color);
				setColor(bottom, i, color);
			}
			for (int j = top; j <= bottom; j++) {
				setColor(j, left, color);
				setColor(j, right, color);
			}
		}

		void fill(Color color, int top, int bottom, int left, int right) {
			for (int j = top + 1; j < bottom; j++) {
				for (int i = left + 1; i < right; i++) {
					setColor(j, i, color);
				}
			}
		}
};

#endif
