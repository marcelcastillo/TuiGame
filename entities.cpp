/* entities.cpp */

#include "entities.h"

/* Entity Method Definitions */
Entity::Entity(const string& name, const string& description)
{
    entName = name;
    entDesc = description;
}
const string& Entity::getName() const
{
    return entName;
}
const string& Entity::getDescription() const
{
    return entDesc;
}
void Entity::display() const
{
    cout << "\n" << entName << endl;
    cout << entDesc << endl;
}

/* Room Method Definitions */
Room::Room(const string& name, const string& description, int row, int col)
{
    roomName = name;
    roomDesc = description;
    gridRow = row;
    gridCol = col;
}
const string& Room::getName() const
{
    return roomName;
}
const string& Room::getDescription() const
{
    return roomDesc;
}
void Room::setDescription(const string& desc)
{
    roomDesc = desc;
}
void Room::display() const
{
    cout << "\n" << roomName << endl;
    cout << roomDesc << endl;
    cout << "Row: " << gridRow << " Col: " << gridCol << endl;
}

/* GameMap Method Definitions */

//@brief gameMap class constructor.
// Reads the mapfile 'map.txt', creates each Room obj
GameMap::GameMap(string filename)
{   
    Room* currRoom;
    ifstream file(filename);
    string line;

    if (!file)
    {
        cout << "Could not open map.txt. Aborting program." << endl;
        exit(EXIT_FAILURE);
    }
    
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
            lineStream >> rows >> cols;
            map.resize(rows, vector<Room*>(cols, nullptr));
        }
        else if (keyword == "START")
        {
            lineStream >> startRow >> startCol;
        }
        else if (keyword == "ROOM")
        {
            int row, col;
            string roomName;
            lineStream >> row >> col >> roomName;
            for (char& ch : roomName) 
                if (ch == '_') ch = ' ';                    // Replace '_' with ' ' in roomName
            currRoom = new Room(roomName, "", row, col);    // Create pointer to new room to reference later
            roomList.push_back(currRoom);
        }
        else if (keyword == "DESC")
        {
            string desc;
            getline(lineStream >> std::ws, desc);
            currRoom->setDescription(desc);
        }
    }
}
//@brief Checks whether provided dimensions fit within grid
//@return bool
bool GameMap::inBounds(int row, int col)
{
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

//@brief Displays all rooms comprising game map
void GameMap::displayRooms()
{
    for (Room* room : roomList)
        room->display();
}