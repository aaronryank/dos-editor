#include "editor.h"
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>

char buffer[2000][2000] = {{0}};

int dummy(void){return 0;}
#define DUMMY(...) dummy()

void load_file(int init)
{
    FILE *in = fopen(filename,"r");

    if (!in || !init) {
        int fd = creat(filename,0644);
        if (fd == -1) {
            screen_clear();
            wprintf(L"Error opening file `%s': %s\n",filename,strerror(errno));
            catchkill(EXIT_FAILURE);
        }
        close(fd);
        return;
    }

    int x, y, c;
    x = y = c = 0;
    while ((c = getc(in)) != EOF)
    {
        if (c == '\n') {
            lines++;
            y++;
            x = 0;
        } else {
            buffer[y][x] = c;
            x++;
        }
    }
    fclose(in);
}

int offset_buffer(void)
{
    int changes = 0;

    if ((pos.y-buffer_offset) >= max_buffer_rows) {
        buffer_offset++;
        changes = 1;
    }
    else if (!(pos.y-buffer_offset+1)) {
        buffer_offset--;
        changes = 1;
    }

    if ((pos.x-line_offset) >= max_buffer_cols) {
        line_offset++;
        changes = 1;
    }
    else if (!(pos.x-line_offset+1)) {
        line_offset--;
        changes = 1;
    }

    return changes;
}

void reprint_buffer(FILE *out)
{
    editor_reset();
    offset_buffer(); // FIX DUPLICATE CALL

    if (out == stdout)
        display_buffer();
    else
        write_buffer(out);
}

void write_buffer(FILE *out)
{
    int i, j;
    for (i = 0; i < lines; i++) {
        for (j = 0; buffer[i][j]; j++)
            isprint(buffer[i][j]) ? putc(buffer[i][j],out) : DUMMY();
        putc('\n',out);
    }
}

void display_buffer(void)
{
    int i, j;
    for (i = buffer_offset; (i < lines+buffer_offset) && (i < max_buffer_rows+buffer_offset); i++) {
        set_color(FG_GRAY);
        for (j = line_offset; buffer[i][j] && (i < max_buffer_cols+line_offset); j++)
            isprint(buffer[i][j]) ? wprintf(L"%c",buffer[i][j]) : DUMMY();
        putwchar('\n');
        reprint_line("");
    }
}

void pop_bufX(void)
{
    int i, l = strlen(buffer[pos.y]);
    for (i = pos.x; i <= l; i++)
        buffer[pos.y][i] = buffer[pos.y][i+1];
}

void push_bufX(char c)
{
    int i, l = strlen(buffer[pos.y]);
    for (i = l; i >= pos.x; i--)
        buffer[pos.y][i] = buffer[pos.y][i-1];

    buffer[pos.y][pos.x] = c;
}

void pop_bufY(void)
{
    int i;
    for (i = pos.y; i < lines; i++) {
        memset(buffer[i],0,2000);
        memcpy(buffer[i],buffer[i+1],strlen(buffer[i+1]));
    }
}

void push_bufY(void)
{
    int i;
    for (i = lines+1; i > pos.y; i--) {
        memset(buffer[i],0,2000);
        memcpy(buffer[i],buffer[i-1],strlen(buffer[i-1]));
    }
    memset(buffer[pos.y],0,2000);
}
