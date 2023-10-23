#ifndef _ORDER_STATUSES_H_
#define _ORDER_STATUSES_H_

class OrderStatuses
{
public:
    enum Status {
        NEW = 0,
        PARTIALLY_FILLED = 1,
        FILLED = 2,
        CANCELLED = 4,
        REPLACED = 5,
        REJECTED = 8,
        EXPIRED = 9
    };
};


#endif