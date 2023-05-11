#include "DataService.h"
#include "Semaphore.h"
#include "Config.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

DataService::DataService(string _date, string _dataPath) {
    date = _date;
    dataPath = _dataPath;
}

void DataService::startAcquisition(vector<string>* eventsToBeProcessed, Semaphore* semaphore)
{
    string sysFileChar = (_WIN64 || _WIN32) ? "\\" : "/";

    string vdaPath = dataPath + sysFileChar + date + sysFileChar + "VDA_SIMPLIFIED.txt";
    string cpaPath = dataPath + sysFileChar + date + sysFileChar + "CPA_SIMPLIFIED.txt";
    string negPath = dataPath + sysFileChar + date + sysFileChar + "NEG_SIMPLIFIED.txt";
    
    ifstream negFile(negPath);
    ifstream vdaFile(vdaPath);
    ifstream cpaFile(cpaPath);

    if (!negFile.is_open()) {
        cerr << "Error opening negotiations file." << endl;
        return;
    }
    else if (!vdaFile.is_open()) {
        cerr << "Error opening sell orders file." << endl;
        return;
    }
    else if (!cpaFile.is_open()) {
        cerr << "Error opening purchases orders file." << endl;
        return;
    }

    string negBuffer;
    string vdaBuffer;
    string cpaBuffer;
    int idx = 0;

    chrono::milliseconds timespan(1000);

    while (true) {
        semaphore->acquire();
        
        getline(cpaFile, cpaBuffer);
        getline(vdaFile, vdaBuffer);
        
        eventsToBeProcessed->push_back(cpaBuffer + ";CPA");
        eventsToBeProcessed->push_back(vdaBuffer+";VDA");

        cpaBuffer = "";
        vdaBuffer = "";
        
        semaphore->release();
        this_thread::sleep_for(timespan);
    }

    vdaFile.close();
    cpaFile.close();
    negFile.close();

}