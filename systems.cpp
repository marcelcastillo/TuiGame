/* systems.cpp */

#include "systems.h"
SysMovement::SysMovement(GameMap* gameMap, Player* player)
{
    mapPtr = gameMap;
    this->player = player;
    currRoom = mapPtr->getStart();
    announce();
}
void SysMovement::movePlayer()
{
    int choice = 0;
    int count;
    bool proceed = false;
    vector<Room*> adjRooms;

    cout<< "From the " << currRoom->getName() << ", it appears the following rooms are accessible." << endl;
    adjRooms = mapPtr->getAdjList(currRoom);
    mapPtr->displayAdjRooms(currRoom);

    while (!proceed)
    {   
        cout << "Which room would you like to travel to?: ";
        cin >> choice;

        if (choice > adjRooms.size())
        {
            cout << "Invalid option." << endl;
            continue;
        }

        currRoom = adjRooms[choice - 1];
        proceed = true;
    }
    announce();
}
void SysMovement::announce() const
{
    cout << currRoom->getDescription() << endl;
}
