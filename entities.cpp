/* entities.cpp */

#include "entities.h"

/*****************************/
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

/*****************************/
/* Player Method Definitions */
Player::Player(const string& name, const string& description) : Entity(name, description)
{
    Entity* crowbar = new Entity("crowbar", "Pry things open!");
    inventory.push_back(crowbar);
}

/***************************/
/* Room Method Definitions */
Room::Room(const string& name, const string& description, int row, int col)
{
    roomName = name;
    roomDesc = description;
    coords.gridRow = row;
    coords.gridCol = col;
}
const string& Room::getName() const
{
    return roomName;
}
const Coords& Room::getCoords() const
{
    return coords;
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
    cout << "Row: " << coords.gridRow << " Col: " << coords.gridCol << endl;
}

/******************************/
/* GameMap Method Definitions */

//@brief gameMap class constructor.
// Reads the mapfile 'map.txt', creates each Room obj, push into roomList and correct gridMap location
// Populates room adjacency list
GameMap::GameMap()
{}
//@brief GameMap Destructor
GameMap::~GameMap()
{
    for (Room* room : roomList)
    {
        delete room;
    }
    roomList.clear();
}

//@brief Checks whether provided coordinates fit within grid
//@return bool
bool GameMap::inBounds(Coords coords)
{
    return coords.gridRow >= 0 && coords.gridRow < rows && coords.gridCol >= 0 && coords.gridCol < cols;
}


//@brief Displays all rooms comprising game map
void GameMap::displayAllRooms()
{
    for (Room* room : roomList)
    {
        room->display();
        for (Room* adjRoom : adjDict[room])
        {
            if (adjRoom != nullptr) cout << adjRoom->getName() << endl;
        }
    }

}
//@brief Returns the rooms adjacent to the current Room ref
// Returned adjList is always [up, down, left, right] with nullptrs for 
// invalid directions or nonexistent rooms.
// It is not yet clear if the adjDict will be necessary
vector<Room*> GameMap::populateAdjDict(Room& currRoom)
{
    vector<Room*> adjRooms;
    auto coords = currRoom.getCoords();
    /* Four possible directions */
    Coords up = {coords.gridRow - 1, coords.gridCol};
    Coords down = {coords.gridRow + 1, coords.gridCol};
    Coords left = {coords.gridRow, coords.gridCol - 1};
    Coords right = {coords.gridRow, coords.gridCol + 1};
    vector<Coords> directions = {up, down, left, right};

    for (auto& dir : directions)
    {
        if (inBounds(dir))
        {
            // Pushes in room
            adjRooms.push_back(gridMap[dir.gridRow][dir.gridCol]);
        } else {
            adjRooms.push_back(nullptr);
        }
    }

    return adjRooms;
}

//@brief Returns the roomPtr assigned to a given grid coordinate
//@return Room* if room exists, else nullptr
Room* GameMap::getRoom(Coords coords) const
{
    return gridMap[coords.gridRow][coords.gridCol];
}

//@brief Returns the roomPtr assigned to the given index in adjDict
//@return Room* if room exists, else nullptr
Room* GameMap::getRoom(Room* currRoom, int adjIndex)
{
    return adjDict[currRoom][adjIndex];
}

Room* GameMap::getStart() const
{
    return gridMap[startLoc.gridRow][startLoc.gridCol];
}

void GameMap::displayAdjRooms(Room* currRoom)
{   
    int count = 0;
    for (Room* adjRoom : adjList[currRoom])
    {
        cout << "   " << count + 1 << ": " << adjRoom->getName() << endl;
        count++;
    }
}

vector<Room*> GameMap::getAdjList(Room* currRoom)
{
    return adjList[currRoom];
}
