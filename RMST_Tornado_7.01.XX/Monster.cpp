// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "StdAfx.h"
#include "User.h"
#include "Main.h"
#include "PCPoints.h"
#include "Console.h"
#include "DropSystem.h"
#include "Utilits.h"
#include "Configs.h"
#include "HappyHour.h"
#include "Monster.h"
#include "Protection.h"
#include "VIP.h"
#include "MapSystem.h"
#ifdef _GameServer_			

void ReadMonsterAdd()
{
	if(cProtection.CheckFeature(cProtection.MonsterSpawn))
	{
		FILE* MonsterFile;
		if((MonsterFile = fopen( "..\\RMST\\MonsterSpawn.ini", "r")) == NULL)
		{						   
			MessageBox(NULL, "Cant Find MonsterSpawn.ini", "Error", 0);
			return;
		}
								
		rewind(MonsterFile);
		char Buff[255];	
		while(!feof(MonsterFile))
		{							
			Buff[0] = 0;
			fgets (Buff, 255, MonsterFile);	 
			if(Buff[0] == '/' || Buff[0] == '#' || Buff[0] == ' ' || strlen(Buff) < 9)
				continue;			

			int Mob = -1, Cnt = -1, Map = -1, Speed = -1, X = -1, Y = -1;
			sscanf(Buff, "%d %d %d %d %d %d", &Mob, &Cnt, &Speed, &Map, &X, &Y);

			if(Mob == -1 || Cnt == -1 || Map == -1 || X == -1 || Y == -1)
				continue;
								  
			for(int i = 0; i < Cnt; i++)
				MonsterAddAndSpawn(Mob,Speed,Map,X,Y);
		}	

		rewind(MonsterFile);
		fclose(MonsterFile);
	}
}

int MonsterAdd(WORD Monster,BYTE Map,BYTE X, BYTE Y)
{
	int MobCount = *(DWORD *)(MonsterCount);
	
	if((MobCount + 1) < OBJECT_MIN)
	{
		MobCount++;
		*(DWORD *)(MonsterCount) = MobCount;
		*(WORD *)(12 * MobCount + MonsterReads) = Monster;
		*(BYTE *)(12 * MobCount + MonsterReads+2) = Map;
		*(BYTE *)(12 * MobCount + MonsterReads+3) = 30;
		*(BYTE *)(12 * MobCount + MonsterReads+4) = X;
		*(BYTE *)(12 * MobCount + MonsterReads+5) = Y;
		*(BYTE *)(12 * MobCount + MonsterReads+6) = 2;
		*(BYTE *)(12 * MobCount + MonsterReads+7) = X;
		*(BYTE *)(12 * MobCount + MonsterReads+8) = Y;
		return MobCount;
	}else
	{		
		MessageBox(NULL,"Monster attribute max over!!", "Monsters overflow", 0);
		return -1;
	}
}

int MonsterAddAndSpawn(WORD Monster,BYTE Speed,BYTE Map,BYTE X, BYTE Y)
{
	int MobCount = *(DWORD *)(MonsterCount);
	
	if((MobCount + 1) < MonsterCount)
	{
		MobCount++;
		*(DWORD *)(MonsterCount) = MobCount;
		*(WORD *)(12 * MobCount + MonsterReads) = Monster;
		*(BYTE *)(12 * MobCount + MonsterReads+2) = Map;
		*(BYTE *)(12 * MobCount + MonsterReads+3) = Speed;
		*(BYTE *)(12 * MobCount + MonsterReads+4) = X;
		*(BYTE *)(12 * MobCount + MonsterReads+5) = Y;
		*(BYTE *)(12 * MobCount + MonsterReads+6) = 2;
		*(BYTE *)(12 * MobCount + MonsterReads+7) = X;
		*(BYTE *)(12 * MobCount + MonsterReads+8) = Y;

		int MobID = gObjAddMonster(*(BYTE *)(12 * MobCount + (MonsterReads+2)));
		if(MobID>=0)
		{
			int MobNr = *(WORD *)(12 * MobCount + MonsterReads);
			gObjSetPosMonster(MobID, MobCount);
			gObjSetMonster(MobID, MobNr);
		}

		return MobID;
	}else
	{		
		MessageBox(NULL,"Monster attribute max over!!", "Monsters overflow", 0);
		return -1;
	}
}
#endif

//Monster Death Control
int MygEventMonsterItemDrop(BYTE *b_MonsterDataAddr,BYTE *a_gObjAddr)
{
	// Get mIndex, aIndex & Monster & Player Objects
	PBYTE a_aIndex = 0;
	PBYTE b_mIndex = 0;

	a_aIndex = (PBYTE)a_gObjAddr;
	b_mIndex = (PBYTE)b_MonsterDataAddr;

	WORD mIndex = 0;
	WORD aIndex = 0;

	memcpy(&mIndex, b_mIndex+0x00,sizeof(WORD));
	memcpy(&aIndex, a_aIndex+0x00,sizeof(WORD));

	OBJECTSTRUCT *mObj = (OBJECTSTRUCT*) OBJECT_POINTER (mIndex);
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*) OBJECT_POINTER (aIndex);

	int SearchIndex = -1;
		for (int i=0; i<g_Happy.AmountLocations; i++)
		{
			if (Config.g_Conf[i].MapNum == pObj->MapNumber)
			{
				SearchIndex = i;
				break;
			}
	}
	//HappyHour
	if ( g_Happy.StartedEvents[SearchIndex] == 1)
	{
		mObj->Money += Config.g_Conf[SearchIndex].PlusZen;
	} 
	//VIP System
	if(VIP.Enabled)
	{
		if(AddTab[pObj->m_Index].IsVipInGame > 0)
		{
			int VIPInfo = AddTab[pObj->m_Index].IsVipInGame;
			mObj->Money += VIP.VIPState[VIPInfo].BonusZen;
		}
		if(AddTab[pObj->m_Index].IsVipInGame > 0)
		{
			int VIPInfo = AddTab[pObj->m_Index].IsVipInGame;
			mObj->m_wItemDropRate += VIP.VIPState[VIPInfo].BonusDrop;
		}
	}
	//MapSystem Module
	if(MapSystem.Enabled)
	{
		if(MapSystem.Config[pObj->MapNumber].Zen != 0)
		{
			mObj->Money += MapSystem.Config[mObj->MapNumber].Zen;
		} 
		if(MapSystem.Config[pObj->MapNumber].Drop != 0)
		{
			mObj->m_wItemDropRate += MapSystem.Config[mObj->MapNumber].Drop;
		}
	}
	if(Config.Zen.Enabled)
	{
			if(pObj->DbClass == 3 || pObj->DbClass == 19 || pObj->DbClass == 35 || pObj->DbClass == 51 || pObj->DbClass == 66 || pObj->DbClass == 83 || pObj->DbClass == 98)
			{
				int NewMoneyML = Util.gObjZenSingle(pObj,mObj,500,700);
				mObj->Money = (NewMoneyML/Config.Zen.MasterDivisor) * Config.Zen.MasterZen; 
			}
			else
			{
				int NewMoneyNormal = Util.gObjZenSingle(pObj,mObj,500,700);
				mObj->Money = (NewMoneyNormal/Config.Zen.NormalDivisor) * Config.Zen.NormalZen;
			}
	}
	if(Config.PartyZen.Enabled)
	{
			if(pObj->PartyNumber != -1)
			{
				int AllPartyLevel = 0;
				int Count = 0;

				for(int i=OBJECT_MIN;i<OBJECT_MAX;i++)
				{
					OBJECTSTRUCT *gObj = (OBJECTSTRUCT*) OBJECT_POINTER (i);

					if(gObj->Connected == 3)
					{
						if(gObj->PartyNumber == pObj->PartyNumber)
						{
							AllPartyLevel += gObj->Level;
							Count++;
						}
					}
				} 
				//Master Party
				if(pObj->DbClass == 3 || pObj->DbClass == 19 || pObj->DbClass == 35 || pObj->DbClass == 51 || pObj->DbClass == 66 || pObj->DbClass == 83 || pObj->DbClass == 98)
					mObj->Money = (mObj->Money * Config.PartyZen.MasterZen)*((AllPartyLevel/Count)/(Config.PartyZen.MasterDivisor)); 
				//Normal
				else  
					mObj->Money = (mObj->Money* Config.PartyZen.NormalZen)*((AllPartyLevel/Count)/Config.PartyZen.NormalDivisor);
			}
	}
	// Drop System
	if(cexDrop.DropItem(mObj,pObj))
	{
		return 1;
	}
	else // Default drop
	{
		int rValue = gEventMonsterItemDrop(b_MonsterDataAddr, a_gObjAddr);
		return rValue;
	}

	return 1;
}

// Monster die Handler (hooked replacement for gObjMonsterDieGiveItem)
void __cdecl MonsterDie(LPOBJ lpObj, LPOBJ lpTargetObj)
{ 

	// This Func For PC Points Monster Point Calc
	g_PointShop.LevelUp(lpTargetObj->m_Index, lpObj->Class);
	g_PointShop.LevelUpWC(lpTargetObj->m_Index, lpObj->Class);

#ifdef _GameServer_
	if(Config.EldaradoEventType)
	{
		int type;
		int level;
		int x;
		int y;
		float dur=0;
		int Option1=0;
		int Option2=0;
		int Option3=0;
		//int iRate;
		if ( lpObj->Class == 43 ) // Golden Budge Dragon
		{
			dur = 255.0f;
			x = lpObj->X;
			y = lpObj->Y;
			level = 0;
			type = ItemGetNumberMake(14, 11);
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, MaxHitUser, 0, 0);
			return; 
		}

		if ( lpObj->Class == 53 ) // Golden Titan
		{
			dur = 255.0f;
			x = lpObj->X;
			y = lpObj->Y;
			level = 9;
			type = ItemGetNumberMake(14, 11);
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, MaxHitUser, 0, 0);
			return;
		}
		else if ( lpObj->Class == 78 ) // Golden Goblin
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			x = lpObj->X;
			y = lpObj->Y;
			dur = 255.0f;
			level = 8;
			type = ItemGetNumberMake(14, 11);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, MaxHitUser, 0, 0);
			return;
		}
		else if ( lpObj->Class == 79 )	// Golden Derkon
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			x = lpObj->X;
			y = lpObj->Y;
			dur = 255.0f;
			level = 10;
			type = ItemGetNumberMake(14, 11);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, MaxHitUser, 0, 0);
			return;	
		}
		else if ( lpObj->Class == 80 )	//Golden Lizard King
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			x = lpObj->X;
			y = lpObj->Y;
			dur = 255.0f;
			level = 11;
			type = ItemGetNumberMake(14, 11);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, MaxHitUser, 0, 0);
			return;	
		}
		else if ( lpObj->Class == 82 )	// Golden Tantalos
		{
			int MaxHitUser = gObjMonsterTopHitDamageUser(lpObj);
			x = lpObj->X;
			y = lpObj->Y;
			dur = 255.0f;
			level = 12;
			type = ItemGetNumberMake(14, 11);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, x, y, type, level, dur,
				Option1, Option2, Option3, MaxHitUser, 0, 0);
			return;	
		}
	}
#endif	   
	// Original function
	gObjMonsterDieGiveItem(lpObj, lpTargetObj);
}

char Messages1[1024];

void NPCMessage(int IndexPlayer, LPOBJ mObj, char* Msg,...)
{						 
	Messages1[0] = 0;
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages1, Msg, pArguments1);
	va_end(pArguments1);

	ChatTargetSend(mObj, Messages1, IndexPlayer);
}

void NPCMessageLog(Logger::sColor LogColor, Logger::sLogType LogType, LPOBJ gObj, LPOBJ mObj, char* Msg,...)
{						  
	Messages1[0] = 0;
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages1, Msg, pArguments1);
	va_end(pArguments1);

	ChatTargetSend(mObj, Messages1, gObj->m_Index);
	// Type = 1 - в блоке сообщений 
	// Type = 0 - золотом по середине
		
	cLog.ConsoleOutPut(1, LogColor, LogType, "[%s]: %s", gObj->Name, Messages1);
}

void NPCMessageNear(LPOBJ mObj, char* Msg,...)
{						   
	Messages1[0] = 0;
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages1, Msg, pArguments1);
	va_end(pArguments1);

	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{						   
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	   
		if(gObj->Connected < 3) continue; 	   
		if(mObj->MapNumber != gObj->MapNumber) continue;	 		 
		if(mObj->X <= gObj->X-10 || mObj->X >= gObj->X+10) continue;
		if(mObj->Y <= gObj->Y-10 || mObj->Y >= gObj->Y+10) continue;
		ChatTargetSend(mObj, Messages1, i);
	}
}