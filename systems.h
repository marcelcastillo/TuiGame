/* systems.h */

#ifndef SYSTEMS_H
#define SYSTEMS_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>

#include "entities.h"

using namespace std;

class SysMovement
{
    GameMap* gamemap;
    Player* player;

    void movePlayer();
    
};


#endif