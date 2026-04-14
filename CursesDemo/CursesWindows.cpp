#include <curses.h>
#include <string.h>
#include <stdlib.h>

#define MAX_INPUT 256
#define MAX_LINES 1000

void draw_bordered_window(WINDOW* win, const char* title) {
    box(win, 0, 0);
    int width = getmaxx(win);
    mvwprintw(win, 0, (width - strlen(title)) / 2, "%s", title);
    wrefresh(win);
}

void draw_output(WINDOW* win, char** lines, int lineCount, int scrollPos) {
    int maxY, maxX;
    getmaxyx(win, maxY, maxX);
    werase(win);
    box(win, 0, 0);

    // Show visible lines
    for (int i = 0; i < maxY - 2; ++i) {
        int idx = scrollPos + i;
        if (idx >= lineCount) break;
        mvwprintw(win, i + 1, 1, "%s", lines[idx]);
    }

    // Simulated scroll bar
    if (lineCount > maxY - 2) {
        int barHeight = maxY - 2;
        int barPos = (scrollPos * barHeight) / lineCount;
        mvwaddch(win, barPos + 1, maxX - 2, ACS_CKBOARD);
    }

    wrefresh(win);
}

int main() {
    WINDOW* win1, * win2, * inputWin;
    int height, width;
    char input[MAX_INPUT];
    int toWin1 = 1;

    // Buffers
    char* lines1[MAX_LINES];
    char* lines2[MAX_LINES];
    int count1 = 0, count2 = 0;
    int scroll1 = 0, scroll2 = 0;

    // Init
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    height = LINES;
    width = COLS;
    int win1Height = height / 3;
    int win2Height = height / 3;
    int inputHeight = height - win1Height - win2Height;

    win1 = newwin(win1Height, width, 0, 0);
    win2 = newwin(win2Height, width, win1Height, 0);
    inputWin = newwin(inputHeight, width, win1Height + win2Height, 0);

    scrollok(win1, TRUE);
    scrollok(win2, TRUE);

    while (1) {
        // Draw outputs
        draw_output(win1, lines1, count1, scroll1);
        draw_output(win2, lines2, count2, scroll2);

        // Draw input
        werase(inputWin);
        box(inputWin, 0, 0);
        mvwprintw(inputWin, 1, 2, "Enter text for Window %d (type 'exit' to quit): ", toWin1 ? 1 : 2);
        wrefresh(inputWin);

        // Input
        echo();
        mvwgetnstr(inputWin, 2, 2, input, MAX_INPUT - 1);
        noecho();

        if (strcmp(input, "exit") == 0)
            break;

        // Handle scrolling commands (when input is empty)
        if (strlen(input) == 0) {
            int ch = wgetch(inputWin);
            if (toWin1) {
                if (ch == KEY_UP && scroll1 > 0) scroll1--;
                else if (ch == KEY_DOWN && scroll1 < count1 - (win1Height - 2)) scroll1++;
            }
            else {
                if (ch == KEY_UP && scroll2 > 0) scroll2--;
                else if (ch == KEY_DOWN && scroll2 < count2 - (win2Height - 2)) scroll2++;
            }
            continue;
        }

        // Store line
        char* copy = _strdup(input);  // Duplicate the input string

        if (toWin1) {
            if (count1 < MAX_LINES) {
                lines1[count1++] = copy;
                if (count1 > win1Height - 2) scroll1 = count1 - (win1Height - 2);
            }
        }
        else {
            if (count2 < MAX_LINES) {
                lines2[count2++] = copy;
                if (count2 > win2Height - 2) scroll2 = count2 - (win2Height - 2);
            }
        }

        toWin1 = !toWin1;
    }

    // Cleanup
    for (int i = 0; i < count1; ++i) free(lines1[i]);
    for (int i = 0; i < count2; ++i) free(lines2[i]);
    delwin(win1);
    delwin(win2);
    delwin(inputWin);
    endwin();
    return 0;
}
