#ifndef COLOR_CPP
#define COLOR_CPP

#include <string>
#include <iostream>

using namespace std;

class Color {
	private:
		short red;
		short green;
		short blue;

	public:
		// ctor
		Color(short red = 0, short green = 0, short blue = 0) {
			this->red = red;
			this->green = green;
			this->blue = blue;
		}

		// cctor
		Color(const Color& color) {
			this->red = color.red;
			this->green = color.green;
			this->blue = color.blue;
		}

		Color& operator=(const Color& color) {
			this->red = color.red;
			this->green = color.green;
			this->blue = color.blue;
		}

		bool isEqual(const Color& color) {
			return this->red == color.red && this->blue == color.blue && this->green == color.green;
		}

		// getter
		short getRedValue() {
			return red;
		}

		short getGreenValue() {
			return green;
		}

		short getBlueValue() {
			return blue;
		}

		// setter
		void setRedValue(short red) {
			this->red = red;
		}

		void setGreenValue(short green) {
			this->green = green;
		}

		void setBlueValue(short blue) {
			this->blue = blue;
		}

		void setColor(string colorCode) {
			cout << "color: " << colorCode << endl;
			setRedValue((short) stoi(colorCode.substr(1, 2), NULL, 16));
			setGreenValue((short) stoi(colorCode.substr(3, 2), NULL, 16));
			setBlueValue((short) stoi(colorCode.substr(5, 2), NULL, 16));
			cout << "end color: " << colorCode << endl;
		}

		void overlay(Color color) {
			// this->red += color.red;
			// this->green += color.green;
			// this->blue += color.blue;
			// if (this->red < 0) this->red = 0;
			// if (this->green < 0) this->green = 0;
			// if (this->blue < 0) this->blue = 0;
			// if (this->red > 255) this->red = 255;
			// if (this->green > 255) this->green = 255;
			// if (this->blue > 255) this->blue = 255;
		}
};

#endif