// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //
#include "User.h"
#ifndef UTILITS_H
#define UTILITS_H

class cUtilits
{
public:
	void HookThis(DWORD dwMyFuncOffset, DWORD dwJmpOffset);
	void SetNop(DWORD dwOffset, int Size);
	void SetRetn(DWORD dwOffset);
	void SetRRetn(DWORD dwOffset);
	void SetByte(DWORD dwOffset, BYTE btValue);	 

	int gObjIsConnected(int Index);
									  
	int GetPlayerIndex(char *Name);
	int GetOnlineCount();				 
	int GetOnlineGMCount();

	int GenExcOpt(int amount);

	int gObjZenSingle(OBJECTSTRUCT* gObj,OBJECTSTRUCT* tObj, int dmg, int tot_dmg);
};
extern cUtilits Util;
#endif