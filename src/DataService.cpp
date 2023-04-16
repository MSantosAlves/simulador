#include "DataService.h"
#include "Semaphore.h"
#include "Config.h"

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>

using namespace std;

DataService::DataService() {}

void DataService::startAcquisition(vector<string>* events, Semaphore* semaphore)
{
    Config* config = new Config();
    string date = config->getDate();
    string path = config->getDataPath();
    string sysFileChar = (_WIN64 || _WIN32) ? "\\" : "/";

    string vdaPath = path + sysFileChar + "data" + sysFileChar + date + sysFileChar + "VDA_SIMPLIFIED.txt";
    string cpaPath = path + sysFileChar + "data" + sysFileChar + date + sysFileChar + "CPA_SIMPLIFIED.txt";
    string negPath = path + sysFileChar + "data" + sysFileChar + date + sysFileChar + "NEG_SIMPLIFIED.txt";
    
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

    while (false) {
        cout << "Running..." << endl;
        //semaphore->acquire();
        //getline(cpaFile, cpaBuffer);
        // getline(vdaFile, vdaBuffer);
        // getline(negFile, negBuffer);
        //events->push_back(cpaBuffer + ";CPA");
        // events->push_back(vdaBuffer+";VDA");
        // events->push_back(negBuffer+";NEG");
        //semaphore->release();
        //Sleep(1);
    }

    vdaFile.close();
    cpaFile.close();
    negFile.close();

}