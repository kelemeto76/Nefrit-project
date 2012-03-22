// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "StdAfx.h"
#include "User.h"
#include "Main.h"
#include "Configs.h"
#include "Console.h"
#include "ChatCommands.h"
#include "cSQL.h"
#include "PCPoints.h"
#include "AntiAFK.h"
#include "DuelManager.h"
#include "News.h"
#include "HappyHour.h"
#include "MossGambler.h"
#include "ChatCommands.h"
#include "Protection.h"
#include "VIP.h"
#include "MapSystem.h"
cUser User;			 
ADDITIONALSTRUCT AddTab[OBJECT_MAX]; 

							  
unsigned time_seed()
{
	time_t now = time ( 0 );
	unsigned char *p = (unsigned char *)&now;
	unsigned seed = 0;
	size_t i;

	for ( i = 0; i < sizeof now; i++ )
		seed = seed * ( UCHAR_MAX + 2U ) + p[i];

	return seed;
}

/*
void gObjSecTick(LPOBJ gObj)
{
	 if(gObj->Type == OBJECT_USER)
		gObjUserTick(gObj);
}*/


void gObjEventsTick()
{ 

		/*Moss The Gambler*/
#ifdef _GameServer_
	if (Config.Moss.Enabled && Config.Moss.EnabledTimer)
	{
		g_Moss.OpenedMoss();
	}
	else if(!Config.Moss.EnabledTimer)		
		g_Moss.Opened = true;
#endif
		/*Happy Hour*/
	if (Config.HappyHourEnabled)
	{
		g_Happy.TickTimerHappyHour();
	}
}

void gObjUserTick(LPOBJ gObj)
{
	int Index = gObj->m_Index;
	if(gObj->Connected == PLAYER_PLAYING)
	{	
		/*AntiAfk*/
		if(Config.AntiAfk.Enabled == 1)
			g_antiafk.SendInfo(gObj);

		AddTab[Index].OnlineSek++;
		if(AddTab[Index].OnlineSek >= 60)
		{
			AddTab[Index].OnlineSek = 0;
			AddTab[Index].OnlineMin++;
		}
		if(AddTab[Index].OnlineMin >= 60)
		{
			AddTab[Index].OnlineMin = 0;
			AddTab[Index].OnlineHour++;
			MySQL.Execute("UPDATE MEMB_STAT SET OnlineHours = OnlineHours + 1 WHERE memb___id = '%s'", gObj->AccountID);
		}
		
		if(Config.AddPCPointsSec > 0 && AddTab[Index].PCPlayerPoints + Config.AddPCPointsCount < Config.MaximumPCPoints)
		{
			int Sec = (3600 * AddTab[Index].OnlineHour) + (60 * AddTab[Index].OnlineMin) + AddTab[Index].OnlineSek;
			if(Sec % Config.AddPCPointsSec == 0 && Sec >= Config.AddPCPointsSec)
			{
				g_PointShop.IncreasePoints(Index, Config.AddPCPointsCount);	
				Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[PointShop] You earned %d Points for being online!", Config.AddPCPointsCount);
				Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[PointShop] You have been online %d sec %d min %d hour!", AddTab[Index].OnlineSek, AddTab[Index].OnlineMin, AddTab[Index].OnlineHour);
				MySQL.Execute("UPDATE Character SET %s = %s + '%d' WHERE Name= '%s'", Config.PCPointsColumn, Config.PCPointsColumn, Config.AddPCPointsCount, gObj->Name);
				    
				if(AddTab[Index].PCInfoReceived == 0)
				{
				    g_PointShop.GetInfo(Index);
				}
			}
		} 
	}
	else if(gObj->Connected == PLAYER_LOGGED)
	{ 
		AddTab[Index].OnlineMin = 0;   
		AddTab[Index].OnlineSek = 0;
		AddTab[Index].OnlineHour = 0;
	}
}

void cUser::gObjCharacterWingsSetPreview(short ItemType, unsigned char *CharSet,int Type,OBJECTSTRUCT *lpObj) 
{	
	if(Type == GUARDIAN)
	{
		register unsigned char PreviewPet = 0;

		switch(ItemType)
		{
		case 0x1A50: //Panda
			PreviewPet = 224;
			break;

		case 0x1A43:
			PreviewPet = 0x80;
			break;

		case 0x1A41:
			PreviewPet = 0x40;
			break;

		case 0x1A40:
			PreviewPet = 0x20;
			break;
		
		case 0x1A7B: //Skeleton
			CharSet[5] -= 3;
			PreviewPet = 96; //112
			break; 

		default:
			PreviewPet = 0;
			break;
		}
		
		CharSet[16] |= PreviewPet;
	}
}
		   
void __stdcall gObjViewportPatchExecute(OBJECTSTRUCT *lpObj) {

	if(lpObj->pInventory[GUARDIAN].m_Type != SLOT_EMPTY)
		User.gObjCharacterWingsSetPreview(lpObj->pInventory[GUARDIAN].m_Type, lpObj->CharSet, GUARDIAN,lpObj);
}
						  
#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )

//Fix Visual in Select Character
void __stdcall JGPGetCharInfoEx(int Loop, unsigned char *DSPacket, unsigned char *toClientPacket) {
	unsigned char ItemType = DSPacket[76 + (66 * Loop)]; //72 - Wings, 76 - Guardian
	register int nCol = (Loop * 34) + 38;
	register int CharSet5 = (Loop * 34) + 27; //CharSet[5]
	register int CharSet9 = (Loop * 34) + 31; //CharSet[9]
	register int CharSet10 = (Loop * 34) + 32; //CharSet[10]
	register int CharSet15 = (Loop * 34) + 37; //CharSet[15]
	register int CharSet17 = (Loop * 34) + 39; //CharSet[17]

	unsigned char Boots = DSPacket[68 + (66 * Loop)]; //Boots on Char
	unsigned char BootsLvl = DSPacket[69 + (66 * Loop)]; //Boots Level

	switch(ItemType)
	{
	case 0x50:
		toClientPacket[nCol] |= 224;
		break;
	case 0x7B:
		toClientPacket[CharSet5]  -= 3;
		toClientPacket[nCol] |= 96;

		break;
	}

	if(Boots == 255)
	{
		toClientPacket[CharSet5] |= CS_SET_BOOTS3(-1);
	}
}					
					   
bool CheckMaxPoints(BYTE type, OBJECTSTRUCT* lpObj)
{			 
	bool bResult = false;

	int MaxPoints = 32000;
	if(Config.Use65kStats >= 1)
		 MaxPoints = -536;	
				   
	int Stats;
	switch (type)
	{
		case 0x00:
			Stats = lpObj->Strength;  
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(lpObj->DbClass == 0 || lpObj->DbClass == 1 || lpObj->DbClass == 2 || lpObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Strength; 		
				else if(lpObj->DbClass == 16 || lpObj->DbClass == 17 || lpObj->DbClass == 18 || lpObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Strength; 		
				else if(lpObj->DbClass == 32 || lpObj->DbClass == 33 || lpObj->DbClass == 34 || lpObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Strength; 		
				else if(lpObj->DbClass == 48 || lpObj->DbClass == 49 || lpObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Strength; 		
				else if(lpObj->DbClass == 64 || lpObj->DbClass == 65 || lpObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Strength; 		
				else if(lpObj->DbClass == 80 || lpObj->DbClass == 81 || lpObj->DbClass == 82 || lpObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Strength; 		
			}
			break;
		case 0x01:
			Stats = lpObj->Dexterity;  	 
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(lpObj->DbClass == 0 || lpObj->DbClass == 1 || lpObj->DbClass == 2 || lpObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Agility; 		
				else if(lpObj->DbClass == 16 || lpObj->DbClass == 17 || lpObj->DbClass == 18 || lpObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Agility; 		
				else if(lpObj->DbClass == 32 || lpObj->DbClass == 33 || lpObj->DbClass == 34 || lpObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Agility; 		
				else if(lpObj->DbClass == 48 || lpObj->DbClass == 49 || lpObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Agility; 		
				else if(lpObj->DbClass == 64 || lpObj->DbClass == 65 || lpObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Agility; 		
				else if(lpObj->DbClass == 80 || lpObj->DbClass == 81 || lpObj->DbClass == 82 || lpObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Agility; 			
			}
			break;
		case 0x02:	  
			Stats = lpObj->Vitality;
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(lpObj->DbClass == 0 || lpObj->DbClass == 1 || lpObj->DbClass == 2 || lpObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Vitality; 		
				else if(lpObj->DbClass == 16 || lpObj->DbClass == 17 || lpObj->DbClass == 18 || lpObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Vitality; 		
				else if(lpObj->DbClass == 32 || lpObj->DbClass == 33 || lpObj->DbClass == 34 || lpObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Vitality; 		
				else if(lpObj->DbClass == 48 || lpObj->DbClass == 49 || lpObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Vitality; 		
				else if(lpObj->DbClass == 64 || lpObj->DbClass == 65 || lpObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Vitality; 		
				else if(lpObj->DbClass == 80 || lpObj->DbClass == 81 || lpObj->DbClass == 82 || lpObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Vitality; 		
			}
			break;	  
		case 0x03:
			Stats = lpObj->Energy;	 			
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(lpObj->DbClass == 0 || lpObj->DbClass == 1 || lpObj->DbClass == 2 || lpObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Energy; 		
				else if(lpObj->DbClass == 16 || lpObj->DbClass == 17 || lpObj->DbClass == 18 || lpObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Energy; 		
				else if(lpObj->DbClass == 32 || lpObj->DbClass == 33 || lpObj->DbClass == 34 || lpObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Energy; 		
				else if(lpObj->DbClass == 48 || lpObj->DbClass == 49 || lpObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Energy; 		
				else if(lpObj->DbClass == 64 || lpObj->DbClass == 65 || lpObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Energy; 		
				else if(lpObj->DbClass == 80 || lpObj->DbClass == 81 || lpObj->DbClass == 82 || lpObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Energy; 			
			}
			break;
		case 0x04:
			Stats = lpObj->Leadership;	 	 		
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(lpObj->DbClass == 64 || lpObj->DbClass == 65 || lpObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Command; 		
			}
			break;
	}
 
	if(MaxPoints > 32767)
		MaxPoints = -32768 + (MaxPoints - 32767);

	if((MaxPoints > 0) && (Stats >= MaxPoints || Stats < 0))
		bResult = true;								 
	else if ((MaxPoints < 0) && (Stats < 0) && (Stats >= MaxPoints))
		bResult = true;

	return bResult;
}
		 
void gObjLevelUpPointAddEx(BYTE type, OBJECTSTRUCT* lpObj)
{
	bool Error = false;

	if(CheckMaxPoints(type, lpObj))
		Error = true;

	if (Error == true)
	{
		BYTE cShowAddPointError[0x30] ={0xC1,0x30,0x0D,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 }  ;
		sprintf((char *)&cShowAddPointError[0x0D], "Error in Stats adding"); 
	}
	else
		gObjLevelUpPointAdd(type, lpObj);

}


//Wait 2 miliseconds for update
void TradeSystem__Cancel(void * lpParam)
{	
	Sleep(200);
	for(int i=OBJECT_MIN;i<OBJECT_MAX;i++)
	{									  
		OBJECTSTRUCT * gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);
		if(gObj->Connected == 3)
		{
			if(gObj->pInventory[RING_01].m_Type == 0x1A7A 
				|| gObj->pInventory[RING_02].m_Type == 0x1A7A) //Skeleton Ring
			{
				gObj->m_Change = 14;

				gObjViewportListProtocolCreate(gObj);
			}
		}
	}
	_endthread();
}

void GCKillPlayerExpSendHook(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)
{   
	// -----
	// -----
     OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*) OBJECT_POINTER (aIndex);
     unsigned int pNewExperience = exp;
     unsigned int pBonusExp = 0;
     unsigned int pNewExperenceML = exp;
     unsigned int pBonusExpML = 0;
     // ----
     if(lpObj->pInventory[8].m_Type == 0x1A50) // Panda
     {          
          pBonusExp               = ((exp * Config.Panda.PetPandaExpirence) / 100);
          pBonusExpML               = ((exp * Config.Panda.PetPandaMLExpirence) / 100);
          // ----
          pNewExperience          += pBonusExp;
          pNewExperenceML          += pBonusExpML;
          // ----
          lpObj->Experience     += pBonusExp;
          lpObj->MLExp          += pBonusExpML;
     }

	if(lpObj->pInventory[10].m_Type == 0x1A4C || lpObj->pInventory[11].m_Type == 0x1A4C) // Panda Ring
     {          
          pBonusExp               = ((exp * Config.Panda.PandaRingExpirence) / 100);
          pBonusExpML               = ((exp * Config.Panda.PandaRingMLExpirence) / 100);
          // ----
          pNewExperience          += pBonusExp;
          pNewExperenceML          += pBonusExpML;
          // ----
          lpObj->Experience     += pBonusExp;
          lpObj->MLExp          += pBonusExpML;
     }
			int Index = gObjGetIndex(AddTab[lpObj->m_Index].MarryName); 
		if(Config.Marry.MarryExpBonus > 0 && Index != -1)
		{
		  pBonusExp               = ((exp * Config.Marry.MarryExpBonus) / 100);
          pBonusExpML               = ((exp * Config.Marry.MarryExpBonus) / 100);
          // ----
          pNewExperience          += pBonusExp;
          pNewExperenceML          += pBonusExpML;
          // ----
          lpObj->Experience     += pBonusExp;
          lpObj->MLExp          += pBonusExpML;
		}

		int VIPInfo = AddTab[lpObj->m_Index].IsVipInGame;
		if(AddTab[lpObj->m_Index].IsVipInGame > 0)
		{
		  pBonusExp               = ((exp * VIP.VIPState[VIPInfo].BonusExp) / 100);
          pBonusExpML               = ((exp * VIP.VIPState[VIPInfo].BonusExp) / 100);
          // ----
          pNewExperience          += pBonusExp;
          pNewExperenceML          += pBonusExpML;
          // ----
          lpObj->Experience     += pBonusExp;
          lpObj->MLExp          += pBonusExpML;
		}
				if(MapSystem.Enabled && MapSystem.Config[lpObj->MapNumber].Exp > 0)
		{
		  pBonusExp               = ((exp * MapSystem.Config[lpObj->MapNumber].Exp) / 100);
          pBonusExpML               = ((exp * MapSystem.Config[lpObj->MapNumber].Exp) / 100);
          // ----
          pNewExperience          += pBonusExp;
          pNewExperenceML          += pBonusExpML;
          // ----
          lpObj->Experience     += pBonusExp;
          lpObj->MLExp          += pBonusExpML;
		}
	if (true)
	{
		int SearchIndex = -1;
		for (int i=0; i<g_Happy.AmountLocations; i++)
		{
			if (Config.g_Conf[i].MapNum == lpObj->MapNumber)
			{
				SearchIndex = i;
				break;
			}
		}

	
		if ( g_Happy.StartedEvents[SearchIndex] == 1)
		{
			pBonusExp               = ((exp * Config.g_Conf[SearchIndex].PlusExp) / 100);
			pBonusExpML               = ((exp * Config.g_Conf[SearchIndex].PlusExp) / 100);
			// ----
			pNewExperience          += pBonusExp;
			pNewExperenceML          += pBonusExpML;
			// ----
			lpObj->Experience     += pBonusExp;
			lpObj->MLExp          += pBonusExpML; 
		}

	}

	
     GCKillPlayerExpSend(aIndex , TargetIndex , pNewExperience , AttackDamage , MSBFlag);
}

void MyObjCalCharacter(int aIndex)
{
     OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*) OBJECT_POINTER (aIndex);
     // -----
     gObjCalCharacter(aIndex);
     // -----
     if(lpObj->pInventory[8].m_Type == 0x1A50) //Panda
     {
          lpObj->m_Defense                  += Config.Panda.PetPandaDefense;
          lpObj->m_AttackDamageMinLeft		+= Config.Panda.PetPandaAttackDamageMinLeft;
          lpObj->m_AttackDamageMaxLeft		+= Config.Panda.PetPandaAttackDamageMaxLeft;
          lpObj->m_AttackDamageMinRight     += Config.Panda.PetPandaAttackDamageMinRight;
          lpObj->m_AttackDamageMaxRight     += Config.Panda.PetPandaAttackDamageMaxRight;
          lpObj->m_MagicDamageMin           += Config.Panda.PetPandaMagicDamageMin;
          lpObj->m_MagicDamageMax           += Config.Panda.PetPandaMagicDamageMax;
          lpObj->m_MagicSpeed               += Config.Panda.PetPandaMagicSpeed;
          lpObj->m_AttackSpeed              += Config.Panda.PetPandaAttackSpeed;
     }
   if(lpObj->pInventory[10].m_Type == 0x1A4C || lpObj->pInventory[11].m_Type == 0x1A4C) // Panda Ring
    {
         lpObj->m_Defense                   += Config.Panda.PandaRingDefense;
         lpObj->m_AttackDamageMinLeft		+= Config.Panda.PandaRingAttackDamageMinLeft;
         lpObj->m_AttackDamageMaxLeft		+= Config.Panda.PandaRingAttackDamageMaxLeft;
         lpObj->m_AttackDamageMinRight		+= Config.Panda.PandaRingAttackDamageMinRight;
         lpObj->m_AttackDamageMaxRight		+= Config.Panda.PandaRingAttackDamageMaxRight;
         lpObj->m_MagicDamageMin            += Config.Panda.PandaRingMagicDamageMin;
         lpObj->m_MagicDamageMax            += Config.Panda.PandaRingMagicDamageMax;
         lpObj->m_MagicSpeed                += Config.Panda.PandaRingMagicSpeed;
         lpObj->m_AttackSpeed               += Config.Panda.PandaRingAttackSpeed;	
    }
}
#ifdef _GameServer_
void CMapClass__ZenDrop(int MapNumber, int X, int Y, unsigned int ZenAmount){ 

	__asm {
		MOV ECX, Y;
		PUSH ECX

		MOV EDX, X;
		PUSH EDX

		MOV EAX, ZenAmount;
		PUSH EAX

		MOV ECX, MapNumber;
		IMUL ECX, ECX, offset_MapClass_BASE;
		ADD ECX, offset_MapClass_OFFSET;
		MOV EDI, offset_MapClass_MoneyItemDrop;
		CALL EDI;
	};
}
#endif
void GCEquipmentSendHook(int aIndex)
{							
	OBJECTSTRUCT * gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

	GCEquipmentSend(aIndex);
	
	//Fix Double Pets on trade cancel/ok/success
	if(gObj->pInventory[8].m_Type != SLOT_EMPTY)
	{
		if(gObj->pInventory[8].m_Type == 0x1A50 || gObj->pInventory[8].m_Type == 0x1A7B)
		{	
			CItem OldItem;
			OldItem = gObj->pInventory[8];
			gObj->pInventory[8].m_Type = -1;

			gObjMakePreviewCharSet(aIndex);
			GCItemListSend(aIndex);
			gObjViewportListProtocolCreate(gObj);

			gObj->pInventory[8] = OldItem;

			gObjMakePreviewCharSet(aIndex);
			GCItemListSend(aIndex);
			gObjViewportListProtocolCreate(gObj);
		}
	}
	
	if(gObj->pInventory[RING_01].m_Type == 0x1A7A 
		|| gObj->pInventory[RING_02].m_Type == 0x1A7A) //Skeleton Ring
		_beginthread( TradeSystem__Cancel, 0, NULL  );
}

BOOL __cdecl gObjGameClose_Func(int aIndex)
{	 

	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);	  
	//MySQL.Execute("UPDATE Character SET %s = '%d' WHERE Name= '%s'", Config.PCPointsColumn, AddTab[aIndex].PCPlayerPoints, gObj->Name); 
	switch(GMS.IsAdmin(gObj->Name))
	{																									   
	case 1:
		Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_GM, gObj, "[Admin] %s exit the game!", gObj->Name);
		break;
	case 2:
		Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_GM, gObj, "[GM] %s exit the game!", gObj->Name);
		break;
	} 
	BOOL rValue = gObjGameClose(aIndex);
	return rValue;		  	
}
#ifdef _GameServer_
bool HookSetDuelOption(int lpObj, BOOL bState)
{
	g_DuelSystem.SetDuelOption(lpObj, bState);
	return true;
}
// --------------------------------------------------------------------------------------------
bool HookIsDuelEnable(int aIndex)
{
	if(!g_DuelSystem.IsDuelEnable(aIndex))
	{
		return false;
	}
	else 
	{
		return true;
	}
}
// --------------------------------------------------------------------------------------------

bool HookIsOnDuel(int lpObj, int lpObj2) 
{
	if(!g_DuelSystem.IsOnDuel2(lpObj, lpObj2))
	{
		return false;
	}
	else 
	{
		return true;
	}
}
// --------------------------------------------------------------------------------------------

bool HookDuelCheck1(LPOBJ lpObj)
{ 
    bool bResult = false;
    // ----
    if(lpObj->Type == OBJECT_USER)
    {                                   
        if(g_DuelSystem.DuelCheck(lpObj)) 
            bResult = true;
        else
            bResult = goBjHoookLoad1(lpObj);
    }                
    return bResult;
}
// --------------------------------------------------------------------------------------------

bool HookDuelCheck2(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	bool cResult = false;
	// ----
	if(lpObj->Type == OBJECT_USER || lpTargetObj->Type == OBJECT_USER)
	{													
		if(g_DuelSystem.DuelCheck(lpObj, lpTargetObj))
			cResult = true;
		else
			goBjHoookLoad2(lpObj, lpTargetObj);
	}							
	return cResult;
}
// --------------------------------------------------------------------------------------------

void GSgObjUserDie(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpTargetObj)
{
	if(lpObj->Type == OBJECT_USER && lpTargetObj->Type == OBJECT_USER ) 
	{
		if(g_DuelSystem.DuelCheck(lpObj, lpTargetObj))
		{
			int aIndex = lpObj->m_Index;
			int DuelRoom = AddTab[aIndex-OBJECT_MIN].m_iDuelRoom;
			g_DuelSystem.PlayerScore(lpTargetObj);
			g_DuelSystem.KillUserProc(lpTargetObj,lpObj);
			return;
		}
	}						  
	gObjUserDie(lpObj, lpTargetObj);
}
// --------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------
// This Function For Set Duel Info in Game;
void DuelSetInfo(int aIndex)
{
    // ----
    OBJECTSTRUCT * lpObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
    // ----
    if(SetDuelState(aIndex, lpObj->AccountID, lpObj->Name) == FALSE)
    {
        AddTab[aIndex - OBJECT_MIN].m_iDuelWins = 0;
        AddTab[aIndex - OBJECT_MIN].m_iDuelLose = 0;
        AddTab[aIndex - OBJECT_MIN].m_iAllDuels = 0;
        // ----
        cLog.ConsoleOutPut(1, cLog.c_Green, cLog.t_Duel, "[Duel System] Character [%s] Duels: [%d] Wins: [%d] Loses: [%d]", 
                                lpObj->Name, AddTab[aIndex - OBJECT_MIN].m_iAllDuels, AddTab[aIndex - OBJECT_MIN].m_iDuelWins,
                                AddTab[aIndex - OBJECT_MIN].m_iDuelLose);
    }
    else
    {
        cLog.ConsoleOutPut(1, cLog.c_Green, cLog.t_Duel, "[Duel System] Character [%s] Duels: [%d] Wins: [%d] Loses: [%d]",
                                 lpObj->Name, AddTab[aIndex - OBJECT_MIN].m_iAllDuels, AddTab[aIndex - OBJECT_MIN].m_iDuelWins,
                                 AddTab[aIndex - OBJECT_MIN].m_iDuelLose);
    }
}
// --------------------------------------------------------------------------------------------

void SaveDuel(char FirstAcc[11], char FirstName[11], char SecondAcc[11], char SecondName[11], int Point1, int Point2)
{
	MySQL.Execute("UPDATE Character SET DuelWins=DuelWins+1, DuelLoses=DuelLoses, LastDuel = GETDATE(),DuelsTotal = DuelsTotal+1 WHERE AccountID = '%s' AND Name = '%s'",FirstAcc, FirstName);
	MySQL.Execute("UPDATE Character SET DuelWins=DuelWins, DuelLoses=DuelLoses+1, LastDuel = GETDATE(),DuelsTotal = DuelsTotal+1 WHERE AccountID = '%s' AND Name = '%s'",SecondAcc, SecondName);
}

int SetDuelState(int aIndex, char AccountID[11], char Name[11])
{	
	MySQL.Execute("SELECT DuelWins FROM Character WHERE AccountID = '%s' AND Name = '%s'", AccountID, Name);
	int Wins = MySQL.GetInt();
	MySQL.Execute("SELECT DuelLoses FROM Character WHERE AccountID = '%s' AND Name = '%s'", AccountID, Name);
	int Lose = MySQL.GetInt();
	int All = Wins+Lose;
	AddTab[aIndex-OBJECT_MIN].m_iDuelWins = Wins;
	AddTab[aIndex-OBJECT_MIN].m_iDuelLose = Lose;
	AddTab[aIndex-OBJECT_MIN].m_iAllDuels = All;
	return true;
}
#endif
