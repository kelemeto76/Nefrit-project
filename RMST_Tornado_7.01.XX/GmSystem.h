// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#ifndef GMSYSTEM_H
#define GMSYSTEM_H

#include "User.h"
class cGmSystem
{
public:
	struct GMSystem
	{
		char Name[12];
		int Drop;
		int Gg;
		int SetZen;
		int Status;
		int Gmove;
		int SetPK;
		int PkClear;  	 
		int BanPlayer;
		int Reload;
		int Skin;
		int BanPost;
		int Voskl;
		int Disconnect;
		int Trace;	 
		int IsAdmin;
	};
				  
	enum Commands {	NONE = -1,
					cDrop,
					cGg,
					cSetZen,
					cStatus,
					cGmove,
					cSetPK,
					cPkClear,
					cBanPlayer,
					cReload,
					cSkin,
					cBanPost,
					cVoskl,
					cDisconnect,
					cTrace};

	//Structures
	GMSystem GM[255];
	int GMCount;
					   
	cGmSystem();
	~cGmSystem();	
	void LoadGMSystem();
	int IsCommand(Commands, char[11]);
	int IsAdmin(char[11]);
	bool IsGMBD(char[11]);
};
extern cGmSystem GMS;

#endif