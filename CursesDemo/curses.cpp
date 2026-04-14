#include <curses.h>

int main()
{
    initscr();
    printw("Hello from PDCurses!");
    refresh();
    getch();
    endwin();
    return 0;
}