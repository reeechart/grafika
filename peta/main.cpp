#include "Reader.cpp"
#include "Renderer.cpp"
#include "Component.cpp"
#include "Canvas.cpp"
#include <pthread.h>
#include <ncurses.h>

pthread_t tid[2];
int offsetX = 0;
int offsetY = 0;
int sizeX = 50;
int sizeY = 50;

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}

void* readInput(void *arg) {
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    scrollok(stdscr, TRUE);
    char ch;
    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == '\033') { // if the first value is esc
                getch(); // skip the [
                switch(getch()) { // the real value
                    case 'A':
                        // code for arrow up
                        offsetY--;
                        if (offsetY < 0) offsetY = 0;
                        break;
                    case 'B':
                        // code for arrow down
                        offsetY++;
                        if (offsetY + sizeY >= 450) offsetY = 450 - 1 - sizeY;
                        break;
                    case 'C':
                        // code for arrow right
                        offsetX++;
                        if (offsetX + sizeX >= 450) offsetX = 450 - 1 - sizeX;
                        break;
                    case 'D':
                        // code for arrow left
                        offsetX--;
                        if (offsetX < 0) offsetX = 0;
                        break;
                }
            } else {
                if (ch == 'z') {
                    sizeX *= 1.1;
                    sizeY *= 1.1;
                    if (offsetY + sizeY >= 450 || offsetX + sizeX >= 450) {
                        sizeY = 450 - 1 - offsetY;
                        sizeX = 450 - 1 - offsetX;
                    }
                } else if (ch == 'x') {
                    sizeX /= 1.1;
                    sizeY /= 1.1;
                    if (sizeX < 10) sizeX = 10;
                    if (sizeY < 10) sizeY = 10;
                }
            }
        }
    }

    return NULL;
}


int main() {
    Canvas miniMapCanvas(Point(50, 50), Point(500, 500));
    Canvas mapCanvas(Point(600, 50), Point(1050, 500));
    Layer layer;
    Reader reader;
    Renderer renderer;

    // initiate the map
    reader.readLayer(&layer, "assets/peta.svg");

    // another thread to read input
    pthread_create(&(tid[0]), NULL, &readInput, NULL);

    Point canvasCenter = Point((mapCanvas.getLeft() + mapCanvas.getRight()) / 2 - mapCanvas.getLeft(), (mapCanvas.getTop() + mapCanvas.getBottom()) / 2 - mapCanvas.getTop());

    while (1) {
        ClippingPlane clippingPlane(0 + offsetY, sizeY + offsetY, 0 + offsetX, sizeX + offsetX);
        clippingPlane.setOverlayColor(Color(0, 255, 0));
        Canvas mapCanvas(Point(600, 50), Point(1050, 500));
        
        Layer copyLayer = layer;
        copyLayer.clip(clippingPlane);
        

        int dx, dy;
        dx = canvasCenter.getX() - offsetX - sizeX / 2;
        dy = canvasCenter.getY() - offsetY - sizeY / 2; 

        copyLayer.translate(dx, dy);
        copyLayer.scale(canvasCenter, 8, 8);

        renderer.renderToCanvas(layer, &miniMapCanvas);
        renderer.renderToCanvas(copyLayer, &mapCanvas);

        miniMapCanvas.overlayClippingPlane(clippingPlane);

        renderer.copyToFrameBuffer(mapCanvas);
        renderer.copyToFrameBuffer(miniMapCanvas);

        mapCanvas.clear();
        miniMapCanvas.clear();
    }

    while(1);

    return 0;
}
