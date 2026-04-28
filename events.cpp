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
        printCopilotText(task->prompt);

        /* Print the prompt options */
        while (true)
        {
            for (int i = 0; i < task->choices.size(); i++)
            {
                printChoiceText(i + 1, task->choices[i].first);
            }
            int playerResp;
            //cin >> playerResp;
            string input;
            cin >> input;

            /* Function to exit game */
            if(input == "exit" || input == "Exit" || input == "EXIT") {
                printCopilotText("Emergency session transmission requested.");
                printCopilotText("Logging final ship status...");
                printLocationText("The terminal fades to black.\nThe ship drifts off silently into the void of space.");
                exit(0);
            }
            /* Catches incorrect string input */
            try {
                playerResp = stoi(input);
            }
            catch (...)
            {
                slowPrint("Invalid input. Please enter a number or type exit to exit the game.\n", 10);
                continue;
            }

            // cout << "[DEBUG] playerResp=" << playerResp
            //     << ", answer=" << task->answer << endl;
            if (playerResp < 1 || playerResp > task->choices.size())
            {
                slowPrint("Invalid choice.\n", 10);
                continue;
            }
            /* Reply with the Reponse to that choice */
            printCopilotText(task->choices[playerResp - 1].second);

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
