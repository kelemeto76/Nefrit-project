#ifndef GOLDENARCHER_H
#define  GOLDENARCHER_H



class GoldenArcher
{
public:				 

	GoldenArcher();
	~GoldenArcher();

	void Golden_Archer(int aIndex);
	bool IsRena(int aIndex);
	DWORD GAGetRewardItem();
	void GAGiveReward(int aIndex, DWORD Item);
	void GetPoints(int aIndex);
private:
	FILE * GA;
	unsigned short *ActualRena;
	int RemainedRena;

};

extern GoldenArcher GAsystem;
#endif