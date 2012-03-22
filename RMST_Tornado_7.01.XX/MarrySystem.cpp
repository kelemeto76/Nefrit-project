#include "StdAfx.h"
#include "MarrySystem.h"
#include "ChatCommands.h"
#include "Monster.h"
//#include "Main.h"
#include "Prodef.h"
#include "Configs.h"
#include "PCPoints.h"
#include "cSQL.h"

Marry cMarry;

Marry::Marry()
{
}

Marry::~Marry()
{
}			
								 
unsigned long __stdcall TickMarriage(int Index)
{
	LPOBJ nObj = cMarry.NpcObj;  
	LPOBJ kObj1 = cMarry.gObj1;
	LPOBJ kObj2 = cMarry.gObj2;
	cMarry.NpcUse = true;

	int i;
	if(nObj == NULL)
		Chat.Message(1, Index, "[Marriage] Write /yes or /no for answer. You have 30 secs.");	
	else
	{
		Sleep(3000);				   
		if(!cMarry.NpcUse) return 1;
		NPCMessage(Index, nObj, "Write /yes or /no for answer. You have 30 secs.");
	}

	for(i = 0; i < 150; i++)
	{
		Sleep(100);			 								   
		if(!cMarry.NpcUse) return 1;
	}															 
	if(nObj == NULL)															   
		Chat.Message(1, Index, "[Marriage] You have 15 secs more for answer...");
	else
		NPCMessage(Index, nObj, "You have 15 secs more for answer...");	

	for(i = 0; i < 50; i++)
	{
		Sleep(100);			 							 
		if(!cMarry.NpcUse) return 1;
	}																	   			
	if(nObj == NULL)															   
		Chat.Message(1, Index, "[Marriage] You have 10 secs more for answer...");
	else
		NPCMessage(Index, nObj, "You have 10 secs more for answer...");	

	for(i = 0; i < 50; i++)
	{
		Sleep(100);			 							 
		if(!cMarry.NpcUse) return 1;
	}																		 		
	if(nObj == NULL)															   
		Chat.Message(1, Index, "[Marriage] You have 5 secs more for answer...");
	else
		NPCMessage(Index, nObj, "You have 5 secs more for answer...");		

	for(i = 0; i < 20; i++)
	{
		Sleep(100);			 							   
		if(!cMarry.NpcUse) return 1;
	}														   		
	if(nObj == NULL)															   
		Chat.Message(1, Index, "[Marriage] You have 3 secs more for answer...");
	else
		NPCMessage(Index, nObj, "You have 3 secs more for answer...");	

	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 							   
		if(!cMarry.NpcUse) return 1;
	} 			
	if(nObj == NULL)															   
		Chat.Message(1, Index, "[Marriage] You have 2 secs more for answer...");
	else
		NPCMessage(Index, nObj, "You have 2 secs more for answer...");		

	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 							  
		if(!cMarry.NpcUse) return 1;
	}											 			
	if(nObj == NULL)															   
		Chat.Message(1, Index, "[Marriage] You have 1 sec more for answer...");
	else
		NPCMessage(Index, nObj, "You have 1 sec more for answer...");	

	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 							   
		if(!cMarry.NpcUse) return 1;
	}
					
	if(AddTab[Index].MarryType < 10)
		cMarry.EndMarriage2(Index);
	/*	else
			cMarry.EndDivorce2(Index);*/
	return 1;
}		
			/*		 
void Marry::EndDivorce2(int Index)
{			
	//LPOBJ gObj;				   
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
	if(AddTab[Index].MarryType == 11 || AddTab[Index].MarryType == 12)	
	{	
		NPCMessageNear(NpcObj, "%s don't answer me. Divorce canceled", gObj->Name);	//AROUND
	}			
	if(AddTab[Index].MarryType == 10)  
	{	
		NPCMessage(Index, NpcObj, "You don't answer me. Divorce canceled");	//AROUND
	}			
								  
	AddTab[Index].MarryType = -1;
	
	if(gObj1 != NULL)
		AddTab[gObj1->m_Index].MarryType = -1;
	if(gObj2 != NULL)
		AddTab[gObj2->m_Index].MarryType = -1;
			
	NpcUse = false;
	cMarry.fIndex = 0;
	cMarry.gObj1 = NULL;
	cMarry.gObj2 = NULL;
	cMarry.NpcObj = NULL;		
}
*/					 
void Marry::EndMarriage2(int Index)
{			
	//LPOBJ gObj;				   
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
	if(AddTab[Index].MarryType == 1 || AddTab[Index].MarryType == 2)	
	{	
		NPCMessageNear(NpcObj, "%s don't answer me. Marriage canceled", gObj->Name);	//AROUND
	}			
	if(AddTab[Index].MarryType == 0)  
	{	
		NPCMessage(Index, NpcObj, "You don't answer me. Marriage canceled");	//AROUND
	}			
								  
	AddTab[Index].MarryType = -1;
	
	if(gObj1 != NULL)
		AddTab[gObj1->m_Index].MarryType = -1;
	if(gObj2 != NULL)
		AddTab[gObj2->m_Index].MarryType = -1;
			
	NpcUse = false;
	cMarry.fIndex = 0;
	cMarry.gObj1 = NULL;
	cMarry.gObj2 = NULL;
	cMarry.NpcObj = NULL;		
}

bool Marry::CheckMarryItem(LPOBJ gObj)
{	    	   														 
	int NumItem	= Config.Marry.TakeItemsForMarry;
	int NumSpesItem;

	int Num_Items = 0;
	int m_NumSpesItem[20];
	int x_NumItem = 0;

	for(int xo = 0; xo < 76; xo++)
		Config.Marry.MarrySerial[xo] = false;

	for(int x = 0; x < NumItem; x++)
	{		   			
		m_NumSpesItem[x] = 0;
		NumSpesItem = Config.Marry.MarryItems[x].Num;
	    x_NumItem += NumSpesItem;
			Config.Marry.MarryItems[x].Index = (Config.Marry.MarryItems[x].Group * 512 + Config.Marry.MarryItems[x].Type);   
			for(BYTE i = 12; i < 76 ; i ++ )
			{
				if(gObj->pInventory[i].m_Type == Config.Marry.MarryItems[x].Index && 
					gObj->pInventory[i].m_Level == Config.Marry.MarryItems[x].Lvl &&		  
					gObj->pInventory[i].m_Option2 == Config.Marry.MarryItems[x].Luck &&
					gObj->pInventory[i].m_Option1 == Config.Marry.MarryItems[x].Skill &&
					gObj->pInventory[i].m_Option3 == Config.Marry.MarryItems[x].Opt &&
					gObj->pInventory[i].m_NewOption == Config.Marry.MarryItems[x].Exc &&
					m_NumSpesItem[x] < NumSpesItem)
				{
					if(!Config.Marry.MarrySerial[i])
					{
						CheckMarryItemSerial(gObj,i);
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
	
void Marry::CheckMarryItemSerial(LPOBJ gObj, BYTE Serial)
{															 		  
	unsigned long Ser = gObj->pInventory[Serial].m_Number;
	for(BYTE i = 12; i < 76 ; i ++ )
		if(gObj->pInventory[i].m_Number == Ser)
			Config.Marry.MarrySerial[i] = true;
}

bool Marry::CheckMarryCost(LPOBJ gObj, LPOBJ nObj, int Delete)
{
	if(Config.Marry.MarryMinLvl > gObj->Level)
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Marriage] You don't have high lvl! Need %d more.", Config.Marry.MarryMinLvl - gObj->Level);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have high lvl! Need %d more.", Config.Marry.MarryMinLvl - gObj->Level);			  //PRIVAT 
		return true;
	}	
	if(Config.Marry.MarryCostZen > gObj->Money)
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Marriage] You don't have enough zen! Need %d more.", Config.Marry.MarryCostZen - gObj->Money);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have enough zen! Need %d more.", Config.Marry.MarryCostZen - gObj->Money);			  //PRIVAT 
		return true;
	}	
	if(Config.Marry.MarryCostPCPoint > AddTab[gObj->m_Index].PCPlayerPoints)
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Marriage] You don't have enough PCPoints! Need %d more.", Config.Marry.MarryCostPCPoint - AddTab[gObj->m_Index].PCPlayerPoints);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have enough PCPoints! Need %d more.", Config.Marry.MarryCostPCPoint - AddTab[gObj->m_Index].PCPlayerPoints);			  //PRIVAT 
		return true;
	}	  
	if(Config.Marry.MarryCostWCoin > gObj->m_wCashPoint)
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Marriage] You don't have enough WCoins! Need %d more.", Config.Marry.MarryCostWCoin - gObj->m_wCashPoint);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have enough WCoins! Need %d more.", Config.Marry.MarryCostWCoin - gObj->m_wCashPoint);			  //PRIVAT 
		return true;
	}
	if(Config.Marry.TakeItemsForMarry > 0)
	{
	 	if(CheckMarryItem(gObj) == false)
		{
			if(nObj == NULL)
				Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Marriage] You don't have right items. Check it one more time.");	
			else
				NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, nObj, "You don't have right items. Check it one more time."); 
			return true;	
		} 
	}
	if(Config.Marry.NumWitnesses > 0)
	{
		int Num = 0;
	 	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
		{						   
			OBJECTSTRUCT *aObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	   
			if(aObj->Connected < 3) continue; 	   
			if(gObj->MapNumber != aObj->MapNumber) continue;	 		 
			if(gObj->X <= aObj->X-10 || gObj->X >= aObj->X+10) continue;
			if(gObj->Y <= aObj->Y-10 || gObj->Y >= aObj->Y+10) continue;
			Num++;
		}
		if((Num - 2) < Config.Marry.NumWitnesses)
		{  
			if(nObj == NULL)
				Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Marriage] You don't have at least %d witnesses.", Config.Marry.NumWitnesses);	
			else
				NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, nObj, "You don't have at least %d witnesses.", Config.Marry.NumWitnesses); 
			return true;
		}
	}
				  
	if(Delete != 1)
		return false;

	if(Config.Marry.MarryCostZen > 0)
	{
		gObj->Money -= Config.Marry.MarryCostZen;
		GCMoneySend(gObj->m_Index, gObj->Money);
	}						
	if(Config.Marry.MarryCostPCPoint > 0)
	{
		g_PointShop.DecreasePoints(gObj->m_Index, Config.Marry.MarryCostPCPoint);	  
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj, "[Marriage] You pay %d PcPoints", Config.Marry.MarryCostPCPoint);
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - %d WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, Config.Marry.MarryCostPCPoint, gObj->Name);	
	}
	if(Config.Marry.MarryCostWCoin > 0)
	{
		gObj->m_wCashPoint -= Config.Marry.MarryCostWCoin;
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[Marriage] You pay %d WCoin", Config.Marry.MarryCostWCoin);
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s - %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, Config.Marry.MarryCostWCoin, gObj->AccountID);	
	}												  
	if(Config.Marry.TakeItemsForMarry > 0)
	{														  
	for ( BYTE i = 0 ; i < 76 ; i ++ )
		{
			if(Config.Marry.MarrySerial[i])
			{												
				gObjInventoryDeleteItem ( gObj->m_Index , i );
				GCInventoryItemDeleteSend ( gObj->m_Index , i  , 1 );
			}
		} 
	}
	return false;
}

bool Marry::StartMarriage(LPOBJ gObj, LPOBJ nObj)
{				 	
	if(NpcUse)
		return false;

	if(AddTab[gObj->m_Index].IsMarried == 0 && AddTab[gObj->m_Index].MarryType == -1 && gObj1 == NULL && gObj2 == NULL)
	{				   			  
		// проверка на lvl, zen и шмотку 	
		if(CheckMarryCost(gObj, nObj, 0))
			return false;

		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Marriage] Do you really want to marry?");	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "Do you really want to marry?");			  //PRIVAT 

		NpcObj = nObj;	   		
		AddTab[gObj->m_Index].MarryType = 0;

		gObj1 = gObj;
		DWORD ThreadID;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TickMarriage, (void*)gObj->m_Index, 0, &ThreadID);

		if ( hThread == 0 )
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
			return false;
		}

		CloseHandle(hThread);
		return false;
	}		  
	else if(AddTab[gObj->m_Index].IsMarried == 0 && gObj2 != NULL && gObj1 == gObj)
	{		 								  
		// збор поинтов, zen и шмотки
		if(CheckMarryCost(gObj, nObj, 1))
		{					   
			AddTab[gObj->m_Index].MarryType = -1;
			if(gObj1 != NULL)
				AddTab[gObj1->m_Index].MarryType = -1;
			if(gObj2 != NULL)
				AddTab[gObj2->m_Index].MarryType = -1;
			
			NpcUse = false;
			cMarry.fIndex = 0;
			cMarry.gObj1 = NULL;
			cMarry.gObj2 = NULL;
			cMarry.NpcObj = NULL;		
			return false;
		}

		Chat.MessageAll(0, 0, gObj->Name, "[Marriage] %s, do you really want to marry on %s?",gObj->Name , gObj2->Name);	//AROUND   
		if(nObj != NULL)
			NPCMessageNear(nObj, "%s, do you really want to marry on %s?",gObj->Name , gObj2->Name);	//AROUND
		NpcObj = nObj;	   		
		AddTab[gObj->m_Index].MarryType = 1;

		DWORD ThreadID;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TickMarriage, (void*)gObj->m_Index, 0, &ThreadID);

		if ( hThread == 0 )
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
			return false;
		}

		CloseHandle(hThread);
		return false;
	}
	else if(AddTab[gObj->m_Index].IsMarried == 0 && gObj1 != NULL && AddTab[gObj->m_Index].MarryType == -1)
	{		 
		// тут проверка на пол	
		if(CheckGender(gObj, nObj))	  
		{
			AddTab[gObj->m_Index].MarryType = -1;
			if(gObj1 != NULL)
				AddTab[gObj1->m_Index].MarryType = -1;
			if(gObj2 != NULL)
				AddTab[gObj2->m_Index].MarryType = -1;
			
			NpcUse = false;
			cMarry.fIndex = 0;
			cMarry.gObj1 = NULL;
			cMarry.gObj2 = NULL;
			cMarry.NpcObj = NULL;		
			return false;
		}

		Chat.MessageAll(0, 0, gObj->Name, "[Marriage] %s, do you really want to marry on %s?",gObj->Name , gObj1->Name);	  //AROUND	 
		if(nObj != NULL)
			NPCMessageNear(nObj, "%s, do you really want to marry on %s?",gObj->Name , gObj1->Name);	  //AROUND
		NpcObj = nObj;	   		
		AddTab[gObj->m_Index].MarryType = 2;

		gObj2 = gObj;
		DWORD ThreadID;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TickMarriage, (void*)gObj->m_Index, 0, &ThreadID);

		if ( hThread == 0 )
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
			return false;
		}

		CloseHandle(hThread);
		return false;
	}
	/*
	if(AddTab[gObj->m_Index].IsMarried == 1 && AddTab[gObj->m_Index].MarryType == -1 && gObj1 == NULL && gObj2 == NULL)
	{				   			  
		// проверка на lvl, zen и шмотку 	
		CheckDivorceCost(gObj, nObj, 0);

		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Marriage] Do you really want to divorce with %s?", AddTab[gObj->m_Index].MarryName);	//PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "Do you really want to divorce with %s?", AddTab[gObj->m_Index].MarryName);			  //PRIVAT 

		fIndex = gObj->m_Index;
		NpcObj = nObj;	   		
		AddTab[gObj->m_Index].MarryType = 10;

		gObj1 = gObj;
		_beginthread(TickMarriage, 0, NULL);
		return false;
	}		  
	else if(AddTab[gObj->m_Index].IsMarried == 1 && gObj2 != NULL && gObj1 == gObj)
	{		 								  
		// збор поинтов, zen и шмотки
		if(CheckDivorceCost(gObj, nObj, 1))
		{					   
			AddTab[gObj->m_Index].MarryType = -1;
			if(gObj1 != NULL)
				AddTab[gObj1->m_Index].MarryType = -1;
			if(gObj2 != NULL)
				AddTab[gObj2->m_Index].MarryType = -1;
			
			NpcUse = false;
			cMarry.fIndex = 0;
			cMarry.gObj1 = NULL;
			cMarry.gObj2 = NULL;
			cMarry.NpcObj = NULL;		
			return false;
		}

		//Chat.MessageAll(0, gObj->Name, "[Divorce] %s, do you really want to divorce with %s?",gObj->Name , gObj2->Name);	//AROUND   
		if(nObj != NULL)
			NPCMessageNear(nObj, "%s, do you really want to divorce with %s?",gObj->Name , gObj2->Name);	//AROUND
		fIndex = gObj->m_Index;		
		NpcObj = nObj;	   		
		AddTab[gObj->m_Index].MarryType = 11;

		_beginthread(TickMarriage, 0, NULL);
		return false;
	}
	else if(AddTab[gObj->m_Index].IsMarried == 1 && gObj1 != NULL && AddTab[gObj->m_Index].MarryType == -1)
	{		 
		// тут проверка на женатость	
		if(!strcmp(gObj1->Name, AddTab[gObj->m_Index].MarryName) || !strcmp(gObj->Name, AddTab[gObj1->m_Index].MarryName))
		{  
			NPCMessageNear(nObj, "%s are not marriege on %s! Divorce canceled.", gObj1->Name, gObj->Name);	  //AROUND

			AddTab[gObj->m_Index].MarryType = -1;
			if(gObj1 != NULL)
				AddTab[gObj1->m_Index].MarryType = -1;
			if(gObj2 != NULL)
				AddTab[gObj2->m_Index].MarryType = -1;
			
			NpcUse = false;
			cMarry.fIndex = 0;
			cMarry.gObj1 = NULL;
			cMarry.gObj2 = NULL;
			cMarry.NpcObj = NULL;		
			return false;
		}

		Chat.MessageAll(0, 0, gObj->Name, "[Divorce] %s, do you really want to divorce with %s?",gObj->Name , gObj1->Name);	  //AROUND	 
		if(nObj != NULL)
			NPCMessageNear(nObj, "%s, do you really want to divorce with %s?",gObj->Name , gObj1->Name);	  //AROUND
		fIndex = gObj->m_Index;		
		NpcObj = nObj;	   		
		AddTab[gObj->m_Index].MarryType = 12;

		gObj2 = gObj;
		_beginthread(TickMarriage, 0, NULL);
		return false;
	}
	*/
return false;

	//AddTab[gObj->m_Index].MarryType = 2;
}	
/*
bool Marry::CheckDivorceItem(LPOBJ gObj)
{	    	   														 
	int NumItem	= Config.Marry.TakeItemsForDivorce;
	int NumSpesItem;

	int Num_Items = 0;
	int m_NumSpesItem[20];
	int x_NumItem = 0;

	for(int xo = 0; xo < 76; xo++)
		Config.Marry.DivorceSerial[xo] = false;

	for(int x = 0; x < NumItem; x++)
	{		   			
		m_NumSpesItem[x] = 0;
		NumSpesItem = Config.Marry.DivorceItems[x].Num;
	    x_NumItem += NumSpesItem;
			Config.Marry.DivorceItems[x].Index = (Config.Marry.DivorceItems[x].Group * 512 + Config.Marry.DivorceItems[x].Type);   
			for(BYTE i = 12; i < 76 ; i ++ )
			{
				if(gObj->pInventory[i].m_Type == Config.Marry.DivorceItems[x].Index && 
					gObj->pInventory[i].m_Level == Config.Marry.DivorceItems[x].Lvl &&		  
					gObj->pInventory[i].m_Option2 == Config.Marry.DivorceItems[x].Luck &&
					gObj->pInventory[i].m_Option1 == Config.Marry.DivorceItems[x].Skill &&
					gObj->pInventory[i].m_Option3 == Config.Marry.DivorceItems[x].Opt &&
					gObj->pInventory[i].m_NewOption == Config.Marry.DivorceItems[x].Exc &&
					m_NumSpesItem[x] < NumSpesItem)
				{
					if(!Config.Marry.DivorceSerial[i])
					{
						CheckDivorceItemSerial(gObj,i);
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
	
void Marry::CheckDivorceItemSerial(LPOBJ gObj, BYTE Serial)
{															 		  
	unsigned long Ser = gObj->pInventory[Serial].m_Number;
	for(BYTE i = 12; i < 76 ; i ++ )
		if(gObj->pInventory[i].m_Number == Ser)
			Config.Marry.DivorceSerial[i] = true;
}

bool Marry::CheckDivorceCost(LPOBJ gObj, LPOBJ nObj, int Delete)
{
	if(Config.Marry.DivorceMinLvl > gObj->Level)
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Divorce] You don't have high lvl! Need %d more.", Config.Marry.DivorceMinLvl - gObj->Level);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have high lvl! Need %d more.", Config.Marry.DivorceMinLvl - gObj->Level);			  //PRIVAT 
		return true;
	}	
	if(Config.Marry.DivorceCostZen > gObj->Money)	   
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Divorce] You don't have enough zen! Need %d more.", Config.Marry.DivorceCostZen - gObj->Money);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have enough zen! Need %d more.", Config.Marry.DivorceCostZen - gObj->Money);			  //PRIVAT 
		return true;
	}	
	if(Config.Marry.DivorceCostPCPoint > AddTab[gObj->m_Index].PCPlayerPoints)
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Divorce] You don't have enough PCPoints! Need %d more.", Config.Marry.DivorceCostPCPoint - AddTab[gObj->m_Index].PCPlayerPoints);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have enough PCPoints! Need %d more.", Config.Marry.DivorceCostPCPoint - AddTab[gObj->m_Index].PCPlayerPoints);			  //PRIVAT 
		return true;
	}	  
	if(Config.Marry.DivorceCostWCoin > gObj->m_wCashPoint)
	{			   
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "[Divorce] You don't have enough WCoins! Need %d more.", Config.Marry.DivorceCostWCoin - gObj->m_wCashPoint);	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You don't have enough WCoins! Need %d more.", Config.Marry.DivorceCostWCoin - gObj->m_wCashPoint);			  //PRIVAT 
		return true;
	}
	if(Config.Marry.TakeItemsForDivorce > 0)
	{
	 	if(CheckDivorceItem(gObj) == false)
		{
			if(nObj == NULL)
				Chat.MessageLog(1, cLog.c_Red, cLog.t_RESET, gObj, "[Divorce] You don't have right items. Check it one more time.");	
			else
				NPCMessageLog(cLog.c_Blue ,cLog.t_COMMANDS, gObj, nObj, "You don't have right items. Check it one more time."); 
			return true;	
		} 
	}
				  
	if(Delete != 1)
		return false;

	if(Config.Marry.DivorceCostZen > 0)
	{
		gObj->Money -= Config.Marry.DivorceCostZen;
		GCMoneySend(gObj->m_Index, gObj->Money);
	}						
	if(Config.Marry.DivorceCostPCPoint > 0)
	{
		g_PointShop.DecreasePoints(gObj->m_Index, Config.Marry.DivorceCostPCPoint);	  
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj, "[Divorce] You pay %d PcPoints", Config.Marry.DivorceCostPCPoint);
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - %d WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, Config.Marry.DivorceCostPCPoint, gObj->Name);
	}
	if(Config.Marry.DivorceCostWCoin > 0)
	{
		gObj->m_wCashPoint -= Config.Marry.MarryCostWCoin;
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[Divorce] You pay %d WCoin", Config.Marry.DivorceCostWCoin);
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s - %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, Config.Marry.DivorceCostWCoin, gObj->AccountID);
	}												  
	if(Config.Marry.TakeItemsForDivorce > 0)
	{														  
	for ( BYTE i = 0 ; i < 76 ; i ++ )
		{
			if(Config.Marry.MarrySerial[i])
			{												
				gObjInventoryDeleteItem ( gObj->m_Index , i );
				GCInventoryItemDeleteSend ( gObj->m_Index , i  , 1 );
			}
		} 
	}
	return false;
}
*/
bool Marry::CheckGender(LPOBJ gObj,LPOBJ nObj)
{
	bool Gender1 = false, Gender2 = false;	
	switch(gObj->DbClass)
	{			
	case 32:
	case 33:
	case 34:
	case 35:  
		Gender2 = true; 							
		break;
	case 80:
	case 81:
	case 82:
	case 83: 			
		Gender2 = true; 	  
		break;
	}

	switch(gObj1->DbClass)
	{			
	case 32:
	case 33:
	case 34:
	case 35:  
		Gender1 = true; 							
		break;
	case 80:
	case 81:
	case 82:
	case 83: 			
		Gender1 = true; 	  
		break;
	}

	if(Gender1 == Gender2)
	{
		if(Config.Marry.MarryHomoSexual == 1)
		{		
			if(Gender1)
			{	 
				Chat.MessageAll(0, 0, gObj->Name, "[Marriage] It would be lesbian wedding!");	  //AROUND	 
				if(nObj != NULL)
					NPCMessageNear(nObj, "It would be lesbian wedding!");	  //AROUND
			}
			else
			{
				Chat.MessageAll(0, 0, gObj->Name, "[Marriage] It would be gey wedding!");	  //AROUND	 
				if(nObj != NULL)
					NPCMessageNear(nObj, "It would be gey wedding!");	  //AROUND
			}
		}
		else
		{  																		 
			Chat.MessageAll(0, 0, gObj->Name, "[Marriage] We don't support untraditional weddings!!! Marriage canceled!!!");	  //AROUND	 
			if(nObj != NULL)
				NPCMessageNear(nObj, "We don't support untraditional weddings!!!");	  //AROUND
			return true;
		}
	}
		
	return false;
}

void FireworkOnTheScreen(int X, int Y, LPOBJ gObj)
{					   
	PMSG_SERVERCMD ServerCmd;
	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 0;
	for(int x = 0; x < 10; x+=2)
	{
		for(int y = 0; y < 10; y+=2)
		{
			ServerCmd.X = X + x - 5;
			ServerCmd.Y = Y + y - 5;
			MsgSendV2(gObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));	   
			DataSend(gObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		}
	}
}

void Marry::MarryFirework(int x, int y, int map, LPOBJ gObj)
{	  
	//for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{						   
		//OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	 
		//if(map != gObj->MapNumber) continue;	 		   
		//if(x >= gObj->X-15 || x <= gObj->X+15) continue;
		//if(x >= gObj->Y-15 || x <= gObj->Y+15) continue;
		
		FireworkOnTheScreen(gObj->X, gObj->Y, gObj);
	} 
}
						   
unsigned long __stdcall FireMarriage()
{
	LPOBJ nObj = cMarry.NpcObj;  
	LPOBJ kObj1 = cMarry.gObj1;
	LPOBJ kObj2 = cMarry.gObj2;		
		
		PMSG_SERVERCMD ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;
		for(int y = 0; y < 2; y++)
		{	

			ServerCmd.X = kObj1->X;
			ServerCmd.Y = kObj1->Y;	   
			DataSend(kObj1->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));		
			MsgSendV2(kObj1, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

			ServerCmd.X = kObj2->X;
			ServerCmd.Y = kObj2->Y;	   
			DataSend(kObj2->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));		
			MsgSendV2(kObj2, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		}

		Sleep(3000);	  

		cMarry.MarryFirework(kObj1->X, kObj1->Y, kObj1->MapNumber, kObj1);	 

		Sleep(1000);

	NPCMessageNear(nObj, "Congratulations with the wedding %s and %s!", kObj1->Name, kObj2->Name);  
	Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_MARRY, kObj1, "[Marriage] Congratulations with the wedding %s and %s!", kObj1->Name, kObj2->Name); 
 					   
	strcpy(AddTab[cMarry.gObj1->m_Index].MarryName, cMarry.gObj2->Name);
	strcpy(AddTab[cMarry.gObj2->m_Index].MarryName, cMarry.gObj1->Name);		
	AddTab[cMarry.gObj1->m_Index].IsMarried = 1;
	AddTab[cMarry.gObj2->m_Index].IsMarried = 1;	

	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET IsMarried = 1 WHERE Name = '%s'",MySQL.szDatabase, cMarry.gObj1->Name);	
	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET IsMarried = 1 WHERE Name = '%s'",MySQL.szDatabase, cMarry.gObj2->Name);	

	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET MarryName = '%s' WHERE Name = '%s'",MySQL.szDatabase, cMarry.gObj2->Name, cMarry.gObj1->Name);	
	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET MarryName = '%s' WHERE Name = '%s'",MySQL.szDatabase, cMarry.gObj1->Name, cMarry.gObj2->Name);	 

	cMarry.NpcUse = false;
	cMarry.fIndex = 0;
	cMarry.gObj1 = NULL;
	cMarry.gObj2 = NULL;
	cMarry.NpcObj = NULL;		

	return 1;
}

void Marry::EndMarriage()
{	   					
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FireMarriage, NULL, 0, &ThreadID);

	if ( hThread == 0 )
	{
		cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
		return;
	}

	CloseHandle(hThread);
}
/*
void Marry::EndDivorce()
{	 		
	NPCMessageNear(NpcObj, "You successfully divorced!", gObj1->Name, gObj2->Name);  
		
		AddTab[cMarry.gObj1->m_Index].MarryType = -1;
		AddTab[cMarry.gObj2->m_Index].MarryType = -1;					 
		strcpy(AddTab[cMarry.gObj1->m_Index].MarryName, NULL);
		strcpy(AddTab[cMarry.gObj2->m_Index].MarryName, NULL);		
		AddTab[cMarry.gObj1->m_Index].IsMarried = 0;
		AddTab[cMarry.gObj2->m_Index].IsMarried = 0;	 
												  												  																							   
		MySQL.Execute("UPDATE [%s].[dbo].[Character]	SET IsMarried = 0 WHERE Name = '%s'", gObj1->Name);
		MySQL.Execute("UPDATE [%s].[dbo].[Character]	SET IsMarried = 0 WHERE Name = '%s'", gObj2->Name);

		MySQL.Execute("UPDATE [%s].[dbo].[Character]	SET MarryName = NULL WHERE Name = '%s'", gObj1->Name);
		MySQL.Execute("UPDATE [%s].[dbo].[Character]	SET MarryName = NULL WHERE Name = '%s'", gObj2->Name);

		cMarry.NpcUse = false;
		cMarry.fIndex = 0;
		cMarry.gObj1 = NULL;
		cMarry.gObj2 = NULL;
		cMarry.NpcObj = NULL;		
}
*/