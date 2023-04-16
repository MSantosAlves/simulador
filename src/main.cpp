#include "DataService.h"
#include "Semaphore.h"

#include <thread>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <atomic>
#include <string>
#include <vector>


int main(int argc, char* argv[])
{

    Semaphore* semaphore = new Semaphore();
    semaphore->release();

    // Shared variables
    vector<string> unprocessedEventsBuffer;

    // Instantiate services
    DataService* data = new DataService();

    thread dataAcquisitionThread(&DataService::startAcquisition, data, &unprocessedEventsBuffer, semaphore);

    dataAcquisitionThread.join();


    return 0;
}