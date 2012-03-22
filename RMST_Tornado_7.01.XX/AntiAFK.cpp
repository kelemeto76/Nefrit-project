#include "Stdafx.h"
#include "Main.h"
#include "AntiAFK.h"
#include "ChatCommands.h"
#include "User.h"
#include "Configs.h"
#include "ConfigDefines.h"

AntiAFK g_antiafk;

AntiAFK::AntiAFK()
{
	this->ReWarning = 0;
}

AntiAFK::~AntiAFK()
{
	
}

void AntiAFK::SendInfo(LPOBJ gObj)
{
	int Index = gObj->m_Index;
	AddTab[Index].AFK_Timer++;

	if ( this->ArrayMap[gObj->MapNumber] == 0 )
	{
		AddTab[Index].AFK_Timer = 0;
	}

	if(gObj->MapNumber != AddTab[Index].AFK_MapNumber || (gObj->MapNumber > 60 && gObj->MapNumber < 65))
	{
		AddTab[Index].AFK_MapNumber = gObj->MapNumber;
		AddTab[Index].AFK_Timer = 0;
		AddTab[Index].AFK_Temp = gObj->m_TotalAttackCount;
	}	
	if(gObj->X != AddTab[Index].AFK_X)
	{
		AddTab[Index].AFK_X = gObj->X;		
		AddTab[Index].AFK_Timer = 0;	 
		AddTab[Index].AFK_Temp = gObj->m_TotalAttackCount;

	}						 
	if(gObj->Y != AddTab[Index].AFK_Y)
	{						   
		AddTab[Index].AFK_Y = gObj->Y;	 
		AddTab[Index].AFK_Timer = 0;	   
		AddTab[Index].AFK_Temp = gObj->m_TotalAttackCount;
	}

	if (gObj->m_TotalAttackCount != AddTab[Index].AFK_Temp && AddTab[Index].AFK_Timer >= Config.AntiAfk.Time && ReWarning != Config.AntiAfk.Warnings)
	{
		AddTab[Index].AFK_Timer = 0;	
		AddTab[Index].AFK_Temp = gObj->m_TotalAttackCount;	 
		AddTab[Index].AFK_Y = gObj->Y;	 	 
		AddTab[Index].AFK_X = gObj->X;			   
		AddTab[Index].AFK_MapNumber = gObj->MapNumber;
		this->ReWarning++;
		Chat.MessageLog(1,cLog.c_Red,cLog.t_Default,gObj,"[AntiAfk][%s] Current Warning Number: %d",gObj->Name,ReWarning);
	}

	if(gObj->m_TotalAttackCount != AddTab[Index].AFK_Temp && AddTab[Index].AFK_Timer >= Config.AntiAfk.Time)
	{			 					   
		AddTab[Index].AFK_Timer = 0;   		  
		AddTab[Index].AFK_Temp = gObj->m_TotalAttackCount;	 
		AddTab[Index].AFK_Y = gObj->Y;	 	 
		AddTab[Index].AFK_X = gObj->X;			   
		AddTab[Index].AFK_MapNumber = gObj->MapNumber;
		Chat.MessageLog(1, cLog.c_Red, cLog.t_Default, gObj, "[AntiAfk] Warping To Lorencia Safe Zone!!!"); 
		int Rand1 = rand() % 5;
		int Rand2 = rand() % 5;
		gObjTeleport(Index,0,139 + Rand1,132 + Rand2);
		ReWarning = 0;
	}
}

void AntiAFK::ReadInfo()
{
	FILE *antiafk;
	char buf[256];
	int nummap, value;
	bool flag = false;
	antiafk = fopen(RMSTAntiAFK,"r");
	if ( antiafk != NULL )
	{
		while ( !feof(antiafk) )
		{
			fgets(buf,256,antiafk);
			if ( buf[0] == '/' && buf[1] == '/' ) continue;
			if ( buf[1] == 'M' && buf[2] == 'a' && buf[3] == 'p') 
			{
				flag = true;
				continue;
			}
			if (flag)
			{
				sscanf(buf,"%d %d",&nummap,&value);
				this->ArrayMap[nummap] = value;
			}
		}
	}
	fclose(antiafk);
}