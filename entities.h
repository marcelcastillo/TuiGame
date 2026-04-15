/* entities.h */

#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <tuple>

using namespace std;

class Entity
{
private:
    string entName;
    string entDesc;
    bool alive;
public:
    Entity(const string& name, const string& description);
    const string& getName() const;
    const string& getDescription() const;
    void display() const;
};

using health = double;
class Player : public Entity
{
private:
    health playerHP = 100.0;
    vector<Entity*> inventory;
public:
};

struct Coords
{
    int gridRow;
    int gridCol;
};

class Room
{
private:
    string roomName;
    string roomDesc;
    Coords coords;

public:
    Room(const string& name, const string& description, int row, int col);
    const string& getName() const;
    const string& getDescription() const;
    const Coords& getCoords() const;
    void setDescription(const string& desc);
    void display() const;
};

class GameMap
{
private:
    vector<Room*> roomList;
    vector<vector<Room*>> gridMap;  //2D Grid of Rooms
    map<Room*, vector<Room*>> adjDict;  // Mapping from Room* -> Adjacent Rooms
    int rows;
    int cols;
    int startRow;
    int startCol;

public:
    GameMap(string filename);
    ~GameMap();
    bool inBounds(Coords coords);
    void displayRooms();
    vector<Room*> adjRooms(Room&);
};

class GameState
{
private:
    GameMap* gamemap;

public:

};

#endif