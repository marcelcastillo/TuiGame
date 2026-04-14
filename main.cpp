#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>


using namespace std;

class gameMap
{
private:
    vector<Room*> roomStorage;
    vector<vector<Room*>> map;  //2D Grid of Rooms
    int rows;
    int cols;
    int startRow;
    int startCol;

public:
    gameMap(string filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << "Could not open map.txt. Aborting program." << endl;
            exit(EXIT_FAILURE);
        }
    }
};

class Room
{
private:
    std::string roomName;
    std::string roomDesc;
    int gridRow;
    int gridCol;
public:
    Room(const string& name, const string& description, int row, int col)
    {
        roomName = name;
        roomDesc = description;
        gridRow = row;
        gridCol = col;
    }

    const string& getName() const
    {
        return roomName;
    }

    const string& getDescription() const
    {
        return roomDesc;
    }

    void setDescription(const string& desc)
    {
        roomDesc = desc;
    }

    void display() const
    {
        cout << "\n" << roomName << endl;
        cout << roomDesc << endl;
    }

};

bool inBounds(int row, int col, int rows, int cols)
{

}




int main()
{
    string filename;

    cout << "Welcome to Tork!"
    // Main Game Loop
    while (true)
    {

    }
    return 0;
}

