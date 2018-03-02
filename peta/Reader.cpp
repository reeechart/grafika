#ifndef READER_CPP
#define READER_CPP

#include <iostream>
#include <fstream>
#include "Component.cpp"
#include "Line.cpp"
#include "Renderer.cpp"
#include "Canvas.cpp"

using namespace std;

class Reader {
    public:
        //ctor
        Reader() {

        }

        //dtor
        ~Reader() {
        }

        void readLayer(Layer *layer, string fileName) {
            ifstream fileOpen;
            fileOpen.open(fileName);

            if (fileOpen.is_open()) {
                string line;
                short minX = 9999, maxX = 0, minY = 9999, maxY = 0;
                while (getline(fileOpen, line)) {
                    Component component;
                    Plane plane;
                    int colorPosition = line.find("fill:") + string("fill:").length();
                    string colorCode = line.substr(colorPosition, 7);
                    component.setColor(colorCode);

                    int borderColorPosition = line.find("stroke:") + string("stroke:").length();
                    string borderColorCode = line.substr(borderColorPosition, 7);
                    component.setBorderColor(borderColorCode);

                    int linesPosition = line.find(" d=\"") + string(" d=\"").length();
                    int index = linesPosition;
                    Point veryFirstPoint;
                    Point firstPoint(0, 500), secondPoint(0, 500);
                    bool first = true;
                    bool negative;
                    int type = 0;
                    int firstDecimal;
                    while (line[index] != '"') {
                        int dx = 0, dy = 0;
                        if (line[index] == 'm' || line[index] == 'l' || line[index] == 'M' || line[index] == 'L') type = 0;
                        else if (line[index] == 'h' || line[index] == 'H') type = 1;
                        else if (line[index] == 'v' || line[index] == 'V') type = 2;
                        else if (line[index] == 'z' || line[index] == 'Z') type = 3;
                        //long long divisor = 1;
                        if (type == 3) {
                            // cout << "found z" << endl;
                            index += 2;
                            if (line[index] != 'm') break;
                            first = true;
                        } else  {
                            if (type == 0 || type == 1) {
                                firstDecimal = -1;
                                index += 2;
                                negative = false;
                                if (line[index] == '-') {
                                    negative = true;
                                    index++;
                                }
                                while (line[index] != '.' && line[index] != ',' && line[index] != ' ') {
                                    dx = 10 * dx + line[index] - '0';
                                    index++;
                                }
                                if (line[index] == '.') {
                                    firstDecimal = line[index + 1] - '0';
                                    while (line[index] != ',' && line[index] != ' ') {
                                        //dx = 10 * dx + line[index] - '0';
                                        //divisor *= 10;
                                        index++;
                                    }
                                }
                                if (firstDecimal >=5) dx++;
                                if (negative) dx *= -1;
                                //cout << "before" << dx << " divisor " << divisor << endl;
                                //dx = (long long)((float)dx * 2.7) / divisor;
                                //cout << " after " << dx << endl;
                            }
                            //divisor = 1;
                            if (type == 0 || type == 2) {
                                firstDecimal = -1;
                                if (line[index] == ',') index++;
                                else index += 2;
                                negative = false;
                                if (line[index] == '-') {
                                    negative = true;
                                    index++;
                                }
                                while (line[index] != '.' && line[index] != ' ' && line[index] != ' ') {
                                    dy = 10 * dy + line[index] - '0';
                                    index++;
                                }
                                if (line[index] == '.') {
                                    firstDecimal = line[index + 1] - '0';
                                    while (line[index] != 'l' && line[index] != ' ') {
                                        //dy = 10 * dy + line[index] - '0';
                                        //divisor *= 10;
                                        index++;
                                    }
                                }
                                if (firstDecimal >= 5) dy++;
                                if (negative) dy *= -1;
                                //dy = (long long)((float)dy * 2.7) / divisor;
                            }
                            index++;
                            // cout << "AFTER DY: " << line[index] << endl;
                            firstPoint = secondPoint;
                            secondPoint.translate(dx, dy);
                            // cout << secondPoint.getX() << " ------ " << secondPoint.getY() << endl;
                            // cout << line << endl;
                            // cout << dx << " - " << dy << endl;
                            if (secondPoint.getX() > maxX) maxX = secondPoint.getX();
                            if (secondPoint.getX() < minX) minX = secondPoint.getX();
                            if (secondPoint.getY() > maxY) maxY = secondPoint.getY();
                            if (secondPoint.getY() < minY) minY = secondPoint.getY();
                            if (!first) {
                                // cout << firstPoint.getX() << "," << firstPoint.getY() << " & " << secondPoint.getX() << "," << secondPoint.getY() << endl;
                                plane.addLine(Line(firstPoint, secondPoint));
                                if (line[index] == 'z') {
                                    // cout << secondPoint.getX() << "," << secondPoint.getY() << " & " << veryFirstPoint.getX() << "," << veryFirstPoint.getY() << endl;
                                    plane.addLine(Line(secondPoint, veryFirstPoint));
                                    firstPoint = secondPoint;
                                    secondPoint = veryFirstPoint;
                                }
                            } else {
                                veryFirstPoint = secondPoint;
                                first = false;
                            }
                        }
                    }
                    component.setPlane(plane);
                    (*layer).setEnvelope(minY, maxY, minX, maxX);
                    // cout << minY << " - " << maxY << " - " << minX << " - " << maxX << endl;
                    (*layer).addComponent(component);
                    // break;
                }
            }
        }

        void readComponent(Component *component, string fileName) {
            Line planeLine;
            Point linePoint;
            short readData;
            Plane plane = component->getPlane();

            ifstream fileOpen;
            fileOpen.open(fileName);

            int dataCounter = 0;
            if(fileOpen.is_open()) {
                while(!fileOpen.eof()) {
                    fileOpen >> readData;
                    dataCounter++;
                    switch (dataCounter) {
                        case 1:
                            linePoint.setY(readData);
                            break;
                        case 2:
                            linePoint.setX(readData);
                            planeLine.setP1(linePoint);
                            break;
                        case 3:
                            linePoint.setY(readData);
                            break;
                        case 4:
                            linePoint.setX(readData);
                            planeLine.setP2(linePoint);
                            plane.addLine(planeLine);
                            dataCounter = 0;
                            break;
                    }
                }
                component->setPlane(plane);
            } else {
                cout << "404 File Not Found" << endl;
            }

            fileOpen.close();
        }
};

#endif
