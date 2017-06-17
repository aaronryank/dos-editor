#include "editor.h"

void handle_escape(int *c)
{
    (*c) = getchar();

    if ((*c) != 91)
        return;

    control = getchar();
    (*c) = control;
    control = 1;

    if (((*c) >= 49) && ((*c) <= 54))
        getchar();
}

void insert_newline(void)
{
    pos.y++;
    lines++;
    push_bufY();
    if (buffer[pos.y-1][pos.x]) {
        char *newstr = &buffer[pos.y-1][pos.x];
        memcpy(buffer[pos.y],newstr,strlen(newstr));

        while (buffer[pos.y-1][pos.x])
            buffer[pos.y-1][pos.x++] = 0;
    }
    reprint_buffer(stdout);
    pos.x = 0;
}

void insert_backspace(void)
{
    if (pos.x > 0) {
        pos.x--;
        wprintf(L"\b \b");
        pop_bufX();
        reprint_line(buffer[pos.y]);
    } else if (pos.y > 0) {
        int up_max_char, _up_max_char, i;
        for (up_max_char = 0; buffer[pos.y-1][up_max_char]; up_max_char++);
        _up_max_char = up_max_char;
        for (i = 0; buffer[pos.y][i]; buffer[pos.y-1][up_max_char++] = buffer[pos.y][i++]);
        pop_bufY();
        pos.y--;
        pos.x = _up_max_char;
        editor_reset();
        lines--;
        reprint_buffer(stdout);
   }
}

void insert_delete()
{
    if (!buffer[pos.y][pos.x+1] && (lines > 1)) {
        int down_max_char, x, i;
        for (down_max_char = 0; buffer[pos.y+1][down_max_char]; down_max_char++);
        x = pos.x;
        for (i = 0; i < down_max_char; buffer[pos.y][x++] = buffer[pos.y+1][i++]);
        pos.y++;
        pop_bufY();
        pos.y--;
        editor_reset();
        lines--;
        reprint_buffer(stdout);
    } else {
        pop_bufX();
        reprint_line(buffer[pos.y]);
    }
}

void insert_character(int c)
{
    push_bufX(c);
    reprint_line(buffer[pos.y]);
    pos.x++;
}
