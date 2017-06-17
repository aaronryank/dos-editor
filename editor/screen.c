#include "editor.h"

int cols, rows;
int max_buffer_cols, max_buffer_rows;
int buffer_offset, line_offset;

void reprint_line(char *str)
{
    putwchar('\r');
    print_edged_color_line();
    putwchar('\r');
    set_color(FG_GRAY);
    wprintf(L"\r\e[%dm \e[%dm",BG_BLUE,BG_BLACK);
    wprintf(BOX_VERTICAL_LINE);
    set_color(BG_BLUE);

    if (strlen(str) < max_buffer_cols) {
        wprintf(L"%s",str);
    } else {
        offset_buffer();
        wprintf(L"%s",&str[line_offset]);
    }
}

void print_in_center(int row, const char *str, const int cols, int fg, int bg)
{
    int startpos = cols/2 - strlen(str)/2;
    cursor_set(startpos,row);
    set_color(fg);
    set_color(bg);
    wprintf(L"%s",str);
}

void print_edged_color_line(void)
{
    set_color(BG_BLUE);
    putwchar(' ');

    set_color(BG_BLACK);
    set_color(FG_GRAY);
    wprintf(BOX_VERTICAL_LINE);

    print_color_line(cols-4,BG_BLUE);

    set_color(BG_BLACK);
    set_color(FG_GRAY);
    wprintf(BOX_VERTICAL_LINE);

    set_color(BG_BLUE);
    putwchar(' ');

    set_color(RESET);
}

void print_color_line(const int cols, int color)
{
    set_color(color);
    print_line(cols,' ');
    set_color(RESET);
}

void print_line(const int cols, int c)
{
    int i;
    for (i = 0; i < cols; i++)
        putwchar(c);
}

void print_box_top(void)
{
    int i;

    set_color(FG_GRAY);
    set_color(BG_BLACK);
    wprintf(BOX_UPPER_LEFT_CORNER);
    set_color(BG_CYAN);
    for (i = 0; i < cols-4; i++)
        putwchar(' ');
    set_color(BG_BLACK);
    wprintf(BOX_UPPER_RIGHT_CORNER);
}

void print_box_bottom(void)
{
    int i;

    set_color(FG_GRAY);
    set_color(BG_BLACK);
    wprintf(BOX_LOWER_LEFT_CORNER);
    set_color(BG_GRAY);
    for (i = 0; i < cols-4; i++)
        putwchar(' ');
    set_color(BG_BLACK);
    wprintf(BOX_LOWER_RIGHT_CORNER);
}
