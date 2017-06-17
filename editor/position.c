#include "editor.h"

void move_position_up(void)
{
    if (pos.y > 0)
        pos.y--;
    if (pos.x)
        while (!buffer[pos.y][pos.x-1])
            pos.x--;
}

void move_position_down(void)
{
    if (pos.y < lines-1)
        pos.y++;
    if (pos.x)
        while (!buffer[pos.y][pos.x-1])
            pos.x--;
}

void move_position_right(void)
{
    if (buffer[pos.y][pos.x])
        pos.x++;
    else if (pos.y < lines-1)
        do { pos.y++; pos.x = 0; } while (0);
}

void move_position_left(void)
{
    if (pos.x > 0)
        pos.x--;
    else if (pos.y > 0)
        for (pos.y--; buffer[pos.y][pos.x]; pos.x++)
            ;
}

void check_position(void)
{
    if (pos.x < 0)
        pos.x = 0;
    if (pos.y < 0)
        pos.y = 0;
}
