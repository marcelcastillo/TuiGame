#ifndef EVENTS_H
#define EVENTS_H

#include "entities.h"
#include "systems.h"

using namespace std;

class Task
{   
    string prompt;
    map<string, string> choices;    // Choice -> Response
    int answer;                     // Index of correct answer in map
};
class Event
{
private:
    vector<Task> tasklist;      // List of tasks to complete to pass event
    Room* room;                 // Room associated with task
    bool complete;              // Has the event been completed or not
public:
    Event(vector<Task> tl, Room* rp);
    void startEvent();

};

#endif