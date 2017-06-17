#include "editor.h"
#include <wchar.h>
#include <locale.h>

int cols, rows;

void initialize_all(int argc, char **argv)
{
    setlocale(LC_ALL,"en_US.UTF-8");

    filename = (argc >= 2) ? argv[1] : "Untitled.txt";

    load_file(argc >= 2);

    cols = termsize(SZ_COLS);
    rows = termsize(SZ_ROWS);

    max_buffer_cols = cols - 4;
    max_buffer_rows = rows - 4;
    buffer_offset   = 0;
    line_offset     = 0;

    editor_reset();

    signal(SIGINT,catchkill);
    signal(SIGQUIT,catchkill);
    signal(SIGTSTP,catchkill);

    stdin_set();

    reprint_buffer(stdout);
}

void editor_reset()
{
    screen_clear();

    print_color_line(cols,BG_GRAY);
    int i = 2;
    while (i++ < rows) {
        print_edged_color_line();
        putwchar('\n');
    }
    print_color_line(cols,BG_CYAN);

    cursor_set(2,2);
    print_box_top();
    cursor_set(2,rows-1);
    print_box_bottom();

    print_in_center(rows,"Ctrl+X Exit    Ctrl+C Save and Exit   Ctrl+V Save",cols,FG_BLACK,BG_CYAN);
    print_in_center(2,filename,cols,FG_BLACK,BG_CYAN);

    putwchar('\n');
    reprint_line("");

    fflush(stdout);
}

void catchkill(int sig)
{
    if (sig == EXITSAVE) {   // save and exit
        stdin_reset();

        FILE *out = fopen(filename,"w");
        reprint_buffer(out);
        fclose(out);

        set_color(RESET);
        screen_clear();

        exit(EXIT_SUCCESS);
    }
    else if (sig == SAVE) {   // save
        FILE *out = fopen(filename,"w");
        reprint_buffer(out);
        fclose(out);

        editor_reset();
        reprint_buffer(stdout);

        cursor_set(pos.x+3,pos.y+3);
        show_cursor();
    }
    else if (sig == EXIT) {   // exit
        stdin_reset();
        set_color(RESET);
        screen_clear();
        exit(EXIT_SUCCESS);
    }
    else if (sig == EXIT_FAILURE) {   // exit, we failed
        stdin_reset();
        set_color(RESET);
        exit(EXIT_FAILURE);
    }
}
