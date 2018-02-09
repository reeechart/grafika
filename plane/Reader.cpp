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
    private:
        ifstream fileOpen;
        string filename;
    public:
        //ctor
        Reader(string filename) {
            this->filename = filename;
            fileOpen.open(filename);
        }
        //cctor
        Reader(const Reader& reader) {
            this->filename = reader.filename;
            this->fileOpen.open(this->filename);
        }

        //dtor
        ~Reader() {
            fileOpen.close();
            filename = "";
        }

        void readComponent(Component *component) {
            Line planeLine;
            Point linePoint;
            short readData;
            Plane plane = component->getPlane();

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
        }
};

#endif
