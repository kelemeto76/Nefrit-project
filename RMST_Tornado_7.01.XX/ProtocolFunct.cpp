// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "StdAfx.h"
#include "Protocol.h"
#include "Prodef.h"
#include "Main.h"
#include "User.h"
#include "Configs.h"
#include "Console.h"
#include "cSQL.h"
#include "GmSystem.h"
#include "ChatCommands.h"
#include "PCPoints.h"
#include "Utilits.h"
#include "Monster.h"
#include "ResetSystem.h"
#include "DuelManager.h"
#include "MossGambler.h"
#include "MarrySystem.h"
#include "Protection.h"
#include "VIP.h" 
cProtoFunc Protocol;
cProtoSendFunc ProtocolSend;
					 
bool cProtoFunc::CharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex)
{
	bool bResult = false;
	for(int i = 0; i < sizeof(lpMsg->Name); i++)
	{
		if(!isalnum(lpMsg->Name[i]) && lpMsg->Name[i] != ' ' && lpMsg->Name[i] != NULL)
		{						
			bResult = true;
		}
	}
	return bResult;
}

bool cProtoFunc::GuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg)
{
	bool bResult = false;
	for(int i = 0; i < sizeof(lpMsg->GuildName); i++)
	{
		if(!isalnum(lpMsg->GuildName[i]) && lpMsg->GuildName[i] != ' ' && lpMsg->GuildName[i] != NULL)
		{						
			bResult = true;

		}
	}		
	if(bResult)
	{
	 	PMSG_GUILDCREATED_RESULT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x56, sizeof(pMsg));
		pMsg.Result = 5;
		
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}	   
	return bResult;
}

bool cProtoFunc::CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex)
{	
	int number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);	 
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*)OBJECT_POINTER(number);
																			 
	if(gObj->Level > pObj->Level && gObj->Level - pObj->Level >= Config.PartyGapLvl)
	{	
		Chat.MessageLog(1, cLog.c_Red, cLog.t_Default, gObj, "[Party] You can't stay with %s in party! %s needs %d more lvl.", pObj->Name, pObj->Name, gObj->Level-Config.PartyGapLvl - pObj->Level);
		return true;
	}

	if(gObj->Level < pObj->Level && pObj->Level - gObj->Level >= Config.PartyGapLvl)
	{																													
		Chat.MessageLog(1, cLog.c_Red, cLog.t_Default, gObj, "[Party] You can't stay with %s in party! You need %d more lvl.", pObj->Name, pObj->Level - Config.PartyGapLvl - gObj->Level);
		return true;
	}	   
	return false;
}

void cProtoFunc::CheckRing(LPOBJ gObj, LPBYTE aRecv)
{
    if((aRecv[4] == RING_01 && gObj->pInventory[RING_02].m_Type != 0x1A4C) 
		|| (aRecv[4] == RING_02 && gObj->pInventory[RING_01].m_Type != 0x1A4C))
        if(gObj->m_Change == 503) //заменяем номер скина на -1 если снимаем ринг
        {
            gObj->m_Change = -1;	
            gObjViewportListProtocolCreate(gObj);	
        }	
}							  

void cProtoFunc::PlayerConnect(LPOBJ gObj)
{	    										
	AddTab[gObj->m_Index].AddValue = 0;	 
	AddTab[gObj->m_Index].ArcherObj = NULL;		 
	AddTab[gObj->m_Index].AFK_MapNumber = 0;  
	AddTab[gObj->m_Index].AFK_Timer = 0;
	AddTab[gObj->m_Index].AFK_X = 0;
	AddTab[gObj->m_Index].AFK_Y = 0;		
	AddTab[gObj->m_Index].PostDelay = 0;  
	AddTab[gObj->m_Index].ExType = -1;
	AddTab[gObj->m_Index].AddType = -1;		  
	AddTab[gObj->m_Index].MarryType = -1;	   	
	AddTab[gObj->m_Index].ResType = -1;		 

	MySQL.Execute("SELECT IsMarried FROM Character WHERE Name = '%s'", gObj->Name);	
	AddTab[gObj->m_Index].IsMarried = MySQL.GetInt();

	MySQL.Execute("SELECT MarryName FROM Character WHERE Name = '%s'", gObj->Name);
	MySQL.GetString(AddTab[gObj->m_Index].MarryName);	  										   

	MySQL.Execute("SELECT %s FROM Character WHERE Name = '%s'", Config.ResetColumn, gObj->Name);	 
	AddTab[gObj->m_Index].Resets = MySQL.GetInt();
	 
	LoginMsg(gObj);
	RingSkin(gObj);	
	g_PointShop.GetInfo(gObj->m_Index); 

	if(cProtection.CheckFeature(cProtection.VIPSystem))
	{ 			
	    if(Config.VIPServ.Enabled)
		{												 
			MySQL.Execute("SELECT %s FROM [%s].[dbo].[MEMB_INFO] WHERE memb___id='%s'",Config.VIPServ.Column,MySQL.szDatabase2,gObj->AccountID);
			AddTab[gObj->m_Index].IsVipServer = MySQL.GetInt();

			MySQL.Execute("SELECT %s FROM [%s].[dbo].[MEMB_INFO] WHERE memb___id='%s'",Config.VIPServ.ColumnDate,MySQL.szDatabase2,gObj->AccountID);
			AddTab[gObj->m_Index].VipMinutesServer = MySQL.GetInt();

			if(AddTab[gObj->m_Index].IsVipServer > 0)
			{											 
				DWORD ThreadID;
				HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VipTickServer, (void*)gObj->m_Index, 0, &ThreadID);

				if ( hThread == 0 )
				{
					cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
					return;
				}

				CloseHandle(hThread);
				Chat.MessageLog(1, cLog.c_Red, cLog.t_VIP, gObj, "[VIP] Left %d minutes of VIP Server.", AddTab[gObj->m_Index].VipMinutesServer);
			}
			else
			{
				Chat.MessageLog(1, cLog.c_Red, cLog.t_VIP, gObj, "[VIP] You are not vip user! Disconnected from the server.");
				CloseClient(gObj->m_Index);
			} 
		}

		if(!Config.VIPServ.Enabled && VIP.Enabled)
		{
			MySQL.Execute("SELECT %s FROM [%s].[dbo].[Character] WHERE Name='%s'",VIP.Column,MySQL.szDatabase,gObj->Name);
			AddTab[gObj->m_Index].IsVipInGame = MySQL.GetInt();

			MySQL.Execute("SELECT %s FROM [%s].[dbo].[Character] WHERE Name='%s'",VIP.ColumnDate,MySQL.szDatabase,gObj->Name);
			AddTab[gObj->m_Index].IsInGameVipMinutes = MySQL.GetInt();

			if(AddTab[gObj->m_Index].IsVipInGame > 0)
			{											 
				DWORD ThreadID;
				HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VipInGameServer, (void*)gObj->m_Index, 0, &ThreadID);

				if ( hThread == 0 )
				{
					cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
					return;
				}

				CloseHandle(hThread);
				Chat.MessageLog(1, cLog.c_Red, cLog.t_VIP, gObj, "[VIP] Left %d minutes of VIP.", AddTab[gObj->m_Index].IsInGameVipMinutes );
			}
		}
	
	}
	
	
#ifdef _GameServer_
	
	if(Config.Duel.Enabled)
	{
		if(Config.Duel.Ranking)
		{
			DuelSetInfo(gObj->m_Index);
		}
				
	if((!g_DuelSystem.IsOnDuel(gObj->m_Index)) && gObj->MapNumber == 64)
		{
			gObjMoveGate(gObj->m_Index, 294);
			cLog.ConsoleOutPut(1, cLog.c_Blue ,cLog.t_Duel, "[Duel System][%s][%s] Spawn on duel map after duel is not allowed", gObj->AccountID, gObj->Name);
		}
		g_DuelSystem.UserDuelInfoReset(gObj);
	}
#endif

} 

void cProtoFunc::LahapDupeBug(LPOBJ gObj)
{
	int Error = 0; 
	for(int i = OBJECT_MIN; i<OBJECT_MAX; i++) 
	{ 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i); 

        if((gObj->TargetNumber == i) && (gObj->pTransaction==1))
		{
            Error = 1;
			Chat.MessageLog(1, cLog.c_Red, cLog.t_Default, gObj, "[AntiHack][%s] Lahap Trade-Dupe Attempt, Trade: %s[%d], Action: %d",gObj->Name,gObj[i].Name,gObj->TargetNumber,gObj->pTransaction);
		}
        
		if(Error == 1) break; 
    } 
}

void cProtoFunc::LoginMsg(LPOBJ gObj)
{															  
	Chat.Message(0, gObj->m_Index, Config.ConnectNotice);
	if(!cProtection.CheckFeature(cProtection.Credits_Removal))
	{			   												  
		char login[49] = {0x50, 0x6F, 0x77, 0x65, 0x72, 0x65, 0x64, 0x20, 0x62, 0x79, 0x20, 0x52, // Powered by RMSTeam Tornado 'http://rmsteam.org/' 
						0x4D, 0x53, 0x54, 0x65, 0x61, 0x6D, 0x20, 0x54, 0x6F, 0x72, 0x6E, 0x61, 
						0x64, 0x6F, 0x20, 0x27, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x72, 
						0x6D, 0x73, 0x74, 0x65, 0x61, 0x6D, 0x2E, 0x6F, 0x72, 0x67, 0x2F, 0x27};

		Chat.Message(gObj->m_Index, "%s",login);
	}
	if (Config.ConnectInfo == 1)
	{
		int totPlayers = Util.GetOnlineCount();
		int GetMaxOnline = GetPrivateProfileInt("GameServerInfo", "NumberOfMaxUser", 0, "..\\Data\\CommonServer.cfg");
		Chat.Message(1, gObj->m_Index, "Total Online: %d/%d", totPlayers, GetMaxOnline);
																			  
		SYSTEMTIME t;
		GetLocalTime(&t);  
		Chat.Message(1, gObj->m_Index, "Server Time & Date: %02d:%02d:%02d %02d-%02d-%04d.", t.wHour, t.wMinute, t.wSecond, t.wDay, t.wMonth, t.wYear);
	}

	if (AddTab[gObj->m_Index].IsMarried)
	{
		if(AddTab[gObj->m_Index].MarryName != NULL)
		{
			if(Util.gObjIsConnected(Util.GetPlayerIndex(AddTab[gObj->m_Index].MarryName)))
			{																								  
				Chat.Message(1, gObj->m_Index, "[Marry] You are married, %s is now online!", AddTab[gObj->m_Index].MarryName);
				Chat.Message(1, Util.GetPlayerIndex(AddTab[gObj->m_Index].MarryName), "[Marry] %s getting online!", gObj->Name);
			}
			else
				Chat.Message(1, gObj->m_Index, "[Marry] You are married, %s is now offline!", AddTab[gObj->m_Index].MarryName);
		}
	}	  

	switch(GMS.IsAdmin(gObj->Name))
	{																									   
	case 1:
		Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_GM, gObj, "[Admin] %s join the game!", gObj->Name);
		break;
	case 2:
		Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_GM, gObj, "[GM] %s join the game!", gObj->Name);
		break;
	}
}

void cProtoFunc::RingSkin(LPOBJ gObj)
{   
    if(gObj->pInventory[RING_01].m_Type == 0x1A4C && gObj->m_Change != 503 ||
        gObj->pInventory[RING_02].m_Type == 0x1A4C && gObj->m_Change != 503)
        {
            gObj->m_Change = 503;
            gObjViewportListProtocolCreate(gObj);
        }
}

bool cProtoFunc::NPCTalkEx(LPOBJ gObj, int NpcId)
{					
	bool bResult = false;
	OBJECTSTRUCT *gObjNPC = (OBJECTSTRUCT*)OBJECT_POINTER(NpcId);

	if ((gObjNPC->Class == Config.ClearNpc.NpcId) && (Config.ClearNpc.Enabled))
	{
		PkClear(gObj, gObjNPC);
		bResult = true;		
	}
	if ((gObjNPC->Class == Config.ResetNpcNumber) && (Config.IsResetSystem == 2))
	{	   		
		if(AddTab[gObj->m_Index].ResType == -1)
		{
			ResetSystem.MainReset(gObj, gObjNPC);
			bResult = true;		
		}
		else
		{
			if(gObjNPC != NULL)
				NPCMessageLog(cLog.c_Blue, cLog.t_RESET, gObj, gObjNPC, "You have %2.1f more secs for answer.", AddTab[gObj->m_Index].ResType/1000.0f);
			else
				Chat.MessageLog(0, cLog.c_Blue, cLog.t_RESET, gObj, "[Reset] You have %2.1f more secs for answer.", AddTab[gObj->m_Index].ResType/1000.0f);
			bResult = true;
		}
	}							 
 	if (gObjNPC->Class == 236) 
 	{
		AddTab[gObj->m_Index].ArcherObj = gObjNPC;
		bResult = false;
 	}
	if (gObjNPC->Class == Config.Marry.MarryNpcNum && !AddTab[gObj->m_Index].IsMarried) 
	{

		if(!cMarry.StartMarriage(gObj, gObjNPC))
		bResult = true;
	}
	/*
	if (gObjNPC->Class == Config.Marry.DivorceNpcNum) 
	{							
		if(Config.Marry.DivorceNpcNum == Config.Marry.MarryNpcNum)	
			return true;
		if(!cMarry.StartMarriage(gObj, gObjNPC))
			bResult = true;
	}
	*/
	if (gObjNPC->Class == 241)
	{
		if( AddTab[gObj->m_Index].Resets < Config.GuildRes)
		{
			Chat.Message(1,gObj->m_Index,"You don't have enought Resets, you need %d more resets.", Config.GuildRes - AddTab[gObj->m_Index].Resets);
			bResult = true;
		}
		if( gObj->Level < Config.GuildLevel)
		{
			Chat.Message(1,gObj->m_Index,"You don't have enought Level, you need %d more Level.", Config.GuildLevel - gObj->Level);
			bResult = true;
		}
	} 
	
#ifdef _GameServer_
	if (gObjNPC->Class == 479 && Config.Duel.Enabled)
	{
		PMSG_SEND_WINDOW aSend;
		// ----
		aSend.uHead			 = 0xC3;
		aSend.uSize			 = 0x04;
		aSend.uPacketType	 = 0x30;
		aSend.uNum			 = 0x21;
		gObj->m_IfState.use  = 479;
		gObj->m_IfState.type = 20;
		// ----
		DataSend(gObj->m_Index, (BYTE*)&aSend, 4);
		g_DuelSystem.SendDuelStatus(gObj);

		bResult = true;
	}
		if(gObjNPC->Class == 492 && Config.Moss.Enabled)
		{
		
			if (gObj->m_PK_Level > 3 && Config.Moss.UsePkPlayers == 0 )
			{															 
				Chat.Message(1,gObj->m_Index,"[Moss The Gambler] PK players can`t use Moss The Gambler");
				bResult = false;
			}
			else
			{
				BYTE Send2[6] = {0xC3,0x06,0x30,0x00,0x27,0x00};
				BYTE Send[71] = {0xC2,0x00,71,0x31,0x00,5,0x00,71,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x02,72,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x04,73,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x18,74,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0x06,75,0x00,0x01,0x00,0x00,13*16,0x00,0xFF,0xFF,0xFF,0xFF,0xFF};
				DataSend(gObj->m_Index,Send2,6);
				DataSend(gObj->m_Index,Send,71);
				gObj->TargetShopNumber = 492;
				gObj->m_IfState.use    = 1;
				gObj->m_IfState.type   = 3;
				bResult = true;
			}
		}
#endif

	return bResult;
}

void cProtoFunc::PkClear(LPOBJ gObj, LPOBJ NpcObj)
{							   			
	if (gObj->m_PK_Level < 4)
	{										 
		NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You are good player. God bless your soul.");
        return;
    }	

	int PriceZen;
	int PricePcPoint;
	int PriceWCoin;
	switch(Config.ClearNpc.Type)
	{	   
	case 1:	
		PriceZen = (Config.ClearNpc.PriceZen * gObj->m_PK_Count); 		 
		PricePcPoint = (Config.ClearNpc.PricePcPoints * gObj->m_PK_Count);
		PriceWCoin = (Config.ClearNpc.PriceWCoins * gObj->m_PK_Count);
		break;
	case 2:	
		PriceZen = Config.ClearNpc.PriceZenForAll;			 
		PricePcPoint = Config.ClearNpc.PricePcPoints;
		PriceWCoin = Config.ClearNpc.PriceWCoins;
		break;
	case 0: 
		PriceZen = 0;					 
		PricePcPoint = 0;
		PriceWCoin = 0;
		break;
	}

	if(gObj->Money < PriceZen)
	{
		NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have enought Zen, you need %d more!", PriceZen - gObj->Money);
        return;
    }	   
	if(gObj->m_wCashPoint < PriceWCoin)
	{	 
		NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have enought WCoin, you need %d more!", PriceWCoin - gObj->m_wCashPoint);
        return;
	}
	if(AddTab[gObj->m_Index].PCPlayerPoints < PricePcPoint)
	{	 
		NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have enought PcPoint, you need %d more!", PricePcPoint - AddTab[gObj->m_Index].PCPlayerPoints);
        return;
	}
	if (PricePcPoint > 0)
	{
		g_PointShop.DecreasePoints(gObj->m_Index, PricePcPoint);	  
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[Guard] You pay %d PcPoints", PricePcPoint);
		MySQL.Execute("UPDATE Character SET %s = %s - %d WHERE Name= '%s'", Config.PCPointsColumn, Config.PCPointsColumn, PricePcPoint, gObj->Name);
	}
				 
	if (PriceWCoin > 0)
	{										
		gObj->m_wCashPoint -= PriceWCoin;
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[Guard] You pay %d WCoin", PriceWCoin);
		MySQL.Execute("UPDATE MEMB_INFO SET %s = %s - %d WHERE memb___id = '%s'", Config.WCoinsColumn, Config.WCoinsColumn, PriceWCoin, gObj->AccountID);
	}

	if (PriceZen > 0)
	{															 
		gObj->Money -= PriceZen; 
		GCMoneySend (gObj->m_Index, gObj->Money);
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[Guard] You pay %d Zen", PriceZen);
	}
												
	NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "Cleaned %d kills. Don't tell anyone!", gObj->m_PK_Count); 

	gObj->m_PK_Level = 3;
	gObj->m_PK_Count = 0;
													   
	GCPkLevelSend (gObj->m_Index,3);
}

void cProtoSendFunc::CheckRing(LPOBJ gObj, unsigned char* aSend)
{
    if(aSend[4] == RING_01 || aSend[4] == RING_02) 
        //фикс если одиваешь панда ринг меняется скин на 503
        if(gObj->pInventory[RING_01].m_Type == 0x1A4C && gObj->m_Change != 503 ||
            gObj->pInventory[RING_02].m_Type == 0x1A4C && gObj->m_Change != 503)
		{
			gObj->m_Change = 503;
			gObjViewportListProtocolCreate(gObj);	       
		}
}