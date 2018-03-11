#include "Reader.cpp"
#include "Renderer.cpp"
#include "Component.cpp"
#include "Canvas.cpp"
#include <pthread.h>
#include <ncurses.h>
#include <fcntl.h>
#include <linux/input.h>

// #define START_X_MINIMAP 100
// #define START_Y_MINIMAP 50
// #define MINIMAP_X_SIZE 450
// #define MINIMAP_Y_SIZE 450
// #define START_X_MAP 600
// #define START_Y_MAP 50
// #define MAP_X_SIZE 450
// #define MAP_Y_SIZE 450
// #define SCREEN_X_SIZE 1360
// #define SCREEN_Y_SIZE 760
// #define CURSOR_SPEED_ADJUSTMENT 0.5
// #define START_Y_LAYER_BOXES 600
// #define START_X_LAYER_BOXES 10
// #define SIZE_Y_LAYER_BOXES 50
// #define SIZE_X_LAYER_BOXES 180

class Tugas6 {
    private:
        const int START_X_MINIMAP = 100;
        const int START_Y_MINIMAP = 50;
        const int MINIMAP_X_SIZE = 450;
        const int MINIMAP_Y_SIZE = 450;
        const int START_X_MAP = 600;
        const int START_Y_MAP = 50;
        const int MAP_X_SIZE = 450;
        const int MAP_Y_SIZE = 450;
        const int SCREEN_X_SIZE = 1360;
        const int SCREEN_Y_SIZE = 760;
        const float CURSOR_SPEED_ADJUSTMENT = 0.5;
        const int START_Y_LAYER_BOXES = 600;
        const int START_X_LAYER_BOXES = 10;
        const int SIZE_Y_LAYER_BOXES = 50;
        const int SIZE_X_LAYER_BOXES = 180;
        const int MY_SCENE_NUMBER = 6;
        int offsetX = 0;
        int offsetY = 0;
        int sizeX = 50;
        int sizeY = 50;

        Color colors[7];

        // saving all the layers
        Layer layers[7];

        int mouseX = 0, mouseY = 0;

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

        void setOffset(int x, int y) {
            offsetX = x;
            if (offsetX + sizeX >= MINIMAP_X_SIZE) offsetX = MINIMAP_X_SIZE - 1 - sizeX;
            if (offsetX < 0) offsetX = 0;
            offsetY = y;
            if (offsetY + sizeY >= MINIMAP_Y_SIZE) offsetY = MINIMAP_Y_SIZE - 1 - sizeY;
            if (offsetY < 0) offsetY = 0;
        }

        #define MOUSEFILE "/dev/input/mice"

        void* readMouse(void *arg) {    
            int fd;
            struct input_event ie;
            //
            unsigned char button,bLeft,bMiddle,bRight;
            char x,y;
            int absolute_x = 0, absolute_y = 0;

            if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
                printf("Device open ERROR\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Device open OK\n");
            }
            //
            // printf("right-click to set absolute x,y coordinates origin (0,0)\n");
            while (read(fd, &ie, sizeof(struct input_event)))
            {
                unsigned char *ptr = (unsigned char*)&ie;
                button = ptr[0];
                bLeft = button & 0x1;
                bMiddle = (button & 0x4) > 0;
                bRight = (button & 0x2) > 0;
                x = (char) ptr[1];
                y = (char) ptr[2];
                absolute_x += x > 11 ? 10 : x;
                absolute_y -= y > 11 ? 10 : y;
                if (absolute_x < 0) absolute_x = 0;
                if (absolute_y < 0) absolute_y = 0;
                if (absolute_x > SCREEN_X_SIZE * CURSOR_SPEED_ADJUSTMENT) absolute_x = SCREEN_X_SIZE * CURSOR_SPEED_ADJUSTMENT;
                if (absolute_y > SCREEN_Y_SIZE * CURSOR_SPEED_ADJUSTMENT) absolute_y = SCREEN_Y_SIZE * CURSOR_SPEED_ADJUSTMENT;
                mouseX = absolute_x / CURSOR_SPEED_ADJUSTMENT;
                mouseY = absolute_y / CURSOR_SPEED_ADJUSTMENT;
                bool dragged = false;
                if (bLeft) {
                    if (mouseX > START_X_MINIMAP + sizeX / 2 && mouseX < START_X_MINIMAP + MINIMAP_X_SIZE - sizeX / 2
                            && mouseY > START_Y_MINIMAP + sizeY / 2 && mouseY < START_Y_MINIMAP + MINIMAP_Y_SIZE + sizeY / 2)
                        setOffset(mouseX - START_X_MINIMAP - sizeX / 2, mouseY - START_Y_MINIMAP - sizeY / 2);
                    if (mouseX > START_X_MAP && mouseX < START_X_MAP + MAP_X_SIZE
                            && mouseY > START_Y_MAP && mouseY < START_Y_MAP + MAP_Y_SIZE) {
                        while (read(fd, &ie, sizeof(struct input_event))) {
                            unsigned char * ptr = (unsigned char*)&ie;
                            if (ptr[0] & 0x1) {
                                // moveOffset((char) ptr[1], (char) ptr[2]);
                                int dx = (char) ptr[1];
                                int dy = (char) ptr[2];
                                moveOffset(-dx / 3, dy / 3);
                            } else {
                                break;
                            }
                        }
                    }
                    if (mouseY > START_Y_LAYER_BOXES && mouseY < START_Y_LAYER_BOXES + SIZE_Y_LAYER_BOXES &&
                        mouseX > START_X_LAYER_BOXES && mouseX < START_X_LAYER_BOXES + 7 * SIZE_X_LAYER_BOXES) {
                        int position = (mouseX - START_X_LAYER_BOXES) / SIZE_X_LAYER_BOXES;
                        // cout << position << endl;
                        layers[position].setVisibility(!layers[position].isVisible());
                    }
                } else {
                    dragged = false;
                }
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
                                if (offsetY + sizeY >= MINIMAP_Y_SIZE) offsetY = MINIMAP_Y_SIZE - 1 - sizeY;
                                break;
                            case 'C':
                                // code for arrow right
                                offsetX++;
                                if (offsetX + sizeX >= MINIMAP_X_SIZE) offsetX = MINIMAP_X_SIZE - 1 - sizeX;
                                break;
                            case 'D':
                                // code for arrow left
                                offsetX--;
                                if (offsetX < 0) offsetX = 0;
                                break;
                        }
                    } else {
                        if (ch == 'z') {
                            ++sizeX;
                            ++sizeY;
                            if (offsetY + sizeY >= MINIMAP_Y_SIZE || offsetX + sizeX >= MINIMAP_Y_SIZE) {
                                sizeY = MINIMAP_Y_SIZE - 1 - offsetY;
                                sizeX = MINIMAP_X_SIZE - 1 - offsetX;
                                if (sizeY > sizeX) sizeY = sizeX;
                                if (sizeX > sizeY) sizeX = sizeY;
                            }
                        } else if (ch == 'x') {
                            --sizeX;
                            --sizeY;
                            if (sizeX < 10) sizeX = 10;
                            if (sizeY < 10) sizeY = 10;
                        } else if (ch >= '1' && ch <= '7') {
                            layers[ch - '1'].setVisibility(!layers[ch - '1'].isVisible());
                        }
                    }
                }
            }

            return NULL;
        }

    public:
        void reset() {
            for (int i = 0; i < 7; i++) {
                layers[i].reset();
            }
        }
        void zoomAndLayer(char x) {
            if (x == 'z') {
                ++sizeX;
                ++sizeY;
                if (offsetY + sizeY >= MINIMAP_Y_SIZE || offsetX + sizeX >= MINIMAP_Y_SIZE) {
                    sizeY = MINIMAP_Y_SIZE - 1 - offsetY;
                    sizeX = MINIMAP_X_SIZE - 1 - offsetX;
                    if (sizeY > sizeX) sizeY = sizeX;
                    if (sizeX > sizeY) sizeX = sizeY;
                }
            } else if (x == 'x') {
                --sizeX;
                --sizeY;
                if (sizeX < 10) sizeX = 10;
                if (sizeY < 10) sizeY = 10;
            } else if (x >= '1' && x <= '7') {
                layers[x - '1'].setVisibility(!layers[x - '1'].isVisible());
            }
        }

        void toggleLayerVisibility(int position) {
            layers[position].setVisibility(!layers[position].isVisible());
        }

        void moveOffset(int x, int y) {
            offsetX += x;
            if (offsetX + sizeX >= MINIMAP_X_SIZE) offsetX = MINIMAP_X_SIZE - 1 - sizeX;
            if (offsetX < 0) offsetX = 0;
            offsetY += y;
            if (offsetY + sizeY >= MINIMAP_Y_SIZE) offsetY = MINIMAP_Y_SIZE - 1 - sizeY;
            if (offsetY < 0) offsetY = 0;
        }

        void moveClippingPlane(char x) {
            switch(x) {
                case 'A':
                    // code for arrow up
                    offsetY--;
                    if (offsetY < 0) offsetY = 0;
                    break;
                case 'B':
                    // code for arrow down
                    offsetY++;
                    if (offsetY + sizeY >= MINIMAP_Y_SIZE) offsetY = MINIMAP_Y_SIZE - 1 - sizeY;
                    break;
                case 'C':
                    // code for arrow right
                    offsetX++;
                    if (offsetX + sizeX >= MINIMAP_X_SIZE) offsetX = MINIMAP_X_SIZE - 1 - sizeX;
                    break;
                case 'D':
                    // code for arrow left
                    offsetX--;
                    if (offsetX < 0) offsetX = 0;
                    break;
            }
        }

        void setMouseOffset() {
            if (mouseX > START_X_MINIMAP + sizeX / 2 && mouseX < START_X_MINIMAP + MINIMAP_X_SIZE - sizeX / 2
                    && mouseY > START_Y_MINIMAP + sizeY / 2 && mouseY < START_Y_MINIMAP + MINIMAP_Y_SIZE + sizeY / 2)
                setOffset(mouseX - START_X_MINIMAP - sizeX / 2, mouseY - START_Y_MINIMAP - sizeY / 2);
        }

        void setMousePosition(int x, int y) {
            mouseX = x;
            mouseY = y;
        }

        Tugas6& operator=(const Tugas6& tugas6) {

        }

        void execute(int* current_scene) {
            Reader reader;
            Renderer renderer;
            Component mouseCursor;
            mouseCursor.setColor(Color(0, 200, 50));
            mouseCursor.setBorderColor(Color(0, 0, 0));

            // initiate the map
            reader.readLayer(&layers[0], "assets/layer_gedung.svg");
            reader.readLayer(&layers[1], "assets/layer_lapangan.svg");
            reader.readLayer(&layers[2], "assets/layer_parkiran.svg");
            reader.readLayer(&layers[3], "assets/layer_kolam.svg");
            reader.readLayer(&layers[4], "assets/layer_jalan_besar.svg");
            reader.readLayer(&layers[5], "assets/layer_jalan_kecil.svg");
            reader.readLayer(&layers[6], "assets/layer_jalur_teduh.svg");
            reader.readComponent(&mouseCursor, "assets/wheel.txt");

            for (int i = 0; i < 7; i++) {
                colors[i] = layers[i].getComponents()[0].getBorderColor();
                // layers[i].translate(START_X_MINIMAP, START_Y_MINIMAP);
                layers[i].translate(0, -50);
                int dx = MINIMAP_X_SIZE / 2 - (layers[i].right + layers[i].left) / 2;
                int dy = MINIMAP_Y_SIZE / 2 - (layers[i].top + layers[i].bottom) / 2;
                // layers[i].translate(dx, dy);
                // float ratioX = MINIMAP_X_SIZE / (layers[i].right - layers[i].left);
                // float ratioY = MINIMAP_Y_SIZE / (layers[i].bottom - layers[i].top);
                Point middlePoint(START_X_MINIMAP + MINIMAP_X_SIZE / 2, START_Y_MINIMAP + MINIMAP_Y_SIZE / 2);
                layers[i].scale(middlePoint, 0.6, 0.6);
                // if (i <= 3)
                //     layers[i].scale(middlePoint, 0.6, 0.6);
                // else {
                //     layers[i].scale(middlePoint, 0.4, 0.4);
                // }
            }

            // another thread to read input
            // pthread_create(&(tid[0]), NULL, &readInput, NULL);
            // pthread_create(&(tid[1]), NULL, &readMouse, NULL);

            while (1) {
                Canvas window(Point(0, 0), Point(SCREEN_X_SIZE, SCREEN_Y_SIZE));
                Component mouseCursorClone = mouseCursor;
                mouseCursorClone.translate(mouseX, mouseY);
                ClippingPlane clippingPlane(START_Y_MINIMAP + offsetY, START_Y_MINIMAP + sizeY + offsetY,
                    START_X_MINIMAP + offsetX, START_X_MINIMAP + sizeX + offsetX);
                

                clippingPlane.setOverlayColor(Color(0, 255, 0));

                int dx, dy;
                dx = MINIMAP_X_SIZE / 2 - offsetX - sizeX / 2;
                dy = MINIMAP_Y_SIZE / 2 - offsetY - sizeY / 2;

                Layer copyLayers[7];
                for (int i = 0; i < 7; i++) {
                    copyLayers[i] = layers[i];
                    if (copyLayers[i].isVisible()) {
                        copyLayers[i].clip(clippingPlane);

                        // after clipping, we need to scale and move to map position
                        copyLayers[i].translate(dx, dy);
                        copyLayers[i].scale(Point(MINIMAP_X_SIZE / 2 + START_X_MINIMAP, MINIMAP_Y_SIZE / 2 + START_Y_MINIMAP),
                            ((float) MINIMAP_X_SIZE / sizeX), ((float) MINIMAP_Y_SIZE / sizeY));
                        copyLayers[i].translate(START_X_MAP - START_X_MINIMAP, START_Y_MAP - START_Y_MINIMAP);

                        bool fill = i <= 3;

                        renderer.renderToCanvas(layers[i], &window, fill);
                        renderer.renderToCanvas(copyLayers[i], &window, fill);
                    }
                }
                renderer.renderToCanvas(mouseCursorClone, &window, true);

                // color the border of map and minimap
                Color borderColor(255, 0, 0);
                window.colorBorder(borderColor, START_Y_MINIMAP, START_Y_MINIMAP + MINIMAP_Y_SIZE,
                    START_X_MINIMAP, START_X_MINIMAP + MINIMAP_X_SIZE);
                window.colorBorder(borderColor, START_Y_MAP, START_Y_MAP + MAP_Y_SIZE,
                    START_X_MAP, START_X_MAP + MAP_X_SIZE);

                for (int i = 0; i < 7; i++) {
                    // coloring layer box border
                    window.colorBorder(borderColor, START_Y_LAYER_BOXES, START_Y_LAYER_BOXES + SIZE_Y_LAYER_BOXES,
                        START_X_LAYER_BOXES + i * SIZE_X_LAYER_BOXES, START_X_LAYER_BOXES + (i + 1) * SIZE_X_LAYER_BOXES);
                    window.fill(colors[i], START_Y_LAYER_BOXES, START_Y_LAYER_BOXES + SIZE_Y_LAYER_BOXES,
                        START_X_LAYER_BOXES + i * SIZE_X_LAYER_BOXES, START_X_LAYER_BOXES + (i + 1) * SIZE_X_LAYER_BOXES);
                }

                window.overlayClippingPlane(clippingPlane);
                renderer.copyToFrameBuffer(window);

                window.clear();
                if (*current_scene != MY_SCENE_NUMBER)
                    break;
            }

            while(1);
        }
};
