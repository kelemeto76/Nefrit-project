#ifndef JEWELS_H
#define JEWELS_H
#define MAX_ITEM_INFO 12
#define ITEMGET(x,y) ((x)*512+(y))
#include <Windows.h>
#include "User.h"

struct JewelConf
{
	int id;
	int rate;
	int opt1;
	int opt2;
	int minopt1;
	int minopt2;
	int maxopt1;
	int maxopt2;
	int fail1;
	int fail2;
	int failopt1;
	int failopt2;
	int successfully1;
	int successfully2;
};

struct PSBMSG_HEAD_Le
{
	BYTE type;
	BYTE size;
	BYTE head;
	BYTE subh;
};

struct PMSG_ITEM_MODIFY_SEND
{
	PSBMSG_HEAD_Le header;
	BYTE slot;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

struct PBMSG_HEAD_it
{
	BYTE type;
	BYTE size;
	BYTE head;
};

struct PMSG_USE_ITEM_RECV
{
	PBMSG_HEAD_it header;
	BYTE SourceSlot;
	BYTE TargetSlot;
	BYTE type;
};

class Jewels
{
public:
	Jewels();
	~Jewels();
	void LoadConfig();
	void DeleteItem(int aIndex, int Slot);
	int AddExcOpt(int exc, int maxopt , int success);
	void IdentifyJewel(PMSG_USE_ITEM_RECV * lpmsg, int aIndex);
	bool Verification(int Opt1, int Opt2,int MinOpt1, int MinOpt2, int MaxOpt1 , int MaxOpt2,int Success1, int Success2, CItem* ProtochItem);
	void Successfully(int Opt1 , int Opt2 , int MaxOpt1 , int MaxOpt2, int Success1 , int Success2 , CItem* ProtochItem);
	void Fail(int Opt1, int Opt2 , int Fail1, int Fail2 , int FailOpt1, int FailOpt2, CItem* ProtochItem);
private:
	int AmountJewel;
	JewelConf ArrayJewelConf[255];
	int RATE;
};

extern Jewels g_Jewels;

#endif