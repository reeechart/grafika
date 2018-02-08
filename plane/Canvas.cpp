#include "Color.cpp"

#define V_SIZE 760
#define H_SIZE 1366

class Canvas {
	private:
		Color colorMap[V_SIZE][H_SIZE];

	public:
		// ctor
		Canvas() {
			for (int i = 0; i < V_SIZE; i++) {
				for (int j = 0; j < H_SIZE; j++) {
					colorMap[i][j] = Color();
				}
			}
		}

		// cctor
		Canvas(const Canvas& canvas) {
			for (int i = 0; i < V_SIZE; i++) {
				for (int j = 0; j < H_SIZE; j++) {
					this->colorMap[i][j] = canvas.colorMap[i][j];
				}
			}
		}

		// getter
		Color getColor(int posI, int posJ) {
			return colorMap[posI][posJ];
		}

		// setter
		void setColor(int posI, int posJ, Color color) {
			colorMap[posI][posJ] = color;
		}
};