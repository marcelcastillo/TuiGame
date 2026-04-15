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

class Room
{
private:
    std::string roomName;
    std::string roomDesc;
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

class SavedGame
{
private:
    GameMap* gamemap;
public:

};

#endif