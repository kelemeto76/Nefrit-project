#ifdef _GameServer_
#ifndef Moss_H
#define Moss_H
#include "User.h"

class MossGambler
{
public:
	MossGambler();
	~MossGambler();

	void OpenedMoss();
	void SpawnMoss();
	void LoadConfig();
	void LoadTimeConfig();

	void DisappearMoss();
	bool SellItem(int Index, unsigned char *aRecv);
	void ConvertToHex(unsigned char *buf);

	void GetRandomItem(unsigned char * aRecv);
	int RandomOption(int typeopt);
	int GetItemDurability(int dur, int lvlitem, int exc);

	int m_hour;
	int m_min;
	bool Opened;
	int Amount;
private:
	CItem ItemSend;
};

extern MossGambler g_Moss;
#endif
#endif