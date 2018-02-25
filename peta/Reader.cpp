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
                while (getline(fileOpen, line)) {
                    Component component;
                    Plane plane;
                    int colorPosition = line.find(" fill=\"") + string(" fill=\"").length();
                    string colorCode = line.substr(colorPosition, 7);
                    component.setColor(colorCode);

                    int borderColorPosition = line.find(" stroke=\"") + string(" stroke=\"").length();
                    string borderColorCode = line.substr(borderColorPosition, 7);
                    component.setBorderColor(borderColorCode);

                    int linesPosition = line.find(" d=\"") + string(" d=\"").length();
                    int index = linesPosition + 1;
                    Point veryFirstPoint;
                    Point firstPoint, secondPoint;
                    bool first = true;
                    bool negative;
                    while (line[index] != 'z') {
                        int dx = 0, dy = 0;
                        negative = false;
                        if (line[index] == '-') {
                            negative = true;
                            index++;
                        }
                        while (line[index] != '.' && line[index] != ',') {
                            dx = 10 * dx + line[index] - '0';
                            index++;
                        }
                        if (line[index] == '.') {
                            while (line[index] != ',') {
                                index++;
                            }
                        }
                        if (negative) dx *= -1;
                        index++;
                        negative = false;
                        if (line[index] == '-') {
                            negative = true;
                            index++;
                        }
                        while (line[index] != '.' && line[index] != 'l' && line[index] != 'z') {
                            dy = 10 * dy + line[index] - '0';
                            index++;
                        }
                        if (line[index] == '.') {
                            while (line[index] != 'l' && line[index] != 'z') {
                                index++;
                            }
                        }
                        if (negative) dy *= -1;
                        firstPoint = secondPoint;
                        secondPoint.translate(dx, dy);
                        if (!first) {
                            if (line[index] == 'z')
                                secondPoint = veryFirstPoint;
                            plane.addLine(Line(firstPoint, secondPoint));
                        } else {
                            veryFirstPoint = secondPoint;
                            first = false;
                        }
                        if (line[index] == 'z') {
                            break;
                        }
                        index++;
                    }
                    component.setPlane(plane);
                    (*layer).addComponent(component);
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
