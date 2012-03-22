#include "Stdafx.h"
#include "GoldenArcher.h"
#include "Main.h"
#include "ChatCommands.h"
#include "Console.h"
#include "Configs.h"
#include "ConfigDefines.h"
#include "Utilits.h"
#include "PCPoints.h"
#include "Monster.h"
#include "cSQL.h"
#include "User.h"

GoldenArcher GAsystem;

struct SaveInfoItem
{
	int Type;
	int Index;
	int Level;
	int Skill;
	int Luck;
	int Option;
	int Excellent;
}GaItem;

GoldenArcher::GoldenArcher()
{
	this->RemainedRena = 0;
}

GoldenArcher::~GoldenArcher()
{

}

void GoldenArcher::Golden_Archer(int aIndex)
{
	if (this->IsRena(aIndex))
	{
		OBJECTSTRUCT * gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
		ActualRena = (unsigned short*)&gObj->m_Quest[29];

		if(*ActualRena == 0xFFFF)
			*ActualRena = 0;

		if(*ActualRena > Config.Archer.NeedRenaAmount)
		{
			*ActualRena = 0;
		}

		*ActualRena += 1;

		if (*ActualRena == Config.Archer.NeedRenaAmount)
		{
			*ActualRena = 0;
			this->GAGiveReward(aIndex,this->GAGetRewardItem()); 
			Chat.Message(1, gObj->m_Index, "[Golden Archer] Thank you for renas! Take my rewards for you.");

			if(Config.Archer.ZenReward > 0)
			{
				gObj->Money += Config.Archer.ZenReward;
				GCMoneySend(gObj->m_Index, gObj->Money);		
				Chat.MessageLog(1, cLog.c_Green, cLog.t_Default, gObj, "[Golden Archer] You lucked %d of zen.",Config.Archer.ZenReward);
			}
			if ( ( Config.Archer.PCPointsReward > 0 && Config.Archer.WCoinsReward > 0 ) || ( Config.Archer.PCPointsReward > 0 || Config.Archer.WCoinsReward > 0 ) )
			{
				this->GetPoints(aIndex);
			}
		}
		else
		{
			RemainedRena = Config.Archer.NeedRenaAmount - *ActualRena;
			Chat.MessageLog(1, cLog.c_Green, cLog.t_Default, gObj, "[Golden Archer] Your 'Rena' accepted, You have %d rens , for reward need %d more.",
				*ActualRena,RemainedRena);

			unsigned char GAMsg[7] = {0xc1, 0x07, 0x94, 0x00};

			memcpy(&GAMsg[4], ActualRena, 2);
			DataSend(aIndex, GAMsg, 7);
		}
	}
}

DWORD GoldenArcher::GAGetRewardItem()
{
	char zBuf[1024];
	bool state=false;
	int amount=0,i=0;
	int stopiter;
	while (i<2)
	{
		GA = fopen(RMSTArcher,"r");

		i++;
		if (i == 2)  stopiter = rand ()%amount+1;

		amount = 0;
		state = false;
		
		while (!feof(GA))
		{
			fgets(zBuf,1024,GA);

			if (zBuf[0] == '/' && zBuf[1] == '/') continue;

			if (zBuf[0] == '0')
			{ state = true; continue; }

			if (!state) continue;		

			if ( strcmp(zBuf,"end") == 0 ) { break; fclose(GA);}

			if (state)
			{
				if ( i == 1)
				{
					amount++; // В первой итерации считаем сколько итемов записано
					continue;
				}
				if ( i == 2) // Во второй итерации выбираем рандомную вещь
				{
					sscanf(zBuf,"%d %d %d %d %d %d %d",
						&GaItem.Type,&GaItem.Index,&GaItem.Level,&GaItem.Skill,&GaItem.Luck,&GaItem.Option,&GaItem.Excellent);
					amount++;
					if (amount == stopiter) break;
				}

			}
		}
	}
	fclose(GA);
	DWORD Reward = DWORD ((GaItem.Type * 512) + GaItem.Index);	 
	return Reward;
}

void GoldenArcher::GAGiveReward(int aIndex, DWORD Item)
{ 
	int Exc = 0;
	OBJECTSTRUCT * gObj = (OBJECTSTRUCT *)OBJECT_POINTER(aIndex);

	if ( (DWORD) GaItem.Excellent >= 1 )	{ Exc = Util.GenExcOpt(Config.Archer.MaxExcOpt); }

	ItemSerialCreateSend (aIndex,gObj->MapNumber,gObj->X,gObj->Y,Item,GaItem.Level,0,GaItem.Skill,GaItem.Luck,GaItem.Option,aIndex,Exc,0);
	cLog.ConsoleOutPut(1, cLog.c_Green,cLog.t_Default,"[Golden Archer] %s's item: %d %d %d %d %d %d %d",gObj->Name, GaItem.Type, GaItem.Index,GaItem.Level,GaItem.Skill,GaItem.Luck,GaItem.Option,Exc);
}

bool GoldenArcher::IsRena(int aIndex)
{
	OBJECTSTRUCT * gObj = (OBJECTSTRUCT *)OBJECT_POINTER(aIndex);

	for (BYTE i = 12; i<76; i++ )
	{
		if (gObj->pInventory[i].m_Type == 0x01C15 && gObj->pInventory[i].m_Level == 0 )
			return true;
	}

	return false;
}

void GoldenArcher::GetPoints(int aIndex)
{
	OBJECTSTRUCT * gObj = (OBJECTSTRUCT *)OBJECT_POINTER(aIndex);
	
	if (Config.Archer.WCoinsReward > 0)
	{
		gObj->m_wCashPoint += Config.Archer.WCoinsReward;	
		MySQL.Execute("UPDATE MEMB_INFO SET %s = %s + %d WHERE memb___id = '%s'", Config.WCoinsColumn, Config.WCoinsColumn, Config.Archer.WCoinsReward, gObj->AccountID);	
		Chat.MessageLog(1, cLog.c_Green, cLog.t_Default, gObj, "[Golden Archer] Points lucked: WCoins:%d", Config.Archer.WCoinsReward);
	}
	if (Config.Archer.PCPointsReward > 0)
	{
		g_PointShop.IncreasePoints(aIndex,Config.Archer.PCPointsReward);
		MySQL.Execute("UPDATE Character SET %s = %s + %d WHERE Name= '%s'", Config.PCPointsColumn, Config.PCPointsColumn, Config.Archer.PCPointsReward, gObj->Name);	
		Chat.MessageLog(1, cLog.c_Green, cLog.t_Default, gObj, "[Golden Archer] Points lucked: PC Points:%d", Config.Archer.PCPointsReward);
	}	
}