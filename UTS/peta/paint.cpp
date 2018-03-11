#include "Reader.cpp"
#include "Renderer.cpp"
#include "Component.cpp"
#include "Canvas.cpp"
#include "Point.cpp"
#include <pthread.h>
#include <ncurses.h>
#include <fcntl.h>
#include <linux/input.h>


class Tugas7 {
    private:
        const int START_X_MINIMAP = 0;
        const int START_Y_MINIMAP = 0;
        const int MINIMAP_X_SIZE = 1360;
        const int MINIMAP_Y_SIZE = 760;
        const int START_X_MAP = 0;
        const int START_Y_MAP = 0;
        const int MAP_X_SIZE = 1360;
        const int MAP_Y_SIZE = 760;
        const int SCREEN_X_SIZE = 1360;
        const int SCREEN_Y_SIZE = 760;
        const float CURSOR_SPEED_ADJUSTMENT = 0.5;
        const int START_Y_LAYER_BOXES = 600;
        const int START_X_LAYER_BOXES = 10;
        const int SIZE_Y_LAYER_BOXES = 50;
        const int SIZE_X_LAYER_BOXES = 180;
        const int MY_SCENE_NUMBER = 7;
        pthread_t tid[3];
        int offsetX = 0;
        int offsetY = 0;
        int sizeX = 50;
        int sizeY = 50;
        Canvas paintedWindow = Canvas(Point(0, 0), Point(SCREEN_X_SIZE, SCREEN_Y_SIZE));

        Color colors[7];

        // saving all the layers
        Layer layers[7];

        int mouseX = 0, mouseY = 0;
        Component mouseCursor;

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
                    paintedWindow.setColor(mouseY, mouseX, mouseCursor.getColor());
                    if (mouseX > START_X_MINIMAP && mouseX < START_X_MINIMAP + MINIMAP_X_SIZE 
                            && mouseY > START_Y_MINIMAP && mouseY < START_Y_MINIMAP + MINIMAP_Y_SIZE)
                        setOffset(mouseX - START_X_MINIMAP, mouseY - START_Y_MINIMAP);
                    // if (mouseX > START_X_MAP && mouseX < START_X_MAP + MAP_X_SIZE
                    //         && mouseY > START_Y_MAP && mouseY < START_Y_MAP + MAP_Y_SIZE) {
                    //     while (read(fd, &ie, sizeof(struct input_event))) {
                    //         unsigned char * ptr = (unsigned char*)&ie;
                    //         if (ptr[0] & 0x1) {
                    //             // moveOffset((char) ptr[1], (char) ptr[2]);
                    //             int dx = (char) ptr[1];
                    //             int dy = (char) ptr[2];
                    //             moveOffset(-dx / 3, dy / 3);
                    //             paintedWindow.setColor(mouseY, mouseX, Color(255, 255, 255));
                    //         } else {
                    //             break;
                    //         }
                    //     }
                    // }
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
                string line;
                cin >> line;
                bool valid = true;
                if (line.length() == 6) {
                    for (auto& ch : line) {
                        if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
                            valid = false;
                            break;
                        }
                    }
                    if (valid) {
                        line = "#" + line;
                        mouseCursor.setColor(line);
                    }
                }
            }

            return NULL;
        }

    public:
        void moveOffset(int x, int y) {
            offsetX += x;
            if (offsetX + sizeX >= MINIMAP_X_SIZE) offsetX = MINIMAP_X_SIZE - 1 - sizeX;
            if (offsetX < 0) offsetX = 0;
            offsetY += y;
            if (offsetY + sizeY >= MINIMAP_Y_SIZE) offsetY = MINIMAP_Y_SIZE - 1 - sizeY;
            if (offsetY < 0) offsetY = 0;
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

        void changeColor(string colorCode) {
            bool valid = true;
            if (colorCode.length() == 6) {
                for (auto& ch : colorCode) {
                    if (!((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))) {
                        valid = false;
                        break;
                    }
                }
                if (valid) {
                    colorCode = "#" + colorCode;
                    mouseCursor.setColor(colorCode);
                }
            }
        }

        void paint() {
            paintedWindow.setColor(mouseY, mouseX, mouseCursor.getColor());
        }

        void execute(int* current_scene) {
            Reader reader;
            Renderer renderer;
            mouseCursor.setColor(Color(0, 200, 50));
            mouseCursor.setBorderColor(Color(0, 0, 0));

            reader.readComponent(&mouseCursor, "assets/wheel.txt");

            // another thread to read input
            // pthread_create(&(tid[0]), NULL, &readInput, NULL);
            // pthread_create(&(tid[1]), NULL, &readMouse, NULL);

            while (1) {
                Canvas window(Point(0, 0), Point(SCREEN_X_SIZE, SCREEN_Y_SIZE));
                
                Component mouseCursorClone = mouseCursor;
                mouseCursorClone.translate(mouseX, mouseY);
                
                window = paintedWindow;
                renderer.renderToCanvas(mouseCursorClone, &window, true);

                renderer.copyToFrameBuffer(window);

                // window.clear();

                if (*current_scene != MY_SCENE_NUMBER) {
                    return;
                }
            }

            while(1);
        }
};
