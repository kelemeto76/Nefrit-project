// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "Prodef.h"
#ifndef MAIN_H
#define MAIN_H
			  
unsigned long __stdcall  LoadGS();	
unsigned long __stdcall  MainTick();			

class CMain
{
public:
	CMain();
	~CMain();
	void Init();
};
extern CMain Main;


#endif