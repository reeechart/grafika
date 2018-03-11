#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <pthread.h>

#define laserRange 200

int laserCounter = 0;

// void* readInputTugas2(char *arg) {
//     unsigned long i = 0;
//     char c = arg[0];

//     for(i=0; i<(0xFFFFFFFF);i++) {
//         char s[256];
//         if (c == ' ')
//             laserCounter++;
//     }

//     return NULL;
// }

class Tugas2 {
    private:
        typedef struct Characters {
            char content;
            short red, green, blue;
            short i,j;
        } Character;

        typedef struct CharacterData {
            short points[16][2];
            short strokes;
        } CharacterData;

        // global variable
        char *fbp = 0;
        struct fb_var_screeninfo vinfo;
        struct fb_fix_screeninfo finfo;
        CharacterData alphabets[26];
        CharacterData numbers[10];
        CharacterData symbols[3];
        CharacterData plane;
        Character credit[255];
        pthread_t tid[2];

        void loadCreditContent(int i_start, int j_start) {
            FILE *fp;
            char x;
            int c = -1;
            int i = i_start,j= j_start;
            int row = 0;
            int red = 0, green = 240, blue = 200;
            fp = fopen("line_credits/credit_content.txt", "r");
            while(fscanf(fp, "%c", &x) != EOF) {
                if (x != '\n') {
                    ++c;
                    credit[c].content = x;
                    credit[c].red = red;
                    credit[c].green = green;
                    credit[c].blue = blue;
                    credit[c].i = i;
                    credit[c].j = j;
                    j += 22;
                } else {
                    j = j_start;
                    i += 112;
                    ++row;
                    if (row == 1) {
                        red = 255;
                        green = 255;
                        blue = 255;
                    } else if (row == 2) {
                        red = 160;
                        green = 200;
                        blue = 220;
                    } else if (row == 3) {
                        red = 240;
                        green = 120;
                        blue = 0;
                    } else if (row == 4) {
                        red = 250;
                        green = 0;
                        blue = 200;
                    } else if (row == 5) {
                        red = 0;
                        green = 255;
                        blue = 0;
                    } else if (row == 6) {
                        red = 160;
                        green = 0;
                        blue = 0;
                    } else if (row == 7) {
                        red = 30;
                        green = 60;
                        blue = 240;
                    } else {
                        red = 180;
                        green = 160;
                        blue = 255;
                    }
                }
            }
        }

        void loadCharacters() {
            FILE *fp;

            for (char c = 'A'; c <= 'Z'; c++) {
                short x;
                int i = 0;
                char filename[22];
                filename[0] = 'l';
                filename[1] = 'i';
                filename[2] = 'n';
                filename[3] = 'e';
                filename[4] = '_';
                filename[5] = 'c';
                filename[6] = 'r';
                filename[7] = 'e';
                filename[8] = 'd';
                filename[9] = 'i';
                filename[10] = 't';
                filename[11] = 's';
                filename[12] = '/';
                filename[13] = 'a';
                filename[14] = 's';
                filename[15] = 's';
                filename[16] = 'e';
                filename[17] = 't';
                filename[18] = 's';
                filename[19] = '/';
                filename[20] = c;
                filename[21] = '\0';
                fp = fopen(filename, "r");
                while (fscanf(fp, " %hd ", &x) != EOF) {
                    alphabets[c-'A'].points[i/2][i%2] = x;
                    i++;
                }
                alphabets[c-'A'].strokes = i/4;
                fclose(fp);
            }

            for (char c = '0'; c <= '9'; c++) {
                short x;
                int i = 0;
                char filename[22];
                filename[0] = 'l';
                filename[1] = 'i';
                filename[2] = 'n';
                filename[3] = 'e';
                filename[4] = '_';
                filename[5] = 'c';
                filename[6] = 'r';
                filename[7] = 'e';
                filename[8] = 'd';
                filename[9] = 'i';
                filename[10] = 't';
                filename[11] = 's';
                filename[12] = '/';
                filename[13] = 'a';
                filename[14] = 's';
                filename[15] = 's';
                filename[16] = 'e';
                filename[17] = 't';
                filename[18] = 's';
                filename[19] = '/';
                filename[20] = c;
                filename[21] = '\0';
                fp = fopen(filename, "r");
                while (fscanf(fp, " %hd ", &x) != EOF) {
                    numbers[c-'0'].points[i/2][i%2] = x;
                    i++;
                }
                numbers[c-'0'].strokes = i/4;
                fclose(fp);
            }

            short x;
            int i = 0;
            char filename[22];
            filename[0] = 'l';
            filename[1] = 'i';
            filename[2] = 'n';
            filename[3] = 'e';
            filename[4] = '_';
            filename[5] = 'c';
            filename[6] = 'r';
            filename[7] = 'e';
            filename[8] = 'd';
            filename[9] = 'i';
            filename[10] = 't';
            filename[11] = 's';
            filename[12] = '/';
            filename[13] = 'a';
            filename[14] = 's';
            filename[15] = 's';
            filename[16] = 'e';
            filename[17] = 't';
            filename[18] = 's';
            filename[19] = '/';
            filename[20] = ':';
            filename[21] = '\0';
            fp = fopen(filename, "r");
            while (fscanf(fp, " %hd ", &x) != EOF) {
                symbols[0].points[i/2][i%2] = x;
                i++;
            }
            symbols[0].strokes = i/4;
            fclose(fp);

            i = 0;
            filename[20] = '-';
            fp = fopen(filename, "r");
            while (fscanf(fp, " %hd ", &x) != EOF) {
                symbols[1].points[i/2][i%2] = x;
                i++;
            }
            symbols[1].strokes = i/4;
            fclose(fp);

            i = 0;
            filename[20] = '<';
            fp = fopen(filename, "r");
            while (fscanf(fp, " %hd ", &x) != EOF) {
                plane.points[i/2][i%2] = x;
                i++;
            }
            plane.strokes = i/4;
            fclose(fp);
        }

        void printPixel(int i, int j, int opacity, int blue, int green, int red) {
            int location;
            if (j < 0 || j >= 1366 || i < 0 || i >= 760) {
                // out of bound, no need to print
            } else {
                location = (j+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                        (i+vinfo.yoffset) * finfo.line_length;
                *(fbp + location) = blue;        // Some blue
                *(fbp + location + 1) = green;     // A little green
                *(fbp + location + 2) = red;    // A lot of red
                *(fbp + location + 3) = opacity;      // No transparency
            }
        }

        void printLine(int i_start, int j_start, int x0, int y0, int x1, int y1, int blue, int green, int red) {
            int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
            int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
            int err = (dx>dy ? dx : -dy)/2, e2;

            for(;;){
                printPixel(i_start + x0, j_start + y0, 0, blue, green, red);
                if (x0==x1 && y0==y1) break;
                e2 = err;
                if (e2 >-dx) { err -= dy; x0 += sx; }
                if (e2 < dy) { err += dx; y0 += sy; }
            }
        }

        void printLaser(int i_start, int j_start, int x0, int y0, int x1, int y1, int blue, int green, int red, int counter) {
            int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
            int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
            int err = (dx>dy ? dx : -dy)/2, e2;
            int i = 0;
            int count = 0;
            for(;;){
                if (count > counter) break;
                if (count >= counter - laserRange) {
                    if (i < 5)
                        printPixel(i_start + x0, j_start + y0, 0, blue, green, red);
                    else if (i == 10) {
                        i = 0;
                    }
                    ++i;
                }
                ++count;
                if (x0==x1 && y0==y1) break;
                e2 = err;
                if (e2 >-dx) { err -= dy; x0 += sx; }
                if (e2 < dy) { err += dx; y0 += sy; }
            }
        }

        void printCharacter(int i_start, int j_start, int opacity, int blue, int green, int red, char content) {
            int i;
            if (content >= 'A' && content <= 'Z') {
                for (i = 0; i < alphabets[content-'A'].strokes; i++) {
                    printLine(i_start, j_start, alphabets[content - 'A'].points[i * 2][0], alphabets[content - 'A'].points[i * 2][1], alphabets[content - 'A'].points[i*2 + 1][0],
                        alphabets[content - 'A'].points[i*2 + 1][1], blue, green, red);
                    printLine(i_start + 1, j_start + 1, alphabets[content - 'A'].points[i * 2][0], alphabets[content - 'A'].points[i * 2][1], alphabets[content - 'A'].points[i*2 + 1][0],
                        alphabets[content - 'A'].points[i*2 + 1][1], blue, green, red);
                }
            } else if (content >= '0' && content <= '9') {
                for (i = 0; i < numbers[content - '0'].strokes; i++) {
                    printLine(i_start, j_start, numbers[content-'0'].points[i * 2][0], numbers[content - '0'].points[i * 2][1], numbers[content - '0'].points[i*2 + 1][0],
                        numbers[content - '0'].points[i*2 + 1][1], blue, green, red);
                    printLine(i_start + 1, j_start + 1, numbers[content-'0'].points[i * 2][0], numbers[content - '0'].points[i * 2][1], numbers[content - '0'].points[i*2 + 1][0],
                        numbers[content - '0'].points[i*2 + 1][1], blue, green, red);
                }
            } else if (content == ' ') {
                printPixel(i_start, j_start, 0, 0, 0, 0);
            } else if (content == ':') {
                for (i = 0; i < symbols[0].strokes; i++) {
                    printLine(i_start, j_start, symbols[0].points[i * 2][0], symbols[0].points[i * 2][1], symbols[0].points[i*2 + 1][0], symbols[0].points[i*2 + 1][1],
                        blue, green, red);
                    printLine(i_start + 1, j_start + 1, symbols[0].points[i * 2][0], symbols[0].points[i * 2][1], symbols[0].points[i*2 + 1][0], symbols[0].points[i*2 + 1][1],
                        blue, green, red);
                }
            } else if (content == '-') {
                for (i = 0; i < symbols[1].strokes; i++) {
                    printLine(i_start, j_start, symbols[1].points[i * 2][0], symbols[1].points[i * 2][1], symbols[1].points[i*2 + 1][0], symbols[1].points[i*2 + 1][1],
                        blue, green, red);
                    printLine(i_start + 1, j_start + 1, symbols[1].points[i * 2][0], symbols[1].points[i * 2][1], symbols[1].points[i*2 + 1][0], symbols[1].points[i*2 + 1][1],
                        blue, green, red);
                }
            } else if (content == '<') {
            for (i = 0; i < plane.strokes; i++) {
                printLine(i_start, j_start, plane.points[i * 2][0], plane.points[i * 2][1], plane.points[i*2 + 1][0],
                plane.points[i*2 + 1][1], blue, green, red);
                printLine(i_start+1, j_start+1, plane.points[i * 2][0], plane.points[i * 2][1], plane.points[i*2 + 1][0],
                plane.points[i*2 + 1][1], blue, green, red);
            }
            }
        }

    public:

        void execute(int* current_scene, char* keyboard_character) {
            const int MY_SCENE_NUMBER = 2;
            int fbfd = 0;
            // struct fb_var_screeninfo vinfo;
            // struct fb_fix_screeninfo finfo;
            long int screensize = 0;
            int x = 0, y = 0;
            int i_start, j_start;
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
            i_start = 800;
            j_start = 400;
            // load credit content from txt
            loadCreditContent(i_start,j_start);

            // load characters format from txt
            loadCharacters();

            int laserA = 0, laserB = 0, laserC = 0;

            // another thread to read input
            // pthread_create(&(tid[0]), NULL, &readInputTugas2, keyboard_character);

            for(long int a = 0 ; a < 999999999; a++ ) {
                if (*keyboard_character == ' ') {
                    laserCounter++;
                    *keyboard_character = '0';
                }
                if (*current_scene != MY_SCENE_NUMBER) {
                    return;
                }
                for (y = 0; y < 760; y++) {
                    for (x = 0; x < 1366; x++) {
                        printPixel(y, x, 0, 0, 0, 0);
                    }
                }
                printCharacter(50, 1400-a, 0, 255, 255, 255, '<');
                #include <iostream>
                using namespace std;
                for(int i = 3; i < 234; i++) {
                    // cout << credit[i].content;
                    printCharacter(credit[i].i-a, credit[i].j, 0, credit[i].blue, credit[i].green, credit[i].red, credit[i].content);
                }

                // printing laser
                if (laserCounter > 0) {
                laserA++;
                printLaser(0, 680, 800, 0, 0, 0, 255, 255, 255, laserA);
                }
                if (laserCounter > 1) {
                laserB++;
                printLaser(0, 680, 800, 0, 0, 680, 255, 255, 255, laserB);
                }
                if (laserCounter > 2) {
                laserC++;
                printLaser(0, 680, 800, 0, 0, -680, 255, 255, 255, laserC);
                }

                usleep(3000);
            }

            munmap(fbp, screensize);
            close(fbfd);
            while (1) {
                char x;
                scanf("%c", &x);
            }
        }
};