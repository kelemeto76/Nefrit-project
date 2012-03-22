#include "StdAfx.h"
#include "HappyHour.h"
#include "Console.h"
#include "ChatCommands.h"
#include "ConfigDefines.h"
#include "Configs.h"

HappyHour::HappyHour()
{

}

HappyHour::~HappyHour()
{

}

void HappyHour::LoadConfig()
{
	FILE *conf;
	char Buff[236];
	bool flag = false;
	int i=0;

	conf = fopen(RMSTHappyHour,"r");

	if ( conf != NULL)
	{
		while ( !feof(conf))
		{
			fgets(Buff,256,conf);
			if ( Buff[0] == '/' && Buff[1] == '/')
				continue;

			if ( Buff[0] == '0' && strlen(Buff) == 2) 
			{
				flag = true;
				continue;
			}
			if (flag)
			{
				sscanf(Buff,"%d %d %d %d %d %d %d %d %d",&Config.g_Conf[i].MapNum,&Config.g_Conf[i].Working,&Config.g_Conf[i].PlusExp,&Config.g_Conf[i].PlusZen,
					&Config.g_Conf[i].Hour,&Config.g_Conf[i].Min,&Config.g_Conf[i].Delay);

				Config.g_Conf[i].CloseMin = Config.g_Conf[i].Min + Config.g_Conf[i].Delay;

				Config.g_Conf[i].CloseHour = Config.g_Conf[i].Hour;

				while ( Config.g_Conf[i].CloseMin >=60)
				{
					Config.g_Conf[i].CloseHour++;
					Config.g_Conf[i].CloseMin -= 60;
				}

				i++;
			}
		}

	}			  
	fclose(conf);
	AmountLocations = i;
	for (int j=0; j<AmountLocations; j++)	StartedEvents[j] = 0;

	cLog.ConsoleOutPut(1, cLog.c_Magenta,cLog.t_Default,"[Happy Hour] Loaded %d locations.",AmountLocations - 1);
}

void HappyHour::TickTimerHappyHour()
{
	CTime t = CTime::GetCurrentTime();
	m_Hour = t.GetHour();
	m_Min = t.GetMinute();
	if (!t.GetSecond())
	{
		for (int i=0; i<AmountLocations; i++)
		{
			if (m_Hour == Config.g_Conf[i].Hour && m_Min == Config.g_Conf[i].Min)
			{
					Chat.MessageAll(0,0,NULL,"[Happy Hour] %s Event Started. Event Close in %d:%02d.",
						GetMapName(Config.g_Conf[i].MapNum),Config.g_Conf[i].CloseHour,Config.g_Conf[i].CloseMin);
					StartedEvents[i] = 1;
			}

			if  (m_Hour == Config.g_Conf[i].CloseHour && m_Min == Config.g_Conf[i].CloseMin )
			{
					Chat.MessageAll(0,0,NULL,"[Happy Hour] In %s Event Close.",GetMapName(Config.g_Conf[i].MapNum));	
					StartedEvents[i] = 0;
			}
		}
	}
}

char* HappyHour::GetMapName(int MapId)
{
	switch (MapId)
	{
	case 0:
		return "Lorencia";
	case 1:
		return "Dungeon";
	case 2:
		return "Devais";
	case 3:
		return "Noria";
	case 4:
		return "Lost Tower";
	case 5:
		return "Null";
	case 6:
		return "Arena";
	case 7:
		return "Atlans";
	case 8:
		return "Tarkan";
	case 9:
		return "Devil Square";
	case 10:
		return "Icarus";
	case 30:
		return "Valley of Loren";
	case 31:
		return "Land of Trial";
	case 32:
		return "Devil Square";
	case 33:
		return "Aida";
	case 34:
		return "Crywolf";
	case 35:
		return "Null";
	case 36:
		return "Kalima";
	case 37:
		return "Kantru1";
	case 38:
		return "Kantru2";
	case 39:
		return "Kantru3";
	case 40:
		return "Silent";
	case 41:
		return "Barracks";
	case 42:
		return "Refuge";
	case 43:
		return "Null";
	case 44:
		return "Null";
	case 51:
		return "Elbeland";
	case 52:
		return "Blood Castle";
	case 53:
		return "Chaos Castle";
	case 56:
		return "Swamp of Calmness";
	case 57:
		return "Raklion";
	case 58:
		return "Raklion Boss";
	case 62:
		return "Santa Town";
	case 63:
		return "Vulcanus";
	case 64:
		return "Coliseum";
	}
	if (MapId >= 11 && MapId <= 17)
		return "Blood Castle";

	if (MapId >= 18 && MapId <= 23)
		return "Chaos Castle";

	if (MapId >= 24 && MapId <= 29)
		return "Kalima";

	if (MapId >= 45 && MapId <= 50)
		return "Illusion Temple";

	return "Unknown";
}

HappyHour g_Happy;