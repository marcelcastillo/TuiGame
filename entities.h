/* entities.h */

#ifndef ENTITIES_H
#define ENTITIES_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

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
    Player(const string& name, const string& description);
};

class Enemy : public Entity
{
private:
    health enemyHP = 50.0;
    vector<Entity*> loot;
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
public:
    vector<Room*> roomList;
    vector<vector<Room*>> gridMap;  //2D Grid of Rooms
    map<Room*, vector<Room*>> adjDict;  // Mapping from Room* -> Adjacent Rooms
                                        // [up, down, left, right]
    map<Room*, vector<Room*>> adjList;  // Mapping from Room* -> Adjacent Rooms
                                        // Only accessible rooms
    Coords startLoc;
    int rows;
    int cols;

    GameMap(string filename);
    ~GameMap();
    bool inBounds(Coords coords);
    vector<Room*> populateAdjDict(Room&);
    vector<Room*> getAdjList(Room*);
    Room* getRoom(Coords) const;
    Room* getRoom(Room*, int);
    Room* getStart() const;
    void displayAdjRooms(Room*);
    void displayAllRooms();
};

class GameState
{
private:
    GameMap* gamemap;

public:

};

#endif