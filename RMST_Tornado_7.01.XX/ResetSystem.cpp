// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "StdAfx.h"
#include "ConfigDefines.h"
#include "ResetSystem.h" 
#include "Console.h"
#include "Configs.h" 
#include "Prodef.h"		   
#include "ChatCommands.h"
#include "Monster.h"
#include "cSQL.h"
#include "PCPoints.h"

RSystem ResetSystem;   

const int MAX_STATES = 20;
const int MAX_ITEMS = 20;

char Character[11];
							
RSystem::RSystem()
{
}

RSystem::~RSystem()
{
}


void RSystem::Init()
{						 
	Config.IsResetSystem			= Config.GetInt(0, 2, 2, "Customs", "EnableResetSystem", RMSTReset);	
	Config.ResetNpcNumber			= Config.GetInt(0, 32000, 371, "Customs", "ResetNpcNumber", RMSTReset); 
	//char Error[255];																		
	if(!Config.IsResetSystem)
	{				  
		cLog.ConsoleOutPut(1, cLog.c_Grey, cLog.t_RESET,"[Reset System] Disabled.");
		return;
	}

	NumStates =  Config.GetInt(0, 20, 2, "Customs", "NumStates", RMSTReset);

	char PState[10];
	char Items[20];		
	
	Worked = 0;
	for(int i = 0; i < NumStates; i++)
	{
		wsprintf(PState, "State%d", i+1);

		if(i > 0)				
			State[i].MaxRes =  Config.GetInt(State[i-1].MaxRes + 1, 32000, State[i-1].MaxRes + 5, PState, "MaxResets", RMSTReset); 
		else
			State[i].MaxRes =  Config.GetInt(-1, 32000, 5, PState, "MaxResets", RMSTReset); 
		cLog.ConsoleOutPut(1, cLog.c_Red, cLog.t_RESET, "[Reset System] [%d] MaxResets = %d",i+1,State[i].MaxRes);
		
		if(i > 0)
		{
			if(State[i-1].MaxRes >= State[i].MaxRes)	
			{
				cLog.ConsoleOutPut(1, cLog.c_Red, cLog.t_RESET, "[Reset System] Error in MaxResets of State[%d]... Is smaller then previous... Worked only %d first States.",i+1,i);
				OffStates(i);
				return;
			}
			else
			{
				State[i].Work = true;	
				Worked++;
			}
		}
		else
		{
			State[i].Work = true;	  									
			Worked++;
		}
			 
		State[i].Need.Zen		= Config.GetInt(0, 2000000000, 100000, PState, "Need.Zen", RMSTReset); 	

		State[i].Need.LvlDK		= Config.GetInt(0, 400, 400, PState, "Need.LvlDK", RMSTReset);
		State[i].Need.LvlDW		= Config.GetInt(0, 400, 400, PState, "Need.LvlDW", RMSTReset);
		State[i].Need.LvlDL		= Config.GetInt(0, 400, 400, PState, "Need.LvlDL", RMSTReset);
		State[i].Need.LvlSUM	= Config.GetInt(0, 400, 400, PState, "Need.LvlSUM", RMSTReset);
		State[i].Need.LvlELF	= Config.GetInt(0, 400, 400, PState, "Need.LvlELF", RMSTReset);
		State[i].Need.LvlMG		= Config.GetInt(0, 400, 400, PState, "Need.LvlMG", RMSTReset);  				  
		State[i].Need.PcPoint	= Config.GetInt(0, Config.MaximumPCPoints, 0, PState, "Need.PcPoint", RMSTReset); 
		State[i].Need.WCoin		= Config.GetInt(0, Config.MaximumWCPoints, 0, PState, "Need.WCoin", RMSTReset); 

		State[i].Need.NumItems	= Config.GetInt(0, 20, 0, PState, "Need.NumItems", RMSTReset);

		State[i].Clear.Invent	= Config.GetInt(0, 1, 1, PState, "Clear.Inventire", RMSTReset); 
		State[i].Clear.Points	= Config.GetInt(0, 1, 1, PState, "Clear.Points", RMSTReset);   
		State[i].Clear.Command	= Config.GetInt(0, 1, 1, PState, "Clear.Command", RMSTReset);  	 
		State[i].Clear.Quest	= Config.GetInt(0, 1, 1, PState, "Clear.Quests", RMSTReset);	  
		State[i].Clear.Skills	= Config.GetInt(0, 1, 1, PState, "Clear.Skills", RMSTReset);
		State[i].Clear.PcPoint	= Config.GetInt(0, 1, 0, PState, "Clear.PcPoint", RMSTReset);	  
		State[i].Clear.WCoin	= Config.GetInt(0, 1, 0, PState, "Clear.WCoin", RMSTReset);	  
		State[i].Clear.Zen		= Config.GetInt(0, 1, 0, PState, "Clear.Zen", RMSTReset);

												
		State[i].Add.Formula	= Config.GetInt(0, 2, 1, PState, "Add.Formula", RMSTReset);

		if(State[i].Add.Formula!=0)
		{
			State[i].Add.PointsDK	= Config.GetInt(0, 32000, 200, PState, "Add.PointsDK", RMSTReset);	
			State[i].Add.PointsDW	= Config.GetInt(0, 32000, 200, PState, "Add.PointsDW", RMSTReset);	
			State[i].Add.PointsDL	= Config.GetInt(0, 32000, 200, PState, "Add.PointsDL", RMSTReset);	
			State[i].Add.PointsMG	= Config.GetInt(0, 32000, 200, PState, "Add.PointsMG", RMSTReset);	
			State[i].Add.PointsSUM	= Config.GetInt(0, 32000, 200, PState, "Add.PointsSUM", RMSTReset);	
			State[i].Add.PointsELF	= Config.GetInt(0, 32000, 200, PState, "Add.PointsELF", RMSTReset);
		}																										
		State[i].Add.PcPoint	= Config.GetInt(0, Config.MaximumPCPoints, 20, PState, "Add.PcPoint", RMSTReset);
		State[i].Add.WCoin		= Config.GetInt(0, Config.MaximumPCPoints, 20, PState, "Add.WCoin", RMSTReset);

		/*if(State[i].NumItems < 0 || State[i].NumItems >20)
		{
			wsprintf(Error, "[Reset System] Error in NumItems[%d]... Worked only %d first States.",i+1,i);
			cLog.ConsoleOutPutWOL(1, Error);
			OffStates(i);
			return;
		}
		else */
		if(State[i].Need.NumItems != 0)
		{
			for(int y = 0; y < State[i].Need.NumItems; y++)
			{							 				   
				wsprintf(Items, "Need.Item[%d].Num", y+1);
				State[i].Need.Items[y].Num		= Config.GetInt(0, 20, 1, PState, Items, RMSTReset);

				wsprintf(Items, "Need.Item[%d].Index", y+1);	
				State[i].Need.Items[y].Group	= Config.GetInt(0, 15, 0, PState, Items, RMSTReset);  

				wsprintf(Items, "Need.Item[%d].Type", y+1);
				State[i].Need.Items[y].Type		= Config.GetInt(0, 255, 0, PState, Items, RMSTReset);  

				wsprintf(Items, "Need.Item[%d].Lvl", y+1);
				State[i].Need.Items[y].Lvl		= Config.GetInt(0, 13, 0, PState, Items, RMSTReset);

				wsprintf(Items, "Need.Item[%d].Luck", y+1);   
				State[i].Need.Items[y].Luck		= Config.GetInt(0, 1, 0, PState, Items, RMSTReset);

				wsprintf(Items, "Need.Item[%d].Skill", y+1);
				State[i].Need.Items[y].Skill	= Config.GetInt(0, 1, 0, PState, Items, RMSTReset);

				wsprintf(Items, "Need.Item[%d].Opt", y+1);
				State[i].Need.Items[y].Opt		= Config.GetInt(0, 7, 0, PState, Items, RMSTReset);

				wsprintf(Items, "Need.Item[%d].Exc", y+1);
				State[i].Need.Items[y].Exc		= Config.GetInt(0, 63, 0, PState, Items, RMSTReset);
			}
		}
	}			 
	cLog.ConsoleOutPut(1, cLog.c_Green, cLog.t_RESET, "[Reset System] Successfully inited. %d states of %d - worked",Worked ,NumStates);		
}	   

void RSystem::OffStates(int i)
{									  
	for(int x = i; x < MAX_STATES; x++)
		State[x].Work = false;		
}
									  
unsigned long __stdcall ResetTimer(int Index)
{		
	int i;
	Chat.Message(1, Index, "[Reset] Write /yes or /no for answer. You have 30 secs.");	
	for(i = 0; i < 150; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ResType == -1) return 1; 
		AddTab[Index].ResType -= 100;
	}

	Chat.Message(1, Index, "[Reset] You have 15 secs more for answer...");	   	
	for(i = 0; i < 50; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ResType == -1) return 1;
		AddTab[Index].ResType -= 100;
	}

	Chat.Message(1, Index, "[Reset] You have 10 secs more for answer...");		
	for(i = 0; i < 50; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ResType == -1) return 1; 
		AddTab[Index].ResType -= 100;
	}

	Chat.Message(1, Index, "[Reset] You have 5 secs more for answer...");		
	for(i = 0; i < 20; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ResType == -1) return 1;	  
		AddTab[Index].ResType -= 100;
	}

	Chat.Message(1, Index, "[Reset] You have 3 secs more for answer...");		
	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ResType == -1) return 1;  	
		AddTab[Index].ResType -= 100;
	}

	Chat.Message(1, Index, "[Reset] You have 2 secs more for answer...");		
	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ResType == -1) return 1;	   
		AddTab[Index].ResType -= 100;
	}

	Chat.Message(1, Index, "[Reset] You have 1 sec more for answer...");		
	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ResType == -1) return 1;	
		AddTab[Index].ResType -= 100;
	}

	Chat.Message(1, Index, "[Reset] Time is over. Reset canceled!");	
	AddTab[Index].ResType = -1;
	return 1;
}
				
void RSystem::Reset(LPOBJ gObj, LPOBJ NpcObj)
{
	AddTab[gObj->m_Index].ResNpcObj = NpcObj;	  
	AddTab[gObj->m_Index].ResType = 30000;		
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ResetTimer, (void*)gObj->m_Index, 0, &ThreadID);

	if ( hThread == 0 )
	{
		cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
		return;
	}

	CloseHandle(hThread);
}					

void RSystem::MainReset(LPOBJ gObj, LPOBJ NpcObj)
{								   
	if(!Config.IsResetSystem)
		return;

	int NumState = CheckState(gObj->m_Index);
	
	if(NumState == -1)
	{
		if(NpcObj == NULL)
			Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Reset System] Disabled.");	
		else
			NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "[Reset System] Disabled.");	   
		return;	
	}			

	if(NumState == -2)
	{
		if(NpcObj == NULL)
			Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Reset System]You have max resets. You can't make more resets.");	
		else
			NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You have max resets. You can't make more resets.");	   
		return;	
	}					  		
						   
	if(!State[NumState].Work)
		return;
							  
	int m_NeedLvl = CheckRole(gObj->DbClass, NumState, NeedLvl);
	if(m_NeedLvl > gObj->Level)
	{	
		if(NpcObj == NULL)
			Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Reset System]You don't have %d level. Need %d level more. Now you are only %d level.", CheckRole(gObj->DbClass, NumState, NeedLvl), CheckRole(gObj->DbClass, NumState, NeedLvl)-gObj->Level, gObj->Level);	
		else
			NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have %d level. Need %d level more.", CheckRole(gObj->DbClass, NumState, NeedLvl), CheckRole(gObj->DbClass, NumState, NeedLvl)-gObj->Level); 
		return;		
	}
		  
	if(State[NumState].Need.Zen > 0)
		if(gObj->Money < State[NumState].Need.Zen)
		{			 
			if(NpcObj == NULL)
				Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Reset System]You don't have %d zen. Need %d more zen. Now you have only %d zen.", State[NumState].Need.Zen, State[NumState].Need.Zen-gObj->Money, gObj->Money);	
			else
				NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have %d zen. Need %d more zen.", State[NumState].Need.Zen, State[NumState].Need.Zen-gObj->Money); 
			return;	
		}		   
		
	if(State[NumState].Need.PcPoint > 0)
		if(AddTab[gObj->m_Index].PCPlayerPoints < State[NumState].Need.PcPoint)
		{			 
			if(NpcObj == NULL)
				Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Reset System]You don't have %d PcPoints. Need %d more PcPoints. Now you have only %d PcPoints.", State[NumState].Need.PcPoint, State[NumState].Need.PcPoint-AddTab[gObj->m_Index].PCPlayerPoints, AddTab[gObj->m_Index].PCPlayerPoints);	
			else
				NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have %d PcPoints. Need %d more PcPoints.", State[NumState].Need.PcPoint, State[NumState].Need.PcPoint-AddTab[gObj->m_Index].PCPlayerPoints); 
			return;	
		}
		
	if(State[NumState].Need.WCoin > 0)
		if(gObj->m_wCashPoint < State[NumState].Need.WCoin)
		{			 
			if(NpcObj == NULL)
				Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Reset System]You don't have %d WCoin. Need %d more WCoin. Now you have only %d WCoin.", State[NumState].Need.WCoin, State[NumState].Need.WCoin-gObj->m_wCashPoint, gObj->m_wCashPoint);	
			else
				NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have %d WCoin. Need %d more WCoin.", State[NumState].Need.WCoin, State[NumState].Need.WCoin-gObj->m_wCashPoint); 
			return;	
		} 

	if(State[NumState].Need.NumItems > 0)
	{
		if(CheckItem(gObj, NumState) == false)
		{
			if(NpcObj == NULL)
				Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Reset System]You don't have right items. Check it one more time.");	
			else
				NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, NpcObj, "You don't have right items. Check it one more time."); 
			return;	
		} 
	}

	if(AddTab[gObj->m_Index].ResType == -1)
	{		  						 
		Reset(gObj, NpcObj);  
		return;		  
	}
											  
	if(State[NumState].Need.Zen > 0)
	{
		gObj->Money -= State[NumState].Need.Zen;		
		GCMoneySend(gObj->m_Index, 0);	
	}

	gObj->Level = 1;
	gObj->Experience = 100;

	if(State[NumState].Need.WCoin > 0)
	{
		gObj->m_wCashPoint -= State[NumState].Need.WCoin;
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s - %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, State[NumState].Need.WCoin, gObj->AccountID);
	}
 	if(State[NumState].Need.PcPoint > 0)
	{
		g_PointShop.DecreasePoints(gObj->m_Index, State[NumState].Need.PcPoint);
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - %d WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, State[NumState].Need.PcPoint, gObj->Name);
	}
																						
	if(State[NumState].Clear.Invent)
	{
		for ( BYTE i = 0 ; i < 76 ; i ++ )
		{
			gObjInventoryDeleteItem ( gObj->m_Index , i );
			GCInventoryItemDeleteSend ( gObj->m_Index , i  , 1 );
		}
	}
	else
	{
		if(State[NumState].Need.NumItems > 0)
		{														  
		for ( BYTE i = 0 ; i < 76 ; i ++ )
			{
				if(State[NumState].Serial[i])
				{												
					gObjInventoryDeleteItem ( gObj->m_Index , i );
					GCInventoryItemDeleteSend ( gObj->m_Index , i  , 1 );
				}
			}

		}
	}
					  
	if(State[NumState].Clear.Points)
	{
		gObj->Strength = GetStartPoints(gObj->DbClass, Strength);
		gObj->Dexterity = GetStartPoints(gObj->DbClass, Dexterity);
		gObj->Vitality = GetStartPoints(gObj->DbClass, Vitality);
		gObj->Energy = GetStartPoints(gObj->DbClass, Energy);
	}								

	if(State[NumState].Clear.Command)
		gObj->Leadership = GetStartPoints(gObj->DbClass, Leadership); 	

	if(State[NumState].Clear.PcPoint)
	{
		g_PointShop.DecreasePoints(gObj->m_Index, AddTab[gObj->m_Index].PCPlayerPoints);
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = 0 WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, gObj->Name);
	}

	if(State[NumState].Clear.WCoin)
	{																	
		gObj->m_wCashPoint = 0;
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = 0 WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, gObj->AccountID);
	}

	if(State[NumState].Clear.Zen)
	{
		gObj->Money = 0;
		GCMoneySend(gObj->m_Index, 0);	
	}
									   
	if(State[NumState].Add.PcPoint > 0)
	{
		g_PointShop.IncreasePoints(gObj->m_Index, State[NumState].Add.PcPoint);
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s + %d WHERE Name = '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, State[NumState].Add.PcPoint, gObj->Name);
	}

	if(State[NumState].Add.WCoin > 0)
	{
		gObj->m_wCashPoint += State[NumState].Add.WCoin;
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s + %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, State[NumState].Add.WCoin, gObj->AccountID);
	}
	

	if(State[NumState].Add.Formula != 0)
		gObj->LevelUpPoint = GetPoints(gObj, NumState, State[NumState].Add.Formula, gObj->LevelUpPoint); 	

	gObjTeleport(gObj->m_Index,0,141,132);
	GCLevelUpMsgSend(gObj->m_Index,0);	

	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s + 1 WHERE Name = '%s'",MySQL.szDatabase, Config.ResetColumn, Config.ResetColumn, gObj->Name);
	AddTab[gObj->m_Index].Resets++;

	//GJSetCharacterInfo(gObj, gObj->m_Index, FALSE);
	
	//SDHP_DBCHARINFOREQUEST pMsg;	  
	//char _name[11];
	//SDHP_DBCHARINFOREQUEST pCRequest;

	//PHeadSetB((LPBYTE)&pCRequest, 0x06, sizeof(pCRequest));
	//memset(_name, 0, sizeof(_name));
	//memcpy(_name, gObj->Name, sizeof(_name));
	//BuxConvert(_name, sizeof(_name));
	//memcpy(pCRequest.Name, _name, sizeof(_name));
	//strcpy(pCRequest.AccountID, gObj->AccountID);
	//pCRequest.Number = gObj->m_Index;

	//GJSendInfo((char*)&pCRequest, pCRequest.h.size);
	{ 
		char sBuf[]={0xC3,0x05,0xF1,0x02,0x02};
		DataSend(gObj->m_Index, (PBYTE)sBuf,sBuf[1]);
	}							
			
		
	if(State[NumState].Clear.Skills)		
	{										  
		DWORD ThreadID;													
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClearSkills, (void*)gObj->Name, 0, &ThreadID);

		if ( hThread == 0 )
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
			return;
		}

		CloseHandle(hThread);
	}
	if(State[NumState].Clear.Quest)					   
	{									  
		DWORD ThreadID;															 
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ClearQuest, (void*)gObj->Name, 0, &ThreadID);

		if ( hThread == 0 )
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
			return;
		}

		CloseHandle(hThread);										  
	}
	//Chat.Message(1, gObj->m_Index, "RESET = %d!", AddTab[gObj->m_Index].Resets);
	//Chat.Message(1, gObj->m_Index, "Column = %s loled", Config.ResetColumn);
	AddTab[gObj->m_Index].ResType = -1;
}		
													
unsigned long __stdcall ClearSkills(char Temp[11])
{	
	char Name[11];
	strcpy(Name, Temp);
	Sleep(4500);
 	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET MagicList = NULL WHERE Name = '%s'",MySQL.szDatabase, Name); 
	// ÓÄÀËÈÒÜ ÎÒÑÞÄÀ ÊÂÅÑÒ È ÄÎÁÀÂÈÒÜ ÎÒÄÅËÜÍÎ ÎÒ×ÈÑÒÊÓ ÊÂÅÑÒ È ÏÐÎÔÛ!!!
	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET Quest = NULL WHERE Name = '%s'",MySQL.szDatabase, Name); 
	return 1;
}
							   
unsigned long __stdcall ClearQuest(char Temp[11])
{	
	char Name[11];
	strcpy(Name, Temp);
	Sleep(4000);				 
 	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET Quest = NULL WHERE Name = '%s'",MySQL.szDatabase, Name); 
			
	int Class = 0;
	MySQL.Execute("SELECT Class FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, Name);
	Class = MySQL.GetInt();
		
	int NewClass;
	switch (Class)
	{
	case 0:
	case 1:
	case 2:
	case 3:	
		NewClass = 0;
		break;
	case 16:
	case 17:
	case 18:
	case 19: 
		NewClass = 16;				
		break;
	case 32:
	case 33:
	case 34:
	case 35:  
		NewClass = 32; 							
		break;
	case 48:
	case 49:
	case 50:   
		NewClass = 48;								  
		break;
	case 64:
	case 65:
	case 66:	
		NewClass = 64; 		  
		break;
	case 80:
	case 81:
	case 82:
	case 83: 	
		NewClass = 80;		  
		break;
	}		  
 	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET Class = %d WHERE Name = '%s'",MySQL.szDatabase, NewClass, Name); 
	return 1;
}

long int RSystem::GetPoints(LPOBJ gObj, int NumState, int Formula, int StartPoint)
{  	   																 	
	if(Formula == 1)
		StartPoint = StartPoint + CheckRole(gObj->DbClass, NumState, Points);
	else if(Formula == 2)
		StartPoint = (AddTab[gObj->m_Index].Resets + 1) * CheckRole(gObj->DbClass, NumState, Points);
	
	return StartPoint;
}

int RSystem::GetStartPoints(unsigned char Class, ForPoints Points)
{												
	switch (Class)
	{
	case 0:
	case 1:
	case 2:
	case 3:								
		if(Points == Strength)return 18;
		if(Points == Dexterity)return 18;
		if(Points == Vitality)return 15;
		if(Points == Energy)return 30;	   
		break;
	case 16:
	case 17:
	case 18:
	case 19: 				
		if(Points == Strength)return 28;
		if(Points == Dexterity)return 20;
		if(Points == Vitality)return 25;
		if(Points == Energy)return 10;
		break;
	case 32:
	case 33:
	case 34:
	case 35:   									  
		if(Points == Strength)return 22;
		if(Points == Dexterity)return 25;
		if(Points == Vitality)return 20;
		if(Points == Energy)return 15; 
		break;
	case 48:
	case 49:
	case 50:   								 
		if(Points == Strength)return 26;
		if(Points == Dexterity)return 26;
		if(Points == Vitality)return 26;
		if(Points == Energy)return 26;	   
		break;
	case 64:
	case 65:
	case 66:	 		
		if(Points == Strength)return 26;
		if(Points == Dexterity)return 20;
		if(Points == Vitality)return 20; 
		if(Points == Energy)return 15;
		if(Points == Leadership)return 25;	   
		break;
	case 80:
	case 81:
	case 82:
	case 83: 			
		if(Points == Strength)return 21;
		if(Points == Dexterity)return 21;
		if(Points == Vitality)return 18;
		if(Points == Energy)return 23;	   
		break;
	}
	return 0;
}

bool RSystem::CheckItem(LPOBJ gObj, int NumState)
{	    	   														 
	int NumItem	= State[NumState].Need.NumItems;
	int NumSpesItem;

	int Num_Items = 0;
	int m_NumSpesItem[20];
	int x_NumItem = 0;

	for(int xo = 0; xo < 76; xo++)
		State[NumState].Serial[xo] = false;

	for(int x = 0; x < NumItem; x++)
	{		   			
		m_NumSpesItem[x] = 0;
		NumSpesItem = State[NumState].Need.Items[x].Num;
	    x_NumItem += NumSpesItem;
			State[NumState].Need.Items[x].Index = (State[NumState].Need.Items[x].Group * 512 + State[NumState].Need.Items[x].Type);   
			for(BYTE i = 12; i < 76 ; i ++ )
			{
				if(gObj->pInventory[i].m_Type == State[NumState].Need.Items[x].Index && 
					gObj->pInventory[i].m_Level == State[NumState].Need.Items[x].Lvl &&		  
					gObj->pInventory[i].m_Option2 == State[NumState].Need.Items[x].Luck &&
					gObj->pInventory[i].m_Option1 == State[NumState].Need.Items[x].Skill &&
					gObj->pInventory[i].m_Option3 == State[NumState].Need.Items[x].Opt &&
					gObj->pInventory[i].m_NewOption == State[NumState].Need.Items[x].Exc &&
					m_NumSpesItem[x] < NumSpesItem)
				{
					if(!State[NumState].Serial[i])
					{
						CheckItemSerial(gObj,NumState,i);
						m_NumSpesItem[x]++;
						Num_Items++;
					}
				}
			}							
	}
	if(Num_Items >= x_NumItem)
		return true;
	else
		return false;
}	  
	
void RSystem::CheckItemSerial(LPOBJ gObj, int NumState, BYTE Serial)
{															 		  
	unsigned long Ser = gObj->pInventory[Serial].m_Number;
	for(BYTE i = 12; i < 76 ; i ++ )
		if(gObj->pInventory[i].m_Number == Ser)
			State[NumState].Serial[i] = true;
}

int RSystem::CheckRole(unsigned char Class,int NumState, ForRole Role)
{											 
	switch (Class)
	{
	case 0:
	case 1:
	case 2:
	case 3:
		if(Role == NeedLvl)
			return State[NumState].Need.LvlDW;
		else if(Role == Points)
			return State[NumState].Add.PointsDW;
		break;
	case 16:
	case 17:
	case 18:
	case 19:  
		if(Role == NeedLvl)
			return State[NumState].Need.LvlDK;
		else if(Role == Points)
			return State[NumState].Add.PointsDK;
		break;
	case 32:
	case 33: 
	case 34:
	case 35:   
		if(Role == NeedLvl)
			return State[NumState].Need.LvlELF;
		else if(Role == Points)
			return State[NumState].Add.PointsELF;
		break;
	case 48:
	case 49:
	case 50:   
		if(Role == NeedLvl)
			return State[NumState].Need.LvlMG;
		else if(Role == Points)
			return State[NumState].Add.PointsMG;
		break;
	case 64:
	case 65:
	case 66:	 
		if(Role == NeedLvl)
			return State[NumState].Need.LvlDL;
		else if(Role == Points)
			return State[NumState].Add.PointsDL;
		break;
	case 80:
	case 81:
	case 82:
	case 83: 
		if(Role == NeedLvl)
			return State[NumState].Need.LvlSUM;
		else if(Role == Points)
			return State[NumState].Add.PointsSUM;
		break;
	}
	return 0;
}

int RSystem::CheckState(DWORD Index)
{
	for(int i = 0; i < NumStates; i++)
	{					   
		if(State[i].Work)
		{
			if(State[i].MaxRes == -1)
				return i;
			else if(State[i].MaxRes >= AddTab[Index].Resets)
				return i;
			else if(i == Worked -1)
				return -2;
		}
	} 
	return -1;
}
						
extern RSystem ResetSystem;