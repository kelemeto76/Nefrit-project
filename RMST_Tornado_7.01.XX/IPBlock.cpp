// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "stdafx.h"
#include "ConfigDefines.h"
#include "User.h"
#include "Main.h"
#include "IPBlock.h"
#include "Console.h"
					  
IpBlock IP;

IpBlock::IpBlock()
{		  
}
IpBlock::~IpBlock()
{		  
}

void IpBlock::LoadIpBlock()
{
	FILE *fp;
	BOOL bRead = FALSE;
	DWORD dwArgv = 0;
	char sLineTxt[255] = {0};
	IPBlockCount = 1;

	fp = fopen(RMSTIPBlock,"r");

	rewind(fp);
	
	while(fgets(sLineTxt, 255, fp) != NULL)
	{
		if(sLineTxt[0] == '/')continue;			
		if(sLineTxt[0] == ';')continue;
		if(sLineTxt[0] == 'e' && sLineTxt[1] == 'n' && sLineTxt[2] == 'd')continue;
		char IP[16];							
		if(strlen(sLineTxt) <= 1)continue;
								  
		sscanf(sLineTxt, "%s", &IP);
		sprintf(IPBlockInfo[IPBlockCount].IP,"%s",IP);
		IPBlockCount++;
	}

	rewind(fp);
	fclose(fp);
}

bool IpBlock::CheckIp(DWORD aIndex)
{
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
	for(int x=1;x < IPBlockCount;x++)
	{
		if(!CheckIp2(IPBlockInfo[x].IP, gObj->Ip_addr))
		{					
			cLog.ConsoleOutPut(1, cLog.c_Red, cLog.t_IPBLOCK,"[Anti-Hack] Blocked, %s try to connect!!!",gObj->Ip_addr);
			CloseClient(aIndex);
			return false;
		}
	}
	return true;
}
						  
void changestar(char*IP,int i)
{
	for(int j=strlen(IP); j>=i; j--)
		IP[j+1] = IP[j];
	IP[i] = '-';
	IP[i+1] = '1';
}

bool IpBlock::CheckIp2(char ConfigIP[16], char GobjIP[16])
{							   
	for(int i=0; i<16;i++)
	{							  
		if(ConfigIP[i] == '.')
			ConfigIP[i] = ' ';
		if(GobjIP[i] == '.')
			GobjIP[i] = ' ';
									 
		if(ConfigIP[i] == '*')
			changestar(ConfigIP, i); 
	}

	int CIP[4];
	int GIP[4];
																	  
	sscanf(ConfigIP,"%d %d %d %d",&CIP[0],&CIP[1],&CIP[2],&CIP[3]);
	sscanf(GobjIP,"%d %d %d %d",&GIP[0],&GIP[1],&GIP[2],&GIP[3]);

	if((CIP[0] == GIP[0] || CIP[0] == -1) && 
		(CIP[1] == GIP[1] || CIP[1] == -1) &&
		(CIP[2] == GIP[2] || CIP[2] == -1) &&
		(CIP[3] == GIP[3] || CIP[3] == -1))
		return false;	
	return true;
}