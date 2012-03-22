#ifndef MoveReq_T
#define MoveReq_T

class cMoveReq
{
public:
	cMoveReq();
	~cMoveReq(); 

	void MoveReqLoad();
	//void MoveProtocol(DWORD aIndex,int mIndex);			 

	struct sMoveReq
	{
		int Index, Zen, Level, Gate, Vip;
		char MapName1[50], MapName2[50];
	}MoveReqInfo[255];
	int Count;
};
extern cMoveReq MoveReq;
#endif