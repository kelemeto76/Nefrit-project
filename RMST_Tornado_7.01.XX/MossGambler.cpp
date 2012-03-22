#include "Stdafx.h"
#ifdef _GameServer_
#define ITEMGET(x,y) ( (x)*512 + (y) );
#include "MossGambler.h"
#include "Main.h"
#include "Prodef.h"
#include "Configs.h"
#include "Console.h"
#include "ChatCommands.h"
#include "ConfigDefines.h"
#include "MarrySystem.h"
#include "Monster.h"
#include "cSQL.h"
#include "PCPoints.h"
#include "Utilits.h"

MossGambler g_Moss;

struct g_TimeMoss
{
	int starthour,startmin,delay,closehour,closemin;
}MossTime[128];

int MossgObjIndex;

void MossBuyDelay(void *lparam)
{
	while ( true )
	{
		int Index = MossgObjIndex;
		AddTab[Index].MossBuyDelay = true;
		Sleep(1500);
		AddTab[Index].MossBuyDelay = false;
		_endthread();
	}
}

struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[12];	// 4
};

MossGambler::MossGambler()
{

}

MossGambler::~MossGambler()
{
	
}

void MossGambler::SpawnMoss()
{
	MonsterAddAndSpawn(492,0,51,22,225);
}

void MossGambler::DisappearMoss()
{
	for(int iIndex = 0; iIndex < OBJ_MAXMONSTER; iIndex++)
	{
		OBJECTSTRUCT *mObj = (OBJECTSTRUCT*)OBJECT_POINTER(iIndex);
		if(mObj->Class == 492)
		{
			gObjDel(iIndex);
		}
	}
	int MobCount = *(DWORD *)(MonsterCount);
	MobCount--;
}

void MossGambler::LoadTimeConfig()
{
	this->Opened = false;
	this->Amount = 0;

	g_Moss.DisappearMoss();

	FILE * mosstime; 
	char buff[256]; 
	bool flag=false;

	mosstime = fopen(RMSTMossTime,"r");

	if ( mosstime != NULL )
	{
		while ( !feof(mosstime) )
		{
			fgets(buff,256,mosstime);

			if ( memcmp(buff,"end",strlen("end")) == 0 ) break;
			if ( memcmp(buff,"//",strlen("//")) == 0 ) 	continue;
			if ( memcmp(buff,"0",strlen("0")) == 0 && !flag)
			{
				flag = true;
				continue;
			}

			if (flag)
			{
				sscanf(buff,"%d %d %d",&MossTime[Amount].starthour,&MossTime[Amount].startmin,&MossTime[Amount].delay);
				
				MossTime[Amount].closemin = MossTime[Amount].startmin + MossTime[Amount].delay;
				MossTime[Amount].closehour = MossTime[Amount].starthour;

				while ( MossTime[Amount].closemin >= 60) MossTime[Amount].closemin -= 60, MossTime[Amount].closehour++;

				Amount++;
			}
		}
		fclose(mosstime);
	}
	else	MessageBox(NULL, "Cant Find MossOpenTimeConfig.dat", "Error", 0);				   
}

void MossGambler::OpenedMoss()
{
	CTime t = CTime::GetCurrentTime();
	if (!t.GetSecond()) 
	{
		m_hour = t.GetHour();
		m_min = t.GetMinute();

		int BeforeStartHour,BeforeStartMin;

		for (int i=0; i<Amount; i++ )
		{
			BeforeStartHour = MossTime[i].starthour;
			BeforeStartMin = MossTime[i].startmin - 5;

			if ( BeforeStartMin < 0) 	BeforeStartMin = 60 - BeforeStartMin,	BeforeStartHour--;

			if ( m_hour == BeforeStartHour && m_min == BeforeStartMin )
					Chat.MessageAll(0, 0,NULL,"[Moss The Gambler] Moss The Gambler Open in a 5 min.");

			if ( m_hour == MossTime[i].starthour && m_min == MossTime[i].startmin)
			{
				Chat.MessageAll(0, 0,NULL,"[Moss The Gambler] Moss The Gambler Opened.");
				this->SpawnMoss();
				Opened = true;
			}
			if ( m_hour == MossTime[i].closehour && m_min == MossTime[i].closemin)
			{
				Chat.MessageAll(0, 0,NULL,"[Moss The Gambler] Moss The Gambler Closed.");
				this->DisappearMoss();
				Opened = false;
			}
		}
	}
} 
bool MossGambler::SellItem(int Index, unsigned char *aRecv)
{
	if ( AddTab[Index].MossBuyDelay )
	{
		Chat.Message(Index,"[Moss The Gambler] Please wait 1.5 second after a previous purchase");
		return false;
	}
	OBJECTSTRUCT * gObj  = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	if(gObj->TargetShopNumber == 492)
	{
		if (!Config.Moss.Enabled)
		{
			Chat.Message(1,Index,"[Moss The Gambler] Moss The Gambler function is disabled.");
			return false;
		}
		if ( gObj->Money < Config.Moss.NeedZen )
		{
			Chat.Message(1,Index,"[Moss The Gambler] You haven't got enougth money.");
			return false;
		}
		if (AddTab[gObj->m_Index].PCPlayerPoints < Config.Moss.NeedPCPnt) 
		{
			Chat.Message(1,Index,"[Moss The Gambler] You haven't got enougth PCPoints.");
			return false;
		}
		if (gObj->m_wCashPoint < Config.Moss.NeedWCn) 
		{
			Chat.Message(1,Index,"[Moss The Gambler] You haven't got enougth WCoins.");
			return false;
		}

		this->GetRandomItem(aRecv);

		PMSG_BUYRESULT pRez;

		pRez.h.c=0xC1;
		pRez.h.headcode=0x32;
		pRez.h.size=16;

		this->ConvertToHex((LPBYTE)&pRez.ItemInfo[0]);
		pRez.ItemInfo[11]=0xFF;
		pRez.ItemInfo[10]=0xFF;
		pRez.ItemInfo[9]=0xFF;
		pRez.ItemInfo[8]=0xFF;
		pRez.ItemInfo[7]=0xFF;

		pRez.Result = gObjShopBuyInventoryInsertItem(Index,ItemSend);

		if (pRez.Result == 0xFF)
		{
			Chat.Message(1,Index,"[Moss The Gambler] Your Inventory is full");
			return false;
		}

		if(ItemSend.m_NewOption	> 0)
		{
			cMarry.MarryFirework(gObj->X, gObj->Y, gObj->MapNumber, gObj);	 
			Chat.Message(0,Index,"[Moss The Gambler] Congratulations, you are very lucky!!!");
		}

		gObj->Money = gObj->Money - Config.Moss.NeedZen;
		GCMoneySend(Index,gObj->Money);

		if(Config.Moss.NeedPCPnt > 0)
		{
			g_PointShop.DecreasePoints(gObj->m_Index, Config.Moss.NeedPCPnt);	  
			MySQL.Execute("UPDATE Character SET %s = %s - %d WHERE Name= '%s'", Config.PCPointsColumn, Config.PCPointsColumn, Config.Moss.NeedPCPnt, gObj->Name);
		}

		if(Config.Moss.NeedWCn > 0)
		{
			gObj->m_wCashPoint -= Config.Moss.NeedWCn;
			MySQL.Execute("UPDATE MEMB_INFO SET %s = %s - %d WHERE memb___id = '%s'", Config.WCoinsColumn, Config.WCoinsColumn, Config.Moss.NeedWCn, gObj->AccountID);
		}

		DataSend(Index,(LPBYTE)&pRez,pRez.h.size);
		MossgObjIndex = Index;
		_beginthread(MossBuyDelay,0,NULL);

		return TRUE;
	}
	return FALSE;
}

void MossGambler::ConvertToHex(unsigned char *buf)
{
	int n=0;

	memset(buf, 0, 7);

	buf[n] = ItemSend.m_Type & 0xFF;
	n++;
	buf[n] = 0;
	buf[n] |= ItemSend.m_Level * 8;
	buf[n] |= ItemSend.m_Option1 * 128;
	buf[n] |= ItemSend.m_Option2 * 4;
	buf[n] |= ItemSend.m_Option3 & 3;
	n++;
	buf[n] = (unsigned char)ItemSend.m_Durability;
	n++;
	buf[n] = 0;
	buf[n] |= ((ItemSend.m_Type & 0x100) >> 1);

	if ( ItemSend.m_Option3 > 3 )	buf[n] |= 0x40; // item +16 option

	buf[n] |= ItemSend.m_NewOption;
	n++;
	buf[n] = ItemSend.m_SetOption;
	n++;
	buf[n] |= (ItemSend.m_Type & 0x1E00 ) >> 5;

	BYTE btItemEffectFor380 = 0;
	btItemEffectFor380 = (ItemSend.m_ItemOptionEx & 0x80 ) >> 4;
	buf[n] |= btItemEffectFor380;
	n++;
	buf[n] = ItemSend.m_JewelOfHarmonyOption;
}

void MossGambler::GetRandomItem(unsigned char * aRecv)
{
	srand((unsigned) time(NULL));
	FILE *moss;
	char buf[1024];
	int arrayitem[255];	  
	int Durr[255];
	char group;
	int count = 0;
	int GenerateItem;
	bool flag = false;

	if(aRecv[3]==0)  group = '0';
	if(aRecv[3]==2)  group = '2';
	if(aRecv[3]==4)	 group = '3';
	if(aRecv[3]==6)	 group = '4';
	if(aRecv[3]==24) group = '5';

	moss = fopen(RMSTMossGambler,"r");
	if ( moss != NULL )
	{
		while (!feof(moss))
		{
			fgets(buf,1024,moss);

			if (memcmp(buf,"//",strlen("//")) == 0 )	continue;

			if (buf[0] == group && !flag)
			{
				flag = true;
				continue;
			}

			if ( memcmp(buf,"end",strlen("end")) == 0  && flag) break;

			if (flag)
			{
				sscanf(buf,"%d %d", &arrayitem[count], &Durr[count]);
				count++;
			}
		}
		fclose(moss);

		GenerateItem = rand()%count;

		/************************************************************************/
		/*                     Заполнение структуры CItem                       */
		/************************************************************************/
		ItemSend.m_Type = ITEMGET((int)group-48, arrayitem[GenerateItem]);

		/*Level Items*/
		ItemSend.m_Level =		this->RandomOption(1);	
		/*skill*/
		ItemSend.m_Option1 =	this->RandomOption(2);
		/*luck*/
		ItemSend.m_Option2 =	this->RandomOption(3);
		/*add*/
		ItemSend.m_Option3 =	this->RandomOption(4);
		/*Excellent options*/
		ItemSend.m_NewOption =	this->RandomOption(5);
		/*Durability*/
		ItemSend.m_Durability = (float) GetItemDurability(Durr[GenerateItem],ItemSend.m_Level,ItemSend.m_NewOption);
		/*No Harmony*/
		ItemSend.m_JewelOfHarmonyOption = 0x00;
		/*No Ancient*/
		ItemSend.m_SetOption = 0x00;
		/*No Sockets*/
		for ( int i = 0; i < 5; i++ ) ItemSend.m_SocketOption[i] = 0xFF;
		/* For All*/
		for ( int i = 0; i < 6; i++ ) ItemSend.m_RequireClass[i] = 0x01;

		ItemSend.m_Number=(unsigned long)gGetItemNumber;
		ItemSend.m_serial = 0;
	}
	else
		 MessageBox(NULL, "Cant Find MossGambler.ini", "Error", 0);
}

int MossGambler::RandomOption(int TypeOpt)
{
	srand((unsigned) GetTickCount());
/************************************************************************/
/*                       GetRandLevelItem                               */
/************************************************************************/
	if (TypeOpt == 1)
	{
		int RandArray[6]={Config.Moss.RateLevelItem1,Config.Moss.RateLevelItem7,Config.Moss.RateLevelItem10,
			Config.Moss.RateLevelItem11,Config.Moss.RateLevelItem12,Config.Moss.RateLevelItem13};
		int GenereteArr[6];
		int k = 6;
		int ValueOpt = -1;

		while ( k > 1)
		{
			k = 0;
			for (int i=0; i<6; i++)	
				if (GenereteArr[i] != -1) GenereteArr[i]=rand()%100+1;
			for (int i=0; i<6; i++)
				if ( GenereteArr[i] > RandArray[i] ) GenereteArr[i] = -1;
			for (int i=0; i<6; i++) 
				if ( GenereteArr[i] != -1 ) k++;
		}
		for (int i=0; i<6; i++)
			if ( GenereteArr[i] != -1 ) ValueOpt = i;
		if (ValueOpt == -1 ) return 0;
		else
			if ( ValueOpt == 0 ) return rand()%7;
			else
				if (ValueOpt == 1) return rand()%3+7;
				else return ValueOpt+8;	
	}
/************************************************************************/
/*                       GetRandSkill                                   */
/************************************************************************/
	if ( TypeOpt == 2)
	{
		int RandomKey = rand()%100+1;
		if (RandomKey <= Config.Moss.RateSkill) return 1;
		else return 0;
	}
/************************************************************************/
/*                       GetRandLuck                                    */
/************************************************************************/
	if (TypeOpt == 3)
	{
		int RandomKey = rand()%100+1;
		if (RandomKey <= Config.Moss.RateLuck) return 1;
		else return 0;
	}
/************************************************************************/
/*                       GetRandAdd                                     */
/************************************************************************/
	if (TypeOpt == 4)
	{
		int RandArray[3]={Config.Moss.RateAddOpt,Config.Moss.RateAddOpt1,Config.Moss.RateAddOpt2};
		int GenereteArr[3];
		int k = 3;
		int ValueOpt = -1;

		while ( k > 1)
		{
			k = 0;
			for (int i=0; i<3; i++)	
				if (GenereteArr[i] != -1) GenereteArr[i]=rand()%100+1;
			for (int i=0; i<3; i++)
				if ( GenereteArr[i] > RandArray[i] ) GenereteArr[i] = -1;
			for (int i=0; i<3; i++) 
				if ( GenereteArr[i] != -1 ) k++;
		}
		for (int i=0; i<3; i++)
			if ( GenereteArr[i] != -1 ) ValueOpt = i;
		if (ValueOpt == -1 ) return 0;
		else
			if ( ValueOpt == 0 ) return 1;
			else
				if (ValueOpt == 1) return rand()%3+1;
				else 
					if (ValueOpt == 2) return rand()%3+5;		
	}
/************************************************************************/
/*                       GetRandExcOpt                                  */
/************************************************************************/
	if ( TypeOpt == 5)
	{
		int NoExl = rand()%100+1;
		if ( NoExl < Config.Moss.RateExcOpt ) return 0;
		int RandArray[6]={Config.Moss.RateExcOptOne,Config.Moss.RateExcOptTwo,Config.Moss.RateExcOptTree,
							Config.Moss.RateExcOptFour,Config.Moss.RateExcOptFive,Config.Moss.RateExcOptSix};
		int GenereteArr[6];
		int k = 6;
		int AmountOptions = -1;

		while ( k > 1)
		{
			k = 0;
			for (int i=0; i<6; i++)	
				if (GenereteArr[i] != -1) GenereteArr[i]=rand()%100+1;
			for (int i=0; i<6; i++)
				if ( GenereteArr[i] > RandArray[i] ) GenereteArr[i] = -1;
			for (int i=0; i<6; i++) 
				if ( GenereteArr[i] != -1 ) k++;
		}
		for (int i=0; i<6; i++)
			if ( GenereteArr[i] != -1 ) AmountOptions = i;
		
		if (AmountOptions != -1) return Util.GenExcOpt(AmountOptions+1);
		else return 0;		
	}
	return 0;
}

int MossGambler::GetItemDurability(int dur, int lvlitem, int exc)
{
	if ( exc > 0 ) dur +=15;
	
	if (  lvlitem == 0 ) return dur;
	else
		if ( lvlitem < 5 ) return dur+lvlitem;
		else
			if (lvlitem < 10 ) return dur+lvlitem+(lvlitem - 4);
			else
				if ( lvlitem == 10 ) return dur+17;
				else
					if ( lvlitem == 11 ) return dur+21;
						else
							if ( lvlitem == 12 ) return dur+26;
							else
								if( lvlitem == 13 )  return dur+32;
								else
									return -1;
}

#endif