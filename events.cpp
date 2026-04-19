#include "events.h"

Task::Task()
{
    prompt = "";
    choices.clear();
    answer = -1;
}

void Task::printTask()
{
    cout << "  TASK: " << prompt << endl;

    for (int i = 0; i < choices.size(); i++)
    {
        cout << "    [" << i << "] " << choices[i].first << endl;
        cout << "         -> " << choices[i].second << endl;
    }

    cout << "    ANSW: " << answer << endl;
}
Event::Event()
{
    name = "";
    id = -1;
    taskList = {};
    room = nullptr;
    preReqs = {};
    complete = false;
    reward = nullptr;
}

void Event::startEvent(Player* player)
{
    for (Task* task : taskList)
    {
        /* Print the Task Prompt */
        ostringstream ostream;
        ostream << task->prompt << endl;
        slowPrint(ostream);
        /* Print the prompt options */
        while (true)
        {
            for (int i = 0; i < task->choices.size(); i++)
            {
                ostringstream ostream;
                ostream << i + 1 << ". " << task->choices[i].first;
                slowPrint(ostream, 10);
            }
            int playerResp;
            cin >> playerResp;
            // cout << "[DEBUG] playerResp=" << playerResp
            //     << ", answer=" << task->answer << endl;
            if (playerResp < 1 || playerResp > task->choices.size())
            {
                slowPrint("Invalid choice.\n", 10);
                continue;
            }
            /* Reply with the Reponse to that choice */
            ostringstream ostream;
            ostream << task->choices[playerResp-1].second << endl; 
            slowPrint(ostream, 10);  
            if (playerResp - 1 == task->answer)
            {
                break;
            }
        }
    }
    complete = true;
    /* If event had an associated reward, reward it to the player */
    if (reward != nullptr)
    {
        player->inventory.push_back(reward);
        ostringstream ostream;
        ostream << "You received: " << reward->getName();
        slowPrint(ostream);
    }


}
void Event::printEvent()
{
    cout << "EVNT " << id << " " << name << endl;

    // Precondition Events
    cout << "  PREQ: ";
    for (int req : preReqsIds)
    {
        cout << req << " ";
    }
    cout << endl;
    // Tasks
    for (int i = 0; i < taskList.size(); i++)
    {
        cout << "  --- Task " << i << " ---" << endl;
        taskList[i]->printTask();
    }

    cout << "  COMPLETE: " << (complete ? "true" : "false") << endl;
}
