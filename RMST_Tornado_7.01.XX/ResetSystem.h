// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "User.h"

#ifndef RSystem_H
#define RSystem_H	 
								      
	unsigned long __stdcall ClearSkills(char Name[11]);
	unsigned long __stdcall ClearQuest(char Name[11]);
	unsigned long __stdcall ResetTimer(int Index);

class RSystem
{
public:	  
	RSystem();
	~RSystem();
	void Init();			 
	void StatusInit();					 
	void Reset(LPOBJ gObj,LPOBJ mObj);	  	
	void MainReset(LPOBJ gObj,LPOBJ mObj);	 

private:
	enum ForRole{NeedLvl, Points};										  
	enum ForPoints{Strength, Dexterity, Vitality, Energy, Leadership};
																			   
	int GetStartPoints(unsigned char Class, ForPoints);
	long int GetPoints(LPOBJ gObj, int NumState, int Formula, int Points);

	int CheckState(DWORD PlayerID);
	int CheckRole(unsigned char Class,int NumState, ForRole);
	bool CheckItem(LPOBJ gObj,int);
	void CheckItemSerial(LPOBJ gObj,int,BYTE);
	void OffStates(int i);
							   	
	int NumStates;	   
	int Worked;	

	struct SClear
	{
		int Points;
		int Invent;
		int Skills;
		int Quest;
		int Command;
		int PcPoint;
		int WCoin;
		int Zen;
	};

	struct SItems
	{
		unsigned long Serial;
		int Group;
		short Index;
		int Type;	
		int Lvl;
		int Skill;
		int Luck;
		int Opt;
		int Exc;
		int Num;
	};

	struct SNeed
	{			 
		int LvlDK;
		int LvlDW;
		int LvlELF;
		int LvlDL;
		int LvlMG;
		int LvlSUM;
		int Zen;
		int PcPoint;
		int WCoin;
		int NumItems;  
		SItems Items[20];
	};

	struct SAdd
	{				  
		int Formula;
		int PointsDW;
		int PointsDK;
		int PointsELF;
		int PointsMG;
		int PointsDL;
		int PointsSUM;
		int PcPoint;   
		int WCoin;
	};

	struct SState
	{ 
		bool Work;
		int MaxRes;
		SClear Clear;
		bool Serial[76];
		SNeed Need;
		SAdd Add;	
	};

	SState State[20];
};				   
extern RSystem ResetSystem;
#endif