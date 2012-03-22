// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //
#include "ConfigDefines.h"
#ifndef CONFIGS_H
#define CONFIGS_H

class cConfigs
{
public:			 
	cConfigs();
	~cConfigs();
	void LoadFixes();	
	void LoadConfigsInGS(); // загружает конфиги из файлов в оффсеты ГС
	void LoadAll();
	long GetInt(long Min, long Max, long Default, LPCSTR BlockName, LPCSTR ConfigName, LPCSTR FolderName);
	char GetChar(long Min, long Max, unsigned char Default, LPCSTR BlockName, LPCSTR ConfigName, LPCSTR FolderName);

	void LoadDuel();										 
	void LoadAntiAfk();		
	void ZenFixes();
	void PartyZenFixes();
	void LoadPoints(); 
	void LoadCommands();
	void LoadNotice(); 
	void CmdSyntax(); 
	void LoadPkClearGuard();
	void LoadGmSystem();
	void LoadPets();
	void LoadArcher();
	void LoadMoss();  
	void LoadNews(); 
	void LoadReset();
	void LoadHappyHour();
	void LoadMarry();
	void VIPServer();
	void LoadAntiFoul(); 
	void MaxStats();

	/*For Happy Hour*/
	int HappyHourEnabled;
	struct
	{
		int Enabled;
		int AmountWarning;
		char FilterText[58];
		char FilterTextPost[58];
		int FineZenCost;
		int DoubleCostZen;
		int ModeBan;
		int TimeBanMin;
	}ConfAntiFoul;
	struct  
	{
		int MapNum;
		int Working;
		int PlusExp;
		int PlusZen;
		int Hour;
		int Min;
		int Delay;
		int CloseHour;
		int CloseMin;
	}g_Conf[128];

	/* For Duel */
	struct sDuel
	{
		int Enabled;
		int Ranking;
		int Logging;
	}Duel;

	//
	#ifdef _GameServer_
	int EldaradoEventType;
	int CCAllowingPlayers;
	int BCAllowingPlayers;
	int BCEnterMasterType;
	int DSAllowingPlayers;
	int CashShopExe;
	#endif

	int UseChecksum;	  
	int PersonalIDFix;	 
	int GuildIDFix;
	int DisableLogs;
	int AllowExeAnc;
	int AllowJohAnc;
	int AllowExeSock;
	int MaxLifeOpt;
	int Use65kStats;
	int CSSkillsOnNOrmal;
	int Enable65kStats;
	int DumpFile;

	int IsGMSystem;

	/* For Commands */
	struct sCommands
	{
		int MaxLvl;

		int IsGg;
		int IsPost;
		int PostLvl;
		DWORD MaxAddedStats;
		DWORD PostPriceZen;	
		int PostPricePCPoint;
		int PostPriceWCoin;
		int PostColor;
		int MassPostDelay;

		int IsEvo;
		int EvoLvlReq;
		DWORD EvoPriceZen;
		int EvoPricePcPoint;
		int EvoPriceWCoin;
				   
		int IsBanPost;
		int IsBanAcc;
		int IsBanChar;
		int IsDrop;
		int IsGmove;
		int IsSetZen;
		int IsSetPK;
		int IsTime;
		int IsOnline;
		int IsMarry;
		int IsZenDrop;
		int IsStatus;
		int IsSetChar;
		int IsMobSpawn;
		int MarryOnline;
		int MarryTrace;
		int MarryTraceLvl;

		int IsMultyVault;
		int NumberOfVaults;	   
		DWORD ZenForChange;
		int PcPointForChange;
		int WCoinForChange;

		int SkinEnabled;
		int SkinOnlyForGm;
		int SkinLevelReq;
		DWORD SkinPriceZen;
		int SkinPricePcPoint;
		int SkinPriceWCoin;

		int AddPointEnabled;
		int AddPointLevelReq;  
		DWORD AddPriceZen; 
		int AddPricePCPoint;
		int AddPriceWCoin;
	}Commands;

	/* For PkClear */
	struct sPkClear
	{
		int Enabled;
		int NpcId;	  
		int OnlyForGm;
		int Type;	
		DWORD PriceZen;
		DWORD PriceZenForAll;	  
		int PricePcPoints;
		int PricePcPointsForAll;	   
		int PriceWCoins;
		int PriceWCoinsForAll;
		int LevelReq;
	};	

	sPkClear ClearNpc;			 
	sPkClear ClearCommand;

	int IsPartyGap;
	int PartyGapLvl;
	int GuildRes;
	int GuildLevel;

	int PartyZenDiv;
	int ZenDiv;
	
	int ExchangeCommands;
	int	ExchangeWcPp;
	int	ExchangeWcZen;
	int	ExchangePpZen;
	int	ExchangePpWc;
	int	ExchangeZenWc;
	int	ExchangeZenPp;
	int ExchangeMinLvl;
	int ExchangeTax;
	int WCoin_PcPoint;
	DWORD WCoin_Zen;
	DWORD PcPoint_Zen;

	struct cZen
	{
		int	Enabled;
		int MasterDivisor;
		int NormalDivisor;
		int	MasterZen;
		int NormalZen;

	}Zen;
	
	struct cParty
	{
		int	Enabled;
		int MasterDivisor;
		int NormalDivisor;
		int	MasterZen;
		int NormalZen;
	}PartyZen;
	int PCPointsSystem;
	int AddPCPointsSec;
	int AddPCPointsCount;
	int MaximumPCPoints;
	int MaximumWCPoints;
	char PCPointsColumn[25];
	char WCoinsColumn[25];		 
	int IsNews;
	char ConnectNotice[100];	
	int ConnectInfo;

	char ResetColumn[25];			
	int IsResetSystem;
	int ResetNpcNumber;

	struct sSyntax
	{
	char GgSyntax[25];
	char PostSyntax[25];
	char BanPostSyntax[25];
	char UnBanPostSytax[25];
	char BanCharSyntax[25];
	char UnBanCharSyntax[25];
	char BanAccSyntax[25];
	char UnBanAccSyntax[25];
	char TimeSyntax[25];
	char EvoSyntax[25];
	char WcPpSyntax[25];
	char WcZenSyntax[25];
	char PpZenSyntax[25];
	char PpWcSyntax[25];
	char ZenWcSyntax[25];
	char ZenPpSyntax[25];
	char YesSyntax[25];
	char NoSyntax[25];
	char AddStrSyntax[25];
	char AddVitSyntax[25];
	char AddEneSyntax[25];
	char AddAgiSyntax[25];
	char AddCmdSyntax[25];
	char SetZenSyntax[25];
	char SetCharSyntax[25];
	char SetPKSyntax[25];
	char PKClearSyntax[25];
	char ResetSyntax[25];
	char SkinSyntax[25];
	char DropSyntax[25];
	char GmoveSyntax[25];
	char WareSyntax[25];
	char OnlineSyntax[25];
	char StatusSyntax[25];
	char ReloadSyntax[25];
	char MarryOnlineSyntax[25];
	char MarryTraceSyntax[25];
	char IsMarrySyntax[25];
	char DivorceSyntax[25];
	char ZenDropSyntax[25];
	char LevelSyntax[25];
	char MobAddSyntax[25];
	char BuyVIPServSyntax[25];
	char CheckVIPSyntax[25];
	}SyntaxCmd;
		
	/* For Panda */
	struct sPanda
	{
		int PetPandaDefense;
		int PetPandaAttackDamageMinLeft;
		int PetPandaAttackDamageMaxLeft;
		int PetPandaAttackDamageMinRight;
		int PetPandaAttackDamageMaxRight;
		int PetPandaMagicDamageMin;
		int PetPandaMagicDamageMax;
		int PetPandaMagicSpeed;
		int PetPandaAttackSpeed;
		int PetPandaExpirence;
		int PetPandaMLExpirence;

		int PandaRingDefense;
		int PandaRingAttackDamageMinLeft;
		int PandaRingAttackDamageMaxLeft;
		int PandaRingAttackDamageMinRight;
		int PandaRingAttackDamageMaxRight;
		int PandaRingMagicDamageMin;
		int PandaRingMagicDamageMax;
		int PandaRingMagicSpeed;
		int PandaRingAttackSpeed;
		int PandaRingExpirence;
		int PandaRingMLExpirence;
	}Panda;

	int MaxDL_Strength ;
	int MaxDL_Agility;
	int MaxDL_Vitality;
	int MaxDL_Energy ;
	int MaxDL_Command;

	int MaxDK_Strength;
	int MaxDK_Agility;
	int MaxDK_Vitality ;
	int MaxDK_Energy ;

	int MaxDW_Strength ;
	int MaxDW_Agility;
	int MaxDW_Vitality;
	int MaxDW_Energy ;

	int MaxELF_Strength ;
	int MaxELF_Agility ;
	int MaxELF_Vitality ;
	int MaxELF_Energy;

	int MaxSUM_Strength;
	int MaxSUM_Agility ;
	int MaxSUM_Vitality;
	int MaxSUM_Energy;

	int MaxMG_Strength ;
	int MaxMG_Agility ;
	int MaxMG_Vitality ;
	int MaxMG_Energy;

	/* For antiafk */
	struct sAntiAfk
	{
		int Enabled;
		int Time;
		int Warnings;
	}AntiAfk;

	/* For archer */
	struct sArcher
	{
		int Enabled;
		int ZenReward;
		int WCoinsReward;
		int PCPointsReward;
		int MaxExcOpt;
		int NeedRenaAmount;
		int ModeWorkGA;  
	}Archer;	  

	/*For marry*/ 
	struct SItems
	{
		unsigned long Serial;
		int Group;
		short Index;
		int Type;	
		int Lvl;
		int Skill;
		int Luck;
		int Opt;
		int Exc;
		int Num;
	};

	struct sMarry
	{
		int MarrySystemEnabled;
		int MarryHomoSexual;
		int MarryCostZen;
		int MarryCostPCPoint;
		int MarryCostWCoin;
		int MarryMinLvl;
		int MarryNpcNum;
		int MarryTrace;
		int MarryOnline;
		int MarryExpBonus;
		int NumWitnesses;

		int IsDivorce;
		//int DivorceNpcNum;
		int DivorceCostZen;
		int DivorceCostPCPoint;
		int DivorceCostWCoin;
		int DivorceMinLvl;

		int TakeItemsForMarry;
		//int TakeItemsForDivorce;
								
		SItems MarryItems[20];	
		//SItems DivorceItems[20];	
						  
		bool MarrySerial[76];
		//bool DivorceSerial[76];

	}Marry;

	struct sVipServer
	{
	int Enabled;
	int EnabledCmd;
	char Column[25];
	char ColumnDate[25];
	char VIPState[25];
	DWORD CostZen;
	int CostPCPoints;
	int CostWCoins;
	}VIPServ;

	/*For moss*/
	struct sMoss
	{
		int Enabled;
		int EnabledTimer;
		int NeedZen;
		int NeedPCPnt;
		int NeedWCn;
		int UsePkPlayers;
		int RateLevelItem1;		
		int RateLevelItem7;	
		int RateLevelItem10;
		int RateLevelItem11;
		int RateLevelItem12;		
		int RateLevelItem13;
		int RateLuck;
		int RateSkill;	
		int RateAddOpt;
		int RateAddOpt1;
		int RateAddOpt2;
		int RateExcOpt;
		int RateExcOptOne;
		int RateExcOptTwo;
		int RateExcOptTree;
		int RateExcOptFour;
		int RateExcOptFive;
		int RateExcOptSix;
	}Moss;

	int IsLocalization;
	char LocalizationFile[25];
};
extern cConfigs Config;

#endif