#pragma once

enum {
    RESET = 0,
    BOLD = 1,
    FG_BLACK = 30,
    FG_RED,
    FG_GREEN,
    FG_ORANGE,
    FG_BLUE,
    FG_PURPLE,
    FG_CYAN,
    FG_GRAY,
    BG_BLACK = 40,
    BG_RED,
    BG_GREEN,
    BG_ORANGE,
    BG_BLUE,
    BG_PURPLE,
    BG_CYAN,
    BG_GRAY
} terminal_colors;

#define set_color(c)  wprintf(L"\e[%dm",c);

