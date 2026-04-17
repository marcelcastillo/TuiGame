/* systems.cpp */
#include "systems.h"
#include "events.h"

/* Movement*/
SysMovement::SysMovement(GameMap* gameMap, Player* player)
{
    mapPtr = gameMap;
    this->player = player;
    currRoom = mapPtr->getStart();
    announce();
}
void SysMovement::movePlayer()
{
    int choice = 0;
    int count;
    bool proceed = false;
    vector<Room*> adjRooms;
    ostringstream ostream;

    ostream << "From the " << currRoom->getName() << ", it appears the following rooms are accessible.";
    slowPrint(ostream);

    adjRooms = mapPtr->getAdjList(currRoom);
    mapPtr->displayAdjRooms(currRoom);

    while (!proceed)
    {   
        ostringstream ostream;
        ostream << "Which room would you like to travel to?: ";
        slowPrint(ostream);
        cin >> choice;

        if (choice > adjRooms.size())
        {
            cout << "Invalid option." << endl;
            continue;
        }

        currRoom = adjRooms[choice - 1];
        proceed = true;
    }
    announce();
}
void SysMovement::announce() const
{
    ostringstream ostream;
    ostream << currRoom->getDescription();
    slowPrint(ostream);
}
Room* SysMovement::getCurrRoom()
{
    return currRoom;
}

/* Event System Methods */
SysEvents::SysEvents()
{}

/* Populate each event's preReq vector with the events matched by ID*/
void SysEvents::linkPreReqs()
{
    for (Event* event : eventList)
    {
        event->preReqs.clear();
        for (int reqId : event->preReqsIds)
        {
            for (Event* possibleMatch : eventList)
            {
                if (possibleMatch->id == reqId)
                {
                    event->preReqs.push_back(possibleMatch);
                    break;
                }
            }
        }
    }
}

void SysEvents::processEvents(SysMovement* moveSys)
{
    Room* currRoom = moveSys->getCurrRoom();
    for (Event* event : eventMap[currRoom])
    {
        // If the player enters the current room, and there's an associated event with no outstanding prereqs:
        if (event->complete)
            continue;
        bool start = true;
        for (Event* preReq : event->preReqs)
        {
            if (!preReq->complete)
            {
                start = false;
                break;
            }
        }
        if (start)
        {
            event->startEvent();
        }

    }

}

void SysEvents::printEvents()
{
    for (Event* event : eventList)
    {
        event->printEvent();
    }  
}


/* Slowprint Function */
void slowPrint(const string& text, int delay_ms)
{
    for (char c : text)
    {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << endl;
}

void slowPrint(const ostringstream& stream, int delay_ms)
{
    slowPrint(stream.str(), delay_ms);
}



//@brief Initialization function.
// Reads the filename and populates the game map and event list
void init(string filename, GameMap* map, SysEvents* eventSys)
{
    Room* currRoom;
    Event* currEvent;
    Task* currTask;
    ifstream file(filename);
    string line;

    if (!file)
    {
        cout << "Could not open map.txt. Aborting program." << endl;
        exit(EXIT_FAILURE);
    }

    /* Populate the roomList and the GridMap*/
    while (getline(file, line))     // Single line from map.txt
    {
        if (line.empty()) continue;

        istringstream lineStream(line);
        string keyword;
        vector<string> lineVec;
        lineStream >> keyword;      // Get the keyword from each line

        /* Parse through mapfile contents */
        if (keyword == "SIZE")
        {   
            lineStream >> map->rows >> map->cols;
            map->gridMap.resize(map->rows, vector<Room*>(map->cols, nullptr)); // Set the map grid to all nullptrs
        }
        else if (keyword == "START")
        {
            lineStream >> map->startLoc.gridRow >> map->startLoc.gridCol;
        }
        else if (keyword == "ROOM")
        {
            int row, col;
            string roomName;
            lineStream >> row >> col >> roomName;
            for (char& ch : roomName) 
                if (ch == '_') ch = ' ';                    // Replace '_' with ' ' in roomName
            currRoom = new Room(roomName, "", row, col);    // Create pointer to new room to reference later
            map->gridMap[row][col] = currRoom;                   // Insert the roomPtr into the gridmap
            map->roomList.push_back(currRoom);
        }
        else if (keyword == "DESC")
        {
            string desc;
            getline(lineStream >> std::ws, desc);   // Discard leading whitespace
            currRoom->setDescription(desc);
        }
        else if (keyword == "EVNT")
        {
            currEvent = new Event(); 
            lineStream >> currEvent->id;                        // Set event id
            currEvent->room = currRoom;
            getline(lineStream >> std::ws, currEvent->name);    // Discard leading whitespace, set event name     

        }
        else if (keyword == "PREQ") 
        {
            vector<int> preReqList;                             // List of pre requisite events
            int preReqID;                                       // Event ID
            while (lineStream >> preReqID)          // Build PreReq list            
                preReqList.push_back(preReqID);
            currEvent->preReqsIds = preReqList;        // Assign to the event
        }
        else if (keyword == "TASK")
        {
            currTask = new Task();
            getline(lineStream >> ws, currTask->prompt);
            currEvent->taskList.push_back(currTask);
        }
        else if (keyword == "CHOI")
        {
            if (currEvent != nullptr)
            {
                string choiceText;
                getline(lineStream >> std::ws, choiceText);
                currTask->choices.push_back({choiceText, ""});
            }
        }
        else if (keyword == "RESP")
        {
            string respText;
            getline(lineStream >> std::ws, respText);
            currTask->choices.back().second = respText;

        }
        else if (keyword == "ANSW")
        {
            lineStream >> currTask->answer;
            eventSys->eventList.push_back(currEvent);
            eventSys->eventMap[currRoom].push_back(currEvent);
        }
    }
    /* Populate the Event's preReq vector */
    eventSys->linkPreReqs();

    /* Populate adjacency lists */
    for (Room* room : map->roomList)
    {   
        // adj contains every possible direction
        vector<Room*> adj = map->populateAdjDict(*room);
        for (Room* adjRoom : adj)
        {
            if (adjRoom != nullptr)
                map->adjList[room].push_back(adjRoom);
        }
        map->adjDict[room] = adj;
    }
}

