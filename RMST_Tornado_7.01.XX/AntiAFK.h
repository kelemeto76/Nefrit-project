// ================================================== //
// # GameServer 1.00.90 WzAG.dll                    # //
// # Credit: Mu Community, BAZUKA                   # //
// # Nefrit Project 2012                            # //
// ================================================== //

#ifndef AntiAFK_H
#define AntiAFK_H

#include "User.h"
class AntiAFK
{
public:
	AntiAFK();
	~AntiAFK();
	void SendInfo(LPOBJ gObj);
	void ReadInfo();
private:
	int ArrayMap[64];
	int ReWarning;
};

extern AntiAFK g_antiafk;

#endif