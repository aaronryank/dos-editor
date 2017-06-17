#include "editor.h"

void show_cursor(void)
{
    if (!buffer[pos.y][pos.x])
        wprintf(L" \b");
}
