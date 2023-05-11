#include "Semaphore.h"
#include "Event.h"
#include "PurchaseOffer.h"
#include "SaleOffer.h"

#include <vector>
#include <string>
#include <map>

using namespace std;

#ifndef __EVENT_SERVICE_H__
#define __EVENT_SERVICE_H__

class EventService
{
private:
	vector<string> targetStocks;
public:
	EventService(vector<string> targetStocks);
	void startProcessEvents(vector<string>* eventsToBeProcessed, vector<string>* offersBook, Semaphore* semaphore, map<string, vector<PurchaseOffer>>* purchases, map<string, vector<SaleOffer>>* sales);
};

#endif