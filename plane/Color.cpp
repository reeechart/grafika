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
};