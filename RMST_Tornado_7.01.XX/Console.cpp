// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "stdafx.h"
#include "Console.h"	
#include "ConfigDefines.h"
#include "Protection.h"
#include <string>
#include "ChatCommands.h"
#include "Configs.h"
#include "DropEvent.h"
#include "Monster.h"
#include "IPBlock.h"
#include "DropSystem.h"
#include "Utilits.h"
#include "cSQL.h"
#include "HappyHour.h"
#include "PCPoints.h"

char Message[1024];
				   
Logger cLog;	   
DWORD PiD;


Logger::Logger(){}
Logger::~Logger(){}




void __stdcall LoggerCore(PVOID pVoid)
{							   
	char Temp[255];
	while (true)
	{					  
		AllocConsole();
		SetConsoleTitleA(CONSOLETITLE);	 
		if(cProtection.CheckFeature(cProtection.Console))
		{ 
			cLog.StdIn(Temp);
			cLog.ConsoleCommands(Temp);
		}
		Sleep(1000);
	}
	_endthread();
}


void Logger::LoggerInit()
{
	CreateThread( 0 , 0 , (LPTHREAD_START_ROUTINE) LoggerCore , 0 , 0 , &PiD );
	Sleep(100);
	cLog.ConsoleOutPut(0, cLog.c_Green,cLog.t_NULL,"RMSТeam: Tornado 7.10.%02d Season 4.6", dBuild);
	cLog.ConsoleOutPut(0, cLog.c_Green,cLog.t_NULL,"Offical Website: http://rmsteam.org/");
	cLog.ConsoleOutPut(0, cLog.c_Green,cLog.t_NULL,"Compile Date: %s %s", __DATE__, __TIME__);
	cLog.ConsoleOutPut(0, cLog.c_Green,cLog.t_NULL,"Credits: Mu Community, RMST & Simp1e");
	cLog.ConsoleOutPut(0, cLog.c_Green,cLog.t_NULL,"Please report any bugs that you found!");
	if(cProtection.Local_Id != -1)
	cLog.ConsoleOutPut(0, cLog.c_Magenta,cLog.t_NULL,"Files are licensed to: %s", cProtection.CheckName());
	else
	cLog.ConsoleOutPut(0, cLog.c_Magenta,cLog.t_NULL,"We have many PRIVATE modules check up our forum..." );
	cLog.ConsoleOutPut(0, cLog.c_Green,cLog.t_NULL,"Contacts: ICQ-5722555, MSN-ma-3x@list.ru, Skype-cool3x \n-----------------------------------------------------\n");
} 
//Иммитация одного Enter пустого ввода.
int Logger::StdIn(char* Buffer)
{							    
  	char inText[255];
	memset(inText, 0x00, 255);
	memset(Buffer, 0x00, 255);
	DWORD dwBytesWritten;							
	SetConsoleTextAttribute(this->Handle(FALSE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);	
	ReadFile(Handle(TRUE), inText, 255, &dwBytesWritten, NULL);
	strncpy(Buffer, inText, strlen(inText)-2);		   
	SetConsoleTextAttribute(this->Handle(FALSE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return dwBytesWritten;
}
		

void Logger::ConsoleCommands(char* InputCommand)
{	
	CheckProcent(InputCommand);

	char Command[50];
	int CountSubBlock = 0;

	GetArraySubChar(InputCommand,CountSubBlock,' ');

	if (CountSubBlock == 0)
		strcpy(Command,InputCommand);
	else
		strcpy(Command,Param[0]);

	//Clean
	//===========================
	if(!strcmp(Command,"/clean") || !strcmp(Command,"/cls"))
	{
		return;
	}
	//Disconnect
	//===========================
	if(!strcmp(Command,"/disconnect"))
	{
		bool DigitsParam[] = {false,false};
		if (!CheckConsoleCommand("Disconnect","/disconnect <NickName>",Param,DigitsParam,CountSubBlock,1))
			return;

		char NickName[11];
		strcpy(NickName,Param[1]);

		int Index = Util.GetPlayerIndex(NickName);
		CloseClient(Index);								  
		cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Disconnect] %s was disconnected.",NickName);
		return;
	}
	//BanChar
	//===========================
	if(!strcmp(Command,"/banchar"))
	{
		bool DigitsParam[] = {false,false};
		if (!CheckConsoleCommand("BanChar","/banchar <NickName>",Param,DigitsParam,CountSubBlock,0))
			return;

		char NickName[11];
		strcpy(NickName,Param[1]);

		int Index = Util.GetPlayerIndex(NickName);
		
		MySQL.Execute("SELECT CtlCode FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, NickName);		
		int Banned = MySQL.GetInt();
							 	
		if(Banned == 1)
		{																											   
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[BanChar] %s is already banned", NickName); 
		}
		else   
		{																				 
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET CtlCode='1' WHERE Name='%s'",MySQL.szDatabase,NickName);		
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,  "[BanChar] %s banned", NickName);
			Chat.Message(Index,"[BanChar] Admin ban your account");
		}

		CloseClient(Index);
		return;
	}
	//UnBanChar
	//===========================
	if(!strcmp(Command,"/unbanchar"))
	{
		bool DigitsParam[] = {false,false};
		if (!CheckConsoleCommand("UnBanChar","/unbanchar <NickName>",Param,DigitsParam,CountSubBlock,0))
			return;
		
		char NickName[11];
		strcpy(NickName,Param[1]);;
		int Index = Util.GetPlayerIndex(NickName);

		MySQL.Execute("SELECT CtlCode FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, NickName);
		
		int Banned = MySQL.GetInt();
							 	
		if(Banned == 0)
		{																											   
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"[UnBanChar] %s is not baned", NickName);
		}
   		else   
		{		 
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET CtlCode='0' WHERE Name='%s'",MySQL.szDatabase,NickName);
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"[UnBanChar] %s UnBaned", NickName);
		}
		return;
	} 

	//Gg
	//===========================
	if(!strcmp(Command,"/gg"))
	{
		Chat.MessageAllLog(0, 0, cLog.c_Yellow, cLog.t_GPOST, NULL, "[ADMIN]: %s",(char*)(InputCommand+4));	 
		cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[GG] [ADMIN]: %s", (char*)(InputCommand+4));
		return;
	}
	//Reload
	//===========================
	if(!strcmp(Command,"/reload"))
	{
		bool DigitsParam[] = {false,true};
		if (!CheckConsoleCommand("Reload","/reload <ValueConfig>",Param,DigitsParam,CountSubBlock,0))
			return;

		int NumberReload = CharToInt(Param[1]);

		switch (NumberReload)
		{
		case 0:
			{
				Config.LoadAll();
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] RMST Data Reloaded.");
				break;
			}
		case 1:
			{
				GameMonsterAllCloseAndReload();
#ifdef _GameServer_
				ReadMonsterAdd();
#endif
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] Monsters Reloaded.");
				break;
			}
		case 2:
			{
				pShopDataLoad();
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] Shops Reloaded.");
				break;
			}
		case 3:
			{
				if (Config.IsGMSystem)
				{
					GMS.LoadGMSystem();
					cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] GMSystem Reloaded.");
				}
				else
					cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] GMSystem is disabled");
				break;
			}
		case 4:
			{
				cexDrop.LoadConfig();		// reload Drop config
#ifdef _GameServer_
				cexDropEvent.updateBC();	// reload BC drop config
				cexDropEvent.updateIT();	// reload IT drop config
				cexDropEvent.updateWW();	// reload WW drop config
				cexDropEvent.updateCC();	// reload CC drop config
#endif
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] DropSystem`s Reloaded.");
				break;
			}
		case 5:
			{
				IP.LoadIpBlock();
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] IPBlock Reloaded.");
				break;
			}
		case 6:
			{
				Config.LoadAll();
				GameMonsterAllCloseAndReload();
				pShopDataLoad();
				IP.LoadIpBlock();

				if (Config.IsGMSystem) GMS.LoadGMSystem();
				
				cexDrop.LoadConfig();		// reload Drop config
#ifdef _GameServer_
				ReadMonsterAdd();
				cexDropEvent.updateBC();	// reload BC drop config
				cexDropEvent.updateIT();	// reload IT drop config
				cexDropEvent.updateWW();	// reload WW drop config
				cexDropEvent.updateCC();	// reload CC drop config
#endif
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] All Options Reloaded.");
				break;
			}
		default:
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[Reload] Usage: /reload <Number>");
				break;
			}

		}
		return;
	}

	if (!strcmp(Command,"/time"))
	{
		SYSTEMTIME t;
		GetLocalTime(&t);
		cLog.ConsoleOutPut(0, cLog.c_Blue,cLog.t_NULL, "Server Time & Date: %02d:%02d:%02d %02d-%02d-%02d.", t.wHour, t.wMinute, t.wSecond, t.wDay, t.wMonth, t.wYear);
		return;
	}

	if (!strcmp(Command,"/evo"))
	{
		bool DigitsParam[] = {false,false};
		if (!CheckConsoleCommand("Evo","/evo <NickName>",Param,DigitsParam,CountSubBlock,1)) return;

		char NickName[11];
		strcpy(NickName,Param[1]);

		int Index = Util.GetPlayerIndex(NickName);

		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

		if(gObj->DbClass == 1 || gObj->DbClass == 17 || gObj->DbClass == 33 || gObj->DbClass == 48 || gObj->DbClass == 64 || gObj->DbClass == 81)
		{		
			gObj->DbClass += 2;
			BYTE btClass = (gObj->Class * 32) + 24;
			GCSendQuestPrize(gObj->m_Index, 204, btClass);
			cLog.ConsoleOutPut(0, cLog.c_MistyRose, cLog.t_COMMANDS, "[Evo] Congratulations %s successfully pass third quest!",NickName);
		}
		else if(gObj->DbClass == 3 || gObj->DbClass == 19 || gObj->DbClass == 35 || gObj->DbClass == 50 || gObj->DbClass == 66 || gObj->DbClass == 83)
		{
			cLog.ConsoleOutPut(0, cLog.c_MistyRose, cLog.t_COMMANDS, "[Evo] %s already finished third quest!",NickName);
		}
		else
		{
			cLog.ConsoleOutPut(0, cLog.c_MistyRose, cLog.t_COMMANDS, "[Evo] %s need to pass seccond quest!",NickName);
		}
		return;
	}

	if (!strcmp(Command,"/setzen"))
	{
		bool DigitsParam[] = {false,false,true};
		if (!CheckConsoleCommand("SetZen","/setzen <NickName> <SetZen>",Param,DigitsParam,CountSubBlock,1)) return;

		int Value = CharToInt(Param[2]);

		if(Value < 0 || Value > 2000000000)
		{
			cLog.ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetZen] Value can't be less than 0 and more than 2000000000!");
			return;
		}	

		char NickName[11];
		strcpy(NickName,Param[1]);

		int aIndex = Util.GetPlayerIndex(NickName);

		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

		gObj->Money = Value;
		GCMoneySend (gObj->m_Index, Value);	

		cLog.ConsoleOutPut(0, cLog.c_Red, cLog.t_GM,  "[SetZen] You sucsessfully changed %s zen.", gObj->Name);
		Chat.Message(aIndex,"[SetZen] Your zen was changed to %d by Admin.", Value);
		return;
	}

	if (!strcmp(Command,"/setpk"))
	{
		bool DigitsParam[] = {false,false,true};
		if ( !CheckConsoleCommand("SetPK","/setpk <NickName> <LevelPK>",Param,DigitsParam,CountSubBlock,1) ) return;

		int SetLevel = CharToInt(Param[2]);

		if(SetLevel < 0 || SetLevel > 100)
		{
			cLog.ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetPK] PK lvl can't be less than 0 and more than 100!");
			return;
		}			 

		char NickName[11];
		strcpy(NickName,Param[1]);

		int aIndex = Util.GetPlayerIndex(NickName);

		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);	

		gObj->m_PK_Level = SetLevel;
		if(SetLevel >= 3)
			gObj->m_PK_Count = SetLevel - 3;

		GCPkLevelSend(gObj->m_Index,SetLevel);

		cLog.ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetPK] You sucsessfully changed %s pk.", gObj->Name);
		Chat.Message(gObj->m_Index,"[SetPK] Your pk was changed to %d by Admin.", SetLevel);
		return;
	}

	if (!strcmp(Command,"/pkclear"))
	{
		bool DigitsParam[] = {false,false};
		if ( !CheckConsoleCommand("PkClear","/pkclear <NickName>",Param,DigitsParam,CountSubBlock,1) ) return;

		char NickName[11];
		strcpy(NickName,Param[1]);

		int aIndex = Util.GetPlayerIndex(NickName);

		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);

		if (gObj->m_PK_Level <=3)
		{			
			cLog.ConsoleOutPut(0, cLog.c_Blue, cLog.t_COMMANDS, "[PkClear] %s are not pk!", gObj->Name);		
			return;
		}

		gObj->m_PK_Level = 3;
		gObj->m_PK_Count = 0;

		GCPkLevelSend (gObj->m_Index, 3);
		cLog.ConsoleOutPut(0, cLog.c_Blue, cLog.t_COMMANDS, "[PkClear] %s Pk sucsessfully cleared!", gObj->Name);
		Chat.Message(aIndex,"[PkClear] You pk successfully cleared!");
		return;
	}

	if (!strcmp(Command,"/skin"))
	{
		bool DigitsParam[] = {false,false,true};
		if ( ! CheckConsoleCommand("Skin","/skin <NickName> <NumberSkin>",Param,DigitsParam,CountSubBlock,1) ) return;

		int SetLevel = CharToInt(Param[2]);

		char NickName[11];
		strcpy(NickName,Param[1]);

		int Index = Util.GetPlayerIndex(NickName); 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

		gObj->m_Change = SetLevel;
		gObjViewportListProtocolCreate(gObj);														 
		cLog.ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[Skin] You sucsessfully change %s Skin.", gObj->Name);
		Chat.Message(Index,"[Skin] Your Skin was changed by Admin.");
		return;
	}

	if (!strcmp(Command,"/drop"))
	{
		bool DigitsParam[] = {false,true,true,true,true,true,true,true,true,true,true,true,true};
		if (CheckConsoleCommand("Drop","/drop <MapNumber> <PozX> <PozY> <ItemCount> <ItemType> <ItemIndex> <ItemLvl> <ItemSkill> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>",Param,DigitsParam,CountSubBlock,0))
		{
			int Params[12]	  = {0,0,0,0,0,0,0,0,0,0,0,0};
			int MinValues[12] = {0,0,0,1,0,0,0,0,0,0,0,0};
			int MaxValues[12] = {64,255,255,20,15,999,13,1,1,7,63,40};
			bool bRezult = true;
			for (int i = 0; i < 12; i++)
			{
				Params[i] = CharToInt(Param[i+1]);
				if (Params[i] > MaxValues[i] || Params[i] < MinValues[i])
					bRezult = false;
			}
			if (bRezult)
			{
				int Item = Params[4] * 512 + Params[5];

				for (int i=0; i < Params[3]; i++ )
					ItemSerialCreateSend (NULL, Params[0], Params[1], Params[2], Item, Params[6], 0, Params[7], Params[8], Params[9], NULL, Params[10], Params[11]);

				ConsoleOutPut(0, cLog.c_Green, cLog.t_GM, "[Drop] %d Item Created %d %d %d %d %d %d %d %d - Success",
					Params[3], Params[4], Params[5], Params[6], Params[7], Params[8], Params[9], Params[10], Params[11]);
			}
			else
				ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[Drop] Usage: /drop <MapNumber> <PozX> <PozY> <ItemCount> <ItemType> <ItemIndex> <ItemLvl> <ItemSkill> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>");
			return;
		}
		return;
	}

	if (!strcmp(Command,"/setdrop"))
	{
		bool DigitsParam[] = {false,true,true,true,true,true,true,true,true,true,true,true,true};

		if (CheckConsoleCommand("SetDrop","/setdrop <MapNumber> <PozX> <PozY> <ItemIndex> <ItemLvl> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>",Param,DigitsParam,CountSubBlock,0))
		{
			int Params[9]	  = {0,0,0,0,0,0,0,0,0,};
			int MinValues[9] = {0,0,0,1,0,0,0,0,0,};
			int MaxValues[9] = {64,255,255,999,13,1,7,63,40};
			int ItemMin = 7;
			bool bRezult = true;
			for (int i = 0; i < 9; i++)
			{
				Params[i] = CharToInt(Param[i+1]);
				if (Params[i] > MaxValues[i] || Params[i] < MinValues[i])
					bRezult = false;
			}
			if (bRezult)
			{
				//MG Set
				if(Params[3] == 15 ||Params[3] == 20 ||Params[3] == 23 ||Params[3] == 32 ||Params[3] == 37 ||Params[3] == 47 ||Params[3] == 48)	ItemMin = 8;

				int Item = Params[4] * 512 + Params[5];

				for(int i = ItemMin; i < 12; i++)
				{
					int Item = i * 512 + Params[3];	 
					int Rand1 = rand() % 2;
					int Rand2 = rand() % 2;
					ItemSerialCreateSend (NULL, Params[0], Params[1] + Rand1, Params[2] + Rand2, Item, Params[4], 0, 0, Params[5], Params[6], NULL, Params[7], Params[8]);
				}
				ConsoleOutPut(0, cLog.c_Green, cLog.t_GM, "[SetDrop] Items Created  %d %d %d %d %d %d - Success", Params[3], Params[4], Params[5], Params[6], Params[7], Params[8]);
			}
			else
				ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetDrop] Usage: /setdrop <MapNumber> <PozX> <PozY> <ItemIndex> <ItemLvl> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>");
			return;
		}
		return;
	}

	if (!strcmp(Command,"/gmove"))
	{
		bool DigitsParam[] = {false,false,false,true,true};
		if (!CheckConsoleCommand("GMove","/gmove <NickName> <Map> <x> <y>",Param,DigitsParam,CountSubBlock,1))
			return;

		char Target[11];
		strcpy(Target,Param[1]);
		char Map[250];
		strcpy(Map,Param[2]);
		int x = CharToInt(Param[3]);
		int y = CharToInt(Param[4]);

		int Index = Util.GetPlayerIndex(Target);

		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

		int RMap;
		if(_strcmpi(Map, "0") == 0 || _strcmpi(Map, "Lorencia") == 0) RMap = 0;
		else if	(_strcmpi(Map, "1") == 0 || _strcmpi(Map, "Dungeon") == 0) RMap = 1;
		else if	(_strcmpi(Map, "2") == 0 || _strcmpi(Map, "Devias") == 0) RMap = 2;
		else if (_strcmpi(Map, "3") == 0 || _strcmpi(Map, "Noria") == 0) RMap = 3;
		else if (_strcmpi(Map, "4") == 0 || _strcmpi(Map, "Losttower") == 0) RMap = 4;
		else if (_strcmpi(Map, "6") == 0 || _strcmpi(Map, "Stadium") == 0 || _strcmpi(Map, "Arena") == 0) RMap = 6;
		else if (_strcmpi(Map, "7") == 0 || _strcmpi(Map, "Atlans") == 0) RMap = 7;
		else if (_strcmpi(Map, "8") == 0 || _strcmpi(Map, "Tarkan") == 0) RMap = 8;		
		else if (_strcmpi(Map, "10") == 0 || _strcmpi(Map, "Icarus") == 0) RMap = 10;			  
		else if (_strcmpi(Map, "24") == 0 || _strcmpi(Map, "kalima1") == 0) RMap = 24;
		else if (_strcmpi(Map, "25") == 0 || _strcmpi(Map, "kalima2") == 0) RMap = 25; 
		else if (_strcmpi(Map, "26") == 0 || _strcmpi(Map, "kalima3") == 0) RMap = 26;		
		else if (_strcmpi(Map, "27") == 0 || _strcmpi(Map, "kalima4") == 0) RMap = 27;   
		else if (_strcmpi(Map, "28") == 0 || _strcmpi(Map, "kalima5") == 0) RMap = 28;
		else if (_strcmpi(Map, "29") == 0 || _strcmpi(Map, "kalima6") == 0) RMap = 29;   
		else if (_strcmpi(Map, "30") == 0 || _strcmpi(Map, "valleyofloren") == 0) RMap = 30;
		else if (_strcmpi(Map, "31") == 0 || _strcmpi(Map, "landoftrial") == 0 || _strcmpi(Map, "landsoftrial") == 0) RMap = 31;			   
		else if (_strcmpi(Map, "33") == 0 || _strcmpi(Map, "aida") == 0) RMap = 33;			   
		else if (_strcmpi(Map, "34") == 0 || _strcmpi(Map, "crywolf") == 0) RMap = 34;			   
		else if (_strcmpi(Map, "36") == 0 || _strcmpi(Map, "kalima7") == 0) RMap = 36;			   
		else if (_strcmpi(Map, "37") == 0 || _strcmpi(Map, "kanturu") == 0) RMap = 37;			   
		else if (_strcmpi(Map, "38") == 0 || _strcmpi(Map, "kanturutruins") == 0 || _strcmpi(Map, "kantrutruins") == 0) RMap = 38;			   
		else if (_strcmpi(Map, "39") == 0 || _strcmpi(Map, "kantrutower") == 0 || _strcmpi(Map, "kanturutower") == 0) RMap = 39;			   
		else if (_strcmpi(Map, "40") == 0 || _strcmpi(Map, "silent") == 0) RMap = 40;			   
		else if (_strcmpi(Map, "41") == 0 || _strcmpi(Map, "balgasbarrack") == 0) RMap = 41;			   
		else if (_strcmpi(Map, "42") == 0 || _strcmpi(Map, "balgasrefuge") == 0) RMap = 42;			   
		else if (_strcmpi(Map, "51") == 0 || _strcmpi(Map, "elbeland") == 0) RMap = 51;			   
		else if (_strcmpi(Map, "56") == 0 || _strcmpi(Map, "swampofcalmness") == 0 || _strcmpi(Map, "calmness") == 0) RMap = 56;			   
		else if (_strcmpi(Map, "57") == 0 || _strcmpi(Map, "raklion") == 0) RMap = 57;			   
		else if (_strcmpi(Map, "58") == 0 || _strcmpi(Map, "raklionboss") == 0) RMap = 58;			   
		else if (_strcmpi(Map, "62") == 0 || _strcmpi(Map, "santatown") == 0 || _strcmpi(Map, "santa") == 0) RMap = 62;			   
		else if (_strcmpi(Map, "63") == 0 || _strcmpi(Map, "vulcanus") == 0) RMap = 63;
		else if (_strcmpi(Map, "64") == 0 || _strcmpi(Map, "coliseum") == 0) RMap = 64;
		else
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM,"[GMove] Such map doesn't exist!");	
			return;
		} 

		if(x > 255 || y > 255 || x < 0 || y < 0)
		{	 								   
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM,"[GMove] Such coords doesn't exist!");	
			return;
		}  	

		gObjTeleport(gObj->m_Index, RMap, x, y);

		ConsoleOutPut(0,cLog.c_Green, cLog.t_GM,"[GMove] %s sucsessfully moved to %s %d %d!",gObj->Name, Map, x, y);
		Chat.Message(Index,"[GMove] Your moving to %s %d %d Admin", Map, x, y);
		return;
	}

	if (!strcmp(Command,"/online"))
	{
		int totGMs = Util.GetOnlineGMCount();
		int totPlayers = Util.GetOnlineCount() - totGMs;
		cLog.ConsoleOutPut(0, cLog.c_Blue, cLog.t_COMMANDS, "[ONLINE]: %d Player(s).",totPlayers);

		for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
		{  	 
			OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	 
			if(gObj->Connected < 3) continue; 	
			if(!GMS.IsGMBD(gObj->Name)) continue;			   
			if(GMS.IsAdmin(gObj->Name) == 1)
				cLog.ConsoleOutPut(0, cLog.c_Indigo,cLog.t_COMMANDS , "[ADMIN] %s", gObj->Name);
			else if(GMS.IsAdmin(gObj->Name) == 2)
				cLog.ConsoleOutPut(0, cLog.c_Indigo,cLog.t_COMMANDS , "[GM] %s", gObj->Name);
			else if(GMS.IsAdmin(gObj->Name) == 0) continue;
		}
		return;
	}

	if (!strcmp(Command,"/status"))
	{
		bool DigitsParam[] = {false,false};
		if (!CheckConsoleCommand("Status","/status <NickName>",Param,DigitsParam,CountSubBlock,1))
			return;

		char Target[11];
		strcpy(Target,Param[1]);

		int Index = Util.GetPlayerIndex(Target);   

		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

		ConsoleOutPut(0, cLog.c_Green, cLog.t_GM, "IP Address: %s",gObj->Ip_addr);
		ConsoleOutPut(0, cLog.c_Green, cLog.t_GM, "Account: %s | Character: %s",gObj->AccountID,gObj->Name);
		ConsoleOutPut(0, cLog.c_Green, cLog.t_GM, "Level: %d | Zen: %d",gObj->Level,gObj->Money);
		ConsoleOutPut(0, cLog.c_Green, cLog.t_GM, "Map: %s (%d)(%d,%d)",g_Happy.GetMapName(gObj->MapNumber),gObj->MapNumber,gObj->X,gObj->Y);

		if (AddTab[gObj->m_Index].IsMarried)
			ConsoleOutPut(0, cLog.c_Green, cLog.t_GM,  "Married on %s | Resets: %d", AddTab[gObj->m_Index].MarryName, AddTab[gObj->m_Index].Resets);
		else																								 
			ConsoleOutPut(0, cLog.c_Green, cLog.t_GM,  "Not married | Resets: %d", AddTab[gObj->m_Index].Resets);

		Chat.Message(gObj->m_Index, "[Status][Admin] Admin get your status!");	
		return;
	}

	if (!strcmp(Command,"/ismarry"))
	{
		bool DigitsParam[] = {false,false};
		if (!CheckConsoleCommand("Marry","/ismarry <NickName>",Param,DigitsParam,CountSubBlock,1))
			return;

		char Target[11];
		strcpy(Target,Param[1]);

		int Index = Util.GetPlayerIndex(Target);   

		OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

		if (AddTab[tObj->m_Index].IsMarried == 1)
			ConsoleOutPut(0, cLog.c_Blue, cLog.t_COMMANDS, "[Marry] %s is married!!!", tObj->Name);
		else
			ConsoleOutPut(0, cLog.c_Blue, cLog.t_COMMANDS, "[Marry] %s is not married!!!", tObj->Name);
		return;
	}

	if (!strcmp(Command,"/zendrop"))
	{
#ifdef _GameServer_
		bool DigitsParam[] = {false,true,true,true,true};

		if (!CheckConsoleCommand("ZenDrop","/zendrop <Map> <PozX> <PozY> <Value>",Param,DigitsParam,CountSubBlock,0))
			return;

		int Map = CharToInt(Param[1]);
		int X = CharToInt(Param[2]);
		int Y = CharToInt(Param[3]);
		DWORD Value = CharToInt(Param[4]);

		if(Value > 1000000000)
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_COMMANDS,"[ZenDrop] Max Value is 1000000000");
			return;
		}

		CMapClass__ZenDrop(Map, X, Y, Value);

		ConsoleOutPut(0, cLog.c_Cyan, cLog.t_COMMANDS,"[ZenDrop] In %s (%d:%d) Dropped %d Zen", g_Happy.GetMapName(Map),X,Y,Value);

		return;
#endif
#ifdef _GameServerCS_
		ConsoleOutPut(0, cLog.c_Red, cLog.t_COMMANDS,  "[ZenDrop] Command works only on Normal Server");
		return;
#endif
	}
	if (!strcmp(Command,"/mobadd"))
	{
#ifdef _GameServer_
		if(cProtection.CheckFeature(cProtection.MonsterSpawn))
		{
			bool DigitsParam[] = {false,true,true,true,true,true,true};
			if (!CheckConsoleCommand("AddMod","/mobadd <mobid> <count> <speed> <map> <x> <y>",Param,DigitsParam,CountSubBlock,0))
				return;

			int Mob = CharToInt(Param[1]);
			int Cnt = CharToInt(Param[2]);
			int Speed = CharToInt(Param[3]);
			int Map = CharToInt(Param[4]);
			int X = CharToInt(Param[5]);
			int Y = CharToInt(Param[6]);

			if (( Mob < 0 || Mob > 999) || (Cnt < 0 || Cnt > 100) || (Speed < 0 || Speed > 100) || (Map < 0 || Map > 64) || ( X < 0 || X > 255) || ( Y < 0 || Y > 255))
			{
				ConsoleOutPut(0, cLog.c_Red, cLog.t_COMMANDS,  "[AddMob] Use: /mobadd <mobid> <count> <speed> <map> <x> <y>");
				return;
			}

			FILE* AddMobFile;  

			if((AddMobFile = fopen( "..\\RMST\\MonsterSpawn.ini", "a+")) == NULL)
			{						   
				MessageBox(NULL, "Cant Find MonsterSpawn.ini", "Error", 0);
			}
			else
			{				
				fprintf(AddMobFile, "\n%d %d %d %d %d %d", Mob, Cnt, Speed, Map, X, Y); 	 
				fclose(AddMobFile);
			}		

			for(int i = 0; i < Cnt; i++)
			{
				MonsterAddAndSpawn(Mob,Speed,Map,X,Y);
			}

			ConsoleOutPut(0, cLog.c_Red, cLog.t_COMMANDS,  "[AddMob] %d Mob Successfully spawned (Map: %d, X: %d, Y: %d, MobID: %d)", Cnt,Map,X,Y, Mob);
		}
		else
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_COMMANDS,  "[AddMob] This is Private module");
		}
#endif
		return;
	}
	if (!strcmp(Command,"/banacc"))
	{
		bool DigitsParam[] = {false,false,false};
		if (!CheckConsoleCommand("BanAccount","/banacc <login> <nick>",Param,DigitsParam,CountSubBlock,0))
			return;

		char Target[11]; // account
		char Target2[11]; // nick
		strcpy(Target,Param[1]);
		strcpy(Target2,Param[2]);

		sscanf(Command, "%s %s", &Target, &Target2);
		int Index = Util.GetPlayerIndex(Target2); 
		OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);


		if(Index != -1)			
		{
			CloseClient(Index);									   
			ConsoleOutPut(0, cLog.c_Red, cLog.t_BAN,  "[BanAcc] %s banned, character %s disconnected.", Target, Target2);	
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET bloc_code='1' WHERE memb___id = '%s'",MySQL.szDatabase2,Target);	
		}
		else 
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET bloc_code='1' WHERE memb___id = '%s'",MySQL.szDatabase2,Target);
		return;

	}
	if (!strcmp(Command,"/unbanacc"))
	{
		bool DigitsParam[] = {false,false};
		if (!CheckConsoleCommand("UnBanAccount","/unbanacc <login>",Param,DigitsParam,CountSubBlock,0))
			return;

		char Target[11];  
		strcpy(Target,Param[1]);

		MySQL.Execute("SELECT bloc_code FROM [%s].[dbo].[MEMB_INFO] WHERE memb___id = '%s'",MySQL.szDatabase2, Target);	
		int Banned = MySQL.GetInt();

		if(Banned == 0)
		{																											   
			ConsoleOutPut(0, cLog.c_Red, cLog.t_BAN, "[UnBanAcc] %s is not baned", Target);
			return;
		}
		else   
		{		 																	  
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET bloc_code='0' WHERE memb___id = '%s'",MySQL.szDatabase2,Target);  
			ConsoleOutPut(0, cLog.c_Red, cLog.t_BAN, "[UnBanAcc] %s UnBaned", Target);		
		}
		return;
	}
	if (!strcmp(Command,"/setchar"))
	{
		bool DigitsParam[] = {false,false,true,true,true,true,true,true};
		if (!CheckConsoleCommand("SetChar","/setchar <Nick> <Zen> <PCPnt> <WCoins> <AddPnt> <lvl> <Prof>",Param,DigitsParam,CountSubBlock,1))
			return;

		DWORD Zen = CharToInt(Param[2]);
		DWORD PCPnt = CharToInt(Param[3]);
		DWORD WCoin = CharToInt(Param[4]);
		DWORD AddPnt = CharToInt(Param[5]);
		DWORD Lvl = CharToInt(Param[6]);
		DWORD Prof = CharToInt(Param[7]);

		char Target[11];
		strcpy(Target,Param[1]);

		if(Zen < 0 || Zen > 2000000000)
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetChar] Zen can't be less than 0 and more than 2000000000!");
			return;
		}	

		if(PCPnt < 0 || PCPnt > Config.MaximumPCPoints)
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetChar] PCPnt can't be less than 0 and more than %d!", Config.MaximumPCPoints);
			return;
		}	

		if(WCoin < 0 || WCoin > Config.MaximumWCPoints)
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetChar] WCoin can't be less than 0 and more than %d!", Config.MaximumWCPoints);
			return;
		}	

		int MaximumAddPnt;
		if(Config.Commands.MaxAddedStats == 0)MaximumAddPnt = 32000;
		else
			MaximumAddPnt = 65000;

		if(AddPnt < 0 || AddPnt > MaximumAddPnt)
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetChar] AddPnt can't be less than 0 and more than %d!", MaximumAddPnt);
			return;
		}	

		if(Lvl < 0 || Lvl > Config.Commands.MaxLvl)
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetChar] Lvl can't be less than 0 and more than %d!", Config.Commands.MaxLvl);
			return;
		}

		if(Prof < 0 || Prof > 3)
		{
			ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetChar] Lvl can't be less than 0 and more than 3!");
			return;
		}

		int Index = Util.GetPlayerIndex(Target); 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

		if(Zen > 0)
		{
			gObj->Money = Zen;
			GCMoneySend (gObj->m_Index, Zen);
		}

		if(PCPnt > 0 )
		{	
			g_PointShop.IncreasePoints(gObj->m_Index, PCPnt);
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - %d WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, PCPnt, gObj->Name);
		}

		if(WCoin > 0)
		{
			gObj->m_wCashPoint += WCoin;
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s + %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, WCoin, gObj->AccountID);
		}

		if(AddPnt > 0)
		{
			gObj->LevelUpPoint += AddPnt;
		}

		if(Lvl > 0)
		{
			gObj->Level	= Lvl;
		}

		if(Prof > 0)
		{
			switch(Prof)
			{
			case 0:
				break;
			case 1:
				if(gObj->DbClass >= 0 && gObj->DbClass <= 3)gObj->DbClass = 0;
				if(gObj->DbClass >= 16 && gObj->DbClass <= 19)gObj->DbClass = 16;
				if(gObj->DbClass >= 32 && gObj->DbClass <= 35)gObj->DbClass = 32;
				if(gObj->DbClass >= 48 && gObj->DbClass <= 50)gObj->DbClass = 48;
				if(gObj->DbClass >= 64 && gObj->DbClass <= 66)gObj->DbClass = 64;
				if(gObj->DbClass >= 80 && gObj->DbClass < 83)gObj->DbClass = 80;
				else
					break;
			case 2:
				if(gObj->DbClass >= 0 && gObj->DbClass <= 3)gObj->DbClass = 1;
				if(gObj->DbClass >= 16 && gObj->DbClass <= 19)gObj->DbClass = 17;
				if(gObj->DbClass >= 32 && gObj->DbClass <= 35)gObj->DbClass = 33;
				if(gObj->DbClass >= 80 && gObj->DbClass < 83)gObj->DbClass = 81;
				else
					break;
			case 3:
				if(gObj->DbClass >= 0 && gObj->DbClass <= 3)gObj->DbClass = 3;
				if(gObj->DbClass >= 16 && gObj->DbClass <= 19)gObj->DbClass = 19;
				if(gObj->DbClass >= 32 && gObj->DbClass <= 35)gObj->DbClass = 35;
				if(gObj->DbClass >= 48 && gObj->DbClass <= 50)gObj->DbClass = 50;
				if(gObj->DbClass >= 64 && gObj->DbClass <= 66)gObj->DbClass = 66;
				if(gObj->DbClass >= 80 && gObj->DbClass < 83)gObj->DbClass = 83;
				else
					break;
			}
		}
		ConsoleOutPut(0, cLog.c_Red, cLog.t_GM, "[SetChar] You sucsessfully changed %s character.", gObj->Name);
		Chat.Message(gObj->m_Index,"[SetChar] Your character was edited by Admin, you must relogin!");
		return;
	}
	
	if (!strcmp(Command,"/help"))
	{
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Clean Console Windows - /clean or /cls");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Disconnect Char - /disconnect");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Ban Character - /banchar");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"UnBan Character - /unbanchar");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Gold Message for Admin - /gg");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Reload Configs - /reload");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Server Time - /time");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Evo Command (Getting 3rd Proffesion) - /evo");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Set Zen on Character - /setzen");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Set PK on Character - /setpk");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Clear PK on Character - /pkclear");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Change Skin on Character - /skin");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Drop Item - /drop");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Drop Set - /setdrop");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"GM Teleport Character - /gmove");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Seeing online server - /online");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Seeing status character - /status");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Cheking marry character - /ismarry");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Drop Zen - /zendrop");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Add Mob in location - /mobadd");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Ban account - /banacc");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"UnBan account - /unbanacc"); 
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Change character - /setchar");
		ConsoleOutPut(0,cLog.c_Magenta,cLog.t_COMMANDS,"Write in post - /post");

		ConsoleOutPut(0,cLog.c_Red,cLog.t_COMMANDS,"\r\n For seeing syntaxic command, write command without parametrs");
		return;
	}
	
	ConsoleOutPut(0,cLog.c_Red,cLog.t_COMMANDS,"<Unkown> %s Command.For Please Entering '/help' for seeing help!",Command);
}

bool Logger::GetArraySubChar(char* InPutMsg,int &CountSubChar,char ParamChar)
{
	CountSubChar = 0;

	for (unsigned int i = strlen(InPutMsg) - 1; i >= 0; i--)
		if (InPutMsg[i] == ParamChar) InPutMsg[i] = '\0';
		else break;

	for (unsigned int i=0; i<strlen(InPutMsg); i++)
		if (InPutMsg[i] == ParamChar && InPutMsg[i-1] != ParamChar) ++CountSubChar;

	if (CountSubChar == 0) return false;

	Param = new char *[CountSubChar + 1];

	char *zBuffer = new char[strlen(InPutMsg)];

	if (Param == NULL || zBuffer == NULL) return false;

	int CounterzBuf = 0;
	int CounterWords = 0;

	for (unsigned int i = 0; i < strlen(InPutMsg); i++)
	{
		if (InPutMsg[i] != ParamChar)
		{
			zBuffer[CounterzBuf] = InPutMsg[i];
			CounterzBuf++;
			zBuffer[CounterzBuf] = '\0';
		}

		if ((InPutMsg[i] == ParamChar && InPutMsg[i-1] != ParamChar) || i == strlen(InPutMsg) - 1)
		{
			Param[CounterWords] = new char [strlen(zBuffer)];
			strcpy(Param[CounterWords],zBuffer);
			CounterzBuf = 0;
			CounterWords++;
		}
	}

	delete[] zBuffer;

	return true;
}

bool Logger::CheckConsoleCommand(char* CommandHeader,char* CommandSyntax,char** ParamArray,bool Digits[],int CountSubString,int CheckPlayer)
{
	int NeedSpace = 0;

	for(int i = 0; i < strlen(CommandSyntax); i++)
		if(CommandSyntax[i]==' ' && CommandSyntax[i-1]!=' ')	NeedSpace++;

	if (NeedSpace > CountSubString)
	{
		ConsoleOutPut(0, cLog.c_Red, cLog.t_COMMANDS, "[%s] Usage: %s", CommandHeader, CommandSyntax);
		return false;
	}

	bool bRez = true;

	for (int i=0; i<NeedSpace + 1; i++)
		if (!IsDigitsChar(ParamArray[i]) && Digits[i]  ) 
			bRez = false;

	if (!bRez)
	{
		ConsoleOutPut(0, cLog.c_Red, cLog.t_COMMANDS, "[%s] Usage: %s", CommandHeader, CommandSyntax);
		return false;
	}


	if (CheckPlayer > 0)
	{
		int aIndex = Util.GetPlayerIndex(ParamArray[1]);

		if (aIndex == -1)
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL, "[%s] %s player offline or doesn't exist!",CommandHeader,ParamArray[1]);
			return false;
		}
	}

	return true;
}

bool Logger::IsDigitsChar(char* inmsg)
{
	for (unsigned int i = 0; i < strlen(inmsg); i++)
	{
		if (i == 0 && inmsg[i] == '-') continue;
		if (!isdigit(inmsg[i])) return false;
	}

	return true;
}

long int Logger::CharToInt(char* inmsg)
{
	long int RetNumber = -1;

	if (IsDigitsChar(inmsg)) sscanf(inmsg,"%d",&RetNumber);

	return RetNumber;
}

void Logger::CheckProcent(char* message)
{
	for (int i = 0; i <= strlen(message); i++)
	{                          						   
		if(message[i] == '%')		  
		{
			for(int j = strlen(message); j >= i; j--)		  
				message[j+1] = message[j];
			i++;
		}
	}
}

void Logger::ConsoleOutPut(int WOL, sColor Color, sLogType Type, const char* Format, ...)
{					
	SYSTEMTIME t;
	GetLocalTime(&t);
	DWORD dwBytesWritten;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	va_list pArguments;
	va_start(pArguments, Format);
	vsprintf_s(Message,Format, pArguments);
	CheckProcent(Message); // "%" Bug Fix 
	va_end(pArguments);

	char currdate[11] = {0};
	char outputmsg[2048];
	if(WOL == 1)
	{
		sprintf_s(currdate, "(%02d:%02d:%02d)", t.wHour, t.wMinute, t.wSecond);
		sprintf_s(outputmsg,"%s %s\n", currdate,Message);
	}
	else
		sprintf_s(outputmsg,"%s\n", Message);
	


	switch(Color)
	{
	case c_Red: 
		SetConsoleTextAttribute(this->Handle(FALSE),FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case c_Green: 
		SetConsoleTextAttribute(this->Handle(FALSE),FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case c_Blue: 
		SetConsoleTextAttribute(this->Handle(FALSE),FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case c_Cyan: 
		SetConsoleTextAttribute(this->Handle(FALSE),FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case c_Yellow: 
		SetConsoleTextAttribute(this->Handle(FALSE),FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case c_Magenta: 
		SetConsoleTextAttribute(this->Handle(FALSE),FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case c_Grey:
		SetConsoleTextAttribute(this->Handle(FALSE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	} 

	CreateLog(Type,outputmsg); 
	WriteFile(this->Handle(FALSE), outputmsg, strlen(outputmsg), &dwBytesWritten, NULL);
	SetConsoleTextAttribute(this->Handle(FALSE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return;
}

HANDLE Logger::Handle(BOOL Input)
{
	if(Input==TRUE)
		return GetStdHandle(STD_INPUT_HANDLE);
	else
		return GetStdHandle(STD_OUTPUT_HANDLE);
}

void Logger::CreateLog(sLogType Type,const char* Format, ...)
{
	SYSTEMTIME now;
	GetLocalTime(&now);  
																														 
	char ConsoleLog[55];
	char GMLog[55];
	char CommandsLog[55]; 
	char GPostLog[55];	
	char PostLog[55];	
	char DropLog[55];	  
	char BanLog[55];
	char DuelLog[55];				  
	char PcPointLog[55];	
	char IpBlockLog[55];	
	char ExchangeLog[55];
	char ResetLog[55];			  
	char MarryLog[55];
	char VIPLog[55];
	char ErrorLog[55];
	char Date[55];
	sprintf(Date, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\", now.wDay, now.wMonth, now.wYear); 
	CreateDirectory(Date,NULL);
																													  
	sprintf(CommandsLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\Commands.log", now.wDay, now.wMonth, now.wYear); 
	sprintf(ConsoleLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\CONSOLE.log", now.wDay, now.wMonth, now.wYear);
	sprintf(GPostLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\[GPOST].log", now.wDay, now.wMonth, now.wYear);
	sprintf(PostLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\[POST].log", now.wDay, now.wMonth, now.wYear); 
	sprintf(ResetLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\Reset.log", now.wDay, now.wMonth, now.wYear);
	sprintf(DropLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\DROP.log", now.wDay, now.wMonth, now.wYear);
	sprintf(BanLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\BAN.log", now.wDay, now.wMonth, now.wYear);			 
	sprintf(GMLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\GM.log", now.wDay, now.wMonth, now.wYear);
	sprintf(DuelLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\DuelManager.log", now.wDay, now.wMonth, now.wYear);
	sprintf(PcPointLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\PCPoint.log", now.wDay, now.wMonth, now.wYear);		   
	sprintf(IpBlockLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\IpBlock.log", now.wDay, now.wMonth, now.wYear);		
	sprintf(ExchangeLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\Exchange.log", now.wDay, now.wMonth, now.wYear);
	sprintf(MarryLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\Marry.log", now.wDay, now.wMonth, now.wYear);
	sprintf(VIPLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\VIP.log", now.wDay, now.wMonth, now.wYear);
	sprintf(ErrorLog, "..\\RMST\\ConsoleLogs\\%02d-%02d-%02d\\Error.log", now.wDay, now.wMonth, now.wYear);

	va_list pArguments1;
	va_start(pArguments1, Format);
	vsprintf_s(Message,Format, pArguments1);
	va_end(pArguments1);

	switch (Type)
	{
		case t_NULL:
		break;

		case t_Error:
		{
		SaveFile(ErrorLog, Message);
		}
		break;
		case t_Default: 
		{
			SaveFile(ConsoleLog,Message);
		}
		break; 		
		case t_MARRY:
		{				   				   
			SaveFile(ConsoleLog,Message);
			SaveFile(MarryLog,Message);
		}
		break;
		case t_RESET:
		{	 
			SaveFile(ConsoleLog,Message);
			SaveFile(ResetLog,Message);	
			SaveFile(CommandsLog,Message);
		}
		break;
		case t_EXCHANGE:
		{		
			SaveFile(ConsoleLog,Message);
			SaveFile(PcPointLog,Message);	
			SaveFile(CommandsLog,Message);
		}
		break;
		case t_PCPOINT:
		{
			SaveFile(ConsoleLog,Message);
			SaveFile(PcPointLog,Message);
		}
		break;
		case t_IPBLOCK:
		{
			SaveFile(ConsoleLog,Message);
			SaveFile(IpBlockLog,Message);
		}
		break;
		case t_GM:
		{
			SaveFile(ConsoleLog,Message);
			SaveFile(GMLog,Message);
		}
		break;
		case t_COMMANDS:
		{									
			SaveFile(ConsoleLog,Message);
			SaveFile(CommandsLog,Message);
		}
		break;
		case t_POST: 
		{								 
			SaveFile(ConsoleLog,Message);
			SaveFile(PostLog,Message);
			SaveFile(CommandsLog,Message);
		}
		break;	
		case t_GPOST:
		{		
			SaveFile(ConsoleLog,Message);
			SaveFile(GPostLog,Message);
			SaveFile(CommandsLog,Message);	 
			SaveFile(GMLog,Message);
		}
		break;
		case t_DROP: 
		{										   
			SaveFile(ConsoleLog,Message);
			SaveFile(DropLog,Message);
			SaveFile(CommandsLog,Message);
			SaveFile(GMLog,Message);
		}
		break;
		case t_BAN: 
		{									   
			SaveFile(ConsoleLog,Message);
			SaveFile(BanLog,Message);
			SaveFile(CommandsLog,Message);
			SaveFile(GMLog,Message);
		}
		break;
		case t_Duel: 
		{									   
			SaveFile(ConsoleLog,Message);
			SaveFile(DuelLog,Message);
		}
		break;
		case t_VIP:
		{
			SaveFile(VIPLog, Message);
			SaveFile(ConsoleLog,Message);
		}
		break;
	}
}

void Logger::SaveFile(char *logString,char *Message)
{
	FILE *stream;  
	stream=fopen(logString, "a+" );
	fprintf(stream, "%s", Message);
	fclose(stream);
}