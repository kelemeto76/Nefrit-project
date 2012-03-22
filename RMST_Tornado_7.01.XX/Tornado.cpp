#include "StdAfx.h"
#include "Main.h"
#include "Configs.h"
#include "Maps.h"
#include "cSQL.h"
#include "Fixes.h"
#include "MossGambler.h"
#include "Protection.h"
#include "Console.h"
//Load Function


extern "C" __declspec (dllexport) void __cdecl RMST()
{
	DWORD OldProtect;
	if(VirtualProtect(LPVOID(0x401000), GSSIZE, PAGE_EXECUTE_READWRITE, &OldProtect))
	{	
		cProtection.Load();
		//Create Log folder in GameServer
		CreateDirectory(RMSTLog,NULL);	   
		//Console Log & WzEvents Folder Create//
		CreateDirectory(RMSTChatLogs,NULL);
		CreateDirectory(RMSTConsoleLogs,NULL);
		CreateDirectory(RMSTWzEvents,NULL);
		//
		MySQL.Load();	
		MapInit();
		Config.LoadFixes();
		Config.LoadConfigsInGS();	 
		Fixes.ASMFixes();
		
		DWORD ThreadID; 
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LoadGS, NULL, 0, &ThreadID);

		if ( hThread == 0 )
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
			return;
		}

		CloseHandle(hThread);
	}
}