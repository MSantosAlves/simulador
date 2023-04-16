#ifndef _EVENT_FIELDS_ENUM_H_
#define _EVENT_FIELDS_ENUM_H_

class EventFields
{
public:
    enum Enum {
        SESSION_DATE = 0,
        INSTRUMENT_SYMBOL = 1,
        ORDER_SIDE = 2,
        SEQUENTIAL_ORDER_NUMBER = 3,
        SECONDARY_ORDER_ID = 4,
        EXECUTION_TYPE = 5,
        PRIORITY_TIME = 6,
        PRIORITY_INDICATOR = 7,
        ORDER_PRICE = 8,
        TOTAL_QUANTITY_OF_ORDER = 9,
        TRADED_QUANTITY_OF_ORDER = 10,
        ORDER_DATE = 11,
        ORDER_DATE_TIME_ENTRY = 12,
        ORDER_STATUS = 13,
        AGGRESSOR_INDICATOR = 14,
        MEMBER = 15,
        TYPE_ORDER = 16,

        // INSTRUMENTSYMBOL = 2,
        // IRADE_NUMBER = 3,
        // IRADE_PRICE = 4,
        // IRADED_QUANTITY = 5,
        // IRADE_TIME = 6,
        // IRADE_INDICATOR = 7,
        // BUY_ORDER_DATE = 8,
        // SEQUENTIAL_BUY_ORDER_NUMBER = 9,
        // SECONDARY_ORDER_ID_BUY_ORDER = 10,
        // AGGRESSOR_BUY_ORDER_INDICATOR = 11,
        // SELL_ORDER_DATE = 12,
        // SEQUENTIAL_SELL_ORDE_RNUMBER = 13,
        // SECONDARY_ORDER_ID_SELL_ORDER = 14,
        // AGGRESSOR_SELL_ORDER_INDICATOR = 15,
        // CROSS_TRADE_INDICATOR = 16,
        // BUY_MEMBER = 17,
        // SELL_MEMBER = 18,
        // TYPE_NEG = 19,
    };
};


#endif