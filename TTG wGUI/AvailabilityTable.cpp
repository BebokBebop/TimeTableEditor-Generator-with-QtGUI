#include "AvailabilityTable.h"

void AvailabilityTable::makeTable() {
	for (int i = 0; i < WORK_WEEK; i++)
		availabilityTable[i] = new int[howManyRows];
}

void AvailabilityTable::copyTable(int * const otherAvailabilityTable[WORK_WEEK])
{
	for (int i = 0; i < WORK_WEEK; i++)
		for (int j = 0; j < howManyRows; j++)
			availabilityTable[i][j] = otherAvailabilityTable[i][j];
}

AvailabilityTable::AvailabilityTable()
{	}

AvailabilityTable::AvailabilityTable(bool setToWhat) :howManyRows(HOW_MANY_ROWS)
{
	makeTable();
	setAvailability(setToWhat);
}

AvailabilityTable::AvailabilityTable(const AvailabilityTable & other)
{
	copyTable(other.availabilityTable);
	isSet = other.isSet;
	howManyRows = other.howManyRows;
}

AvailabilityTable::AvailabilityTable(AvailabilityTable && other)
{
	for (int i = 0; i < WORK_WEEK; i++)
	{
		availabilityTable[i] = other.availabilityTable[i];
		other.availabilityTable[i] = nullptr;
	}
	isSet = other.isSet;
	howManyRows = other.howManyRows;
}

AvailabilityTable::~AvailabilityTable()
{
	for (int i = 0; i < WORK_WEEK; i++)
		if (availabilityTable[i] != nullptr)
			delete[] availabilityTable[i];
}

void AvailabilityTable::showAvailability()
{
	for (int i = 0; i < WORK_WEEK; i++) {
		for (int j = 0; j < howManyRows; j++)
			std::cout << availabilityTable[i][j] << " ";
		std::cout << '\n';
	}
}

void AvailabilityTable::claimAvailabilityFromArg(int * otherAvTable[WORK_WEEK]) {
	for (int i = 0; i < WORK_WEEK; i++)
	{
		for (int j = 0; j < howManyRows; j++)
		{
			if (this->availabilityTable[i][j])
				otherAvTable[i][j] = false;
		}
	}
}

void AvailabilityTable::updAvailFromArgAND(int * otherAvTable[WORK_WEEK]) {
	for (int i = 0; i < WORK_WEEK; i++)
	{
		for (int j = 0; j < howManyRows; j++)
		{
			if (otherAvTable[i][j] != -1 && availabilityTable[i][j] != -1)
				this->availabilityTable[i][j] = availabilityTable[i][j] && otherAvTable[i][j];
		}
	}
}

void AvailabilityTable::updAvailToArgAND(int * otherAvTable[WORK_WEEK]) {
	for (int i = 0; i < WORK_WEEK; i++)
	{
		for (int j = 0; j < howManyRows; j++)
		{
			if (otherAvTable[i][j] != -1)
				otherAvTable[i][j] = availabilityTable[i][j] && otherAvTable[i][j];
			if (availabilityTable[i][j] == -1)
				std::cout << "nie powinno ever\n";
		}
	}
}

void AvailabilityTable::claimAvailability(int whatDay, int whatRow, int nRows) //set false to coords where arg has true
{
	while (--nRows >= 0)
		availabilityTable[whatDay][whatRow + nRows] = false;
}

void AvailabilityTable::setAvailability(bool toWhat) {

	for (int i = 0; i < WORK_WEEK; i++) {
		for (int j = 0; j < howManyRows; j++)
			availabilityTable[i][j] = toWhat;
	}
}

bool AvailabilityTable::getSet() {
	return isSet;
}

void AvailabilityTable::setSet(bool newIsSet) {
	isSet = newIsSet;
}

void AvailabilityTable::reclaim(int * otherAvTable[WORK_WEEK]) //set true to coords where arg has true
{
	for (int i = 0; i < WORK_WEEK; i++)
	{
		for (int j = 0; j < howManyRows; j++)
		{
			if (otherAvTable[i][j] == true)
				this->availabilityTable[i][j] = true;
		}
	}
}

void AvailabilityTable::reclaim(int whatDay, int whatRow, int nRows) //set true to coords where arg has true
{
	while (--nRows >= 0)
		availabilityTable[whatDay][whatRow + nRows] = true;
}

void AvailabilityTable::claimAvailability(int * otherAvTable[WORK_WEEK]) //set false to coords where arg has true
{
	for (int i = 0; i < WORK_WEEK; i++)
	{
		for (int j = 0; j < howManyRows; j++)
		{
			if (otherAvTable[i][j] == true)
				this->availabilityTable[i][j] = false;
		}
	}
}
