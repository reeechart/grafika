#ifndef RENDERER_CPP
#define RENDERER_CPP

#include "Component.cpp"
#include "Canvas.cpp"
#include "Layer.cpp"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <algorithm>
#include <stack>

#include <iostream>
using namespace std;



class Renderer {
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    private:
        char *fbp = 0;
        long int screensize = 0;
        int fbfd = 0;

        void scanLineComponent(Component component, Canvas *canvas) {
            for (int y = component.getTopLeftPosition().getY(); y <= component.getBottomRightPosition().getY(); y++) {
                vector<pair<short, short>> intersections;
                for (auto& line : component.getPlane().getLines()) {
                    pair<short, short> intersection = line.getIntersectionStatusY(y);
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
                        for (int x = intersections[i].first; x < intersections[i + 1].first; x++) {
                            canvas->setColor(y, x, component.getColor());
                        }
                    }
                }
            }
        }

    public:
        Renderer() {
            int x = 0, y = 0;
            int y_start, x_start;
            long int location = 0;

            // Open the file for reading and writing
            fbfd = open("/dev/fb0", O_RDWR);
            if (fbfd == -1) {
                perror("Error: cannot open framebuffer device");
                exit(1);
            }

            // Get fixed screen information
            if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
                perror("Error reading fixed information");
                exit(2);
            }

            // Get variable screen information
            if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
                perror("Error reading variable information");
                exit(3);
            }

            // Figure out the size of the screen in bytes
            screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

            // Map the device to memory
            fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
            if (*fbp == -1) {
                perror("Error: failed to map framebuffer device to memory");
                exit(4);
            }
        }

        ~Renderer() {
            munmap(fbp, screensize);
            close(fbfd);
        }

        void floodFill(Canvas *canvas, Component component, int i, int j, Color color, Color borderColor) {
            stack<pair<short, short> > toBeColored;
            toBeColored.push(make_pair(i, j));
            while (!toBeColored.empty()) {
                pair<short, short> colorPosition = toBeColored.top();
                toBeColored.pop();
                //cout << colorPosition.first << " " << colorPosition.second << endl;
                if (colorPosition.first >= component.getTopLeftPosition().getY() && colorPosition.second >= component.getTopLeftPosition().getX() &&
                colorPosition.first <= component.getBottomRightPosition().getY() && colorPosition.second <= component.getBottomRightPosition().getX()) {
                    canvas->setColor(colorPosition.first, colorPosition.second, color);

                    if (!canvas->getColor(colorPosition.first - 1, colorPosition.second).isEqual(color) && !canvas->getColor(colorPosition.first - 1, colorPosition.second).isEqual(borderColor))
                        toBeColored.push(make_pair(colorPosition.first - 1, colorPosition.second));
                    if (!canvas->getColor(colorPosition.first, colorPosition.second - 1).isEqual(color) && !canvas->getColor(colorPosition.first, colorPosition.second - 1).isEqual(borderColor))
                        toBeColored.push(make_pair(colorPosition.first, colorPosition.second - 1));
                    if (!canvas->getColor(colorPosition.first, colorPosition.second + 1).isEqual(color) && !canvas->getColor(colorPosition.first, colorPosition.second + 1).isEqual(borderColor))
                        toBeColored.push(make_pair(colorPosition.first, colorPosition.second + 1));
                    if (!canvas->getColor(colorPosition.first + 1, colorPosition.second).isEqual(color) && !canvas->getColor(colorPosition.first + 1, colorPosition.second).isEqual(borderColor))
                        toBeColored.push(make_pair(colorPosition.first + 1, colorPosition.second));
                }
            }
        }

        void renderToCanvas(Component component, Canvas *canvas) {
            for(auto& line : component.getPlane().getLines() ) {
                for(auto& point : line.getAllPoints()) {
                    canvas->setColor(point.getY(), point.getX(), component.getColor());
                }
            }
            // scanLineComponent(component, canvas);
            // EXPERIMENT
            // int midI = (component.getBottomRightPosition().getY() + component.getTopLeftPosition().getY()) / 2;
            // int midJ = (component.getBottomRightPosition().getX() + component.getTopLeftPosition().getX()) / 2;
            for (auto& point : component.getFloodfillStartPoint()) {
                floodFill(canvas, component, point.getY(), point.getX(), component.getColor(), component.getBorderColor());
            }
            for(auto& line : component.getPlane().getLines() ) {
                for(auto& point : line.getAllPoints()) {
                    canvas->setColor(point.getY(), point.getX(), component.getBorderColor());
                }
            }
        }

        void renderToCanvas(Layer& layer, Canvas *canvas) {
            for (auto& component : layer.getComponents()) {
                renderToCanvas(component, canvas);
            }
        }

        void copyToFrameBuffer(Canvas& canvas) {
            for (int y = canvas.getTop(); y < canvas.getBottom(); y++) {
                for (int x = canvas.getLeft(); x < canvas.getRight(); x++) {
                    int location;
                    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                               (y+vinfo.yoffset) * finfo.line_length;
                    *(fbp + location) = canvas.getOffsetColor(y, x).getBlueValue();
                    *(fbp + location + 1) = canvas.getOffsetColor(y, x).getGreenValue();
                    *(fbp + location + 2) = canvas.getOffsetColor(y, x).getRedValue();
                    *(fbp + location + 3) = 0;
                }
            }
        }
};

#endif
