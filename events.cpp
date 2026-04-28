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

void Event::startEvent(Player *player)
{
    for (Task *task : taskList)
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

            extern string playerName;
            string input;
            cin >> input;

            /* Function to exit game */
            if (input == "exit" || input == "Exit" || input == "EXIT")
            {
                string confirm;
                printCopilotText("Termination command detected.\nDo you wish to proceed? (y/n)");
                cin >> confirm;

                if (confirm == "y" || confirm == "Y")
                {
                    printCopilotText("Termination command accepted.");
                    printCopilotText("Initiating system hibernation sequence...");
                    printLocationText("The lights dim across the ship.\nDisplays flicker and power down one by one.\nA low hum settles into stillness.");
                    printCopilotText("All non-essential systems offline.");
                    printCopilotText("Goodbye, Captain " + playerName + ".");
                    printCopilotText("Connection lost...");
                    exit(0);
                }
                else
                {
                    printCopilotText("Termination cancelled. Resuming operations.");
                    continue;
                }
            }

            /* Catches incorrect string input */
            int playerResp;

            try
            {
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
