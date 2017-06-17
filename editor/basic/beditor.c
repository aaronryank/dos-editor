#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "stdin.h"

#define ARROW_OFFSET 200
#define isarrow(c)   ((c) - ARROW_OFFSET > 0)
#define catch(sig)   signal(sig,SIG_IGN)

char buffer[2000][2000] = {0};

int filename_set = 0;
char filename[100] = {0};

typedef struct {
    int x, y;
} coord;

coord pos = {0,0};

int dummy(void){}
#define DUMMY(...) dummy()

void reprint_buffer(FILE *out)
{
    int i, j;
    for (i = 0; buffer[i][0]; i++) {
        for (j = 0; buffer[i][j]; j++)
            isprint(buffer[i][j])?putc(buffer[i][j],out):DUMMY();
        putc('\n',out);
    }
}

void catchkill(int sig)
{
    stdin_reset();
    printf("\e[H\e[2J");

    FILE *out = fopen(filename_set?filename:"Untitled.txt","w");
    reprint_buffer(out);
    fclose(out);

    exit(EXIT_SUCCESS);
}

void push_char(char c)
{
    int i, l = strlen(buffer[pos.y]);
    for (i = l; i >= pos.x; i--)
        buffer[pos.y][i] = buffer[pos.y][i-1];

    buffer[pos.y][pos.x] = c;
}

void pop_bufY(void)
{
    int i;
    for (i = pos.y; buffer[i][0]; i++) {
        memset(buffer[i],0,2000);
        memcpy(buffer[i],buffer[i+1],strlen(buffer[i+1]));
    }
}

void push_bufY(void)
{
    int i, l;
    for (l = 0; buffer[l][0]; l++);
    l++;

    for (i = l; i > pos.y; i--) {
        memset(buffer[i],0,2000);
        memcpy(buffer[i],buffer[i-1],strlen(buffer[i-1]));
    }
    memset(buffer[pos.y],0,2000);
    buffer[pos.y][0] = 1;
}

void move_position_up(void);
void move_position_down(void);
void move_position_right(void);
void move_position_left(void);

enum { UP_ARROW = 1, DOWN_ARROW, RIGHT_ARROW, LEFT_ARROW };

int main(int argc, char **argv)
{
    filename_set = !(argc < 2);
    if (argc >= 2)
        memcpy(filename,argv[1],strlen(argv[1]));
    printf("\e[H\e[2J");
    stdin_set();
    int c = 0;

    signal(SIGINT,catchkill);
    signal(SIGTSTP,catchkill);

    while (1)
    {
        c = getchar();
        if (c == 27)
            if (getchar() == 91)
                c = getchar() + ARROW_OFFSET;

        if (isarrow(c))
        {
            switch (c - ARROW_OFFSET - 64) {
              case UP_ARROW:
                move_position_up();
                break;
              case DOWN_ARROW:
                move_position_down();
                break;
              case RIGHT_ARROW:
                move_position_right();
                break;
              case LEFT_ARROW:
                move_position_left();
                break;
            }
        } else {
            printf("\e[%d;%dH",pos.y+2,pos.x+1);
            if (c == '\n') {
                pos.y++;
                push_bufY();
                printf("\e[H\e[2J\n");
                reprint_buffer(stdout);
                buffer[pos.y][0] = 0;
                pos.x = 0;
            } else if (c == 127 || c == '\b') {
                if (pos.x-1 >= 0) {
                    pos.x--;
                    printf("\b \b");
                    buffer[pos.y][pos.x] = 0;
                } else {
                    int up_max_char, curr_max_char, i = 0;
                    for (up_max_char = 0;   buffer[pos.y-1][up_max_char]; up_max_char++);

                    while (buffer[pos.y][i]) {
                        buffer[pos.y-1][up_max_char++] = buffer[pos.y][i++];
                    }
                    pop_bufY();
                    pos.y--;
                    pos.x = up_max_char;
                    printf("\e[H\e[2J\n");
                    reprint_buffer(stdout);
                }
            } else {
                push_char(c);
                printf("\r%s",buffer[pos.y]);
                buffer[pos.y][pos.x] = c;
                pos.x++;
            }
        }
        //printf("\e[%d;%dH",pos.y+1,pos.x+1);
        printf("\e[H                           \rCURRENT POSITION %d:%d\n\e[%d;%dH",pos.x,pos.y,pos.y+2,pos.x+1);
    }
}

void move_position_up(void)
{
    if (pos.y-1 >= 0)
        pos.y--;
    if (pos.x)
        while (!buffer[pos.y][pos.x-1])
            pos.x--;
}

void move_position_down(void)
{
    if (buffer[pos.y+1][0])
        pos.y++;
    if (pos.x)
        while (!buffer[pos.y][pos.x-1])
            pos.x--;
}

void move_position_right(void)
{
    if (buffer[pos.y][pos.x])
        pos.x++;
    else if (buffer[pos.y+1][0])
        do { pos.y++; pos.x = 0; } while (0);
}

void move_position_left(void)
{
    if (pos.x-1 >= 0)
        pos.x--;
    else if (pos.y-1 >= 0)
        for (pos.y--; buffer[pos.y][pos.x]; pos.x++)
            ;
}

/* TODO:
DONE Fix newline (so that it ACTUALLY INSERTS A NEWLINE)
DONE Fix backspace on pos=0:*
DONE Fix right arrow (so that it can go to the space after the last character)
*/
