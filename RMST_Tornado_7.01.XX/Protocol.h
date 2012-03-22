// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "User.h"
#include "Prodef.h"

#ifndef PROTOC_H
#define PROTOC_H	   								
 
int gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo);	 
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj); 
bool ProtocolCore (BYTE protoNum, LPBYTE aRecv, DWORD aLen, int aIndex, DWORD Encrypt, int Serial);
void ProtocolCoreSend(int aIndex,unsigned char* aSend, int aLen); 
class cProtoFunc
{
public:
	void CheckRing(LPOBJ gObj, LPBYTE aRecv);
	void PlayerConnect(LPOBJ gObj);			   
	bool CGPartyRequestRecv(PMSG_PARTYREQUEST* lpMsg, int aIndex);		
	bool NPCTalkEx(LPOBJ gObj, int NpcId);						  
	bool CharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex);
	bool GuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg);
	void LahapDupeBug(LPOBJ gObj);	
private:										   
	void PkClear(LPOBJ gObj, LPOBJ NpcObj);
	void ResetNpc(LPOBJ gObj, LPOBJ NpcObj);	   	
//------PlayerConnect Protocol Functions------//
	void LoginMsg(LPOBJ gObj);
	void RingSkin(LPOBJ gObj);
//--------------------------------------------//
};

class cProtoSendFunc
{
public:
	void CheckRing(LPOBJ gObj, unsigned char* aSend);
};

extern cProtoFunc Protocol;
extern cProtoSendFunc ProtocolSend;

#endif PROTOC_H