#ifndef CANVAS_CPP
#define CANVAS_CPP

#include "Color.cpp"

#define V_SIZE 760
#define H_SIZE 1366

class Canvas {
	private:
		Color colorMap[V_SIZE][H_SIZE];

	public:
		// ctor
		Canvas() {
			for (int y = 0; y < V_SIZE; y++) {
				for (int x = 0; x < H_SIZE; x++) {
					colorMap[y][x] = Color();
				}
			}
		}

		// cctor
		Canvas(const Canvas& canvas) {
			for (int y = 0; y < V_SIZE; y++) {
				for (int x = 0; x < H_SIZE; x++) {
					this->colorMap[y][x] = canvas.colorMap[y][x];
				}
			}
		}

		// getter
		Color getColor(int posY, int posX) {
			return colorMap[posY][posX];
		}

		// setter
		void setColor(int posY, int posX, Color color) {
			if (!(posY >= V_SIZE || posY < 0 || posX >= H_SIZE || posX < 0))
				colorMap[posY][posX] = color;
		}
};

#endif