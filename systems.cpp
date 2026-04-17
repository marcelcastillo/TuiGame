/* systems.cpp */
#include "systems.h"
#include "events.h"

/* Movement*/
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
    ostringstream ostream;

    ostream << "From the " << currRoom->getName() << ", it appears the following rooms are accessible.";
    slowPrint(ostream);

    adjRooms = mapPtr->getAdjList(currRoom);
    mapPtr->displayAdjRooms(currRoom);

    while (!proceed)
    {   
        ostringstream ostream;
        ostream << "Which room would you like to travel to?: ";
        slowPrint(ostream);
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
    ostringstream ostream;
    ostream << currRoom->getDescription();
    slowPrint(ostream);
}

/* Event Methods */
SysEvents::SysEvents()
{
    
}


/* Slowprint Function */
void slowPrint(const ostringstream& stream, int delay_ms)
{
    string text = stream.str();
    for (char c : text)
    {
        cout << c << flush;  // print immediately
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << endl;
}

//@brief Initialization function.
// Reads the filename and populates the game map and event list
void init(string filename, GameMap* map, SysEvents* eventSys)
{
    Room* currRoom;
    ifstream file(filename);
    string line;

    if (!file)
    {
        cout << "Could not open map.txt. Aborting program." << endl;
        exit(EXIT_FAILURE);
    }

        /* Populate the roomList and the GridMap*/
    while (getline(file, line))     // Single line from map.txt
    {
        if (line.empty()) continue;

        istringstream lineStream(line);
        string keyword;
        vector<string> lineVec;
        lineStream >> keyword;      // Get the keyword from each line

        /* Parse through mapfile contents */
        if (keyword == "SIZE")
        {   
            lineStream >> map->rows >> map->cols;
            map->gridMap.resize(map->rows, vector<Room*>(map->cols, nullptr)); // Set the map grid to all nullptrs
        }
        else if (keyword == "START")
        {
            lineStream >> map->startLoc.gridRow >> map->startLoc.gridCol;
        }
        else if (keyword == "ROOM")
        {
            int row, col;
            string roomName;
            lineStream >> row >> col >> roomName;
            for (char& ch : roomName) 
                if (ch == '_') ch = ' ';                    // Replace '_' with ' ' in roomName
            currRoom = new Room(roomName, "", row, col);    // Create pointer to new room to reference later
            map->gridMap[row][col] = currRoom;                   // Insert the roomPtr into the gridmap
            map->roomList.push_back(currRoom);
        }
        else if (keyword == "DESC")
        {
            string desc;
            getline(lineStream >> std::ws, desc);
            currRoom->setDescription(desc);
        }
    }

    /* Populate adjacency lists */
    for (Room* room : map->roomList)
    {
        vector<Room*> adj = map->populateAdjDict(*room);
        for (Room* adjRoom : adj)
        {
            if (adjRoom != nullptr)
                map->adjList[room].push_back(adjRoom);
        }
        map->adjDict[room] = adj;
    }
}
