#ifndef CANVAS_CPP
#define CANVAS_CPP

#include "Color.cpp"

#include <iostream>

#define V_SIZE 500
#define H_SIZE 1360
#define GROUND_HEIGHT 10

class Canvas {
	private:
		Color colorMap[V_SIZE][H_SIZE];
		const Color grassColor = Color(100, 255, 0);
		const short cliffLength = 500;

	public:
		// ctor
		Canvas() {
			for (int y = 0; y < V_SIZE - GROUND_HEIGHT; y++) {
				for (int x = 0; x < H_SIZE; x++) {
					colorMap[y][x] = Color();
				}
			}
			for (int y = V_SIZE - GROUND_HEIGHT; y < V_SIZE; y++) {
				for (int x = 0; x < H_SIZE; x++) {
					if (x < cliffLength)
						colorMap[y][x] = Color();
					else
						colorMap[y][x] = Color(); //grassColor
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

		void clear() {
			for (int y = 0; y < V_SIZE - GROUND_HEIGHT; y++) {
				for (int x = 0; x < H_SIZE; x++) {
					colorMap[y][x] = Color();
				}
			}
			for (int y = V_SIZE - GROUND_HEIGHT; y < V_SIZE; y++) {
				for (int x = 0; x < H_SIZE; x++) {
					if (x < cliffLength)
						colorMap[y][x] = Color();
					else
						colorMap[y][x] = Color(); //grassColor
				}
			}
		}

		// getter
		Color getColor(int posY, int posX) {
			return colorMap[posY][posX];
		}

		// setter
		void setColor(int posY, int posX, Color color) {
			if (!(posY >= V_SIZE || posY < 0 || posX >= H_SIZE || posX < 0)) {
				colorMap[posY][posX] = color;
			}
		}
};

#endif
