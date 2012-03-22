// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#ifndef PROTECT_H
#define PROTECT_H


class Protection
{
public:		
	Protection();
	~Protection();  

	struct CColumn
	{
		char m_Mac[18];
		char* m_Name;	  
		int m_Features[12];	  
	};
						
	int Local_Id; 
	enum Features{ Credits_Removal, Localization, VIPSystem, MonsterSpawn, AntiFoul, QuestSys, MaxStats, Console, MeSQL, MapSys, OffTrade, Season5};	 

	void Load();
	bool CheckFeature(Features);  
	char* CheckName();
};

extern Protection cProtection;

#endif