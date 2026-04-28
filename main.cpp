/* Main Program */

#include "entities.h"
#include "systems.h"

using namespace std;
string playerName;

int main()
{
    string mapfile = "map.txt";
    string savedGames = "games.txt";
    bool proceed = false;
    int choice;

    printASCII('*', "logo.txt");
    cout << endl << "Welcome to Tork!" << endl;

    while (!proceed)
    {   
        cout << "   Press 1 to start a new game." << endl;
        // cout << "   Press 2 to continue an existing game: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                /* TODO */
                proceed = true;
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
    }
    /* Initialize game map */
    cout << "Reading from " << mapfile << endl;
    GameMap* map = new GameMap();
    SysEvents* eventSys = new SysEvents();
    init(mapfile, map, eventSys);
    // eventSys->printEvents(); 
    // map.displayAllRooms();

    /* Initialize Player & Movement System */
    char choice2;
    proceed = false;

    while (!proceed)
    {
        cout << "Enter a name for your player: ";
        cin >> playerName;
        cout << "Are you sure you want to proceed with " << playerName << "? (y/n): ";
        cin >> choice2;

        switch (choice2)
        {
            case 'y':
                cout << "Welcome aboard the USS Athena, " << playerName << "!" << endl;
                proceed = true;
                break;
            case 'n':
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }

    }
    Player* player = new Player(playerName, "Voyager on the USS Artemis.");
    SysMovement* moveSys = new SysMovement(map, player);

    /* Main Game Loop */
    while (true)
    {
        eventSys->processEvents(moveSys);
        moveSys->movePlayer();
    }
    return 0;
}

