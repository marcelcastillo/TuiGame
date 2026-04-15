/* Main Program */

#include "entities.h"

using namespace std;

int main()
{
    string mapfile = "map.txt";
    string savedGames = "games.txt";
    bool proceed = false;
    int choice;

    cout << "Welcome to Tork!" << endl;

    while (!proceed)
    {   
        cout << "   Press 1 to start a new game." << endl;
        cout << "   Press 2 to continue an existing game." << endl;
        cin >> choice;

        switch (choice)
        {
            case 1:
                /* TODO */
                proceed = true;
                break;
            case 2:
                /* TODO */
                proceed = true;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }
    cout << "Reading from " << mapfile << endl;
    GameMap map = GameMap(mapfile);
  
    /* Main Game Loop */
    while (true)
    {
        break;
    }
    return 0;
}

