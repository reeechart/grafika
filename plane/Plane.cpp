#include <vector>
#include "Line.cpp"

using namespace std;

class Plane {
	private:
		vector<Line> lines;

	public:
		// ctor
		Plane() {
			lines = vector<Line>();
		}

		// cctor
		Plane(const Plane& plane) {
			this->lines = vector<Line>();
			this->lines = plane.lines;
		}

		// getter
		vector<Line> getLines() {
			return lines;
		}

		void addLine(Line line) {
			lines.push_back(line);
		}

		void scale(Point point, float scaleX, float scaleY) {
			for (auto& iterator : lines) {
				iterator.scale(point, scaleX, scaleY);
			}
		}

		void translate(short deltaX, short deltaY) {
			for (auto& iterator : lines) {
				iterator.translate(deltaX, deltaY);
			}
		}

		// angle in degrees
		void rotate(Point point, float angle) {
			for (auto& iterator : lines) {
				iterator.rotate(point, angle);
			}
		}
};