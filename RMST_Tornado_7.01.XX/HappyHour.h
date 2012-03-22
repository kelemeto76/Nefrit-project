#ifndef HappyHour_h
#define HappyHour_h

#include "User.h"

class HappyHour
{
public:
	HappyHour();
	~HappyHour();

	void LoadConfig();
	void TickTimerHappyHour();
	char* GetMapName(int MapId);

	int StartedEvents[64];
	int AmountLocations;
private:
	int m_Hour;
	int m_Min;
	int m_Sec;
};

extern HappyHour g_Happy;

#endif