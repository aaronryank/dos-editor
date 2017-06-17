#include "editor.h"
#include <unistd.h>

char *filename;

int control, lines = 0;

coord pos = {0,0};

int main(int argc, char **argv)
{
    initialize_all(argc,argv);

    while (1) {
        control = 0;
        int c = getchar();
        cursor_set((pos.x+3)-line_offset,(pos.y+3)-buffer_offset);
        if (c == ESC)
            handle_escape(&c);

        if (isarrow(c)) {
            switch (c - 64) {
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
            if (offset_buffer())
                reprint_buffer(stdout);
        } else if (c == 22) {
            catchkill(SAVE);
        } else if (c == 24) {
            catchkill(EXIT);
        } else if (c == '\n') {
            insert_newline();
        } else if (c == CTRL_BACKSPACE || c == '\b') {
            insert_backspace();
        } else if (isdelete(c)) {
            insert_delete();
        } else if (c == CTRL_L) {
            reprint_buffer(stdout);
        } else {
            insert_character(c);
        }
        if (lines == 0)
            lines++;
        check_position();
        cursor_set(pos.x+3,pos.y+3-buffer_offset);
        show_cursor();

// TODO: Buffer scrolling right
// TODO: Shortcuts to delete lines and stuff
    }
}

