#include <curses.h>
#include <vector>
#include <string>

// Function to print the menu
void drawMenu(const std::vector<std::string>& items, int highlight) {
    clear();
    for (size_t i = 0; i < items.size(); ++i) {
        if (i == highlight) {
            attron(A_REVERSE); // Highlight selection
            mvprintw(static_cast<int>(i), 0, items[i].c_str());
            attroff(A_REVERSE);
        }
        else {
            mvprintw(static_cast<int>(i), 0, items[i].c_str());
        }
    }
    mvprintw(static_cast<int>(items.size() + 1), 0, "Use up down arrows to navigate, Enter to select, 'q' to quit.");
    refresh();
}

int main() {
    // Initialize PDCurses
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    // Sample menu items
    std::vector<std::string> menuItems = {
        "Option 1: Start",
        "Option 2: Settings",
        "Option 3: Help",
        "Option 4: Exit"
    };

    int highlight = 0;
    int choice = -1;
    int ch;

    drawMenu(menuItems, highlight);

    while ((ch = getch()) != 'q') {
        switch (ch) {
        case KEY_UP:
            highlight = (highlight == 0) ? static_cast<int>(menuItems.size()) - 1 : highlight - 1;
            break;
        case KEY_DOWN:
            highlight = (highlight == static_cast<int>(menuItems.size()) - 1) ? 0 : highlight + 1;
            break;
        case 10: // Enter key
            choice = highlight;
            mvprintw(static_cast<int>(menuItems.size() + 3), 0, "You selected: %s", menuItems[choice].c_str());
            break;
        default:
            break;
        }
        drawMenu(menuItems, highlight);
    }

    endwin(); // Exit curses mode
    return 0;
}