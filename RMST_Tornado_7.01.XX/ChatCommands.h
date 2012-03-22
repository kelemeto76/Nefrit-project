// ================================================== //
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //  

#ifndef CHAT_H
#define CHAT_H
#include "User.h"
#include "Console.h"	
#include "GmSystem.h"
#include "ConfigDefines.h"
  
unsigned long __stdcall ExchangeTimer(int Index);	      
unsigned long __stdcall AddTimer(int Index);	
unsigned long __stdcall PostDelay(int Index);	  
unsigned long __stdcall ShowGMOnline(int Index);
unsigned long __stdcall ShowInfo(int Index);

class cChat
{
public:
	cChat();
	~cChat();  
	bool ChatDataSend(LPOBJ gObj, LPBYTE aRecv);													   
	void MessageLog(int Type, Logger::sColor LogColor, Logger::sLogType LogType, LPOBJ gObj, char* Msg,...);	   
	void Message(int Type, int Index, char* Msg,...);		  
	void Message(int Index, char* Msg,...);		  
	void MessageAllLog(int Type, int Type2, Logger::sColor LogColor, Logger::sLogType LogType, LPOBJ gObj, char* Msg,...);
	void MessageAll(int Type, int Type2, char *Sender, char* Msg,...);		
	
	void MassLog(LPOBJ gObj, LPBYTE Message);

private:
	void AntiInject(char* message); 	
	bool CheckCommand(LPOBJ gObj, int Config, 
						cGmSystem::Commands Command, int NeedZen, 
						int NeedPcPoint, int NeedWCoin,
						int NeedLvl, int Filled, 
						int CheckPlayer, char *CommandName,
						char *CommandUsage, char *Msg);
	void TakeCommand(LPOBJ gObj, int NeedZen, int NeedPcPoint, int NeedWCoin, char *CommandName);
	bool InfoCommand(int Index, char* Name);
	bool CreditsCommand(int Index, char *Name);  
	bool GgCommand(LPOBJ gObj, char* Msg);		 
	bool VosklCommand(LPOBJ gObj, char* Msg);
	bool TraceCommand(LPOBJ gObj, char *Msg);
	bool SummonCommand(LPOBJ gObj, char *Msg);
	bool DiskCommand(LPOBJ gObj, char* Msg);  
	bool PostCommand(LPOBJ gObj, char *Msg);	 	 
	bool BanPostCommand(LPOBJ gObj, char *Msg);
	bool UnBanPostCommand(LPOBJ gObj, char *Msg);
	bool BanAccCommand(LPOBJ gObj, char *Msg);
	bool UnBanAccCommand(LPOBJ gObj, char *Msg);
	bool BanCharCommand(LPOBJ gObj, char *Msg);
	bool UnBanCharCommand(LPOBJ gObj, char *Msg);	 
	bool TimeCommand(LPOBJ gObj, char *Msg);					 
	bool ExchangeCommands(LPOBJ gObj, char *Msg, int Type);
	bool AddCommands(LPOBJ gObj, char *Msg, int Type);
	void MsgSrv(char *Sender,char* Message, int Type2);		   
	bool LevelCommand(LPOBJ gObj, char *Msg);		
	bool SetZenCommand(LPOBJ gObj, char *Msg);	
	bool SetChar(LPOBJ gObj, char *Msg);	
	bool SetPKCommand(LPOBJ gObj, char *Msg);	
	bool PKClearCommand(LPOBJ gObj, char *Msg);	 
	bool ResetCommand(LPOBJ gObj, char *Msg);	 
	bool SkinCommand(LPOBJ gObj, char *Msg);	
	bool DropCommand(LPOBJ gObj, char *Msg);	  
	bool SetDropCommand(LPOBJ gObj, char *Msg);	   
	bool GmoveCommand(LPOBJ gObj, char *Msg);		
	bool WareCommand(LPOBJ gObj, char *Msg);		
	bool OnlineCommand(LPOBJ gObj, char *Msg);	
	bool StatusCommand(LPOBJ gObj, char *Msg);
	bool ReloadCommand(LPOBJ gObj, char *Msg);
	bool EvoCommand(LPOBJ gObj, char *Msg);
	bool ZenDrop(LPOBJ gObj, char *Msg);
	bool IsMarryCommand(LPOBJ gObj, char *Msg);
	bool MarryOnlineCommand(LPOBJ gObj, char *Msg);
	bool MarryTraceCommand(LPOBJ gObj, char *Msg);
	bool MarryDivorce(LPOBJ gObj, char *Msg);			  	
	bool YesCommand(LPOBJ gObj, char *Msg);
	bool NoCommand(LPOBJ gObj, char *Msg);		
	bool TestCommand(LPOBJ gObj, char*msg);
	bool AddMobCommand(LPOBJ gObj, char *Msg);
	bool BuyVIPServCommand(LPOBJ gObj, char *Msg);
	bool BuyVIPCommand(LPOBJ gObj, char *Msg);
	bool CheckVIPCommand(LPOBJ gObj, char *Msg);
	bool FireworkCommand(LPOBJ gObj, char *Msg);
	bool GuildPost(LPOBJ gObj, char *Msg);
	bool Move(LPOBJ gObj, char *Msg);
	bool License(LPOBJ gObj, char *Msg);
};

extern cChat Chat;

#endif				 