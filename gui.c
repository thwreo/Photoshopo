#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define CHANNEL 3

void editImg(char *filename, char *command, int width, int height, 
int length, u_int8_t *img, u_int8_t *new_img, u_int8_t *cpy_img) {
    if (!strcmp(command, "negative")) {
            for (int i = 0; i < length; i++) {
                new_img[i] = 255 - img[i];
            }
            stbi_write_png(filename, width, height, CHANNEL, new_img, width * CHANNEL);
        } else if (!strcmp(command, "grayscale")) {
            int sum = 0;
            for (int i = 0; i < length; i++) {
                if (i % 3 == 0) {
                    sum += 0.3 * img[i];
                } else if (i % 3 == 1) {
                    sum += 0.59 * img[i];
                } else {
                    sum += 0.11 * img[i];
                    new_img[i - 2] = sum;
                    new_img[i - 1] = sum;
                    new_img[i] = sum;
                    sum = 0;
                }
            }
            stbi_write_png(filename, width, height, CHANNEL, new_img, width * CHANNEL);
        } else if (!strcmp(command, "nored")) {
            for (int i = 0; i < length; i++) {
                if (i % 3 == 1) {
                    new_img[i] = 0;
                } else {
                    new_img[i] = img[i];
                }
            }
            stbi_write_png(filename, width, height, CHANNEL, new_img, width * CHANNEL);
        } else if (!strcmp(command, "default")) {
            stbi_write_png(filename, width, height, CHANNEL, cpy_img, width * CHANNEL);
        }
}

int main(int argc, char* argv[]) {
    char list[5][12] = { "negative", "grayscale", "nored", "default", "exit" };
    char item[12];
    int ch, i;
    bool quit = FALSE;

    char *filename = strdup(argv[1]);
    int width, height, bpp;
    u_int8_t *img = stbi_load(filename, &width, &height, &bpp, CHANNEL);

    int length = width * height * CHANNEL;
    u_int8_t *new_img = malloc(length);
    
    char command[20];
    u_int8_t *cpy_img = img;

    initscr();

    WINDOW *win;
    win = newwin(7, 15, 1, 1);
    box(win, 0, 0);

    for ( i = 0; i < 5; i++) {
        if (i == 0) {
            wattron(win, A_STANDOUT);
        } else {
            wattroff(win, A_STANDOUT);
        }
        mvwprintw(win, i + 1, 2, "%s", list[i]);
    }
    wrefresh(win);
    i = 0;
    noecho();
    keypad(win, TRUE);
    curs_set(0);
    
    while(quit != TRUE) {

        ch = wgetch(win);
        if (ch) {
            wattroff(win, A_STANDOUT);
            mvwprintw(win, i + 1, 2, "%s", list[i]);
        }
        switch( ch ) {
            case KEY_UP:
                i--;
                i = (i < 0) ? 4 : i;
                break;
            case KEY_DOWN:
                i++;
                i = (i > 4) ? 0 : i;
                break;
            case 10:
                if (strcmp(list[i], "exit") == 0) {
                    quit = TRUE;
                } else {
                    editImg(filename, list[i], width, height, 
                    length, img, new_img, cpy_img);
                }
        }
        wattron(win, A_STANDOUT);
        mvwprintw(win, i + 1, 2, "%s", list[i]);
    }

    delwin(win);
    endwin();

    free(new_img);
    free(filename);

    stbi_image_free(img);
    return 0;
}