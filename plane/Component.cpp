#ifndef COMPONENT_CPP
#define COMPONENT_CPP

#include "Plane.cpp"
#include "Canvas.cpp"
#include "ClippingPlane.cpp"
#include <algorithm>

class Component {
	private:
		Plane plane;
		Point topLeftPosition;
		Point bottomRightPosition;
		Color color;
		float velocityX;
		float velocityY;
		float accelerationX;
		float accelerationY;
		void updateEnvelope() {
			short minX = H_SIZE, maxX = 0, minY = V_SIZE, maxY = 0;
			for (auto& line : plane.getLines()) {
				if (line.getP1().getX() < minX) minX =line.getP1().getX();
				if (line.getP2().getX() < minX) minX =line.getP2().getX();
				if (line.getP1().getX() > maxX) maxX =line.getP1().getX();
				if (line.getP2().getX() > maxX) maxX =line.getP2().getX();
				if (line.getP1().getY() < minY) minY =line.getP1().getY();
				if (line.getP2().getY() < minY) minY =line.getP2().getY();
				if (line.getP1().getY() > maxY) maxY =line.getP1().getY();
				if (line.getP2().getY() > maxY) maxY =line.getP2().getY();
			}
			bottomRightPosition = Point(maxX, maxY);
			topLeftPosition = Point(minX, minY);
		}
		float springConstant;

		bool doesEnvelop(Point point) {
			vector<pair<short, short>> intersections;
            for (auto& line : plane.getLines()) {
                pair<short, short> intersection = line.getIntersectionStatusY(point.getY());
                if (intersection.second == EXIST_NORMAL) {
                    intersections.push_back(intersection);
                } else if (intersection.second == EXIST_ABOVE) {
                    bool oppositeFound = find(intersections.begin(), intersections.end(),
                        make_pair(intersection.first, (short)EXIST_BELOW)) != intersections.end();
                    bool unilateralFound = find(intersections.begin(), intersections.end(),
                        intersection) != intersections.end();
                    if (unilateralFound) {
                        intersections.push_back(intersection);
                    } else {
                        if (!oppositeFound) {
                            intersections.push_back(intersection);
                        }
                    }
                } else if (intersection.second == EXIST_BELOW) {
                    bool oppositeFound = find(intersections.begin(), intersections.end(),
                        make_pair(intersection.first, (short)EXIST_ABOVE)) != intersections.end();
                    bool unilateralFound = find(intersections.begin(), intersections.end(),
                        intersection) != intersections.end();
                    if (unilateralFound) {
                        intersections.push_back(intersection);
                    } else {
                        if (!oppositeFound) {
                            intersections.push_back(intersection);
                        }
                    }
                }
            }
            sort(intersections.begin(), intersections.end());
            if (intersections.size() >= 2) {
            	for (int i = 0; i < intersections.size() - 1; i += 2) {
            		if (point.getX() >= intersections[i].first && point.getX() <= intersections[i + 1].first) {
            			return true;
            		} 
            	}
            }
            return false;
		}

	public:
		// ctor
		Component() {

		}

		Component(Plane plane, Point topLeftPosition = Point(), Point bottomRightPosition = Point(),
			Color color = Color()) {
			this->plane = plane;
			this->topLeftPosition = topLeftPosition;
			this->bottomRightPosition = bottomRightPosition;
			this->color = color;
			this->velocityX = 0;
			this->velocityY = 0;
			this->accelerationX = 0;
			this->accelerationY = 0;
			this->springConstant = 0;
		}

		// cctor
		Component(const Component& component) {
			this->plane = component.plane;
			this->topLeftPosition = component.topLeftPosition;
			this->bottomRightPosition = component.bottomRightPosition;
			this->color = component.color;
			this->velocityX = component.velocityX;
			this->velocityY = component.velocityY;
			this->accelerationX = component.accelerationX;
			this->accelerationY = component.accelerationY;
			this->springConstant = component.springConstant;
		}

		// getter
		Plane getPlane() {
			return plane;
		}

		Point getTopLeftPosition() {
			return topLeftPosition;
		}

		Point getBottomRightPosition() {
			return bottomRightPosition;
		}

		Color getColor() {
			return color;
		}

		// setter
		void setPlane(Plane plane) {
			this->plane = plane;
			updateEnvelope();
		}

		void setTopLeftPosition(Point position) {
			this->topLeftPosition = position;
		}

		void setBottomRightPosition(Point position) {
			this->bottomRightPosition = position;
		}

		void setColor(Color color) {
			this->color = color;
		}

		void setVelocity(float velocityX, float velocityY) {
			this->velocityX = velocityX;
			this->velocityY = velocityY;
		}

		void setAcceleration(float accelerationX, float accelerationY) {
			this->accelerationX = accelerationX;
			this->accelerationY = accelerationY;
		}

		void setSpringConstant(float springConstant) {
			this->springConstant = springConstant;
		}

		void scale(Point point, float scaleX, float scaleY) {
			plane.scale(point, scaleX, scaleY);
			updateEnvelope();
		}

		void translate(short deltaX, short deltaY) {
			plane.translate(deltaX, deltaY);
			updateEnvelope();
		}

		// angle in degrees
		void rotate(Point point, float angle) {
			plane.rotate(point, angle);
			updateEnvelope();
		}

		void rotateAgainstCenter(float angle) {
			short middleX, middleY;
			middleX = (bottomRightPosition.getX() + topLeftPosition.getX()) / 2;
			middleY = (bottomRightPosition.getY() + topLeftPosition.getY()) / 2;
			Point middlePoint = Point(middleX, middleY);
			rotate(middlePoint, angle);
		}

		void update() {
			plane.translate(velocityX, velocityY);
			velocityY += accelerationY;
			velocityX += accelerationX;
			updateEnvelope();
		}

		void bounce() {
			velocityY *= -springConstant;
		}

		Component& clip(ClippingPlane& clippingPlane) {
			static Component newComponent = *this;
			Plane newPlane;
			vector<short> topX, bottomX, leftY, rightY;

			// Get all visible lines
			for (auto& line : plane.getLines()) {
				short p1Code = clippingPlane.getCode(line.getP1());
				short p2Code = clippingPlane.getCode(line.getP2());
				Point newP1(-1, -1), newP2(-1, -1);
				short xorResult = p1Code ^ p2Code;
				
				if (p1Code == 0) {
					newP1 = line.getP1();
				}
				if (p2Code == 0) {
					if (newP1.isEqual(Point(-1, -1))) {
						newP1 = line.getP2();
					} else {
						newP2 = line.getP2();
					}
				}

				short intersection;
					
				if (xorResult % 16 >= 8) {
					intersection = line.getIntersectionPointX(clippingPlane.getTopLine());
					if (intersection != -1) {
						if (newP1.isEqual(Point(-1, -1))) {
							newP1 = Point(intersection, clippingPlane.getTopLine());
						} else {
							newP2 = Point(intersection, clippingPlane.getTopLine());
						}	
					}
				}
				if (xorResult % 8 >= 4) {
					intersection = line.getIntersectionPointX(clippingPlane.getBottomLine());
					if (intersection != -1) {
						if (newP1.isEqual(Point(-1, -1))) {
							newP1 = Point(intersection, clippingPlane.getBottomLine());
						} else {
							newP2 = Point(intersection, clippingPlane.getBottomLine());
						}	
					}
				}
				if (xorResult % 4 >= 2) {
					intersection = line.getIntersectionPointY(clippingPlane.getLeftLine());
					if (intersection != -1) {
						if (newP1.isEqual(Point(-1, -1))) {
							newP1 = Point(clippingPlane.getLeftLine(), intersection);
						} else {
							newP2 = Point(clippingPlane.getLeftLine(), intersection);
						}	
					}
				}
				if (xorResult % 2 >= 1) {
					intersection = line.getIntersectionPointY(clippingPlane.getRightLine());
					if (intersection != -1) {
						if (newP1.isEqual(Point(-1, -1))) {
							newP1 = Point(clippingPlane.getRightLine(), intersection);
						} else {
							newP2 = Point(clippingPlane.getRightLine(), intersection);
						}	
					}
				}

				if (!newP1.isEqual(Point(-1, -1)) && !newP2.isEqual(Point(-1, -1))) {
					if (newP1.getY() == clippingPlane.getTopLine()) {
						topX.push_back(newP1.getX());
					}
					if (newP1.getY() == clippingPlane.getBottomLine()) {
						bottomX.push_back(newP1.getX());
					}
					if (newP1.getX() == clippingPlane.getLeftLine()) {
						leftY.push_back(newP1.getY());
					}
					if (newP1.getX() == clippingPlane.getRightLine()) {
						rightY.push_back(newP1.getY());
					}
					if (newP2.getY() == clippingPlane.getTopLine()) {
						topX.push_back(newP2.getX());
					}
					if (newP2.getY() == clippingPlane.getBottomLine()) {
						bottomX.push_back(newP2.getX());
					}
					if (newP2.getX() == clippingPlane.getLeftLine()) {
						leftY.push_back(newP2.getY());
					}
					if (newP2.getX() == clippingPlane.getRightLine()) {
						rightY.push_back(newP2.getY());
					}
					newPlane.addLine(Line(newP1, newP2));
				}
			}

			if (doesEnvelop(Point(clippingPlane.getLeftLine(), clippingPlane.getTopLine()))) {
				topX.push_back(clippingPlane.getLeftLine());
				leftY.push_back(clippingPlane.getTopLine());
			}
			if (doesEnvelop(Point(clippingPlane.getRightLine(), clippingPlane.getTopLine()))) {
				topX.push_back(clippingPlane.getRightLine());
				rightY.push_back(clippingPlane.getTopLine());
			}
			if (doesEnvelop(Point(clippingPlane.getLeftLine(), clippingPlane.getBottomLine()))) {
				bottomX.push_back(clippingPlane.getLeftLine());
				leftY.push_back(clippingPlane.getBottomLine());
			}
			if (doesEnvelop(Point(clippingPlane.getRightLine(), clippingPlane.getBottomLine()))) {
				bottomX.push_back(clippingPlane.getRightLine());
				rightY.push_back(clippingPlane.getBottomLine());
			}

			sort(topX.begin(), topX.end());
			sort(bottomX.begin(), bottomX.end());
			sort(leftY.begin(), leftY.end());
			sort(rightY.begin(), rightY.end());

			if (topX.size() >= 2) {
            	for (int i = 0; i < topX.size() - 1; i += 2) {
            		newPlane.addLine(Line(Point(topX[i], clippingPlane.getTopLine()), Point(topX[i + 1], clippingPlane.getTopLine())));
            	}
            }
            if (bottomX.size() >= 2) {
            	for (int i = 0; i < bottomX.size() - 1; i += 2) {
            		newPlane.addLine(Line(Point(bottomX[i], clippingPlane.getBottomLine()), Point(bottomX[i + 1], clippingPlane.getBottomLine())));
            	}
            }
            if (leftY.size() >= 2) {
            	for (int i = 0; i < leftY.size() - 1; i += 2) {
            		newPlane.addLine(Line(Point(clippingPlane.getLeftLine(), leftY[i]), Point(clippingPlane.getLeftLine(), leftY[i + 1])));
            	}
            }
            if (rightY.size() >= 2) {
            	for (int i = 0; i < rightY.size() - 1; i += 2) {
            		newPlane.addLine(Line(Point(clippingPlane.getRightLine(), rightY[i]), Point(clippingPlane.getRightLine(), rightY[i + 1])));
            	}
            }

			newComponent.setPlane(newPlane);
			return newComponent;

		}

};

#endif
