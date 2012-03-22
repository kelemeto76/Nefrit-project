// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //
					  	
#include "StdAfx.h"
#include "Localize.h"
#include "ConfigDefines.h"
#include "Configs.h"
#include "Protection.h"
#include "Console.h"

Localization cLocalization;

Localization::Localization()
{
}

Localization::~Localization()
{							   
}			 
char* Localization::Localize(char *pString)
{		
	if(!cProtection.CheckFeature(cProtection.Localization))
		return pString;

	m_FileEmpty = false;
														
	char TempFolder[55];	
	wsprintf(TempFolder, "..\\RMST\\Translations\\%s", Config.LocalizationFile);  

	if((m_File = fopen( TempFolder, "r")) == NULL || Config.IsLocalization == 0)
		m_FileEmpty = true;					 

	if(m_FileEmpty)
		return pString;
								
    rewind(m_File);
	char Buff[255];
	bool Found = false;			
    while(!feof(m_File))
    {							
		Buff[0] = 0;
		fgets (Buff, 255, m_File);	 
		if(Buff[0] != '"' && Buff[0] != '=')
            continue;										
						  						   		   
		//if(Found && Buff[0] == '=' && Buff[1] == '=' && Buff[2] == ' ')
		//{
		//	//Buff += 3;   
		//	for(int x = 3; x <= sizeof(Buff); x++)
		//	{
		//		Buff[x-3] = Buff[x];
		//		Buff[x] = ' ';
		//	}
		//	rewind(m_File);		
		//	fclose(m_File);  
		//	return Buff;
		//}		

		if(Found && Buff[0] == '=' && Buff[1] == '=' && Buff[2] == ' ' && Buff[3] == '"')  
		{
			char Temp[255];
			sscanf(Buff, "== \"%255[^\"]\"",Temp);
			rewind(m_File);		
			fclose(m_File);  
			if(Buff[0] == '=' && Buff[1] == '=' && Buff[2] == ' ' && Buff[3] == '"' && Buff[4] == '"')
				return pString;
			return Temp;
		}
						
		char Temp1[255];	
		sscanf(Buff, "\"%255[^\"]\"",Temp1);						  
		if(strncmp(pString, Temp1, strlen(pString)) == 0)	 
			Found = true;			 				 
	}	

    rewind(m_File);
	fclose(m_File);
	return pString;
}						