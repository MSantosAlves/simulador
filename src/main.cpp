#include "DataService.h"
#include "EventService.h"
#include "Semaphore.h"
#include "Config.h"

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

    // Instantiate application semaphore
    Semaphore* semaphore = new Semaphore();
    semaphore->release();

    // Instantiate shared variables
    vector<string> eventsToBeProcessed;
    vector<string> offersBook;
    vector<Event> processedEvents;

    // Get custom configs
    Config* config = new Config();
    string date = config->getDate();
    string dataPath = config->getDataPath();
    vector<string> stocks = config->getTargetStocks();

    // Instantiate services
    DataService* dataService = new DataService(date, dataPath);
    EventService* eventService = new EventService(stocks);
    
    thread dataAcquisitionThread(&DataService::startAcquisition, dataService, &eventsToBeProcessed, semaphore);
    thread eventsProcessorThread(&EventService::startProcessEvents, eventService, &eventsToBeProcessed, &offersBook, semaphore, &processedEvents);

    dataAcquisitionThread.join();
    eventsProcessorThread.join();


    return 0;
}