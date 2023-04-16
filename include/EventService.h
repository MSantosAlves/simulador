#include "Semaphore.h"
#include "Event.h"

#include <vector>
#include <string>

using namespace std;

#ifndef __EVENT_SERVICE_H__
#define __EVENT_SERVICE_H__

class EventService
{
private:
	vector<string> targetStocks;
public:
	EventService(vector<string> targetStocks);
	void startProcessEvents(vector<string>* eventsToBeProcessed, vector<string>* offersBook, Semaphore* semaphore, vector<Event>* processedEvents);
};

#endif