#include "DataService.h"
#include "Semaphore.h"
#include "Config.h"
#include "StringUtils.h"

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
    
    ifstream vdaFile(vdaPath);
    ifstream cpaFile(cpaPath);

    if (!vdaFile.is_open()) {
        cerr << "Error opening sell orders file." << endl;
        return;
    }
    
    if (!cpaFile.is_open()) {
        cerr << "Error opening purchases orders file." << endl;
        return;
    }

    StringUtils stringUtils;
    string cpaHeader;
    string vdaHeader;
    vector<string> splitedCpaHeader;
    vector<string> splitedVdaHeader;

    getline(cpaFile, cpaHeader);
    getline(vdaFile, vdaHeader);

    splitedCpaHeader = stringUtils.split(cpaHeader, " ");
    int nbOfCpaOffers = stoi(splitedCpaHeader[splitedCpaHeader.size() - 1]);

    splitedVdaHeader = stringUtils.split(vdaHeader, " ");
    int nbOfVdaOffers = stoi(splitedVdaHeader[splitedVdaHeader.size() - 1]);

    cout << "Number of purchase offers: " << nbOfCpaOffers << endl;
    cout << "Number of sale offers: " << nbOfVdaOffers << endl;
    cout << "Total number of offers: " << (nbOfCpaOffers + nbOfVdaOffers) << endl;

    int countReadOffers = 0;
    string vdaBuffer;
    string cpaBuffer;

    chrono::milliseconds timespan(1000);

    while (true) {
        semaphore->acquire();
        
        getline(cpaFile, cpaBuffer);
        getline(vdaFile, vdaBuffer);
        
        eventsToBeProcessed->push_back(cpaBuffer + ";CPA");
        countReadOffers += 1;
        eventsToBeProcessed->push_back(vdaBuffer+";VDA");
        countReadOffers += 1;

        semaphore->release();
        this_thread::sleep_for(timespan);
    }

    vdaFile.close();
    cpaFile.close();

}