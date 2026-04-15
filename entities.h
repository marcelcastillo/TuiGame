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

using namespace std;

class Entity
{
private:
    string entName;
    string entDesc;
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

class Room
{
private:
    string roomName;
    string roomDesc;
    int gridRow;
    int gridCol;
public:
    Room(const string& name, const string& description, int row, int col);
    const string& getName() const;
    const string& getDescription() const;
    void setDescription(const string& desc);
    void display() const;
};

class GameMap
{
private:
    vector<Room*> roomList;
    vector<vector<Room*>> map;  //2D Grid of Rooms
    int rows;
    int cols;
    int startRow;
    int startCol;

public:
    GameMap(string filename);
    bool inBounds(int row, int col);
    void displayRooms();
};

class GameState
{
private:
    GameMap* gamemap;

public:

};

#endif