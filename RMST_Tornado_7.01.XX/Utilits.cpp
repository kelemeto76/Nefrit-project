// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "StdAfx.h"
#include <algorithm>

#include "Utilits.h"
#include "User.h"
#include "Main.h"		  
#include "GmSystem.h"
#include "GmSystem.h"

cUtilits Util;

void cUtilits::HookThis(DWORD dwMyFuncOffset, DWORD dwJmpOffset)
{
	*(DWORD*)(dwJmpOffset + 1) = dwMyFuncOffset-(dwJmpOffset+5);
}
									
void cUtilits::SetNop(DWORD dwOffset, int Size)
{
	for(int n=0; n < Size; n++)
		*(BYTE*)(dwOffset+n) = 0x90;
}

void cUtilits::SetRetn(DWORD dwOffset)
{
	*(BYTE*)(dwOffset) = 0xC3;
}

void cUtilits::SetRRetn(DWORD dwOffset)
{
	*(BYTE*)(dwOffset)=0xC3;
	*(BYTE*)(dwOffset+1)=0x90;
	*(BYTE*)(dwOffset+2)=0x90;
	*(BYTE*)(dwOffset+3)=0x90;
	*(BYTE*)(dwOffset+4)=0x90;
}
			   
void cUtilits::SetByte(DWORD dwOffset, BYTE btValue)
{
	*(BYTE*)(dwOffset) = btValue;
}

int cUtilits::GetPlayerIndex(char *Name)
{			 
	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{  	 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	 
		if(gObj->Connected < 3) continue; 	   
		if(!strcmp(gObj->Name, Name)) return i;
	}
	return -1;
}

							
int cUtilits::GetOnlineCount()
{		
	int online = 0;
	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{  	 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	 
		if(gObj->Connected < 3) continue; 	   
		online++;
	}
	return online;
}

int cUtilits::GetOnlineGMCount()
{		
	int online = 0;
	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{  	 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	 
		if(gObj->Connected < 3) continue; 	
		if(!GMS.IsGMBD(gObj->Name)) continue;
		online++;
	}
	return online;
}

int cUtilits::gObjIsConnected(int Index)
{
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);  
	if(gObj->Connected >= 3) 
		return 1;     
	return 0;
}

int cUtilits::GenExcOpt(int amount)
{
	// User input errors
	if (amount > 6) amount = 6;
	if (amount < 1) amount = 1;

	int opt_db[6]  = {1, 2, 4, 8, 16, 32};
	int exc = 0;

	std::random_shuffle(opt_db, opt_db + 6);

	for(int n=0; n < amount; n++)
	{
		exc += opt_db[n];
	}

	return exc;
}
int cUtilits::gObjZenSingle(OBJECTSTRUCT* gObj,OBJECTSTRUCT* tObj, int dmg, int tot_dmg)
{

    int exp;
    int maxexp = 0;
    int level = (tObj->Level+25)*tObj->Level/3;

    if((tObj->Level+10) < gObj->Level)
    {
        level = level*(tObj->Level+10)/gObj->Level;
    }

    if(tObj->Level >= 65)
    {
        level = level + (tObj->Level-64)*(tObj->Level/4);
    }

    if(level > 0)
    {
        maxexp = level/2;
    }
    else
    {
        level = 0;
    }

    if(maxexp < 1)
    {
        exp = level;
    }
    else
    {
        exp = level + rand()%maxexp;
    }

    exp = dmg * exp / tot_dmg;

    exp *= 1000;

    exp =  (float)exp * ((float)1000  / 100.0f);

    return exp;
}