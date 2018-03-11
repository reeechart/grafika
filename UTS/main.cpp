#include <iostream>
#include <pthread.h>
#include <ncurses.h>
#include <fcntl.h>
#include <linux/input.h>
#include "credits/credits.cpp"
#include "line_credits/credits.cpp"
#include "floodfill/floodfill.cpp"
#include "peta/plane.cpp"
#include "peta/peta.cpp"
#include "peta/paint.cpp"

using namespace std;

pthread_t tid_master[4];
int current_scene;
char keyboard_character;
Tugas1 tugas1;
Tugas2 tugas2;
Tugas3 tugas3;
Tugas5 tugas5;
Tugas6 tugas6;
Tugas7 tugas7;

#define MOUSEFILE "/dev/input/mice"
void* readMouse(void *arg) {
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
        int mouseX = absolute_x / CURSOR_SPEED_ADJUSTMENT;
        int mouseY = absolute_y / CURSOR_SPEED_ADJUSTMENT;
        tugas6.setMousePosition(mouseX, mouseY);
        tugas7.setMousePosition(mouseX, mouseY);
        if (current_scene == 7) {
            bool dragged = false;
            if (bLeft) {
                tugas7.paint();
                tugas7.setMouseOffset();
            } else {
                dragged = false;
            }
        } else {
            bool dragged = false;
            if (bLeft) {
                tugas6.setMouseOffset();
                // if (mouseX > START_X_MINIMAP + sizeX / 2 && mouseX < START_X_MINIMAP + MINIMAP_X_SIZE - sizeX / 2
                //         && mouseY > START_Y_MINIMAP + sizeY / 2 && mouseY < START_Y_MINIMAP + MINIMAP_Y_SIZE + sizeY / 2)
                //     setOffset(mouseX - START_X_MINIMAP - sizeX / 2, mouseY - START_Y_MINIMAP - sizeY / 2);
                if (mouseX > START_X_MAP && mouseX < START_X_MAP + MAP_X_SIZE
                        && mouseY > START_Y_MAP && mouseY < START_Y_MAP + MAP_Y_SIZE) {
                    while (read(fd, &ie, sizeof(struct input_event))) {
                        unsigned char * ptr = (unsigned char*)&ie;
                        if (ptr[0] & 0x1) {
                            // moveOffset((char) ptr[1], (char) ptr[2]);
                            int dx = (char) ptr[1];
                            int dy = (char) ptr[2];
                            tugas6.moveOffset(-dx / 3, dy / 3);
                            tugas7.moveOffset(-dx / 3, dy / 3);
                        } else {
                            break;
                        }
                    }
                }
                if (mouseY > START_Y_LAYER_BOXES && mouseY < START_Y_LAYER_BOXES + SIZE_Y_LAYER_BOXES &&
                    mouseX > START_X_LAYER_BOXES && mouseX < START_X_LAYER_BOXES + 7 * SIZE_X_LAYER_BOXES) {
                    int position = (mouseX - START_X_LAYER_BOXES) / SIZE_X_LAYER_BOXES;
                    // cout << position << endl;
                    tugas6.toggleLayerVisibility(position);
                }
            } else {
                dragged = false;
            }
        }
    }
}

void* startScene(void* arg) {
    pthread_cancel(tid_master[2]);
    switch (current_scene) {
        case 1:
            tugas1.execute(&current_scene);
            break;
        case 2:
            tugas2.execute(&current_scene, &keyboard_character);
            break;
        case 3:
            tugas3.execute(&current_scene);
            break;
        case 5:
            tugas5 = Tugas5();
            tugas5.execute(&current_scene);
        case 6:
            tugas6.reset();
            pthread_create(&(tid_master[2]), NULL, &readMouse, NULL);
            tugas6.execute(&current_scene);
            break;
        case 7:
            pthread_create(&(tid_master[2]), NULL, &readMouse, NULL);
            tugas7.execute(&current_scene);
            break;
        default:
            break;
    }
}

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

void* readInputMaster(void *arg) {
    while (1) {
        initscr();
        cbreak();
        noecho();
        nodelay(stdscr, TRUE);
        scrollok(stdscr, TRUE);
        char ch;

        while (1) {
            if (kbhit()) {
                char ch = getch();
                if (ch == 'q' || ch == 'Q') {
                    current_scene = 7;
                    pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
                }
                if (ch == '#') {
                    string line;
                    cin >> line;
                    tugas7.changeColor(line);
                }
                if (ch != '\n') {
                    keyboard_character = ch;
                }
                if (ch == 'z')
                    tugas5.zoomX(0.1);
                else if (ch == 'x')
                    tugas5.zoomX(-0.1);
                if (ch == '\033') { // if the first value is esc
                    getch(); // skip the [
                    ch = getch();
                    if (ch == '[') {
                        ch = getch();
                        pthread_cancel(tid_master[1]);
                        if (ch == 'A') {
                            current_scene = 1;
                            pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
                        } else if (ch == 'B') {
                            current_scene = 2;
                            pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
                        } else if (ch == 'C') {
                            current_scene = 3;
                            pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
                        } else if (ch == 'D') {
                            current_scene = 6;
                            pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
                        } else if (ch == 'E') {
                            current_scene = 5;
                            pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
                        }
                    } else {
                        tugas6.moveClippingPlane(ch);
                    }
                } else {
                    tugas6.zoomAndLayer(ch);
                }
            }
        }
        // char input = getchar();
        // if (input != '\n') {
        //     keyboard_character = input;
        // }
        // if (input == 0x1b) {
        //     input = getchar();
        //     input = getchar();
        //     input = getchar();
        //     pthread_cancel(tid_master[1]);
        //     if (input == 'A') {
        //         current_scene = 1;
        //         pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
        //     } else if (input == 'B') {
        //         current_scene = 2;
        //         pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
        //     } else if (input == 'C') {
        //         current_scene = 3;
        //         pthread_create(&(tid_master[1]), NULL, &startScene, NULL);
        //     }
        // }
    }

    return NULL;
}

int main() {
    current_scene = 1;
    pthread_create(&(tid_master[0]), NULL, &readInputMaster, NULL);
    pthread_create(&(tid_master[2]), NULL, &readMouse, NULL);

    while (1);

    return 0;
}