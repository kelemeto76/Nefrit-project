#ifndef MARRY_H
#define MARRY_H

#include	"User.h"

unsigned long __stdcall TickMarriage(int Index);	 
unsigned long __stdcall FireMarriage();
class Marry
{
public:
	Marry();
	~Marry();

	bool StartMarriage(LPOBJ gObj, LPOBJ nObj);
	void EndMarriage();		   			
	//	void EndDivorce();		   					 
	void EndMarriage2(int Index);
	//	void EndDivorce2(int Index);
	void MarryFirework(int x, int y, int map, LPOBJ nObj);

	bool CheckMarryItem(LPOBJ gObj);
	void CheckMarryItemSerial(LPOBJ gObj,BYTE);  
	//	bool CheckDivorceItem(LPOBJ gObj);
	//	void CheckDivorceItemSerial(LPOBJ gObj,BYTE);

	bool CheckMarryCost(LPOBJ gObj, LPOBJ nObj, int Delete);
	//	bool CheckDivorceCost(LPOBJ gObj, LPOBJ nObj, int Delete);

	bool CheckGender(LPOBJ gObj, LPOBJ nObj);				

	bool NpcUse;
	int fIndex;
	LPOBJ NpcObj;			
	LPOBJ gObj1, gObj2;

	//...//				
};

extern Marry cMarry;

#endif