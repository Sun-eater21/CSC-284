#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>

using namespace std;

void clearScreen() {
    system("cls");
}

int main() {
    vector<string> rooms = {"General", "Sports", "Programming"};
    int selected = 0;

    while (true) {
        clearScreen();

        cout << "=== Advanced C++ Chat Client ===\n\n";

        cout << "Rooms:\n";
        for (int i = 0; i < rooms.size(); i++) {
            if (i == selected)
                cout << "> " << rooms[i] << endl;
            else
                cout << "  " << rooms[i] << endl;
        }

        cout << "\nMessages:\n";
        cout << "Welcome to the chat!\n";
        cout << "More features coming soon...\n";

        cout << "\nType here...\n";
        cout << "(Use UP/DOWN arrows, press q to quit)\n";

        int ch = _getch();

        if (ch == 'q') break;

        if (ch == 224) {
            int arrow = _getch();
            if (arrow == 72 && selected > 0) selected--;
            if (arrow == 80 && selected < rooms.size() - 1) selected++;
        }
    }

    return 0;
}