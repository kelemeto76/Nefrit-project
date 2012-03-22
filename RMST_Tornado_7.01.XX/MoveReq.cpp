#include "StdAfx.h"
#include "MoveReq.h"
#include "User.h"
#include "ConfigDefines.h"
#include "Console.h"
cMoveReq MoveReq;

cMoveReq::cMoveReq()
{

}

cMoveReq::~cMoveReq()
{

}
//TODO: Убрать чувствительность к регистру, при написании /move lorencia не портит.	   done
//TODO2: Добавить имена карт в Ковычки "Lorencia"								done
//TODO3: Вывод в консоль если в MoveReq стоит VIP и проверка на VIP не прошла	done
void cMoveReq::MoveReqLoad()
{
	FILE *fp;
	BOOL bRead = FALSE;
	DWORD dwArgv = 0;
	char sLineTxt[255] = {0};
	int MoveReqCount = 1;

	fp = fopen("..\\RMST\\MoveReq.txt","r");

	if(!fp)
	{
		MessageBoxA(NULL, "Cant Find MoveReq.txt!", "Error!", MB_OK);
		::ExitProcess(0);
	}
	
	rewind(fp);
	int leestatus=-1;
	
	while(fgets(sLineTxt, 255, fp) != NULL)
	{
		if(sLineTxt[0] == '/')continue;
		if(sLineTxt[0] == ';')continue;

		int n[5];
		char Name[50], Name2[50];

		sscanf(sLineTxt, "%d %s %s %d %d %d %d", &n[0], &Name, &Name2, &n[1], &n[2], &n[3], &n[4]);

		for(int j=0; j <= strlen(Name); j++)
		{
			if(Name[j] == '"')
			{
				for(int k=j; k<=strlen(Name); k++)
				{
					Name[k] = Name[k+1];
				}
			}
		}

		for(int j=0; j <= strlen(Name2); j++)
		{
			if(Name2[j] == '"')
			{
				for(int k=j; k<=strlen(Name2); k++)
				{
					Name2[k] = Name2[k+1];
				}
			}
		}

		MoveReqInfo[MoveReqCount].Index	= n[0];				 
		strcpy(MoveReqInfo[MoveReqCount].MapName1, Name);
		strcpy(MoveReqInfo[MoveReqCount].MapName2, Name2);
		MoveReqInfo[MoveReqCount].Zen	= n[1];
		MoveReqInfo[MoveReqCount].Level	= n[2];
		MoveReqInfo[MoveReqCount].Gate	= n[3];
		MoveReqInfo[MoveReqCount].Vip	= n[4];
		MoveReqCount++;
	}

	Count = MoveReqCount;
	rewind(fp);
	fclose(fp);
	
	return;
}
/*
void cMoveReq::MoveProtocol(DWORD aIndex,int mIndex)
{
	if(MoveReqInfo[mIndex].Index != mIndex)return;
	OBJECTSTRUCT *pObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
	DWORD ZenActual;
	ZenActual = (DWORD)pObj->Money;
	if(ZenActual < (DWORD)MoveReqInfo[mIndex].Zen)return;
	DWORD ZenDec = ZenActual - (DWORD)MoveReqInfo[mIndex].Zen;
	
	if(MoveReq.MoveReqInfo[mIndex].Vip != AddTab[pObj->m_Index].IsVipInGame)
	{
		
			GCServerMsgStringSend("[Move] Sorry but map is for V.I.P.",aIndex,1);
			return;
	} 
	if(pObj->m_PK_Level > 4)
	{
			GCServerMsgStringSend("[Move] Fonomans can't move",aIndex,1);
			return;
	}
	pObj->Money = ZenDec;
	GCMoneySend (aIndex,ZenDec);
	gObjMoveGate(aIndex,MoveReqInfo[mIndex].Gate); 
}
*/