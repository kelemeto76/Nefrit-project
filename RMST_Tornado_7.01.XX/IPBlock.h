#ifndef IpBlock_H
#define IpBlock_H
typedef struct IPBlock
{
	char IP[16];
}IPBLOCK;

#endif

class IpBlock
{				  
public:
	IpBlock();
	~IpBlock();
	void LoadIpBlock();
	bool CheckIp(DWORD aIndex);
	bool CheckIp2(char ConfigIP[16], char GobjIP[16]);

private:
	IPBLOCK IPBlockInfo[255];
	int IPBlockCount;
};
extern IpBlock IP;