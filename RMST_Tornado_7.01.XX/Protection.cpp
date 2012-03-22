// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "Protection.h"	
#include "StdAfx.h"		    
Protection cProtection;	   

//MAC						//Name		//CreditsRem //Localization	//VIP //MobSpawn	//AntiFoul	//QuestSytem	//MaxStats		//Console		//MeSQL			//MapSys		//OffTrade		//Season 5
Protection::CColumn Clients[] = {					  
	{"00-1A-92-7C-5D-CA", "DeadMan",		{1,			0,			1,		1,			0,			1,				0,				1,				1,				0,				0,				0}}, //RMST
};	 

Protection::Protection()
{
}

Protection::~Protection()
{							   
}				 

bool Protection::CheckFeature(Features Feat)
{
	if(Local_Id == -1)
		return false;
	else if(Clients[Local_Id].m_Features[Feat] == 1)
		return true;
	return false;
}

char* Protection::CheckName()
{
	if(Local_Id == -1)
		return NULL;
	return Clients[Local_Id].m_Name;
}

void Protection::Load()
{			
	Local_Id = -1;

	unsigned char MACData[6];
	UUID uuid;			     
	UuidCreateSequential( &uuid );	
	for (int U = 2 ; U < 8 ; U++)
		MACData[U - 2] = uuid.Data4[U];

	char Local_Mac[18];
	wsprintf(Local_Mac, "%02X-%02X-%02X-%02X-%02X-%02X", MACData[0], MACData[1], MACData[2], MACData[3], MACData[4], MACData[5]);

	//	char szTemp[255];	
	for(int x = 0 ; x < (sizeof Clients / sizeof Clients[0]) ; x++)
	{
		if(!strcmp (Local_Mac , Clients[x].m_Mac))
		{
			//	wsprintf(szTemp,"Files are licensed to : %s", Clients[x].m_Name);
			//	MessageBox(NULL, szTemp,"RMST Files License",MB_OK);
			Local_Id = x;
		}
	}
}						