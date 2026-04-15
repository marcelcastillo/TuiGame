/* systems.h */

#ifndef SYSTEMS_H
#define SYSTEMS_H

#include "entities.h"

using namespace std;

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


#endif