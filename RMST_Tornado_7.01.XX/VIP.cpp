#include "StdAfx.h"
#include "VIP.h"
#include "cSQL.h"
#include "Configs.h"
#include "User.h"
#include "Console.h"
#include "ChatCommands.h"
cVIP VIP;	 
cVIP::cVIP()
{
}

cVIP::~cVIP()
{
}

const int VIP_MAX_STATES = 10;

void cVIP::Init()
{

	Enabled = Config.GetInt(0, 1,				1,		"VipSystem",			"EnableVip",			RMSTVip); 
	if(!Enabled)return;

	GetPrivateProfileString("VipSystem","VIPColumn","VIP",Column,sizeof(Column), RMSTVip);
	MySQL.CheckColumn(MySQL.szDatabase,Column, "Character", "ALTER TABLE [%s].[dbo].[Character] ADD [%s][tinyint] DEFAULT (0) NOT NULL",MySQL.szDatabase, Column);
	GetPrivateProfileString("VipSystem","VIPColumnDate","VIP_DATE",ColumnDate,sizeof(ColumnDate), RMSTVip);	 				 
	MySQL.CheckColumn(MySQL.szDatabase,ColumnDate, "Character", "ALTER TABLE [%s].[dbo].[Character] ADD [%s][varchar](10) DEFAULT (0) NOT NULL",MySQL.szDatabase, ColumnDate);
							 	
	NumStates = Config.GetInt(0, VIP_MAX_STATES, 3, "VipSystem", "NumStates", RMSTVip);	
		
	char PState[10]; 
	for(int i = 1; i <= NumStates; i++)
	{
		wsprintf(PState, "State%d", i);

		GetPrivateProfileString(PState,"VIPStateName","bronze",VIPState[i].VIPName,sizeof(VIPState[i].VIPName), RMSTVip);	  
		VIPState[i].EnabledCmd		= Config.GetInt(0, 1,									1,		PState,			"AllowAutoBuy",		RMSTVip); 
		VIPState[i].CostPCPoints	= Config.GetInt(0, Config.MaximumPCPoints,				5,		PState,			"CostPCPoints",		RMSTVip);
		VIPState[i].CostWCoins		= Config.GetInt(0, Config.MaximumWCPoints,				5,		PState,			"CostWCoins",		RMSTVip);
		VIPState[i].CostZen			= Config.GetInt(0, 2000000000,							5000,	PState,			"CostZen",			RMSTVip);

		VIPState[i].BonusExp		= Config.GetInt(0, 9999,								5,		PState,			"BonusExp",			RMSTVip);
		VIPState[i].BonusZen		= Config.GetInt(0, 9999,								5,		PState,			"BonusZen",			RMSTVip);
		VIPState[i].BonusDrop		= Config.GetInt(0, 9999,								5,		PState,			"BonusDrop",		RMSTVip);
	}
}

bool cVIP::CheckVipTime(int TimeInMin)
{
	switch(TimeInMin)
	{
	case 1:
	case 2:
	case 3:
	case 5:
	case 15:
	case 30:
	case 60:
	case 120:
	case 340:
	case 680:
		return true;
	}
	return false;
}

unsigned long __stdcall VipInGameServer(int Index)
{ 	
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);	  
	while(true)
	{			   
		if(gObj->Connected == PLAYER_PLAYING)
		{
			if(AddTab[gObj->m_Index].IsInGameVipMinutes > 0)
			{
				AddTab[gObj->m_Index].IsInGameVipMinutes --;
				MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s=%d WHERE Name='%s'",MySQL.szDatabase,VIP.ColumnDate, AddTab[gObj->m_Index].IsInGameVipMinutes, gObj->Name);
			}
			else
			{
				Chat.MessageLog(1, cLog.c_Red, cLog.t_VIP, gObj, "[VIP] Your vip time is over! You are normal player again."); 
				AddTab[gObj->m_Index].IsVipInGame = 0;
				AddTab[gObj->m_Index].IsInGameVipMinutes = 0;
				MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s=0 WHERE Name='%s'",MySQL.szDatabase,VIP.Column, gObj->Name);
				return 1;
			}
			if(VIP.CheckVipTime(AddTab[gObj->m_Index].IsInGameVipMinutes))
			{			
				Chat.MessageLog(1, cLog.c_Red, cLog.t_VIP, gObj, "[VIP] You have %d more vip minutes", AddTab[gObj->m_Index].IsInGameVipMinutes); 
			}
		}
		else
			return 1;
		/*Коректировка тиканья функции ( чтобы тикало равно в 0 сек по времени )*/
		CTime t = CTime::GetCurrentTime();
		int TimeDelayCorrect = ( 60 - t.GetSecond() ) * 1000;
		Sleep(TimeDelayCorrect);	
	}
	return 1;
}

unsigned long __stdcall VipTickServer(int Index)
{	
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);	
	while(true)
	{			   
		if(gObj->Connected == PLAYER_PLAYING)
		{
			if(AddTab[gObj->m_Index].VipMinutesServer > 0)
			{
				AddTab[gObj->m_Index].VipMinutesServer --;
				MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s=%d WHERE memb___id='%s'",MySQL.szDatabase2,Config.VIPServ.ColumnDate, AddTab[gObj->m_Index].VipMinutesServer, gObj->AccountID);
			}
			else
			{
				Chat.MessageLog(1, cLog.c_Red, cLog.t_VIP, gObj, "[VIPServer] Your vip time is over! Disconnected from the server.");
				AddTab[gObj->m_Index].VipMinutesServer = 0;
				MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s=0 WHERE memb___id='%s'",MySQL.szDatabase2,Config.VIPServ.Column, gObj->AccountID);
				CloseClient(gObj->m_Index);
				return 1;
			}		  			
			if(VIP.CheckVipTime(AddTab[gObj->m_Index].VipMinutesServer))
			{			
				Chat.MessageLog(1, cLog.c_Red, cLog.t_VIP, gObj, "[VIP] You have %d more vip minutes", AddTab[gObj->m_Index].VipMinutesServer); 
			}
		}
		else
			return 1;
		/*Коректировка тиканья функции ( чтобы тикало равно в 0 сек по времени )*/
		CTime t = CTime::GetCurrentTime();
		int TimeDelayCorrect = ( 60 - t.GetSecond() ) * 1000;
		Sleep(TimeDelayCorrect);	
	}
	return 1;
}