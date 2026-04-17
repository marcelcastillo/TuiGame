/* systems.h */

#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <thread>
#include <chrono>

#include "entities.h"
#include "events.h"

using namespace std;
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

};

class SysEvents
{
public:
    vector<Event*> eventList;
    SysEvents();

};

void slowPrint(const ostringstream& stream, int delay_ms = 25);
void init(string filename, GameMap* map, SysEvents* eventSys);

#endif