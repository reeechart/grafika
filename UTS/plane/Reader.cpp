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
