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