#pragma once

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <wchar.h>
#include "termsize.h"
#include "colors.h"
#include "stdin.h"
#include "position.h"

typedef struct {
    int x, y;
} coord;

enum { CTRL_L = 12,
       ESC = 27,
       CTRL_BACKSPACE = 127
};

#define EXITSAVE SIGINT
#define EXIT     0
#define SAVE     1

#define CLEAR_SCREEN L"\e[H\e[2J"
#define putcharat(x,y,c) wprintf(L"\e[%d;%dH%c",y,x,c);
#define printat(x,y,s)   wprintf(L"\e[%d;%dH%s",y,x,s);
#define cursor_set(x,y)  wprintf(L"\e[%d;%dH",(y),(x));
#define screen_clear()   wprintf(CLEAR_SCREEN);

extern void reprint_line(char *);
extern void print_line(const int, int);
extern void print_color_line(const int, int);
extern void print_edged_color_line(void);
extern void print_in_center(int, const char *, const int, int, int);

extern void editor_reset(void);
extern void initialize_all(int argc, char **argv);

extern void catchkill(int);
extern char *filename;
extern coord pos;
extern int lines, control;

extern char buffer[2000][2000];
extern int cols, rows;
extern int max_buffer_cols, max_buffer_rows;
extern int buffer_offset, line_offset;
extern int offset_buffer(void);
extern void reprint_buffer(FILE *);
extern void write_buffer(FILE *);
extern void display_buffer(void);
extern void pop_bufX(void);
extern void push_bufX(char);
extern void pop_bufY(void);
extern void push_bufY(void);
extern void load_file(int);

#define isarrow(c)  (control && (c >= 65) && (c <= 68))
#define isdelete(c) (control && (c == 51))

extern void handle_escape(int *);
extern void insert_newline(void);
extern void insert_backspace(void);
extern void insert_delete(void);
extern void insert_character(int);
extern void check_position(void);
extern void show_cursor(void);

extern void print_box_top(void);
extern void print_box_bottom(void);

#define BOX_UPPER_LEFT_CORNER  L"\u250C"
#define BOX_UPPER_RIGHT_CORNER L"\u2510"
#define BOX_HORIZONTAL_LINE    L"\u2500"
#define BOX_VERTICAL_LINE      L"\u2502"
#define BOX_LOWER_LEFT_CORNER  L"\u2514"
#define BOX_LOWER_RIGHT_CORNER L"\u2518"
