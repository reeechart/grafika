#include "Component.cpp"
#include "Canvas.cpp"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <algorithm>

#include <iostream>
using namespace std;

struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

class Renderer {
    private:
        char *fbp = 0;
        long int screensize = 0;
        int fbfd = 0;
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

        void renderToCanvas(Component component, Canvas *canvas) {
            for(auto& line : component.getPlane().getLines() ) {
                for(auto& point : line.getAllPoints()) {
                    canvas->setColor(point.getY(), point.getX(), component.getColor());
                }
            }
        }

        void scanLineComponent(Component component, Canvas *canvas) {
            // TODO: finish this
            for (int y = component.getTopLeftPosition().getY(); y <= component.getBottomRightPosition().getY(); y++) {
                vector<pair<short, short>> intersections;
                for (auto& line : component.getPlane().getLines()) {
                    pair<short, short> intersection = line.getIntersectionPoint(y);
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
                intersections.push_back(make_pair(component.getBottomRightPosition().getX(), 0));
                sort(intersections.begin(), intersections.end());
                for (int i = 0; i < intersections.size() - 1; i += 2) {
                    for (int x = intersections[i].first; x < intersections[i + 1].first; x++) {
                        canvas->setColor(y, x, component.getColor());
                    }
                }
            }
        }

        void copyToFrameBuffer(Canvas& canvas) {
            for (int y = 0; y < V_SIZE; y++) {
                for (int x = 0; x < H_SIZE; x++) {
                    int location;
                    location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                               (y+vinfo.yoffset) * finfo.line_length;
                    *(fbp + location) = canvas.getColor(y, x).getBlueValue();
                    *(fbp + location + 1) = canvas.getColor(y, x).getGreenValue();
                    *(fbp + location + 2) = canvas.getColor(y, x).getRedValue();
                    *(fbp + location + 3) = 0;
                }
            }
        }
};

int main() {
    Plane plane;
    Component component;
    Canvas canvas;
    Renderer renderer;

    plane.addLine(Line(Point(400,400), Point(400,600)));
    plane.addLine(Line(Point(400,600), Point(600,600)));
    plane.addLine(Line(Point(600,600), Point(600,400)));
    plane.addLine(Line(Point(600,400), Point(400,400)));
    component.setPlane(plane);
    component.setColor(Color(255,255,255));
    component.setTopLeftPosition(Point(0,0));
    component.setBottomRightPosition(Point(600,600));

    // renderer.renderToCanvas(component,&canvas);
    // renderer.copyToFrameBuffer(canvas);
    // sleep(5);

    component.rotate(Point(500,500), 20);
    renderer.renderToCanvas(component,&canvas);
    renderer.scanLineComponent(component,&canvas);
    renderer.copyToFrameBuffer(canvas);
    while (1);
    return 0;
}
