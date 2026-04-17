/* systems.h */

#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <thread>
#include <chrono>

#include "entities.h"
#include "events.h"

using namespace std;

constexpr int DELAY = 5;

class Event;

class SysMovement
{
private:
    GameMap* mapPtr;
    Player* player;
    Room* currRoom;
public:
    SysMovement(GameMap*, Player*);
    void movePlayer();
    void announce() const;
    Room* getCurrRoom();

};

class SysEvents
{
public:
    vector<Event*> eventList;               // Master list of Game Events
    map<Room*, vector<Event*>> eventMap;    // Room -> List of Events associated with that room
    SysEvents();
    void linkPreReqs();
    void processEvents(SysMovement* moveSys);
    void printEvents();
};

void slowPrint(const ostringstream& stream, int delay_ms = DELAY);
void slowPrint(const string& text, int delay_ms = DELAY);
void init(string filename, GameMap* map, SysEvents* eventSys);

#endif