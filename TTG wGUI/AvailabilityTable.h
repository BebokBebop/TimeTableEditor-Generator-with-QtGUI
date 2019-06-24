#pragma once
#include "ShortTime.h"
#include <iostream>
#define WORK_WEEK 5
#define HOW_MANY_ROWS 20

class AvailabilityTable
{
protected:
	bool isSet;
	void makeTable();
	int *availabilityTable[WORK_WEEK];
	int howManyRows;

	void copyTable(int *const otherAvailabilityTable[WORK_WEEK]);

public:

	AvailabilityTable();

	AvailabilityTable(bool setToWhat);

	AvailabilityTable(const AvailabilityTable& other);

	AvailabilityTable(AvailabilityTable&& other);

	~AvailabilityTable();

	void showAvailability();

	void claimAvailabilityFromArg(int *otherAvTable[WORK_WEEK]);

	void updAvailFromArgAND(int *otherAvTable[WORK_WEEK]);

	void updAvailToArgAND(int *otherAvTable[WORK_WEEK]);

	virtual void updAvailability() = 0;

	void claimAvailability(int whatDay, int whatRow, int nRows);

	void setAvailability(bool toWhat);

	virtual bool getSet();

	void setSet(bool newIsSet);

	void reclaim(int *otherAvTable[WORK_WEEK]);

	void reclaim(int whatDay, int whatRow, int nRows);

	void claimAvailability(int *otherAvTable[WORK_WEEK]);
};
