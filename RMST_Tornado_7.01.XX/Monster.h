// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#ifndef MONSTER_H
#define MONSTER_H

#include "User.h"
#include "Console.h"

#ifdef _GameServer_
void ReadMonsterAdd();
int MonsterAdd(WORD Monster,BYTE Map,BYTE X, BYTE Y);
int MonsterAddAndSpawn(WORD Monster,BYTE Speed,BYTE Map,BYTE X, BYTE Y);
#endif

int MygEventMonsterItemDrop(BYTE *b_MonsterDataAddr, BYTE *a_gObjAddr);
void __cdecl MonsterDie(LPOBJ lpObjOfMonster, LPOBJ lpObjOfPlayer);	 
																												  
void NPCMessage(int IndexPlayer, LPOBJ mObj, char* Msg,...);	
void NPCMessageLog(Logger::sColor LogColor, Logger::sLogType LogType, LPOBJ gObj, LPOBJ mObj, char* Msg,...);
void NPCMessageNear(LPOBJ mObj, char* Msg,...);	
						 	
							   
#ifdef _GameServer_						  
#define OBJMON_BASE (*(char*)0x7D800B0)
#define OBJMON_SIZE	12
#endif

#ifdef _GameServerCS_
#define OBJMON_BASE	(*(char**)0x3E74614)  //неправильный оффсет
#define OBJMON_SIZE	0x0C
#endif
								 	 
#define OBJ_MAXMONSTER 7400
#define OBJMON_POINTER(aIndex) (12 * aIndex + OBJMON_BASE)

#define MonsterCount	0x7D95B94
		 
struct MONSTER_POSITION
{						  
  /*<thisrel this+0x0>*/ /*|0x2|*/ WORD m_Type;		   
  /*<thisrel this+0x2>*/ /*|0x1|*/ BYTE m_MapNumber;
  /*<thisrel this+0x3>*/ /*|0x1|*/ BYTE m_Dis;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE m_X;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE m_Y;
  /*<thisrel this+0x6>*/ /*|0x1|*/ BYTE m_Dir;
  /*<thisrel this+0x7>*/ /*|0x1|*/ BYTE m_W;
  /*<thisrel this+0x8>*/ /*|0x1|*/ BYTE m_H;
};
											

	//WORD m_Type;	// 2
	//BYTE m_MapNumber;	// 4
	//BYTE m_Dis;	// 5
	//BYTE m_X;	// 6
	//BYTE m_Y;	// 7
	//BYTE m_Dir;	// 
	//BYTE m_W;	// 9
	//BYTE m_H;	// A

/*	BYTE btType;	// unk0
	WORD wMonsterIndex; // unk2
	BYTE btMapNumber;	// unk4
	BYTE btDistance;	// unk5
	BYTE btStartX;	// unk6
	BYTE btStartY;	// unk7
	BYTE btDirection;	// unk8
	BYTE btEndX;	// unk9
	BYTE btEndY;	// unkA*/

  //MONSTER_POSITION m_Mp[OBJ_MAXMONSTER];	// 4

typedef MONSTER_POSITION *MOBJ;
extern MONSTER_POSITION *MobjTab;

#endif