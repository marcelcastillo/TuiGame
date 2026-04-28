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
    vector<Room*> accRooms;
    ostringstream ostream;

    ostream << "From the " << currRoom->getName() << ", it appears the following rooms are accessible.";
    slowPrint(ostream);

    /* From the list of adjacent rooms, provide only the ones accessible to player */
    adjRooms = mapPtr->getAdjList(currRoom);
    for (Room* possRoom : adjRooms)
    {
        if (possRoom->accessible)
            accRooms.push_back(possRoom);
    }
    mapPtr->displayAdjRooms(adjRooms);

    while (!proceed)
    {   
        slowPrint("Which room would you like to travel to?: ");
        cin >> choice;

        if (choice > adjRooms.size())
        {
            slowPrint("Invalid option.");
            continue;
        }
        Room* selectedRoom = adjRooms[choice - 1];
        /* If selectedRoom not in accRooms */
        auto iter = find(accRooms.begin(), accRooms.end(), selectedRoom);
        if (iter == accRooms.end())
        {
            slowPrint("Hmm, it seems this room is locked...");
            continue;
        }
        currRoom = adjRooms[choice - 1];
        proceed = true;
    }
    announce();
}
void SysMovement::announce() const
{
    printLocationText(currRoom->getDescription());
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
            event->startEvent(moveSys->player);
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

string processNewlines(string text){
    size_t pos = 0;
    while ((pos = text.find("\\n", pos)) != string::npos){
        text.replace(pos, 2, "\n");
        pos += 1;
    }
    return text;
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

/* Colorprint Functions */
const string RESET = "\033[0m";
const string ITALIC = "\033[3m";
const string DIM = "\033[2m";
const string GREEN = "\033[0;38;5;10m";

void printLocationText(const string& text) {
    slowPrint(DIM + ITALIC + processNewlines(text)+ RESET);
}
void printCopilotText(const string& text){
    string prefix = "[Co-Pilot] > ";
    slowPrint(GREEN + prefix + processNewlines(text) + RESET, 20); //slightly slower slowprint
}
void printChoiceText(int number, const string& text){
    cout << number << ". " << text << endl;
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
        else if (keyword == "ACCS")
        {
            lineStream >> std::boolalpha >> currRoom->accessible;
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
        else if (keyword == "RWRD")
        {   
            /* Read reward and push into player's inventory */
            string itemName, itemDesc;
            int itemWeight;
            lineStream >> itemName >> itemDesc >> itemWeight;
            for (char& ch : itemName) 
                if (ch == '_') ch = ' ';   
            for (char& ch : itemDesc) 
                if (ch == '_') ch = ' ';                  
            Item* item = new Item(itemName, itemDesc, itemWeight);
            currEvent->reward = item;

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
// @brief Outputs a border from the user's requested border character
// @param border    User's requested border char
// @param size      Number of times to print the border char
void printHeaderFooter(char border, int size)
{
    for (int i = 0; i < size; i++){
        cout << border;
    }
    cout << endl;
    return;
}

/* Pretty Printing ASCII*/
// @brief Outputs the requested painting to standard out
// @param border    User's requested border char
// @param filename  The textfile storing the ASCII art
void printASCII(char border, string filename)
{
    string line;
    string artImage;
    int size = 0;
    ifstream inStream(filename);
    ostringstream cout_Art;

    if (inStream.is_open()){
        while (getline(inStream, line)){
            if (line.size() > size){
                size = line.size();
            }
            // Remove carriage return characters
            if (!line.empty() && line.back() == '\r'){
                line.pop_back();
            }
            cout_Art << line << endl;
        }
    } else {
        cout << "Sorry, " << filename << " could not be opened." << endl;
    }

    inStream.close();
    artImage = cout_Art.str();
    istringstream cin_Art(artImage);

    // Print the ASCII Art
    printHeaderFooter(border, size + 2);
    while (getline(cin_Art, line)){
        cout << border << line << string(size - line.size(), ' ') << border << endl;
    }
    printHeaderFooter(border, size + 2);
    return;
}