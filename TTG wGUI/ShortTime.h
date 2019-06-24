#pragma once
#include <string>
class ShortTime
{
	int hour;
	int minute;
public:
	ShortTime(int hour, int minute) : hour(hour), minute(minute) {
		if (minute >= 60) {
			minute -= 60;
			if (++hour >= 24)
				hour -= 24;
		}
	}
	ShortTime() : hour(0), minute(0) {};
	ShortTime(int hour_minute) { //1430 = 14:30
		hour = hour_minute / 100;
		minute = hour_minute % 100;
		if (minute >= 60) {
			minute -= 60;
			if (++hour >= 24)
				hour -= 24;
		}
	}
	ShortTime operator+(const ShortTime other) {
		return ShortTime(this->getTime() + other.getTime());
	}
	ShortTime operator-(const ShortTime other) {
		return ShortTime(this->getTime() - other.getTime());
	}
	int operator/(ShortTime other) {
		int wholes = 0;
		ShortTime copy(this->getTime());
		while (!copy.subTime(other))
		{
			wholes++;
		}
		return wholes;
	}
	bool operator==(const ShortTime other) {
		if (this->hour == other.hour)
			if (this->minute == other.minute)
				return true;
		return false;
	}
	bool operator>(const ShortTime other) { //16:30 > 15:30 = true
		if (this->hour > other.hour)
			return true;
		else if (this->hour == other.hour)
			if (this->minute > other.minute)
				return true;
		return false;
	}
	bool operator>(const int other) { //16:30 > 15:30 = true
		if (this->hour * 100 + this->minute > other)
			return true;
		return false;
	}
	bool operator<(const ShortTime other) {
		return !(*this >= other);
	}
	bool operator>=(const ShortTime other) {
		if (*this == other)
			return true;
		return (*this > other);
	}
	bool operator<=(const ShortTime other) {
		return !(*this > other);
	}

	void setTime(int newHour, int newMinute) {
		hour = newHour;
		minute = newMinute;
		if (minute >= 60) {
			minute -= 60;
			if (++hour >= 24)
				hour -= 24;
		}
	}
	void setTime(int newHout_newMinute) {
		setTime(newHout_newMinute / 100, newHout_newMinute % 100);
	}
	void setTime(ShortTime newTime) {
		setTime(newTime.hour, newTime.minute);
	}

	void operator=(const int t) {
		this->setTime(t / 100, t % 100);
	}
	friend std::ostream& operator<<(std::ostream& out, const ShortTime& o) {
		if (o.minute < 10)
			out << o.hour << ":" << "0" << o.minute;
		else
			out << o.hour << ":" << o.minute;

		return out;
	}

	friend std::istream& operator>>(std::istream& in, ShortTime& o) {
		std::string h;
		in >> h;
		//if (h.length() < 4) {
		//	int timeInMin;
		//	o.setTime((h[0] - '0') * 1000 + (h[1] - '0') * 100 + (h[3] - '0') * 10 + (h[4] - '0'));
		//}
		//else
		o.setTime((h[0] - '0') * 1000 + (h[1] - '0') * 100 + (h[3] - '0') * 10 + (h[4] - '0'));
		return in;
	}

	bool addTime(int newHour, int newMinute) {
		hour += newHour;
		minute += newMinute;
		bool wrap[2] = { 0 };
		if (minute >= 60) {
			minute -= 60;
			hour++;
			wrap[0] = true;
		}
		else if (minute < 0) {
			minute += 60;
			hour--;
			wrap[0] = true;
		}
		if (hour >= 24) {
			hour -= 24;
			wrap[1] = true;
		}
		else if (hour < 0) {
			hour += 24;
			wrap[1] = true;
		}
		if (wrap[0] && wrap[1])
			return true;
		return false;
	}
	bool addTime(ShortTime timeUnit) {
		return addTime(timeUnit.hour, timeUnit.minute);
	}
	bool addTime(int newHour_newMinute) {
		return addTime(newHour_newMinute / 100, newHour_newMinute % 100);
	}

	bool subTime(int newHour, int newMinute) {
		hour -= newHour;
		minute -= newMinute;
		bool wrap = 0;
		if (minute < 0) {
			minute += 60;
			hour--;
		}
		if (hour < 0) {
			hour += 24;
			wrap = true;
		}
		if (wrap)
			return true;
		return false;
	}
	bool subTime(ShortTime timeUnit) {
		return subTime(timeUnit.hour, timeUnit.minute);
	}
	bool subTime(int newHour_newMinute) {
		return subTime(newHour_newMinute / 100, newHour_newMinute % 100);
	}

	int getHour()
	{
		return hour;
	}
	int getMinute()
	{
		return minute;
	}
	int getTime() const {
		return hour * 100 + minute;
	}
	std::string getString()
	{
		std::string str = "";
		str += std::to_string(hour);
		str += ':';
		if (minute < 10)
			str += '0';
		str += std::to_string(minute);
		return str;
	}
};
