#ifndef VIP_H
#define VIP_H
unsigned long __stdcall VipInGameServer(int Index);
unsigned long __stdcall VipTickServer(int Index);
class cVIP
{
public:
	cVIP();
	~cVIP();

	void Init();   
	bool CheckVipTime(int TimeInMin);

	int NumStates;
	int Enabled;
	char Column[25];
	char ColumnDate[25];

	struct VIPConfig
	{ 
		char VIPName[8];
		int EnabledCmd;
		DWORD CostZen;
		int CostPCPoints;

		int CostWCoins;
		int BonusExp;
		int BonusZen;
		int BonusDrop; 
	};
	VIPConfig VIPState[20]; 

};
extern cVIP VIP; 
#endif