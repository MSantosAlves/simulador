#include "DataService.h"
#include "Semaphore.h"
#include "Config.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

DataService::DataService(string _date, string _dataPath) {
    date = _date;
    dataPath = _dataPath;
}

void DataService::startAcquisition(vector<string>* eventsToBeProcessed, Semaphore* semaphore)
{
    string sysFileChar = (_WIN64 || _WIN32) ? "\\" : "/";

    string vdaPath = dataPath + sysFileChar + "data" + sysFileChar + date + sysFileChar + "VDA_SIMPLIFIED.txt";
    string cpaPath = dataPath + sysFileChar + "data" + sysFileChar + date + sysFileChar + "CPA_SIMPLIFIED.txt";
    string negPath = dataPath + sysFileChar + "data" + sysFileChar + date + sysFileChar + "NEG_SIMPLIFIED.txt";
    
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

    while (true) {

        semaphore->acquire();

        getline(cpaFile, cpaBuffer);
        //getline(vdaFile, vdaBuffer);
        //getline(negFile, negBuffer);
        
        eventsToBeProcessed->push_back(cpaBuffer + ";CPA");
        //eventsToBeProcessed->push_back(vdaBuffer+";VDA");
        //eventsToBeProcessed->push_back(negBuffer+";NEG");
        semaphore->release();
        //Sleep(1);
    }

    vdaFile.close();
    cpaFile.close();
    negFile.close();

}