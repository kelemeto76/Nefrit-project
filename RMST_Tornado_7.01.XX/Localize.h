// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#ifndef LOCALIZE_H
#define LOCALIZE_H


class Localization
{
public:		
	Localization();
	~Localization();  
	
	FILE* m_File;
	bool m_FileEmpty; 
	char* Localize(char *pString);

};

extern Localization cLocalization;

#endif