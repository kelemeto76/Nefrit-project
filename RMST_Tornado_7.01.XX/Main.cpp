// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "StdAfx.h"

#include "Main.h"
#include "Console.h"
#include "cSQL.h"
#include "User.h"
#include "Utilits.h"
#include "Protocol.h"
#include "Configs.h"
#include "GmSystem.h"
#include "ChatCommands.h"
#include "IPBlock.h"
#include "PCPoints.h"
#include "ResetSystem.h"
#include "Monster.h"
#include "DuelManager.h"
#include "ConfigDefines.h"
#include "Monster.h"
#include "Localize.h"	
#include "MoveReq.h"

CMain Main;

CMain::CMain()
{
}

CMain::~CMain()
{
}
 

unsigned long __stdcall MainTick()
{
	while(true)
	{
		for(int i=OBJECT_MIN; i<=OBJECT_MAX; i++)
		{
			OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);
			gObjUserTick(gObj); 
		} 
		gObjEventsTick(); 


		Sleep(1000);
	}
	_endthread();	
} 


unsigned long __stdcall  LoadGS()
{
	while(true)
	{
		Sleep(500);
		//
		cLog.LoggerInit();
		Sleep(500);			 
		MySQL.CheckColumns();
		Config.LoadAll();
		Main.Init();	   	
	//	cLocalization.Load();
		_endthread();
	}
}

void CMain::Init()
{
		IP.LoadIpBlock();
		g_PointShop.Init();			
		MoveReq.MoveReqLoad();


		if(Config.Moss.Enabled == 1)							
		{														
		memset((int*)0x0041E867,0x1F6,4);						
		memset((int*)0x0048130C,0x1F6,4);					
		}

	#ifdef _GameServer_
		ReadMonsterAdd();
		if(Config.Duel.Enabled)
		{
		g_DuelSystem.DuelCreateThread();
		Util.HookThis((DWORD)&HookSetDuelOption,0x004037B5);
		Util.HookThis((DWORD)&HookIsDuelEnable,0x00404BCE);
		Util.HookThis((DWORD)&HookIsOnDuel,0x00404B56);
		Util.HookThis((DWORD)&HookDuelCheck1,0x00403017);
		Util.HookThis((DWORD)&HookDuelCheck2,0x004012DA);
		Util.HookThis((DWORD)&GSgObjUserDie,0x0040367A);
		}
		Util.HookThis((DWORD)&MygEventMonsterItemDrop, 0x00402AD1);
		Util.HookThis((DWORD)&gObjGameClose_Func, 0x004034B8);
		Util.HookThis((DWORD)&MonsterDie, 0x004184C4);
		Util.HookThis((DWORD)&ProtocolCore, 0x004038BE);
		Util.HookThis((DWORD)&ProtocolCoreSend,0x004055BF);
		Util.HookThis((DWORD)&gObjLevelUpPointAddEx,0x00406D7F);
		Util.HookThis((DWORD)&gObjPlayerKiller, 0x0040655F); 
		Util.HookThis((DWORD)&gObjAttack, 0x00403CA6);
		Util.HookThis((DWORD)&MyObjCalCharacter,0x004031A7);
		Util.HookThis((DWORD)&GCEquipmentSendHook, 0x00407509); 
		Util.HookThis((DWORD)&GCKillPlayerExpSendHook, 0x00406D11);
	#endif
	
	#ifdef _GameServerCS_
		Util.HookThis((DWORD)&gObjPlayerKiller, 0x0040655F); 
		Util.HookThis((DWORD)&gObjAttack, 0x00403CA6);
		Util.HookThis((DWORD)&MygEventMonsterItemDrop, 0x004029F0);
		Util.HookThis((DWORD)&MonsterDie, 0x0041A1AE);
		Util.HookThis((DWORD)&ProtocolCore, 0x00403A17);
		Util.HookThis((DWORD)&ProtocolCoreSend,0x00405AE7);
		Util.HookThis((DWORD)&gObjLevelUpPointAddEx,0x004075B3);
		Util.HookThis((DWORD)&MygEventMonsterItemDrop, 0x004029F0);
		Util.HookThis((DWORD)&MyObjCalCharacter,0x004031F7);
	    Util.HookThis((DWORD)&GCEquipmentSendHook, 0x00407E37); 
        Util.HookThis((DWORD)&GCKillPlayerExpSendHook, 0x0040751D);
	#endif
			DWORD ThreadID;  
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainTick, NULL, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return;
			}

			CloseHandle(hThread);
}