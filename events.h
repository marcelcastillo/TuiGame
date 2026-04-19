#ifndef EVENTS_H
#define EVENTS_H

#include "entities.h"
#include "systems.h"

using namespace std;

class Task
{   
public:
    string prompt;
    vector<pair<string, string>> choices;    // Choice -> Response
    int answer;                     // Index of correct answer in map
    Task();
    void printTask();
};
class Event
{
public:
    string name;
    int id;
    Room* room;                 // Room associated with task
    vector<Task*> taskList;     // List of tasks to complete to pass event
    vector<int> preReqsIds;     // List of event IDs required to be completed before this event
    vector<Event*> preReqs;     // List of Pre Req Event Pts
    bool complete;              // Has the event been completed or not
    Item* reward;               // Reward for completing the event
    Event();
    void startEvent(Player*);
    void printEvent();
};

#endif