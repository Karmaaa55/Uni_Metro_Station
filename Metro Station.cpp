#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

struct Station
{
    string name;
    int minute;
    struct Station* next;
    struct Station* prev;
    struct Station* connection;
};

struct Train
{
    string trainName;
    int travelTime;
    bool outbound;
    struct Station* currentStation;
};

Station* createStation(int lineNo, int StationNo)
{
    Station *newStation = new Station;
    string name = "line " + to_string(lineNo) + " Station " + to_string(StationNo);
    newStation->name = name;
    int number = rand()%5 + 1;
    newStation->minute = number;
    newStation->prev = nullptr;
    newStation->next = nullptr;

    return newStation;
}

Station* generate_metro_line(int lineNo, int stationNo)
{
    Station* stationOne = createStation(lineNo, 1);
    Station *head = stationOne;

    for (int i = 2; i <= stationNo; i++)
    {
        Station* newStation = createStation(lineNo, i);
        if (i == stationNo)
        {
            newStation->minute = 0;
        }
        Station *temp = head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newStation;
        newStation->prev = temp;
    }

    return head;
}


void output(Station *p)
{
    if (p->next != nullptr)
    {
        cout << p->name << " time to next: " << p->minute << endl;
    }
    else
    {
        cout << p->name << " end station " << endl;
    }
}

void output_line(Station *head)
{
    Station *p = head;
    while (p->next != nullptr)
    {
        output(p);
        p = p->next;
    }
    output(p);
}

void init(Train &train, int trainName, Station *head)
{
    string name = "train " + to_string(trainName);
    train.trainName = name;
    train.travelTime = 0;
    train.currentStation = head;
    train.outbound = true;
}

void output(Train &train)
{
    cout << train.trainName << ": ";
    if (train.outbound)
    {
        if (train.travelTime == 0 || train.travelTime == train.currentStation->minute + 1)
        {
            cout << "stop at " << train.currentStation->name << endl;
        }
        else
        {

            cout << train.travelTime << " minute(s) to " << train.currentStation->next->name << endl;
        }
    }
    else
    {

        if (train.travelTime == 0 || train.travelTime == train.currentStation->prev->minute + 1)
        {
            cout << "stop at " << train.currentStation->name << endl;
        }
        else
        {
            cout << train.travelTime << " minute(s) to " << train.currentStation->prev->name << " (on way back)" << endl;
        }
    }
}

void output_all_trains(int simulationMin, Train arrOfTrains[], int arrSize)
{
    cout << "time simulation: " << simulationMin << " min" << endl;
    for (int i = 0; i < arrSize; i++)
    {
        output(arrOfTrains[i]);
    }
}

int main()
{
    Station *arrMetroStation[2];
    Train arrOfTrains[2];
    int simulationMin = 0, numberOfStation, lineNo;
    char choice;

    cout << "input number of stations of line 1: ";
    cin >> numberOfStation;
    arrMetroStation[0] = generate_metro_line(1, numberOfStation);

    cout << "input number of stations of line 2: ";
    cin >> numberOfStation;
    arrMetroStation[1] = generate_metro_line(2, numberOfStation);

    init(arrOfTrains[0], 1, arrMetroStation[0]);
    init(arrOfTrains[1], 2, arrMetroStation[1]);

    arrOfTrains[0].travelTime = arrOfTrains[0].currentStation->minute + 1;
    arrOfTrains[1].travelTime = arrOfTrains[1].currentStation->minute + 1;

    do
    {
        cout << "------------------" << endl << "e end" << endl;
        cout << "l line output" << endl;
        cout << "t train output" << endl;
        cout << "1 simulate 1 minute" << endl << ">> ";
        cin >> choice;
        switch(choice)
        {
        case 'l':
            cout << "which line? ";
            cin >> lineNo;
            if (lineNo == 1 || lineNo == 2)
            {
                output_line(arrMetroStation[lineNo - 1]);
            }
            else
            {
                cout << "There are only 2 lines sir :)" << endl;
            }
            break;
        case 't':
            output_all_trains(simulationMin, arrOfTrains, 2);
            break;
        case '1':
            simulationMin = simulationMin + 1;
            for (int i = 0; i < sizeof(arrOfTrains)/sizeof(arrOfTrains[0]) ; i++)
            {
                arrOfTrains[i].travelTime = arrOfTrains[i].travelTime - 1;
                if (arrOfTrains[i].travelTime == 0)
                {
                    if (arrOfTrains[i].currentStation->next != nullptr && arrOfTrains[i].outbound)
                    {
                        arrOfTrains[i].currentStation = arrOfTrains[i].currentStation->next;
                        if (arrOfTrains[i].currentStation->next == nullptr)
                        {
                            arrOfTrains[i].outbound = !arrOfTrains[i].outbound;
                            arrOfTrains[i].travelTime = arrOfTrains[i].currentStation->prev->minute + 1;
                        }
                        else
                        {
                            arrOfTrains[i].travelTime = arrOfTrains[i].currentStation->minute + 1;
                        }
                    }
                    else if (arrOfTrains[i].currentStation->prev != nullptr && !arrOfTrains[i].outbound)
                    {
                        arrOfTrains[i].currentStation = arrOfTrains[i].currentStation->prev;
                        if (arrOfTrains[i].currentStation->prev == nullptr)
                        {
                            arrOfTrains[i].outbound = !arrOfTrains[i].outbound;
                            arrOfTrains[i].travelTime = arrOfTrains[i].currentStation->minute + 1;
                        }
                        else
                        {
                            arrOfTrains[i].travelTime = arrOfTrains[i].currentStation->prev->minute + 1;
                        }
                    }
                }
            }
            output_all_trains(simulationMin, arrOfTrains, 2);
            break;
        case 'e':
            cout << endl << "Good bye :)" << endl << endl;
            break;
        default:
            cout << "enter an appropriate character :(" << endl;
        }
    }
    while (choice != 'e');
    return 0;
}
