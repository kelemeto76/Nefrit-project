// ================================================== //			  
// # GameServer 1.00.90 WzAG.dll					# //
// # RMST Storm & Tornado Projects 2010-2011		# //
// ================================================== //

#include "StdAfx.h"
#include "ChatCommands.h"  
#include "User.h"
//#include "Main.h"
#include "Console.h"
#include "GmSystem.h"
#include "Utilits.h"
#include "cSQL.h"
#include "PCPoints.h"
#include "Prodef.h"
#include "ResetSystem.h"
#include "PCPoints.h"
#include "Configs.h"
#include "News.h"
#include "IPBlock.h"
#include "DropSystem.h"
#include "ConfigDefines.h"
#include "MarrySystem.h"
#include "Monster.h"
#include "HappyHour.h"
#include "DropEvent.h"
#include "Protection.h"
#include "Localize.h"
#include "Utilits.h"
#include "MoveReq.h"
#include "VIP.h"

cChat Chat;
						 
cChat::cChat()
{
}

cChat::~cChat()
{
}

void cChat::AntiInject(char* message)
{
	for (UINT i = 0; i <= strlen(message); i++)
	{                          						   
		if(message[i] == '\'')		  
		{
			message[i] = ' ';
		}
	}
}
char Messages[1024]; 

unsigned long __stdcall ShowInfo(int Index)
{														  		  
	Chat.Message(Index, "RMST: Tornado 7.10.%02d Season 4.6",dBuild);	
	Sleep(500);		
	Chat.Message(Index, "Official Website http://rmsteam.org/");	   
	Sleep(500);		
	Chat.Message(Index, "Compile Date: %s %s", __DATE__, __TIME__);			
	Sleep(500);		
	Chat.Message(Index, "Credits: Mu Community, RMST & Simp1e");	  
	Sleep(500);		
	Chat.Message(Index, "Please report any bugs that you found!"); 
	Sleep(500);		
	Chat.Message(Index, "Contacts: ICQ-5722555, MSN-ma-3x@list.ru, Skype-cool3x");	
	return true;
}
  
bool cChat::ChatDataSend(LPOBJ gObj,LPBYTE aRecv)
{
	//////////////////////////////////////////////////////////////////
	//				Пример добавления команды /test					//	
	//////////////////////////////////////////////////////////////////
	// 1)В файле ChatCommands.h добавить функцию команды:			//
	//		  bool TestCommand(LPOBJ gObj, char*msg);				//
	//			(параметры в скобках можно изменять)				//
	//																//
	// 2)Ниже добавить проверку на команду и запуск её функции		//
	// 		  														//
	// if (!memcmp(&aRecv[13],"/test",strlen("/test")))				//
	//	bResult = TestCommand(gObj,(char*)aRecv+13+strlen("/test"));//
	//																//
	// 3)Добавить и заполнить саму функцию команды					//
	// 																//
	// bool cChat::TestCommand(LPOBJ gObj, char*msg)				//
	// {															// 
	//   ...														//
	//   return true; // если надо чтобы протокол потом выходил 	//
	//                // из функции и не давал запускаться функции	//
	//				  // чата находящегося в ГС						//
	//				  // и false, если этого делать не надо			//
	// }															// 
	//////////////////////////////////////////////////////////////////
	AntiInject((char*)aRecv+13);									
	bool bResult = false;		 
	if(!cProtection.CheckFeature(cProtection.Localization))
	{		
		if (!memcmp(&aRecv[13],"/level",strlen("/level")))
			bResult = LevelCommand(gObj,(char*)aRecv+13+strlen("/level"));	 
		if (!memcmp(&aRecv[13],"/gg",strlen("/gg")))
			bResult = GgCommand(gObj,(char*)aRecv+13+strlen("/gg"));	 
		if (!memcmp(&aRecv[13],"/post",strlen("/post")))
			bResult = PostCommand(gObj,(char*)aRecv+13+strlen("/post"));	
		if (!memcmp(&aRecv[13],"/banpost",strlen("/banpost")))
			bResult = BanPostCommand(gObj,(char*)aRecv+13+strlen("/banpost"));	 
		if (!memcmp(&aRecv[13],"/unbanpost",strlen("/unbanpost")))
			bResult = UnBanPostCommand(gObj,(char*)aRecv+13+strlen("/unbanpost"));	
		if (!memcmp(&aRecv[13],"/banchar",strlen("/banchar")))
			bResult = BanCharCommand(gObj,(char*)aRecv+13+strlen("/banchar"));		
		if (!memcmp(&aRecv[13],"/unbanchar",strlen("/unbanchar")))
			bResult = UnBanCharCommand(gObj,(char*)aRecv+13+strlen("/unbanchar"));	
		if (!memcmp(&aRecv[13],"/banacc",strlen("/banacc")))
			bResult = BanAccCommand(gObj,(char*)aRecv+13+strlen("/banacc"));	 
		if (!memcmp(&aRecv[13],"/unbanacc",strlen("/unbanacc")))
			bResult = UnBanAccCommand(gObj,(char*)aRecv+13+strlen("/unbanacc"));
		if (!memcmp(&aRecv[13],"/time",strlen("/time")))
			bResult = TimeCommand(gObj,(char*)aRecv+13+strlen("/time"));	
		if (!memcmp(&aRecv[13],"/evo",strlen("/evo")))
			bResult = EvoCommand(gObj,(char*)aRecv+13+strlen("/evo"));											   
		if (!memcmp(&aRecv[13],"/wcoin-pcpoint",strlen("/wcoin-pcpoint")))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen("/wcoin-pcpoint"),21);
		if (!memcmp(&aRecv[13],"/wcoin-zen",strlen("/wcoin-zen")))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen("/wcoin-zen"),31);
		if (!memcmp(&aRecv[13],"/pcpoint-zen",strlen("/pcpoint-zen")))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen("/pcpoint-zen"),32);
		if (!memcmp(&aRecv[13],"/pcpoint-wcoin",strlen("/pcpoint-wcoin")))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen("/pcpoint-wcoin"),12);
		if (!memcmp(&aRecv[13],"/zen-wcoin",strlen("/zen-wcoin")))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen("/zen-wcoin"),13);
		if (!memcmp(&aRecv[13],"/zen-pcpoint",strlen("/zen-pcpoint")))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen("/zen-pcpoint"),23);
		if (!memcmp(&aRecv[13],"/yes",strlen("/yes")))
			bResult = YesCommand(gObj,(char*)aRecv+13+strlen("/yes"));
		if (!memcmp(&aRecv[13],"/no",strlen("/no")))
			bResult = NoCommand(gObj,(char*)aRecv+13+strlen("/no"));		
		if (!memcmp(&aRecv[13],"/addstr",strlen("/addstr")))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen("/addstr"),0);
		if (!memcmp(&aRecv[13],"/addagi",strlen("/addagi")))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen("/addagi"),1);
		if (!memcmp(&aRecv[13],"/addvit",strlen("/addvit")))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen("/addvit"),2);
		if (!memcmp(&aRecv[13],"/addene",strlen("/addene")))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen("/addene"),3);
		if (!memcmp(&aRecv[13],"/addcmd",strlen("/addcmd")))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen("/addcmd"),4);														
		if (!memcmp(&aRecv[13],"/setzen",strlen("/setzen")))
			bResult = SetZenCommand(gObj,(char*)aRecv+13+strlen("/setzen"));
		if (!memcmp(&aRecv[13],"/setchar",strlen("/setchar")))
			bResult = SetChar(gObj,(char*)aRecv+13+strlen("/setchar"));	
		if (!memcmp(&aRecv[13],"/setpk",strlen("/setpk")))
			bResult = SetPKCommand(gObj,(char*)aRecv+13+strlen("/setpk"));		
		if (!memcmp(&aRecv[13],"/pkclear",strlen("/pkclear")))
			bResult = PKClearCommand(gObj,(char*)aRecv+13+strlen("/pkclear"));	   
		if (!memcmp(&aRecv[13],"/reset",strlen("/reset")))
			bResult = ResetCommand(gObj,(char*)aRecv+13+strlen("/reset"));	   
		if (!memcmp(&aRecv[13],"/skin",strlen("/skin")))
			bResult = SkinCommand(gObj,(char*)aRecv+13+strlen("/skin"));	
		if (!memcmp(&aRecv[13],"/drop",strlen("/drop")))
			bResult = DropCommand(gObj,(char*)aRecv+13+strlen("/drop"));	 
		if (!memcmp(&aRecv[13],"/gmove",strlen("/gmove")))
			bResult = GmoveCommand(gObj,(char*)aRecv+13+strlen("/gmove"));	
		if (!memcmp(&aRecv[13],"/ware",strlen("/ware")))
			bResult = WareCommand(gObj,(char*)aRecv+13+strlen("/ware"));
		if (!memcmp(&aRecv[13],"/online",strlen("/online")))
			bResult = OnlineCommand(gObj,(char*)aRecv+13+strlen("/online"));
		if (!memcmp(&aRecv[13],"/status",strlen("/status")))
			bResult = StatusCommand(gObj,(char*)aRecv+13+strlen("/status"));
		if (!memcmp(&aRecv[13],"/reload",strlen("/reload")))
			bResult = ReloadCommand(gObj,(char*)aRecv+13+strlen("/reload"));
		if (!memcmp(&aRecv[13],"/marryonline",strlen("/marryonline")))
			bResult = MarryOnlineCommand(gObj,(char*)aRecv+13+strlen("/marryonline"));
		if (!memcmp(&aRecv[13],"/marrytrace",strlen("/marrytrace")))
			bResult = MarryTraceCommand(gObj,(char*)aRecv+13+strlen("/marrytrace"));
		if (!memcmp(&aRecv[13],"/ismarry",strlen("/ismarry")))
			bResult = IsMarryCommand(gObj,(char*)aRecv+13+strlen("/ismarry"));
		if (!memcmp(&aRecv[13],"/divorce",strlen("/divorce")))
			bResult = MarryDivorce(gObj,(char*)aRecv+13+strlen("/divorce"));
		if (!memcmp(&aRecv[13],"/zendrop",strlen("/zendrop")))
			bResult = ZenDrop(gObj,(char*)aRecv+13+strlen("/zendrop"));		
		if (!memcmp(&aRecv[13],"/mobadd",strlen("/mobadd")))
			bResult = AddMobCommand(gObj,(char*)aRecv+13+strlen("/mobadd"));		
		if (!memcmp(&aRecv[13],"/checkvip",strlen("/checkvip")))
			bResult = CheckVIPCommand(gObj,(char*)aRecv+13+strlen("/checkvip"));		
	}
	else
	{
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.LevelSyntax,strlen(Config.SyntaxCmd.LevelSyntax)))
			bResult = LevelCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.LevelSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.GgSyntax,strlen(Config.SyntaxCmd.GgSyntax)))
			bResult = GgCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.GgSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.PostSyntax,strlen(Config.SyntaxCmd.PostSyntax)))
			bResult = PostCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.PostSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.BanPostSyntax,strlen(Config.SyntaxCmd.BanPostSyntax)))
			bResult = BanPostCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.BanPostSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.UnBanPostSytax,strlen(Config.SyntaxCmd.UnBanPostSytax)))
			bResult = UnBanPostCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.UnBanPostSytax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.BanCharSyntax,strlen(Config.SyntaxCmd.BanCharSyntax)))
			bResult = BanCharCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.BanCharSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.UnBanCharSyntax,strlen(Config.SyntaxCmd.UnBanCharSyntax)))
			bResult = UnBanCharCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.UnBanCharSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.BanAccSyntax,strlen(Config.SyntaxCmd.BanAccSyntax)))
			bResult = BanAccCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.BanAccSyntax));	 
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.UnBanAccSyntax,strlen(Config.SyntaxCmd.UnBanAccSyntax)))
			bResult = UnBanAccCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.UnBanAccSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.TimeSyntax,strlen(Config.SyntaxCmd.TimeSyntax)))
			bResult = TimeCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.TimeSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.EvoSyntax,strlen(Config.SyntaxCmd.EvoSyntax)))
			bResult = EvoCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.EvoSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.WcPpSyntax,strlen(Config.SyntaxCmd.WcPpSyntax)))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.WcPpSyntax),21);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.WcZenSyntax,strlen(Config.SyntaxCmd.WcZenSyntax)))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.WcZenSyntax),31);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.PpZenSyntax,strlen(Config.SyntaxCmd.PpZenSyntax)))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.PpZenSyntax),32);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.PpWcSyntax,strlen(Config.SyntaxCmd.PpWcSyntax)))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.PpWcSyntax),12);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.ZenWcSyntax,strlen(Config.SyntaxCmd.ZenWcSyntax)))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.ZenWcSyntax),13);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.ZenPpSyntax,strlen(Config.SyntaxCmd.ZenPpSyntax)))
			bResult = ExchangeCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.ZenPpSyntax),23);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.YesSyntax,strlen(Config.SyntaxCmd.YesSyntax)))
			bResult = YesCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.YesSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.NoSyntax,strlen(Config.SyntaxCmd.NoSyntax)))
			bResult = NoCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.NoSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.AddStrSyntax,strlen(Config.SyntaxCmd.AddStrSyntax)))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.AddStrSyntax),0);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.AddAgiSyntax,strlen(Config.SyntaxCmd.AddAgiSyntax)))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.AddAgiSyntax),1);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.AddVitSyntax,strlen(Config.SyntaxCmd.AddVitSyntax)))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.AddVitSyntax),2);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.AddEneSyntax,strlen(Config.SyntaxCmd.AddEneSyntax)))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.AddEneSyntax),3);
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.AddCmdSyntax,strlen(Config.SyntaxCmd.AddCmdSyntax)))
			bResult = AddCommands(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.AddCmdSyntax),4);	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.SetZenSyntax,strlen(Config.SyntaxCmd.SetZenSyntax)))
			bResult = SetZenCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.SetZenSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.SetCharSyntax,strlen(Config.SyntaxCmd.SetCharSyntax)))
			bResult = SetChar(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.SetCharSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.SetPKSyntax,strlen(Config.SyntaxCmd.SetPKSyntax)))
			bResult = SetPKCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.SetPKSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.PKClearSyntax,strlen(Config.SyntaxCmd.PKClearSyntax)))
			bResult = PKClearCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.PKClearSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.ResetSyntax,strlen(Config.SyntaxCmd.ResetSyntax)))
			bResult = ResetCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.ResetSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.SkinSyntax,strlen(Config.SyntaxCmd.SkinSyntax)))
			bResult = SkinCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.SkinSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.DropSyntax,strlen(Config.SyntaxCmd.DropSyntax)))
			bResult = DropCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.DropSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.GmoveSyntax,strlen(Config.SyntaxCmd.GmoveSyntax)))
			bResult = GmoveCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.GmoveSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.WareSyntax,strlen(Config.SyntaxCmd.WareSyntax)))
			bResult = WareCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.WareSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.OnlineSyntax,strlen(Config.SyntaxCmd.OnlineSyntax)))
			bResult = OnlineCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.OnlineSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.StatusSyntax,strlen(Config.SyntaxCmd.StatusSyntax)))
			bResult = StatusCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.StatusSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.ReloadSyntax,strlen(Config.SyntaxCmd.ReloadSyntax)))
			bResult = ReloadCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.ReloadSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.MarryOnlineSyntax,strlen(Config.SyntaxCmd.MarryOnlineSyntax)))
			bResult = MarryOnlineCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.MarryOnlineSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.MarryTraceSyntax,strlen(Config.SyntaxCmd.MarryTraceSyntax)))
			bResult = MarryTraceCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.MarryTraceSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.IsMarrySyntax,strlen(Config.SyntaxCmd.IsMarrySyntax)))
			bResult = IsMarryCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.IsMarrySyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.DivorceSyntax,strlen(Config.SyntaxCmd.DivorceSyntax)))
			bResult = MarryDivorce(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.DivorceSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.ZenDropSyntax,strlen(Config.SyntaxCmd.ZenDropSyntax)))
			bResult = ZenDrop(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.ZenDropSyntax));	
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.MobAddSyntax,strlen(Config.SyntaxCmd.MobAddSyntax)))
			bResult = AddMobCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.MobAddSyntax));
		if (!memcmp(&aRecv[13],Config.SyntaxCmd.CheckVIPSyntax,strlen(Config.SyntaxCmd.CheckVIPSyntax)))
			bResult = CheckVIPCommand(gObj,(char*)aRecv+13+strlen(Config.SyntaxCmd.CheckVIPSyntax));
	} 
	//==//Credits
	char dLabel[9] ={0x2F, 0x63, 0x72, 0x65, 0x64, 0x69, 0x74, 0x73};
	if (!memcmp(&aRecv[13],dLabel,strlen(dLabel)))
		bResult = CreditsCommand(gObj->m_Index, gObj->Name);	   
	if (!memcmp(&aRecv[13],"/serverinfo",strlen("/serverinfo")))
		bResult = InfoCommand(gObj->m_Index, gObj->Name);	 
	if (!memcmp(&aRecv[13],"!",strlen("!")))
		bResult = VosklCommand(gObj,(char*)aRecv+13+strlen("!"));	
	if (!memcmp(&aRecv[13],"/trace",strlen("/trace")))
			bResult = TraceCommand(gObj,(char*)aRecv+13+strlen("/trace"));	 
	if (!memcmp(&aRecv[13],"/disconnect",strlen("/disconnect")))
			bResult = DiskCommand(gObj,(char*)aRecv+13+strlen("/disconnect"));	  
	if (!memcmp(&aRecv[13],"/track",strlen("/track")))
			bResult = SummonCommand(gObj,(char*)aRecv+13+strlen("/track"));	  
	if (!memcmp(&aRecv[13],"/setdrop",strlen("/setdrop")))
			bResult = SetDropCommand(gObj,(char*)aRecv+13+strlen("/setdrop"));	  
	if (!memcmp(&aRecv[13],"/firework",strlen("/firework")))
			bResult = FireworkCommand(gObj,(char*)aRecv+13+strlen("/firework"));
	if (!memcmp(&aRecv[13],"/buyvip",strlen("/buyvip")))
			bResult = BuyVIPCommand(gObj,(char*)aRecv+13+strlen("/buyvip"));	  
	if (!memcmp(&aRecv[13],"@>",strlen("@>")))
			bResult = GuildPost(gObj,(char*)aRecv+13+strlen("@>"));	  
	if (!memcmp(&aRecv[13],"/move",strlen("/move")))
			bResult = Move(gObj,(char*)aRecv+13+strlen("/move"));	
	if (!memcmp(&aRecv[13],"/warp",strlen("/warp")))
			bResult = Move(gObj,(char*)aRecv+13+strlen("/warp"));	  
	
	
	MassLog(gObj, aRecv);

	return bResult;
}									   			   

bool cChat::TestCommand(LPOBJ gObj, char*msg)
{	
	return true;
}

void cChat::MassLog(LPOBJ gObj, LPBYTE Message)
{		
	char Type[20];	
	char Msg2[512];			
	if (!memcmp(&Message[13],"@@",strlen("@@")))
	{
		strcpy(Type, "Guild Aliance");	   
		strcpy(Msg2, (char*)Message+13+strlen("@@"));
	}	 
	else if (!memcmp(&Message[13],"@",strlen("@")))
	{
		strcpy(Type, "Guild");
		strcpy(Msg2, (char*)Message+13+strlen("@"));
	}
	else if (!memcmp(&Message[13],"/post",strlen("/post")))
	{
		strcpy(Type, "Post");
		strcpy(Msg2, (char*)Message+13+strlen("/post"));
	}
	else if (!memcmp(&Message[13],"~",strlen("~")))
	{
		strcpy(Type, "Party");	
		strcpy(Msg2, (char*)Message+13+strlen("~"));
	}	
	else
	{
		strcpy(Type, "Chat");
		strcpy(Msg2, (char*)Message+13);
	}						  

	SYSTEMTIME t;
	GetLocalTime(&t);
	char currdate[11] = {0};
	sprintf(currdate, "%02d:%02d:%02d", t.wHour, t.wMinute, t.wSecond);
	char Msg[512];
	sprintf(Msg,"[%s] [%s:%s] [%s]: %s\n", currdate, gObj->AccountID, gObj->Name, Type,  Msg2);	

	char Date[55];
	sprintf(Date, "..\\RMST\\ChatLogs\\%02d-%02d-%02d\\", t.wDay, t.wMonth, t.wYear); 
	CreateDirectory(Date,NULL);

	char ChatLog[55];		  		
	sprintf(ChatLog, "..\\RMST\\ChatLogs\\%02d-%02d-%02d\\ChatLogs.log", t.wDay, t.wMonth, t.wYear);
	cLog.SaveFile(ChatLog, Msg);  
}
			  
void cChat::MsgSrv(char *Sender,char *Message, int Type)
{		 
	BYTE *Packet;
	Packet = (BYTE*) malloc(200);
	memset(Packet, 0x00, 200);
	*Packet = 0xC1;
	if(Type)
		*(Packet+2) = 0x02;	  
	else				   
		*(Packet+2) = 0x00;
	memcpy((Packet+3), Sender, strlen(Sender));
	memcpy((Packet+13), Message, strlen(Message));
	int Len = (strlen(Message) + 0x13);
	*(Packet+1) = Len;
	DataSendAll(Packet, Len);
	free (Packet);
}

void cChat::MessageLog(int Type, Logger::sColor LogColor, Logger::sLogType LogType, LPOBJ gObj, char *Msg,...)
{	
	Messages[0] = 0;
	char Temp[255];
	strcpy(Temp, cLocalization.Localize(Msg));
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages, Temp, pArguments1);
	va_end(pArguments1);

	GCServerMsgStringSend(Messages, gObj->m_Index, Type);
	// Type = 1 - в блоке сообщений 
	// Type = 0 - золотом по середине
	if(Type)
		cLog.ConsoleOutPut(1, LogColor, LogType, "[%s:%s]: %s", gObj->AccountID, gObj->Name, Messages);
	else														
		cLog.ConsoleOutPut(1, LogColor, LogType, "[GoldMessage] %s", Messages);
}						  

void cChat::Message(int Type, int Index, char *Msg,...)
{				 	   
	Messages[0] = 0;
	char Temp[255];
	strcpy(Temp, cLocalization.Localize(Msg));
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages, Temp, pArguments1);
	va_end(pArguments1);

	GCServerMsgStringSend(Messages, Index, Type);
	// Type = 1 - в блоке сообщений 
	// Type = 0 - золотом по середине
}

void cChat::Message(int Index, char *Msg,...)
{				 	   
	Messages[0] = 0;	
	char Temp[255];
	strcpy(Temp, cLocalization.Localize(Msg));
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages, Temp, pArguments1);
	va_end(pArguments1);

	GCServerMsgStringSend(Messages, Index, 1);
	// Type = 1 - в блоке сообщений 
	// Type = 0 - золотом по середине
}
					 			
void cChat::MessageAllLog(int Type, int Type2, Logger::sColor LogColor, Logger::sLogType LogType, LPOBJ gObj, char *Msg,...)
{					   
	Messages[0] = 0;
	char Temp[255];
	strcpy(Temp, cLocalization.Localize(Msg));
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages, Temp, pArguments1);
	va_end(pArguments1);
		
	if(Type == 2 && gObj != NULL)
	{
		MsgSrv(gObj->Name, Messages, Type2);
	}
	else
		for(int i=OBJECT_MIN; i<=OBJECT_MAX; i++)
		{											 
			OBJECTSTRUCT *gObj2 = (OBJECTSTRUCT*)OBJECT_POINTER(i);
			if(gObj2->Connected < 3)	continue;
			GCServerMsgStringSend(Messages, i, Type);
		}	

	if(gObj == NULL)
		return;
	if(Type)
		cLog.ConsoleOutPut(1, LogColor, LogType, "[%s:%s]: %s", gObj->AccountID, gObj->Name, Messages);
	else														
		cLog.ConsoleOutPut(1, LogColor, LogType, "[%s:%s][GoldMessage] %s", gObj->AccountID, gObj->Name, Messages);
}

void cChat::MessageAll(int Type, int Type2, char *Sender, char *Msg,...)
{					  
	Messages[0] = 0;
	char Temp[255];
	strcpy(Temp, cLocalization.Localize(Msg));
	va_list pArguments1;
	va_start(pArguments1, Msg);
	vsprintf_s(Messages, Temp, pArguments1);
	va_end(pArguments1);

	if(Type == 2)
	{
		MsgSrv(Sender, Messages, Type2);
	}
	else
		for(int i=OBJECT_MIN; i<=OBJECT_MAX; i++)
		{											 
			OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);
			if(gObj->Connected < 3)	continue;
			GCServerMsgStringSend(Messages, i, Type);
		}											 
}

bool cChat::CheckCommand(LPOBJ gObj, int lConfig, cGmSystem::Commands Command, 
						int NeedZen, int NeedPcPoint, int NeedWCoin,
						int NeedLvl, int Filled, int CheckPlayer, 
						char *CommandName, char *CommandUsage, char *Msg)
{
	bool bResult = false;
	
	int spaces = 0;
	for(unsigned int i = 0; i < strlen(Msg); i++)
		if(Msg[i]==' ' && Msg[i-1]!=' ')spaces++;	

	if((Filled > 0) && ((Filled > spaces) || (((strlen(Msg) < 1) || (strlen(Msg) == 1)) && (Msg[0] == ' '))))
	{				
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] Usage: %s", CommandName, CommandUsage);
		return true;
	}

	if (lConfig == 0)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] Function temporarily disabled.", CommandName);
		return true;
	}

	if (!GMS.IsCommand(Command, gObj->Name))
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] You can't use this command.", CommandName);
		return true;
	}		
					   
	if (NeedLvl > 0 && gObj->Level < NeedLvl)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] You haven't got enougth level.", CommandName);
		bResult = true;
	}

	if (NeedZen > 0 && NeedZen > gObj->Money)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] You haven't got enougth money.", CommandName);
		bResult = true;
	}					  

	if (NeedPcPoint > 0 && NeedPcPoint > AddTab[gObj->m_Index].PCPlayerPoints)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] You need %d more PcPoints.", CommandName, NeedPcPoint - AddTab[gObj->m_Index].PCPlayerPoints);
		bResult = true;
	}

	if (NeedWCoin > 0 && NeedWCoin > gObj->m_wCashPoint)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] You need %d more WCoins.", CommandName, NeedWCoin - gObj->m_wCashPoint);
		bResult = true;
    }
						 								  
	if(CheckPlayer > 0)
	{						
		char Target[11], Target2[11];
		int Index = 0, Index2 = 0;
		if(CheckPlayer == 1)
		{
			sscanf(Msg,"%s",&Target);
			Index = Util.GetPlayerIndex(Target);
		}
		else if(CheckPlayer == 2)
		{	 
			sscanf(Msg,"%s %s",&Target,&Target2);	  
			Index = Util.GetPlayerIndex(Target);
			Index2 = Util.GetPlayerIndex(Target2);
		}
	
		if(Index == -1 || Index2 == -1)
		{													
			MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[%s] Player offline or doesn't exist!", CommandName);
			bResult = true;
		}
	}		 
	return bResult;
}			  
		
void cChat::TakeCommand(LPOBJ gObj, int NeedZen, int NeedPcPoint, int NeedWCoin, char *CommandName)
{
	if(NeedZen > 0)
	{
		gObj->Money -= NeedZen;
		GCMoneySend(gObj->m_Index, gObj->Money);
	}
	if(NeedPcPoint > 0)
	{
		g_PointShop.DecreasePoints(gObj->m_Index, NeedPcPoint);	  
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[%s] You payed %d PcPoints", CommandName, NeedPcPoint);
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - %d WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, NeedPcPoint, gObj->Name);
	}	
	if(NeedWCoin > 0)
	{
		gObj->m_wCashPoint -= NeedWCoin;
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_PCPOINT, gObj,"[%s] You payed %d WCoin", CommandName, NeedWCoin);
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s - %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, NeedWCoin, gObj->AccountID);
	}
}			  

bool cChat::InfoCommand(int Index, char *Name)
{					
	if(!cProtection.CheckFeature(cProtection.Credits_Removal))
	{			   			   
		DWORD ThreadID;
		HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowInfo, (void*)Index, 0, &ThreadID);

		if ( hThread == 0 )
		{
			cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
			return true;
		}

		CloseHandle(hThread);												   
		cLog.ConsoleOutPut(1, cLog.c_Blue, cLog.t_COMMANDS, "[ServerInfo] %s get server status", Name);

	}
	return false;
}	

bool cChat::CreditsCommand(int Index, char *Name)
{								   
	char aLabel[] = { 0x54 , 0x6F , 0x72 , 0x6E , 0x61 , 0x64 , 
		0x6F , 0x20 , 0x50 , 0x72 , 0x6F , 0x6A , 0x65 , 
		0x63 , 0x74 , 0x20 , 0x50 , 0x55 , 0x42 , 0x4C , 
		0x49 , 0x43 , 0x20 , 0x2D , 0x20 , 0x39 , 0x30 , 
		0x47 , 0x53 , 0x20 , 0x44 , 0x4C , 0x4C, 0x00};
	char bLabel[] = {0x43 , 0x72 , 0x65 , 0x64 , 0x69 , 0x74 , 
		0x73 , 0x3A , 0x20 , 0x4D , 0x75 , 0x43 , 0x6F , 0x6D ,
		0x6D , 0x75 , 0x6E , 0x69 , 0x74 , 0x79 , 0x20 , 0x26 ,
		0x20 , 0x52 , 0x4D , 0x53 , 0x54 , 0x65 , 0x61 , 0x6D,
		0x00};
	char cLabel[] = {0x4D , 0x61 , 0x54 , 0x68 , 0x72 , 0x65 , 0x65 , 
		0x58 , 0x2C , 0x20 , 0x44 , 0x65 , 0x61 , 0x64 , 0x4D , 0x61 , 
		0x6E , 0x2C , 0x20 , 0x52 , 0x75 , 0x6D , 0x7A , 0x69 , 0x6B , 0x2C , 
		0x20 , 0x42 , 0x65 , 0x6F , 0x73 , 0x2C , 0x20 , 0x4D , 0x72 , 0x2E , 0x4B , 
		0x65 , 0x72 , 0x6E , 0x69 , 0x67 , 0x68 , 0x61 , 0x6E , 0x20 , 0x26 ,
		0x20 , 0x53 , 0x69 , 0x6D , 0x70 , 0x31 , 0x65, 0x00};											  		  
	Chat.Message(1, Index, "%s",aLabel);	
	Chat.Message(1, Index, "%s",bLabel);	  
	Chat.Message(1, Index, "%s",cLabel);
	return true;
}


bool cChat::GgCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsGg, GMS.cGg, 0, 0, 0, 0, 1, 0, "GG", "/gg <msg>", Msg))
		return true;												
							
	cLog.CheckProcent(Msg);
	MessageAllLog(0, 0, cLog.c_Yellow, cLog.t_GPOST, gObj, "[%s]: %s", gObj->Name, Msg);	
	return true;
}			
													   
bool cChat::VosklCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, 1, GMS.cVoskl, 0, 0, 0, 0, 1, 0, "!", "! <msg>", Msg))
		return true;												
						
	cLog.CheckProcent(Msg);
	MessageAllLog(0, 0, cLog.c_Yellow, cLog.t_GPOST, gObj, Msg);
	return true;
}		

bool cChat::TraceCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, 1, GMS.cTrace, 0, 0, 0, 0, 1, 1, "Trace", "/trace <name>", Msg))
		return true;

	char Target[11];
	sscanf(Msg, "%s", &Target);

	int Index = Util.GetPlayerIndex(Target);
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	gObjTeleport(gObj->m_Index, tObj->MapNumber, (int)tObj->X, (int)tObj->Y);	
	MessageLog(1, cLog.c_Green, cLog.t_GPOST, gObj, "[Trace] You successfully traced to %s",Target);
	return true;
}		

bool cChat::SummonCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, 1, GMS.cTrace, 0, 0, 0, 0, 1, 1, "Track", "/Track <name>", Msg))
		return true;

	char Target[11];
	sscanf(Msg, "%s", &Target);

	int Index = Util.GetPlayerIndex(Target);
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	gObjTeleport(tObj->m_Index, gObj->MapNumber, (int)gObj->X, (int)gObj->Y);	
	MessageLog(1, cLog.c_Green, cLog.t_GPOST, gObj, "[Track] You successfully summoned %s",Target);
	return true;
}		

bool cChat::DiskCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, 1, GMS.cDisconnect, 0, 0, 0, 0, 1, 1, "DC", "/disconnect <name>", Msg))
		return true;

	char Target[11];
	sscanf(Msg, "%s", &Target);
	CloseClient(Util.GetPlayerIndex(Target));

	MessageLog(1, cLog.c_Green, cLog.t_GPOST, gObj, "[Disconnect] %s was disconnected.",Target);	
	return true;
}		 
								      
unsigned long __stdcall PostDelay(int Index)
{	
	for(AddTab[Index].PostDelay = Config.Commands.MassPostDelay; AddTab[Index].PostDelay >= 0; AddTab[Index].PostDelay--)
		Sleep(1000);	
	return 1;
}

bool cChat::PostCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsPost, GMS.NONE, Config.Commands.PostPriceZen, Config.Commands.PostPricePCPoint, Config.Commands.PostPriceWCoin, Config.Commands.PostLvl, 1, 0, "Post", "/post <msg>", Msg))
		return true;
							   
	MySQL.Execute("SELECT BanPost FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, gObj->Name);		
	int Banned = MySQL.GetInt();
	if(Banned == 1)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[POST] You can't post.");
		return true;
	}

	if(AddTab[gObj->m_Index].PostDelay > 0 && gObj->Authority != 32 && gObj->Authority != 8)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[ANTI-FLOOD] Wait %d sec until you can post!", AddTab[gObj->m_Index].PostDelay);
		return true;
	}

	TakeCommand(gObj, Config.Commands.PostPriceZen, Config.Commands.PostPricePCPoint, Config.Commands.PostPriceWCoin, "Post");
	//cLog.CheckProcent(Msg);
	switch(Config.Commands.PostColor)
	{
	case 0:
	case 1:
		MessageAllLog(2, 1, cLog.c_Yellow, cLog.t_POST, gObj, "[POST] %s", Msg);	
		break;
	case 2:
		MessageAllLog(2, 0, cLog.c_Green, cLog.t_POST, gObj, "@[POST] %s", Msg);	
		break;
	case 3:
		MessageAllLog(2, 0, cLog.c_Blue, cLog.t_POST, gObj, "~[POST] %s", Msg);	
		break;
	} 
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PostDelay, (void*)gObj->m_Index, 0, &ThreadID);

	if ( hThread == 0 )
	{
		cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
		return true;
	}

	CloseHandle(hThread);	
	return true;
}

bool cChat::BanPostCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsBanPost, GMS.cBanPost, 0, 0, 0, 0, 2, 0, "BanPost", "/banpost <nick> <minute> <hour> <day> <month>", Msg))
		return true;	

	char Target[11];  
	 

	sscanf(Msg, "%s", &Target);
	int Index = Util.GetPlayerIndex(Target); 
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	MySQL.Execute("SELECT BanPost FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, Target);		
	int Banned = MySQL.GetInt(); 
							 	
	if(Banned == 1)
	{																											   
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[BanPost] %s Already Banned", Target);
	}
   	else   
	{
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[BanPost] %s Added BanPosts", Target);		
	}

	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET BanPost = '1' WHERE Name = '%s'",MySQL.szDatabase,  Target);
	return true;	   
}

bool cChat::UnBanPostCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsBanPost, GMS.cBanPost, 0, 0, 0, 0, 1, 0, "UnBanPost", "/unbanpost <nick>", Msg))
		return true;	

	char Target[11];  
				 
	sscanf(Msg, "%s", &Target);
	int Index = Util.GetPlayerIndex(Target); 
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	MySQL.Execute("SELECT BanPost FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, Target);		
	int Banned = MySQL.GetInt();

	if(Banned == 0)
	{																											   
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[BanPost] %s's post is not banned", Target);
		return true;
	}
   	else   
	{
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[BanPost] %s's post sucsessfuly UnBanned!", Target);
	} 
											
	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET  BanPost = '0' WHERE Name = '%s'", MySQL.szDatabase,Target);	
	return true;	   
} 

bool cChat::BanCharCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsBanChar, GMS.cBanPlayer, 0, 0, 0, 0, 1, 0, "BanChar", "/banchar <nick>", Msg))
		return true;	

	char Target[11];  

	sscanf(Msg, "%s", &Target);
	int Index = Util.GetPlayerIndex(Target); 
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	MySQL.Execute("SELECT CtlCode FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, Target);		
	int Banned = MySQL.GetInt();
							 	
	if(Banned == 1)
	{																											   
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[BanChar] %s is already banned", Target);
		return true;
	}
   	else   
	{																				 
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET CtlCode='1' WHERE Name='%s'",MySQL.szDatabase,Target);		
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[BanChar] %s banned", Target);		
	}
		 
	CloseClient(Index);
	return true;	 
}

bool cChat::BanAccCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsBanAcc, GMS.cBanPlayer, 0, 0, 0, 0, 1, 0, "BanAcc", "/banacc <login> <nick>", Msg))
		return true;	

	char Target[11]; // account
	char Target2[11]; // nick

	sscanf(Msg, "%s %s", &Target, &Target2);
	int Index = Util.GetPlayerIndex(Target2); 
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
							  

	if(Index != -1)			
	{
		CloseClient(Index);									   
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[BanAcc] %s banned, character %s disconnected.", Target, Target2);		
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET bloc_code='1' WHERE memb___id = '%s'",MySQL.szDatabase2,Target);	
	}
	else 
  		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET bloc_code='1' WHERE memb___id = '%s'",MySQL.szDatabase2,Target);	   
	 
	return true;	 
}

bool cChat::UnBanCharCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsBanChar, GMS.cBanPlayer, 0, 0, 0, 0, 1, 0, "UnBanChar", "/unbanchar <nick>", Msg))
		return true;	

	char Target[11];  

	sscanf(Msg, "%s", &Target);

	MySQL.Execute("SELECT CtlCode FROM [%s].[dbo].[Character] WHERE Name = '%s'",MySQL.szDatabase, Target);
	int Banned = MySQL.GetInt();
							 	
	if(Banned == 0)
	{																											   
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[UnBanChar] %s is not baned", Target);
		return true;
	}
   	else   
	{		 
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET CtlCode='0' WHERE Name='%s'",MySQL.szDatabase,Target);
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[UnBanChar] %s UnBaned", Target);		
	}
	return true;	 
}

bool cChat::UnBanAccCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsBanAcc, GMS.cBanPlayer, 0, 0, 0, 0, 1, 0, "UnBanAcc", "/unbanacc <login>", Msg))
		return true;	

	char Target[11];  

	sscanf(Msg, "%s", &Target);

	MySQL.Execute("SELECT bloc_code FROM [%s].[dbo].[MEMB_INFO] WHERE memb___id = '%s'",MySQL.szDatabase2, Target);	
	int Banned = MySQL.GetInt();
							 	
	if(Banned == 0)
	{																											   
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[UnBanAcc] %s is not baned", Target);
		return true;
	}
   	else   
	{		 																	  
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET bloc_code='0' WHERE memb___id = '%s'",MySQL.szDatabase2,Target);  
		MessageLog(1, cLog.c_Red, cLog.t_BAN, gObj, "[UnBanAcc] %s UnBaned", Target);		
	} 
	 
	return true;	 
}

bool cChat::TimeCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsTime, GMS.NONE, 0, 0, 0, 0, 0, 0, "Time", "/time", Msg))
		return true;

	SYSTEMTIME t;
	GetLocalTime(&t);
	MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "Server Time & Date: %02d:%02d:%02d %02d-%02d-%02d.", t.wHour, t.wMinute, t.wSecond, t.wDay, t.wMonth, t.wYear);
	return true;
}

bool cChat::ExchangeCommands(LPOBJ gObj, char *Msg, int Type)
{
#pragma warning(disable: 4244)
	switch(Type)
	{
	case 21:
		{
			if(AddTab[gObj->m_Index].ExType != -1)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't make other purchase. Answer previous please.");  
				return true;
			}
			if(CheckCommand(gObj, Config.ExchangeWcPp, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 1, 0, "Exchanger", "/wcoin-pcpoint <num>", Msg))
			{				  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD Buy;
			int Sell = gObj->m_wCashPoint;
			sscanf(Msg, "%d", &Buy);	   

			DWORD ConfigBuy = Config.WCoin_PcPoint;
			DWORD MaxBuy = Config.MaximumPCPoints; 	   
														
			if(Buy % ConfigBuy != 0)
			{							
				if(Buy > ConfigBuy)
				{
					DWORD Min = Buy - (Buy % ConfigBuy);
					DWORD Max = Min + ConfigBuy;			
					MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You write not right value. Write %d or %d.", Min, Max);  
					AddTab[gObj->m_Index].ExType = -1;
					return true;
				}
				else
				{
					MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You write not right value. Write %d.", ConfigBuy);  
					AddTab[gObj->m_Index].ExType = -1;
					return true;
				}
			}

			int NeededToSell = (Buy / ConfigBuy);	 
			if(Sell < NeededToSell)
			{
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't have enough WCoins. %d more need.", NeededToSell - Sell);  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			} 
			if(AddTab[gObj->m_Index].PCPlayerPoints + Buy > MaxBuy)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't buy more PCPoints.");  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD ConfigTax = Config.ExchangeTax;
			float temp = (Buy/100)*ConfigTax;
			DWORD Tax = ceil(temp);																																					  
			MessageLog(1, cLog.c_Green, cLog.t_EXCHANGE, gObj, "[Exchanger] You really want to buy %d PCPoints with %d tax and pay %d WCoins?", Buy, Tax, NeededToSell);
			AddTab[gObj->m_Index].ExType = 21;
			AddTab[gObj->m_Index].WantedNum = Buy; 	   

			//_beginthread(ExchangeTimer,0,NULL);	 		
			DWORD ThreadID;
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExchangeTimer, (void*)gObj->m_Index, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return true;
			}

			CloseHandle(hThread);	
			return true;
		}
		break;	
	case 31:
		{
			if(AddTab[gObj->m_Index].ExType != -1)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't make other purchase. Answer previous please.");  
				return true;
			}
			if(CheckCommand(gObj, Config.ExchangeWcZen, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 1, 0, "Exchanger", "/wcoin-zen <num>", Msg))
			{				  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD Buy;
			int Sell = gObj->m_wCashPoint;
			sscanf(Msg, "%d", &Buy);

			DWORD ConfigBuy = Config.WCoin_Zen;
			DWORD MaxBuy = 2000000000; 

			if(Buy % ConfigBuy != 0)
			{							
				if(Buy > ConfigBuy)
				{									  
					DWORD Min = Buy - (Buy % ConfigBuy);
					DWORD Max = Min + ConfigBuy;			
					MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You write not right value. You can write %d or %d.", Min, Max);  
					AddTab[gObj->m_Index].ExType = -1;
					return true;
				}
				else
				{
					MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You write not right value. You can write %d.", ConfigBuy);  
					AddTab[gObj->m_Index].ExType = -1;
					return true;
				}
			}  				  
			int NeededToSell = (Buy / ConfigBuy);	
			if(Sell < NeededToSell)
			{
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't have enough WCoins. %d more need.", NeededToSell - Sell);  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			} 
			if(gObj->Money + Buy > MaxBuy)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't buy more Zen.");  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD ConfigTax = Config.ExchangeTax;
			float temp = ((Buy/100)*ConfigTax); 
			DWORD Tax = ceil(temp);																															  
			MessageLog(1, cLog.c_Green, cLog.t_EXCHANGE, gObj, "[Exchanger] You really want to buy %d Zen with %d tax and pay %d WCoins?", Buy, Tax, NeededToSell);
			AddTab[gObj->m_Index].ExType = 31;  
			AddTab[gObj->m_Index].WantedNum = Buy; 
			//_beginthread(ExchangeTimer,0,NULL);	 		
			DWORD ThreadID;
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExchangeTimer, (void*)gObj->m_Index, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return true;
			}

			CloseHandle(hThread);	
			return true;
		}
		break;	 
	case 32:
		{
			if(AddTab[gObj->m_Index].ExType != -1)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't make other purchase. Answer previous please.");  
				return true;
			}
			if(CheckCommand(gObj, Config.ExchangePpZen, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 1, 0, "Exchanger", "/pcpoint-zen <num>", Msg))
			{				  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD Buy;
			int Sell = AddTab[gObj->m_Index].PCPlayerPoints;
			sscanf(Msg, "%d", &Buy);

			DWORD ConfigBuy = Config.PcPoint_Zen;
			DWORD MaxBuy = 2000000000; 

			if(Buy % ConfigBuy != 0)
			{							
				if(Buy > ConfigBuy)
				{
					DWORD Min = Buy - (Buy % ConfigBuy);
					DWORD Max = Min + ConfigBuy;
					MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You write not right value. You can write %d or %d.", Min, Max);  
					AddTab[gObj->m_Index].ExType = -1;
					return true;
				}
				else
				{
					MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You write not right value. You can write %d.", ConfigBuy);  
					AddTab[gObj->m_Index].ExType = -1;
					return true;
				}
			}  				  
			int NeededToSell = (Buy / ConfigBuy);	
			if(Sell < NeededToSell)
			{
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't have enough PCPoint. %d more need.", NeededToSell - Sell);  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			} 
			if(gObj->Money + Buy > MaxBuy)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't buy more Zen.");  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD ConfigTax = Config.ExchangeTax;
			float temp = ((Buy/100)*ConfigTax);
			DWORD Tax = ceil(temp);																															  
			MessageLog(1, cLog.c_Green, cLog.t_EXCHANGE, gObj, "[Exchanger] You really want to buy %d Zen with %d tax and pay %d PCPoint?", Buy, Tax, NeededToSell);
			AddTab[gObj->m_Index].ExType = 32;
			AddTab[gObj->m_Index].WantedNum = Buy; 
			DWORD ThreadID;
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExchangeTimer, (void*)gObj->m_Index, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return true;
			}

			CloseHandle(hThread);	
			return true;
		}
		break;	
	case 13:
		{
			if(AddTab[gObj->m_Index].ExType != -1)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't make other purchase. Answer previous please.");  
				return true;
			}
			if(CheckCommand(gObj, Config.ExchangeZenWc, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 1, 0, "Exchanger", "/zen-wcoin <num>", Msg))
			{				  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD Buy;
			int Sell = gObj->Money;
			sscanf(Msg, "%d", &Buy);

			DWORD ConfigBuy = Config.WCoin_Zen;
			DWORD MaxBuy = Config.MaximumWCPoints; 

			int NeededToSell = (Buy * ConfigBuy);	
			if(Sell < NeededToSell)
			{
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't have enough Zen. %d more need.", NeededToSell - Sell);  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			} 
			if(gObj->m_wCashPoint + Buy > MaxBuy)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't buy more WCoins.");  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Tax = ((NeededToSell/100)*ConfigTax);																															  
			MessageLog(1, cLog.c_Green, cLog.t_EXCHANGE, gObj, "[Exchanger] You really want to buy %d Wcoin and pay %d zen with %d zen tax?", Buy, NeededToSell, Tax);
			AddTab[gObj->m_Index].ExType = 13;
			AddTab[gObj->m_Index].WantedNum = NeededToSell; 
			DWORD ThreadID;
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExchangeTimer, (void*)gObj->m_Index, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return true;
			}

			CloseHandle(hThread);	
			return true;
		}
		break;
	case 23:
		{
			if(AddTab[gObj->m_Index].ExType != -1)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't make other purchase. Answer previous please.");  
				return true;
			}
			if(CheckCommand(gObj, Config.ExchangeZenPp, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 1, 0, "Exchanger", "/zen-pcpoint <num>", Msg))
			{				  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD Buy;
			int Sell = gObj->Money;
			sscanf(Msg, "%d", &Buy);

			DWORD ConfigBuy = Config.PcPoint_Zen;
			DWORD MaxBuy = Config.MaximumPCPoints; 

			int NeededToSell = (Buy * ConfigBuy);	
			if(Sell < NeededToSell)
			{
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't have enough Zen. %d more need.", NeededToSell - Sell);  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			} 
			if(AddTab[gObj->m_Index].PCPlayerPoints + Buy > MaxBuy)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't buy more PCPoints.");  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Tax = ((NeededToSell/100)*ConfigTax);																															  
			MessageLog(1, cLog.c_Green, cLog.t_EXCHANGE, gObj, "[Exchanger] You really want to buy %d PCPoints and pay %d zen with %d zen tax?", Buy, NeededToSell, Tax);
			AddTab[gObj->m_Index].ExType = 23;
			AddTab[gObj->m_Index].WantedNum = NeededToSell; 
			DWORD ThreadID;
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExchangeTimer, (void*)gObj->m_Index, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return true;
			}

			CloseHandle(hThread);
			return true;
		}
		break;
	case 12:
		{
			if(AddTab[gObj->m_Index].ExType != -1)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't make other purchase. Answer previous please.");  
				return true;
			}
			if(CheckCommand(gObj, Config.ExchangePpWc, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 1, 0, "Exchanger", "/pcpoint-wcoin <num>", Msg))
			{				  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD Buy;
			int Sell = AddTab[gObj->m_Index].PCPlayerPoints;
			sscanf(Msg, "%d", &Buy);

			DWORD ConfigBuy = Config.WCoin_PcPoint;
			DWORD MaxBuy = Config.MaximumWCPoints; 

			int NeededToSell = (Buy * ConfigBuy);	
			if(Sell < NeededToSell)
			{
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't have enough PCPoints. %d more need.", NeededToSell - Sell);  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			} 
			if(gObj->m_wCashPoint + Buy > MaxBuy)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You can't buy more WCoins.");  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}

			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Tax = ((NeededToSell/100)*ConfigTax);																															  
			MessageLog(1, cLog.c_Green, cLog.t_EXCHANGE, gObj, "[Exchanger] You really want to buy %d WCoins and pay %d PCPoints with %d PCPoints tax?", Buy, NeededToSell, Tax);
			AddTab[gObj->m_Index].ExType = 12;
			AddTab[gObj->m_Index].WantedNum = NeededToSell; 
			DWORD ThreadID;
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ExchangeTimer, (void*)gObj->m_Index, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return true;
			}

			CloseHandle(hThread);
			return true;
		}
		break;
	}	
	return true;
}

bool cChat::YesCommand(LPOBJ gObj, char *Msg)
{	 
	int TypeMarry;
	if(AddTab[gObj->m_Index].MarryType != -1)
		TypeMarry = AddTab[gObj->m_Index].MarryType;

	switch(TypeMarry)
	{					 
	case 0:
		Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_MARRY, gObj, "Devias Marriage Started!");
		//AddTab[gObj->m_Index].MarryType = 0;
		cMarry.NpcUse = false;
		break;
	case 1:
	case 2:
		if(strlen(Msg) > 1)
			Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_MARRY, gObj, "[%s] %s", gObj->Name, Msg);
		else																									
			Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_MARRY, gObj, "[%s] Yes!", gObj->Name);	 
		AddTab[gObj->m_Index].MarryType = 3;
		if(AddTab[cMarry.gObj1->m_Index].MarryType == 3 && AddTab[cMarry.gObj2->m_Index].MarryType == 3)
			cMarry.EndMarriage();		 
		cMarry.NpcUse = false;
		break;
			/*
	case 10:
		NPCMessageNear(cMarry.NpcObj, "Other partner come here please...");	

		//AddTab[gObj->m_Index].MarryType = 0;
		cMarry.NpcUse = false;
		break;

	case 11:
	case 12:																	
		AddTab[gObj->m_Index].MarryType = 13;

		if(AddTab[cMarry.gObj1->m_Index].MarryType == 13 && AddTab[cMarry.gObj2->m_Index].MarryType == 13)
			cMarry.EndDivorce();	
		else									 
			NPCMessageNear(cMarry.NpcObj, "Other partner come here please...");	

		cMarry.NpcUse = false;
		break;
		*/
	}
		  
	if(AddTab[gObj->m_Index].ResType != -1)
	{
		ResetSystem.MainReset(gObj, AddTab[gObj->m_Index].ResNpcObj);
		return true;
	}

	if(AddTab[gObj->m_Index].ExType != -1)
	{
		if(CheckCommand(gObj, Config.ExchangeCommands, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 0, 0, "Exchanger", "/yes", Msg))
		{				  
			AddTab[gObj->m_Index].ExType = -1;
			return true;
		}	  
		if(AddTab[gObj->m_Index].ExType == -1)
		{ 
			MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't make purchase. Make it before answer.");  
			return true;
		}
				   
		if(AddTab[gObj->m_Index].ExType == 21)
		{				
			DWORD Buy = AddTab[gObj->m_Index].WantedNum;					   
			DWORD ConfigBuy = Config.WCoin_PcPoint;
			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Sell = Buy/ConfigBuy;
			float Tax = ((Buy/100)*ConfigTax);	 
			DWORD Final = Buy - ceil(Tax);

			g_PointShop.IncreasePoints(gObj->m_Index, Final);	 
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s + '%d' WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, Final, gObj->Name);

			gObj->m_wCashPoint -= Sell;
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s - %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, Sell, gObj->AccountID);
        
			MessageLog(1, cLog.c_Yellow, cLog.t_EXCHANGE, gObj, "[Exchanger] You buy %d PCPoints for %d WCoins! Thx you!", Final, Sell);
			AddTab[gObj->m_Index].ExType = -1;
			return true;
		}
		else if(AddTab[gObj->m_Index].ExType == 32)
		{										   
			DWORD Buy = AddTab[gObj->m_Index].WantedNum;					   
			DWORD ConfigBuy = Config.PcPoint_Zen;
			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Sell = Buy/ConfigBuy;
			float Tax = ((Buy/100)*ConfigTax);	 
			DWORD Final = Buy - ceil(Tax);
				
			gObj->Money += Final; 
			GCMoneySend(gObj->m_Index, gObj->Money); 

			g_PointShop.DecreasePoints(gObj->m_Index, Sell);	 
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - '%d' WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, Sell, gObj->Name);

			MessageLog(1, cLog.c_Yellow, cLog.t_EXCHANGE, gObj, "[Exchanger] You buy %d Zen for %d PCPoints! Thx you!", Final, Sell);
			AddTab[gObj->m_Index].ExType = -1;
			return true;
		}
		else if(AddTab[gObj->m_Index].ExType == 31)
		{										  
			DWORD Buy = AddTab[gObj->m_Index].WantedNum;					   
			DWORD ConfigBuy = Config.WCoin_Zen;
			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Sell = Buy/ConfigBuy;
			float Tax = ((Buy/100)*ConfigTax);	 
			DWORD Final = Buy - ceil(Tax);

			gObj->Money += Final; 
			GCMoneySend(gObj->m_Index, gObj->Money); 

			gObj->m_wCashPoint -= Sell;
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s - %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, Sell, gObj->AccountID);
        
			MessageLog(1, cLog.c_Yellow, cLog.t_EXCHANGE, gObj, "[Exchanger] You buy %d Zen for %d WCoins! Thx you!", Final, Sell);
			AddTab[gObj->m_Index].ExType = -1;
			return true;
		}   
		else if(AddTab[gObj->m_Index].ExType == 12)
		{									
			DWORD Sell = AddTab[gObj->m_Index].WantedNum;			   
			DWORD ConfigBuy = Config.WCoin_PcPoint;
			DWORD ConfigTax = Config.ExchangeTax;	   					
			int Buy = Sell/ConfigBuy;	
			DWORD Tax = ((Sell/100)*ConfigTax);

			DWORD Final = Sell + Tax;

			g_PointShop.DecreasePoints(gObj->m_Index, Final);	 
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - %d WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, Final, gObj->Name);

			gObj->m_wCashPoint += Buy;
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s + %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, Buy, gObj->AccountID);
        
			MessageLog(1, cLog.c_Yellow, cLog.t_EXCHANGE, gObj, "[Exchanger] You buy %d WCoins for %d PCPoints! Thx you!", Buy, Final);
			AddTab[gObj->m_Index].ExType = -1;
			return true;
		}
		else if(AddTab[gObj->m_Index].ExType == 23)
		{										   
			DWORD Sell = AddTab[gObj->m_Index].WantedNum;					   
			DWORD ConfigBuy = Config.PcPoint_Zen;
			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Buy = Sell/ConfigBuy;
			DWORD Tax = ((Sell/100)*ConfigTax);	 
			DWORD Final = Sell + Tax;
				
			gObj->Money -= Final; 
			GCMoneySend(gObj->m_Index, gObj->Money); 

			g_PointShop.IncreasePoints(gObj->m_Index, Buy);	 
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s + '%d' WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, Buy, gObj->Name);

			MessageLog(1, cLog.c_Yellow, cLog.t_EXCHANGE, gObj, "[Exchanger] You buy %d PCPoints for %d Zen! Thx you!", Buy, Final);
			AddTab[gObj->m_Index].ExType = -1;
			return true;
		}
		else if(AddTab[gObj->m_Index].ExType == 13)
		{										  
			DWORD Sell = AddTab[gObj->m_Index].WantedNum;					   
			DWORD ConfigBuy = Config.WCoin_Zen;
			DWORD ConfigTax = Config.ExchangeTax;
			DWORD Buy = Sell/ConfigBuy;
			DWORD Tax = ((Sell/100)*ConfigTax);	 
			DWORD Final = Sell + Tax;

			gObj->Money -= Final; 
			GCMoneySend(gObj->m_Index, gObj->Money); 

			gObj->m_wCashPoint += Buy;
			MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s + %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, Buy, gObj->AccountID);
        
			MessageLog(1, cLog.c_Yellow, cLog.t_EXCHANGE, gObj, "[Exchanger] You buy %d WCoins for %d Zen! Thx you!", Buy, Final);
			AddTab[gObj->m_Index].ExType = -1;
			return true;
		}
	}
	return true;
}

bool cChat::NoCommand(LPOBJ gObj, char *Msg)
{			      					
	int TypeMarry;
	if(AddTab[gObj->m_Index].MarryType != -1)
		TypeMarry = AddTab[gObj->m_Index].MarryType;

	LPOBJ nObj = cMarry.NpcObj;

	
	if(AddTab[gObj->m_Index].ResType != -1)
	{									 
		Chat.Message(1, gObj->m_Index, "[Reset] You cancel reset");	 
		AddTab[gObj->m_Index].ResType = -1;
		return true;
	}

	switch(TypeMarry)
	{	  			
	case 0:												
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "You cancel marriage");	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You cancel marriage");	  //PRIVAT 


		if(cMarry.gObj1 != NULL)
			AddTab[cMarry.gObj1->m_Index].MarryType = -1;
		if(cMarry.gObj2 != NULL)
			AddTab[cMarry.gObj2->m_Index].MarryType = -1;
			
		cMarry.NpcUse = false;
		cMarry.fIndex = 0;
		cMarry.gObj1 = NULL;
		cMarry.gObj2 = NULL;
		cMarry.NpcObj = NULL;		
		break;	 
	case 1:
	case 2:
		if(strlen(Msg) > 1)
			Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_MARRY, gObj, "[%s] %s", gObj->Name, Msg);
		else																									   
			Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_MARRY, gObj, "[%s] No!", gObj->Name);

		NPCMessageNear(nObj, "Marriage canceled by %s!!!", gObj->Name);	
		Chat.MessageAllLog(0, 0, cLog.c_Green, cLog.t_MARRY, gObj, "[Marriage] Marriage canceled by %s!!!", gObj->Name);	 
	  

		if(cMarry.gObj1 != NULL)
			AddTab[cMarry.gObj1->m_Index].MarryType = -1;
		if(cMarry.gObj2 != NULL)
			AddTab[cMarry.gObj2->m_Index].MarryType = -1;
			
		cMarry.NpcUse = false;
		cMarry.fIndex = 0;
		cMarry.gObj1 = NULL;
		cMarry.gObj2 = NULL;
		cMarry.NpcObj = NULL;		
		break;
		/*
	case 10:												
		if(nObj == NULL)
			Chat.Message(1, gObj->m_Index, "You cancel divorce");	  //PRIVAT
		else																			  
			NPCMessage(gObj->m_Index, nObj, "You cancel divorce");	  //PRIVAT 


		if(cMarry.gObj1 != NULL)
			AddTab[cMarry.gObj1->m_Index].MarryType = -1;
		if(cMarry.gObj2 != NULL)
			AddTab[cMarry.gObj2->m_Index].MarryType = -1;
			
		cMarry.NpcUse = false;
		cMarry.fIndex = 0;
		cMarry.gObj1 = NULL;
		cMarry.gObj2 = NULL;
		cMarry.NpcObj = NULL;		
		break;	 
	case 11:
	case 12:
		NPCMessageNear(nObj, "Divorce canceled by %s!!!", gObj->Name);		  

		if(cMarry.gObj1 != NULL)
			AddTab[cMarry.gObj1->m_Index].MarryType = -1;
		if(cMarry.gObj2 != NULL)
			AddTab[cMarry.gObj2->m_Index].MarryType = -1;
			
		cMarry.NpcUse = false;
		cMarry.fIndex = 0;
		cMarry.gObj1 = NULL;
		cMarry.gObj2 = NULL;
		cMarry.NpcObj = NULL;		
		break;
		*/
	}

	if(AddTab[gObj->m_Index].ExType != -1)
	{
			if(CheckCommand(gObj, Config.ExchangeCommands, GMS.NONE, 0, 0, 0, Config.ExchangeMinLvl, 0, 0, "Exchanger", "/no", Msg))
			{				  
				AddTab[gObj->m_Index].ExType = -1;
				return true;
			}	  
			if(AddTab[gObj->m_Index].ExType == -1)
			{ 
				MessageLog(1, cLog.c_Red, cLog.t_EXCHANGE, gObj, "[Exchanger] You don't make purchase. Make it before answer.");  
				return true;
			} 
			MessageLog(1, cLog.c_Green, cLog.t_EXCHANGE, gObj, "[Exchanger] You canceled purchase.");  
			AddTab[gObj->m_Index].ExType = -1;
			return true;
	}
	return true;
}	   

unsigned long __stdcall ExchangeTimer(int Index)
{	
	int i;
	Chat.Message(1, Index, "[Exchanger] Write /yes or /no for answer. You have 30 secs.");	
	for(i = 0; i < 150; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ExType == -1) return 1;
	}

	Chat.Message(1, Index, "[Exchanger] You have 15 secs more for answer...");	   	
	for(i = 0; i < 50; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ExType == -1) return 1;
	}

	Chat.Message(1, Index, "[Exchanger] You have 10 secs more for answer...");		
	for(i = 0; i < 50; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ExType == -1) return 1;
	}

	Chat.Message(1, Index, "[Exchanger] You have 5 secs more for answer...");		
	for(i = 0; i < 20; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ExType == -1) return 1;
	}

	Chat.Message(1, Index, "[Exchanger] You have 3 secs more for answer...");		
	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ExType == -1) return 1;
	}

	Chat.Message(1, Index, "[Exchanger] You have 2 secs more for answer...");		
	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ExType == -1) return 1;
	}

	Chat.Message(1, Index, "[Exchanger] You have 1 sec more for answer...");		
	for(i = 0; i < 10; i++)
	{
		Sleep(100);			 
		if(AddTab[Index].ExType == -1) return 1;
	}

	Chat.Message(1, Index, "[Exchanger] Time is over. The deal canceled!");	
	AddTab[Index].ExType = -1;
	return 1;
}
			  	
bool cChat::AddCommands(LPOBJ gObj, char *Msg, int Type)
{		
	if(AddTab[gObj->m_Index].AddType != -1)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddStats] You can't use this command twice at the time!");
		return true;
	}
	switch(Type)
	{
	case 0:		  
		if(CheckCommand(gObj, Config.Commands.AddPointEnabled, GMS.NONE, Config.Commands.AddPriceZen, Config.Commands.AddPricePCPoint, Config.Commands.AddPriceWCoin, Config.Commands.AddPointLevelReq, 1, 0, "AddStats", "/addstr <num>", Msg))
			return true;
		break;
	case 1:	  
		if(CheckCommand(gObj, Config.Commands.AddPointEnabled, GMS.NONE, Config.Commands.AddPriceZen, Config.Commands.AddPricePCPoint, Config.Commands.AddPriceWCoin, Config.Commands.AddPointLevelReq, 1, 0, "AddStats", "/addagi <num>", Msg))
			return true;
		break;
	case 2:
		if(CheckCommand(gObj, Config.Commands.AddPointEnabled, GMS.NONE, Config.Commands.AddPriceZen, Config.Commands.AddPricePCPoint, Config.Commands.AddPriceWCoin, Config.Commands.AddPointLevelReq, 1, 0, "AddStats", "/addvit <num>", Msg))
			return true;
		break;
	case 3:
		if(CheckCommand(gObj, Config.Commands.AddPointEnabled, GMS.NONE, Config.Commands.AddPriceZen, Config.Commands.AddPricePCPoint, Config.Commands.AddPriceWCoin, Config.Commands.AddPointLevelReq, 1, 0, "AddStats", "/addene <num>", Msg))
			return true;
		break;
	case 4:
		if(CheckCommand(gObj, Config.Commands.AddPointEnabled, GMS.NONE, Config.Commands.AddPriceZen, Config.Commands.AddPricePCPoint, Config.Commands.AddPriceWCoin, Config.Commands.AddPointLevelReq, 1, 0, "AddStats", "/addcmd <num>", Msg))
			return true;
		break;
	}
	DWORD Points;
	sscanf(Msg,"%d",&Points);		  


	int MaxPoints = 32000;
	if(Config.Use65kStats >= 1)
		 MaxPoints = -536;	
				   
	int Stats;
	bool bResult = false;
	switch (Type)
	{
		case 0x00:
			Stats = gObj->Strength;  
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(gObj->DbClass == 0 || gObj->DbClass == 1 || gObj->DbClass == 2 || gObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Strength; 		
				else if(gObj->DbClass == 16 || gObj->DbClass == 17 || gObj->DbClass == 18 || gObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Strength; 		
				else if(gObj->DbClass == 32 || gObj->DbClass == 33 || gObj->DbClass == 34 || gObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Strength; 		
				else if(gObj->DbClass == 48 || gObj->DbClass == 49 || gObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Strength; 		
				else if(gObj->DbClass == 64 || gObj->DbClass == 65 || gObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Strength; 		
				else if(gObj->DbClass == 80 || gObj->DbClass == 81 || gObj->DbClass == 82 || gObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Strength; 		
			}
			break;
		case 0x01:
			Stats = gObj->Dexterity;  	 
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(gObj->DbClass == 0 || gObj->DbClass == 1 || gObj->DbClass == 2 || gObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Agility; 		
				else if(gObj->DbClass == 16 || gObj->DbClass == 17 || gObj->DbClass == 18 || gObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Agility; 		
				else if(gObj->DbClass == 32 || gObj->DbClass == 33 || gObj->DbClass == 34 || gObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Agility; 		
				else if(gObj->DbClass == 48 || gObj->DbClass == 49 || gObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Agility; 		
				else if(gObj->DbClass == 64 || gObj->DbClass == 65 || gObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Agility; 		
				else if(gObj->DbClass == 80 || gObj->DbClass == 81 || gObj->DbClass == 82 || gObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Agility; 			
			}
			break;
		case 0x02:	  
			Stats = gObj->Vitality;
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(gObj->DbClass == 0 || gObj->DbClass == 1 || gObj->DbClass == 2 || gObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Vitality; 		
				else if(gObj->DbClass == 16 || gObj->DbClass == 17 || gObj->DbClass == 18 || gObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Vitality; 		
				else if(gObj->DbClass == 32 || gObj->DbClass == 33 || gObj->DbClass == 34 || gObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Vitality; 		
				else if(gObj->DbClass == 48 || gObj->DbClass == 49 || gObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Vitality; 		
				else if(gObj->DbClass == 64 || gObj->DbClass == 65 || gObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Vitality; 		
				else if(gObj->DbClass == 80 || gObj->DbClass == 81 || gObj->DbClass == 82 || gObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Vitality; 		
			}
			break;	  
		case 0x03:
			Stats = gObj->Energy;	 			
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(gObj->DbClass == 0 || gObj->DbClass == 1 || gObj->DbClass == 2 || gObj->DbClass == 3) 
					MaxPoints = Config.MaxDW_Energy; 		
				else if(gObj->DbClass == 16 || gObj->DbClass == 17 || gObj->DbClass == 18 || gObj->DbClass == 19) 	
					MaxPoints = Config.MaxDK_Energy; 		
				else if(gObj->DbClass == 32 || gObj->DbClass == 33 || gObj->DbClass == 34 || gObj->DbClass == 35)   
					MaxPoints = Config.MaxELF_Energy; 		
				else if(gObj->DbClass == 48 || gObj->DbClass == 49 || gObj->DbClass == 50) 						
					MaxPoints = Config.MaxMG_Energy; 		
				else if(gObj->DbClass == 64 || gObj->DbClass == 65 || gObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Energy; 		
				else if(gObj->DbClass == 80 || gObj->DbClass == 81 || gObj->DbClass == 82 || gObj->DbClass == 83)   
					MaxPoints = Config.MaxSUM_Energy; 			
			}
			break;
		case 0x04:
			Stats = gObj->Leadership;	 	 		
			if(cProtection.CheckFeature(cProtection.MaxStats))
			{
				if(gObj->DbClass == 64 || gObj->DbClass == 65 || gObj->DbClass == 66) 	 
					MaxPoints = Config.MaxDL_Command; 		
			}
			break;
	}
 
	int MaxPointsTemp = MaxPoints; 
	if(MaxPoints > 32767)
		MaxPoints = -32768 + (MaxPoints - 32767);
																 	
	if((MaxPoints > 0) && (Stats >= MaxPoints || Stats < 0))
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddStats] You have max points for this type!!!");
		return true;								 
	}
	else if ((MaxPoints < 0) && (Stats < 0) && (Stats >= MaxPoints))
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddStats] You have max points for this type!!!");
		return true;
	}
				  
	int StatsTemp = Stats; 
	Stats += Points;
	#pragma warning(disable: 4018)
	if(gObj->LevelUpPoint < Points)
	{	
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddStats] You don't have enought points to add. Need %d more.", Points - gObj->LevelUpPoint);
		return true;
	}
	
	if((MaxPoints > 0) && (Stats > MaxPoints || Stats < 0))
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddStats] You can't add more than %d points for this type!!!", MaxPoints - StatsTemp);
		return true;								 
	}
	else if ((MaxPoints < 0) && (Stats < 0) && (Stats > MaxPoints))
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddStats] You can't add more than %d points for this type!!!", MaxPoints - StatsTemp);
		return true;
	}		
	
	TakeCommand(gObj, Config.Commands.AddPriceZen, Config.Commands.AddPricePCPoint, Config.Commands.AddPriceWCoin, "AddStats");
	AddTab[gObj->m_Index].AddType = Type;
	AddTab[gObj->m_Index].AddValue = Points;
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AddTimer, (void*)gObj->m_Index, 0, &ThreadID);

	if ( hThread == 0 )
	{
		cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
		return true;
	}

	CloseHandle(hThread);	
	return true;
}
			 
unsigned long __stdcall AddTimer(int Index)
{
	int Type = AddTab[Index].AddType;
	BYTE lpMsg[5] = {0xC1, 0x05, 0xF3, 0x06, Type};	
	int Sec = 0;					
	OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
					
	if(AddTab[Index].AddValue > 500)
	{
		float temp = AddTab[Index].AddValue / 200;
		int Time = ceil(temp);
		Chat.MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[AddStats] Adding stats started. Please wait %d sec until it ends.", Time);
	}
	while(AddTab[Index].AddValue > 0)
	{
		for(int i = 0; i<200; i++)
		{
			if(AddTab[Index].AddValue <= 0 || gObj->LevelUpPoint <= 0) continue;
			if(gObj->CloseCount >= 0) 
			{
				Chat.MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddStats] Adding stats canceled.");
				return 1;
			}
			CGLevelUpPointAdd(lpMsg, Index);
			AddTab[Index].AddValue--;
		}
		Sleep(1000);		
	}			  
	GCLevelUpMsgSend(Index, 0);	   
	Chat.MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[AddStats] Your stats successfully added!");	 
	AddTab[Index].AddValue = 0;
	AddTab[Index].AddType = -1;

	return 1;
}

bool cChat::LevelCommand(LPOBJ gObj, char *Msg)
{					
	MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "Level: %d, Master Level: %d", gObj->Level, gObj->MLevel);
	return true;
}

bool cChat::DropCommand(LPOBJ gObj, char *Msg)
{					
	if(CheckCommand(gObj, Config.Commands.IsDrop, GMS.cDrop, 0, 0, 0, 0, 3, 0, "Drop", "/drop <Value> <ItemCount> <ItemType> <ItemIndex> <ItemLvl> <ItemSkill> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>", Msg))
		return true;
					
	int ItemCount = 0;
	int ItemType = 0;
	int ItemIndex = 0;
	int ItemLevel = 0;
	int ItemSkill = 0;
	int ItemLuck = 0;
	int ItemOpt = 0;
	int ItemExc = 0;
	int ItemAncient = 0;

	char Target[11];
	int Index = 0;
	sscanf(Msg,"%s",&Target);
	Index = Util.GetPlayerIndex(Target);
	if(Index == -1)
	{	
		sscanf(Msg, "%d %d %d %d %d %d %d %d %d", &ItemCount, &ItemType, &ItemIndex, &ItemLevel, &ItemSkill, &ItemLuck, &ItemOpt, &ItemExc, &ItemAncient);
		int Item = ItemType * 512 + ItemIndex;
		if ((ItemCount < 0) || (ItemCount > 20))
			ItemCount = 1;
		if(ItemIndex < 0 || (ItemType < 0 || ItemType > 15) || (ItemLevel<0 || ItemLevel > 13) || (ItemOpt < 0 || ItemOpt > 7) || (ItemLuck < 0 || ItemLuck > 1) || (ItemSkill < 0 || ItemSkill > 1) || (ItemExc < 0 || ItemExc > 63) || (ItemAncient < 0 || ItemAncient > 40))
		{
			MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[Drop] Usage: /drop <ItemCount> <ItemType> <ItemIndex> <ItemLvl> <ItemSkill> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>");
			return true;
		}
		else
		{
			for (int i=0; i < ItemCount; i++ )
				ItemSerialCreateSend (gObj->m_Index, gObj->MapNumber, (int)gObj->X, (int)gObj->Y, Item, ItemLevel, 0, ItemSkill, ItemLuck, ItemOpt, gObj->m_Index, ItemExc, ItemAncient);
			MessageLog(1, cLog.c_Green, cLog.t_GM, gObj, "[Drop] %d Item Created %d %d %d %d %d %d %d %d - Success", ItemCount, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemLuck, ItemOpt, ItemExc, ItemAncient);
		}
	}
	else
	{	  
		sscanf(Msg, "%s %d %d %d %d %d %d %d %d %d", &Target, &ItemCount, &ItemType, &ItemIndex, &ItemLevel, &ItemSkill, &ItemLuck, &ItemOpt, &ItemExc, &ItemAncient);
		int Item = ItemType * 512 + ItemIndex;	 
		if ((ItemCount < 0) || (ItemCount > 20))
			ItemCount = 1;
		if(ItemIndex < 0 || (ItemType < 0 || ItemType > 15) || (ItemLevel<0 || ItemLevel > 13) || (ItemOpt < 0 || ItemOpt > 7) || (ItemLuck < 0 || ItemLuck > 1) || (ItemSkill < 0 || ItemSkill > 1) || (ItemExc < 0 || ItemExc > 63) || (ItemAncient < 0 || ItemAncient > 40))
		{
			MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[Drop] Usage: /drop <Name> <ItemCount> <ItemType> <ItemIndex> <ItemLvl> <ItemSkill> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>");
			return true;
		}
		else
		{			  
			OBJECTSTRUCT *gUbj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
			for (int i=0; i < ItemCount; i++ )
				ItemSerialCreateSend (gUbj->m_Index, gUbj->MapNumber, (int)gUbj->X, (int)gUbj->Y, Item, ItemLevel, 0, ItemSkill, ItemLuck, ItemOpt, gUbj->m_Index, ItemExc, ItemAncient);
			MessageLog(1, cLog.c_Green, cLog.t_GM, gObj, "[Drop] %d Item Created to %s %d %d %d %d %d %d %d %d - Success", ItemCount, gUbj->Name,ItemType, ItemIndex, ItemLevel, ItemSkill, ItemLuck, ItemOpt, ItemExc, ItemAncient);
			Message(1, gUbj->m_Index, "[Drop] You lucked by %s with %d items! Take them faster!", gObj->Name, ItemCount);
		}
	} 
	return true;
}


bool cChat::SetDropCommand(LPOBJ gObj, char *Msg)
{					
	if(CheckCommand(gObj, Config.Commands.IsDrop, GMS.cDrop, 0, 0, 0, 0, 1 , 0, "SetDrop", "/setdrop  <ItemIndex> <ItemLvl> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>", Msg))
		return true;

	int ItemIndex = 0;
	int ItemLevel = 0;
	int ItemLuck = 0;
	int ItemOpt = 0;
	int ItemExc = 0;
	int ItemAncient = 0;
	int ItemMin = 7;
	//MG Set
	if(ItemIndex == 15 ||ItemIndex == 20 ||ItemIndex == 23 ||ItemIndex == 32 ||ItemIndex == 37 ||ItemIndex == 47 ||ItemIndex == 48)ItemMin = 8;
	//
	char Target[11];
	int Index = 0;
	sscanf(Msg,"%s",&Target);
	Index = Util.GetPlayerIndex(Target);
	if(Index == -1)
	{	
		sscanf(Msg, "%d %d %d %d %d %d", &ItemIndex, &ItemLevel, &ItemLuck, &ItemOpt, &ItemExc, &ItemAncient);
		
		if(ItemIndex < 0 ||   (ItemLevel<0 || ItemLevel > 13) || (ItemOpt < 0 || ItemOpt > 7) || (ItemLuck < 0 || ItemLuck > 1) || (ItemExc < 0 || ItemExc > 63) || (ItemAncient < 0 || ItemAncient > 40))
		{
			MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetDrop] Usage: /setdrop <ItemIndex> <ItemLvl> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>");
			return true;
		}
		else
		{
			for(int i = ItemMin; i < 12; i++)
			{
				int Item = i * 512 + ItemIndex;
				int Rand1 = rand() % 3;
				int Rand2 = rand() % 3;
				ItemSerialCreateSend (gObj->m_Index, gObj->MapNumber, (int)gObj->X + Rand1, (int)gObj->Y + Rand2, Item, ItemLevel, 0, 0, ItemLuck, ItemOpt, gObj->m_Index, ItemExc, ItemAncient);
			}
			MessageLog(1, cLog.c_Green, cLog.t_GM, gObj, "[SetDrop] Item Created %d %d %d %d %d %d - Success", ItemIndex, ItemLevel, ItemLuck, ItemOpt, ItemExc, ItemAncient);
		}
	}
	else
	{	  
		sscanf(Msg, "%s %d %d %d %d %d %d", &Target, &ItemIndex, &ItemLevel, &ItemLuck, &ItemOpt, &ItemExc, &ItemAncient);
	
		if(ItemIndex < 0 || (ItemLevel<0 || ItemLevel > 13) || (ItemOpt < 0 || ItemOpt > 7) || (ItemLuck < 0 || ItemLuck > 1) || (ItemExc < 0 || ItemExc > 63) || (ItemAncient < 0 || ItemAncient > 40))
		{
			MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetDrop] Usage: /setdrop <Name> <ItemIndex> <ItemLvl> <ItemLuck> <ItemOpt> <ItemExc> <ItemAnc>");
			return true;
		}
		else
		{			  
			OBJECTSTRUCT *gUbj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
			for(int i = ItemMin; i < 12; i++)
			{
				int Item = i * 512 + ItemIndex;	 
				int Rand1 = rand() % 2;
				int Rand2 = rand() % 2;
				ItemSerialCreateSend (gUbj->m_Index, gUbj->MapNumber, (int)gUbj->X + Rand1, (int)gUbj->Y + Rand2, Item, ItemLevel, 0, 0, ItemLuck, ItemOpt, gUbj->m_Index, ItemExc, ItemAncient);
			}
			MessageLog(1, cLog.c_Green, cLog.t_GM, gObj, "[SetDrop] Items Created to %s %d %d %d %d %d %d - Success", gUbj->Name, ItemIndex, ItemLevel, ItemLuck, ItemOpt, ItemExc, ItemAncient);
			Message(1, gUbj->m_Index, "[SetDrop] You lucked by %s with items! Take them faster!", gObj->Name);
		}
	} 
	return true;
}
bool cChat::SetZenCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsSetZen, GMS.cSetZen, 0, 0, 0, 0, 2, 1, "SetZen", "/setzen <nick> <value>", Msg))
		return true;
					  
	int Value;
	char Target[11]; 	 
	sscanf(Msg, "%s %d", &Target, &Value);	 	

	if(Value < 0 || Value > 2000000000)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetZen] Value can't be less than 0 and more than 2000000000!");
		return true;
	}			 

	int Index = Util.GetPlayerIndex(Target); 
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	tObj->Money = Value;
	GCMoneySend (tObj->m_Index, Value);		

	MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetZen] You sucsessfully changed %s zen.", tObj->Name);
	MessageLog(1, cLog.c_Red, cLog.t_GM, tObj, "[SetZen] Your zen was changed to %d by %s.", Value, gObj->Name);
	return true;
}

bool cChat::SetChar(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsSetChar, GMS.cSetZen, 0, 0, 0, 0, 2, 1, "SetChar", "/setchar <Nick> <Zen> <PCPnt> <WCoins> <AddPnt> <lvl> <Prof>", Msg))
		return true;

	DWORD Zen = 0, PCPnt = 0, WCoin = 0, AddPnt = 0, Lvl = 0, Prof = 0;
	char Target[11]; 	 
	sscanf(Msg, "%s %d %d %d %d %d %d", &Target, &Zen, &PCPnt, &WCoin, &AddPnt, &Lvl, &Prof);

	if(Zen < 0 || Zen > 2000000000)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetChar] Zen can't be less than 0 and more than 2000000000!");
		return true;
	}	

	if(PCPnt < 0 || PCPnt > Config.MaximumPCPoints)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetChar] PCPnt can't be less than 0 and more than %d!", Config.MaximumPCPoints);
		return true;
	}	

	if(WCoin < 0 || WCoin > Config.MaximumWCPoints)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetChar] WCoin can't be less than 0 and more than %d!", Config.MaximumWCPoints);
		return true;
	}	

	int MaximumAddPnt;
	if(Config.Commands.MaxAddedStats == 0)MaximumAddPnt = 32000;
	else
		MaximumAddPnt = 65000;

	if(AddPnt < 0 || AddPnt > MaximumAddPnt)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetChar] AddPnt can't be less than 0 and more than %d!", MaximumAddPnt);

		return true;
	}	

	if(Lvl < 0 || Lvl > Config.Commands.MaxLvl)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetChar] Lvl can't be less than 0 and more than %d!", Config.Commands.MaxLvl);
		return true;
	}

	if(Prof < 0 || Prof > 3)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetChar] Lvl can't be less than 0 and more than 3!");
		return true;
	}

	int Index = Util.GetPlayerIndex(Target); 
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	if(Zen > 0)
	{
		tObj->Money = Zen;
		GCMoneySend (tObj->m_Index, Zen);
	}

	if(PCPnt >0 )
	{
		g_PointShop.IncreasePoints(tObj->m_Index, PCPnt);	 
		MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %s - %d WHERE Name= '%s'",MySQL.szDatabase, Config.PCPointsColumn, Config.PCPointsColumn, PCPnt, tObj->Name);
	}

	if(WCoin > 0)
	{
		tObj->m_wCashPoint += WCoin;
		MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = %s + %d WHERE memb___id = '%s'",MySQL.szDatabase2, Config.WCoinsColumn, Config.WCoinsColumn, WCoin, tObj->AccountID);
	}

	if(AddPnt > 0)
	{
		tObj->LevelUpPoint += AddPnt;
	}

	if(Lvl > 0)
	{
		tObj->Level	= Lvl;
	}

	if(Prof > 0)
	{
		switch(Prof)
		{
		case 0:
			break;
		case 1:
			if(gObj->DbClass >= 0 && gObj->DbClass <= 3)gObj->DbClass = 0;
			if(gObj->DbClass >= 16 && gObj->DbClass <= 19)gObj->DbClass = 16;
			if(gObj->DbClass >= 32 && gObj->DbClass <= 35)gObj->DbClass = 32;
			if(gObj->DbClass >= 48 && gObj->DbClass <= 50)gObj->DbClass = 48;
			if(gObj->DbClass >= 64 && gObj->DbClass <= 66)gObj->DbClass = 64;
			if(gObj->DbClass >= 80 && gObj->DbClass < 83)gObj->DbClass = 80;
			else
				break;
		case 2:
			if(gObj->DbClass >= 0 && gObj->DbClass <= 3)gObj->DbClass = 1;
			if(gObj->DbClass >= 16 && gObj->DbClass <= 19)gObj->DbClass = 17;
			if(gObj->DbClass >= 32 && gObj->DbClass <= 35)gObj->DbClass = 33;
			if(gObj->DbClass >= 80 && gObj->DbClass < 83)gObj->DbClass = 81;
			else
				break;
		case 3:
			if(gObj->DbClass >= 0 && gObj->DbClass <= 3)gObj->DbClass = 3;
			if(gObj->DbClass >= 16 && gObj->DbClass <= 19)gObj->DbClass = 19;
			if(gObj->DbClass >= 32 && gObj->DbClass <= 35)gObj->DbClass = 35;
			if(gObj->DbClass >= 48 && gObj->DbClass <= 50)gObj->DbClass = 50;
			if(gObj->DbClass >= 64 && gObj->DbClass <= 66)gObj->DbClass = 66;
			if(gObj->DbClass >= 80 && gObj->DbClass < 83)gObj->DbClass = 83;
			else
				break;
		}
	}
	MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetChar] You sucsessfully changed %s character.", tObj->Name);
	MessageLog(1, cLog.c_Red, cLog.t_GM, tObj, "[SetChar] Your character was edited by %s, you must relogin!", gObj->Name);

	return true;
}												

bool cChat::SetPKCommand(LPOBJ gObj, char *Msg)
{								 			
	if(CheckCommand(gObj, Config.Commands.IsSetPK, GMS.cSetPK, 0, 0, 0, 0, 2, 1, "SetPK", "/setpk <nick> <pklvl>", Msg))
		return true;

	char Target[11];
	int SetLevel;
	sscanf(Msg,"%s %d",&Target, &SetLevel);		

	if(SetLevel < 0 || SetLevel > 100)
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetPK] PK lvl can't be less than 0 and more than 100!");
		return true;
	}			 

	int Index = Util.GetPlayerIndex(Target); 
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	tObj->m_PK_Level = SetLevel;
	if(SetLevel >= 3)
		tObj->m_PK_Count = SetLevel - 3;
	GCPkLevelSend(tObj->m_Index,SetLevel);	 

	MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[SetPK] You sucsessfully changed %s pk.", tObj->Name);
	MessageLog(1, cLog.c_Red, cLog.t_GM, tObj, "[SetPK] Your pk was changed to %d by %s.", SetLevel, gObj->Name);
	return true;
}

bool cChat::PKClearCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.ClearCommand.Enabled, GMS.NONE, 0, 0, 0, 0, 0, 0, "PKClear", "/pkclear", Msg))
		return true;

	if(GMS.IsCommand(GMS.cPkClear, gObj->Name))
	{
		int spaces=0;			
		char Target[11];
		for(unsigned int i=0;i < strlen(Msg);i++)
			if(Msg[i]==' ')spaces++;		
		sscanf(Msg,"%s",&Target);
		if(strlen(Msg) <= 0 || spaces != 1 || strlen(Target) > 11)
		{  	  
			if (gObj->m_PK_Level <=3)
			{												
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[PkClear] Your are not pk!");			
				return true;
			}					  
			gObj->m_PK_Level = 3;
			gObj->m_PK_Count = 0;
			GCPkLevelSend (gObj->m_Index, 3);	 
			MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[PkClear] Your Pk sucsessfully cleared!");
			return true;
		}
			
		if(CheckCommand(gObj, Config.ClearCommand.Enabled, GMS.NONE, 0, 0, 0, 0, 1, 1, "PKClear", "/pkclear (nick)", Msg))
			return true;
		   
		int Index = Util.GetPlayerIndex(Target); 
		OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
			  
		if (tObj->m_PK_Level <=3)
		{												
			MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[PkClear] %s is not pk!", tObj->Name);			
			return true;
		}	 				   
		tObj->m_PK_Level = 3;
		tObj->m_PK_Count = 0;
		GCPkLevelSend (tObj->m_Index, 3);	 															 
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[PkClear] You sucsessfully clear %s pk.", tObj->Name);
		MessageLog(1, cLog.c_Red, cLog.t_GM, tObj, "[PkClear] Your pk was cleared by %s.", gObj->Name);	
	}
	else if(!Config.ClearCommand.OnlyForGm)
	{
		int PriceZen;
		int PricePcPoint;
		int PriceWCoin;
		switch(Config.ClearCommand.Type)
		{	   
		case 1:	
			PriceZen = (Config.ClearCommand.PriceZen * gObj->m_PK_Count); 		 
			PricePcPoint = (Config.ClearCommand.PricePcPoints * gObj->m_PK_Count);
			PriceWCoin = (Config.ClearCommand.PriceWCoins * gObj->m_PK_Count);
			break;
		case 2:	
			PriceZen = Config.ClearCommand.PriceZenForAll;			 
			PricePcPoint = Config.ClearCommand.PricePcPointsForAll;
			PriceWCoin = Config.ClearCommand.PriceWCoinsForAll;
			break;
		case 0: 
			PriceZen = 0;					 
			PricePcPoint = 0;
			PriceWCoin = 0;
			break;
		}
					   
		if (gObj->m_PK_Level <=3)
		{												
			MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[PkClear] Your are not pk!");			
			return true;
		}			
		if(CheckCommand(gObj, Config.ClearCommand.Enabled, GMS.NONE, PriceZen, PricePcPoint, PriceWCoin, Config.ClearCommand.LevelReq, 0, 0, "PKClear", "/pkclear", Msg))
			return true;
			
		TakeCommand(gObj, PriceZen, PricePcPoint, PriceWCoin, "PKClear");	
		gObj->m_PK_Level = 3;
		gObj->m_PK_Count = 0;
		GCPkLevelSend (gObj->m_Index, 3);	 
		MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[PkClear] Your Pk sucsessfully cleared!");
	}
	return true;				
}
			 
bool cChat::ResetCommand(LPOBJ gObj, char *Msg)
{  
	if(Config.IsResetSystem != 1)
	{			
		MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[ResetSystem] Command disabled!");
		return true;
	}				 
	ResetSystem.Reset(gObj, NULL);
	return true;
}
		 	
bool cChat::SkinCommand(LPOBJ gObj, char *Msg)
{  
	if(CheckCommand(gObj, Config.Commands.SkinEnabled, GMS.NONE, 0, 0, 0, 0, 1, 0, "Skin", "/skin <num>", Msg))
		return true;

	if(GMS.IsCommand(GMS.cSkin, gObj->Name))
	{
		int spaces = 0, NumSkin = 0;			
		char Target[11];
		for(unsigned int i=0;i < strlen(Msg);i++)
			if(Msg[i]==' ')spaces++;		
		sscanf(Msg,"%s %d",&Target, &NumSkin);
		if(strlen(Msg) <= 0 || spaces != 2 || strlen(Target) > 11)
		{  	  									 
			sscanf(Msg,"%d", &NumSkin);	 
			gObj->m_Change = NumSkin;
			gObjViewportListProtocolCreate(gObj);
			MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[Skin] Your Skin sucsessfully changed!");
			return true;
		}
			
		if(CheckCommand(gObj, Config.Commands.SkinEnabled, GMS.NONE, 0, 0, 0, 0, 1, 1, "Skin", "/skin <name> <num>", Msg))
			return true;
		   
		int Index = Util.GetPlayerIndex(Target); 
		OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
			  												   		
		tObj->m_Change = NumSkin;
		gObjViewportListProtocolCreate(tObj);															 
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj, "[Skin] You sucsessfully change %s Skin.", tObj->Name);
		MessageLog(1, cLog.c_Red, cLog.t_GM, tObj, "[Skin] Your Skin was changed by %s.", gObj->Name);	
	}
	else if(!Config.Commands.SkinOnlyForGm)
	{
		if(CheckCommand(gObj, Config.Commands.SkinEnabled, GMS.NONE, Config.Commands.SkinPriceZen, Config.Commands.SkinPricePcPoint, Config.Commands.SkinPriceWCoin, Config.Commands.SkinLevelReq, 1, 0, "Skin", "/skin <num>", Msg))
			return true;
		TakeCommand(gObj, Config.Commands.SkinPriceZen, Config.Commands.SkinPricePcPoint, Config.Commands.SkinPriceWCoin, "Skin");	
		int NumSkin;
		sscanf(Msg,"%d", &NumSkin);	 
		gObj->m_Change = NumSkin;
		gObjViewportListProtocolCreate(gObj);
		MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[Skin] Your Skin sucsessfully changed!");
	}
	return true;				
}

bool cChat::GmoveCommand(LPOBJ gObj, char *Msg)
{					
	if(CheckCommand(gObj, Config.Commands.IsGmove, GMS.cGmove, 0, 0, 0, 0, 3, 0, "Gmove", "/gmove <map> <x> <y>", Msg))
		return true;
					
	char Target[11];   
	char Map[250];
	int x,y;

	int Index = 0;
	sscanf(Msg,"%s",&Target);
	Index = Util.GetPlayerIndex(Target);
	LPOBJ tObj;
	if(Index == -1)
	{	
		sscanf(Msg,"%s %d %d", &Map, &x, &y);
		tObj = gObj;
	}
	else
	{	  
		sscanf(Msg,"%s %s %d %d",&Target, &Map, &x, &y); 
		Index = Util.GetPlayerIndex(Target);  
		tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
	} 

	int RMap;
	if(_strcmpi(Map, "0") == 0 || _strcmpi(Map, "Lorencia") == 0) RMap = 0;
	else if	(_strcmpi(Map, "1") == 0 || _strcmpi(Map, "Dungeon") == 0) RMap = 1;
	else if	(_strcmpi(Map, "2") == 0 || _strcmpi(Map, "Devias") == 0) RMap = 2;
	else if (_strcmpi(Map, "3") == 0 || _strcmpi(Map, "Noria") == 0) RMap = 3;
	else if (_strcmpi(Map, "4") == 0 || _strcmpi(Map, "Losttower") == 0) RMap = 4;
	else if (_strcmpi(Map, "6") == 0 || _strcmpi(Map, "Stadium") == 0 || _strcmpi(Map, "Arena") == 0) RMap = 6;
	else if (_strcmpi(Map, "7") == 0 || _strcmpi(Map, "Atlans") == 0) RMap = 7;
	else if (_strcmpi(Map, "8") == 0 || _strcmpi(Map, "Tarkan") == 0) RMap = 8;		
	else if (_strcmpi(Map, "10") == 0 || _strcmpi(Map, "Icarus") == 0) RMap = 10;			  
	else if (_strcmpi(Map, "24") == 0 || _strcmpi(Map, "kalima1") == 0) RMap = 24;
	else if (_strcmpi(Map, "25") == 0 || _strcmpi(Map, "kalima2") == 0) RMap = 25; 
	else if (_strcmpi(Map, "26") == 0 || _strcmpi(Map, "kalima3") == 0) RMap = 26;		
	else if (_strcmpi(Map, "27") == 0 || _strcmpi(Map, "kalima4") == 0) RMap = 27;   
	else if (_strcmpi(Map, "28") == 0 || _strcmpi(Map, "kalima5") == 0) RMap = 28;
	else if (_strcmpi(Map, "29") == 0 || _strcmpi(Map, "kalima6") == 0) RMap = 29;   
	else if (_strcmpi(Map, "30") == 0 || _strcmpi(Map, "valleyofloren") == 0) RMap = 30;
	else if (_strcmpi(Map, "31") == 0 || _strcmpi(Map, "landoftrial") == 0 || _strcmpi(Map, "landsoftrial") == 0) RMap = 31;			   
	else if (_strcmpi(Map, "33") == 0 || _strcmpi(Map, "aida") == 0) RMap = 33;			   
	else if (_strcmpi(Map, "34") == 0 || _strcmpi(Map, "crywolf") == 0) RMap = 34;			   
	else if (_strcmpi(Map, "36") == 0 || _strcmpi(Map, "kalima7") == 0) RMap = 36;			   
	else if (_strcmpi(Map, "37") == 0 || _strcmpi(Map, "kanturu") == 0) RMap = 37;			   
	else if (_strcmpi(Map, "38") == 0 || _strcmpi(Map, "kanturutruins") == 0 || _strcmpi(Map, "kantrutruins") == 0) RMap = 38;			   
	else if (_strcmpi(Map, "39") == 0 || _strcmpi(Map, "kantrutower") == 0 || _strcmpi(Map, "kanturutower") == 0) RMap = 39;			   
	else if (_strcmpi(Map, "40") == 0 || _strcmpi(Map, "silent") == 0) RMap = 40;			   
	else if (_strcmpi(Map, "41") == 0 || _strcmpi(Map, "balgasbarrack") == 0) RMap = 41;			   
	else if (_strcmpi(Map, "42") == 0 || _strcmpi(Map, "balgasrefuge") == 0) RMap = 42;			   
	else if (_strcmpi(Map, "51") == 0 || _strcmpi(Map, "elbeland") == 0) RMap = 51;			   
	else if (_strcmpi(Map, "56") == 0 || _strcmpi(Map, "swampofcalmness") == 0 || _strcmpi(Map, "calmness") == 0) RMap = 56;			   
	else if (_strcmpi(Map, "57") == 0 || _strcmpi(Map, "raklion") == 0) RMap = 57;			   
	else if (_strcmpi(Map, "58") == 0 || _strcmpi(Map, "raklionboss") == 0) RMap = 58;			   
	else if (_strcmpi(Map, "62") == 0 || _strcmpi(Map, "santatown") == 0 || _strcmpi(Map, "santa") == 0) RMap = 62;			   
	else if (_strcmpi(Map, "63") == 0 || _strcmpi(Map, "vulcanus") == 0) RMap = 63;
	else if (_strcmpi(Map, "64") == 0 || _strcmpi(Map, "coliseum") == 0) RMap = 64;
	else
	{
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj,"[GMove] Such map doesn't exist!");	
		return true;
	} 
	if(x > 255 || y > 255 || x < 0 || y < 0)
	{	 								   
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj,"[GMove] Such coords doesn't exist!");	
		return true;
	}  			
	gObjTeleport(tObj->m_Index, RMap, x, y);		
	if(tObj == gObj)		 
		MessageLog(1, cLog.c_Green, cLog.t_GM, gObj,"[GMove] You sucsessfully moved to %s %d %d!", Map, x, y);
	else
	{	   
		MessageLog(1, cLog.c_Green, cLog.t_GM, gObj,"[GMove] You sucsessfully move %s to %s %d %d!", tObj->Name, Map, x, y);
		MessageLog(1, cLog.c_Green, cLog.t_GM, tObj,"[GMove] You sucsessfully moved to %s %d %d by %s!", Map, x, y, gObj->Name);
	}
	return true;
}

bool cChat::WareCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsMultyVault, GMS.NONE, Config.Commands.ZenForChange, Config.Commands.PcPointForChange, Config.Commands.WCoinForChange, 0, 1, 0, "Ware", "/ware <num>", Msg))
		return true;   	

	if(Config.Commands.NumberOfVaults == 1)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Ware] You can use only 1 vault!");
		return true;
	}
				  
	if(gObj->WarehouseSave != 0)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Ware] Close your vault fisrt!");
		return true;
	}
	int WantSlot;
	sscanf(Msg, "%d", &WantSlot);

	MySQL.Execute("SELECT UsedSlot FROM [%s].[dbo].[warehouse] WHERE AccountID = '%s'",MySQL.szDatabase, gObj->AccountID);
	int UsedSlot = MySQL.GetInt();

	if(UsedSlot == NULL || UsedSlot == 0)
	{			   
		MySQL.Execute("UPDATE [%s].[dbo].[warehouse] SET Items01 = Items WHERE AccountID = '%s'",MySQL.szDatabase, gObj->AccountID);
	}
	
	if(WantSlot < 1 || WantSlot > Config.Commands.NumberOfVaults)
	{		  
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Ware] You can use from 1 to %d vaults!", Config.Commands.NumberOfVaults);
		return true;
	}

	if(UsedSlot == WantSlot)		 
	{		  
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Ware] You need to chose other vault number!");
		return true;
	}
		 			
	TakeCommand(gObj, Config.Commands.ZenForChange, Config.Commands.PcPointForChange, Config.Commands.WCoinForChange, "Ware");																								  
	
	MySQL.Execute("UPDATE [%s].[dbo].[warehouse] SET Items%02d = Items WHERE AccountID = '%s'",MySQL.szDatabase, UsedSlot, gObj->AccountID);
	MySQL.Execute("UPDATE [%s].[dbo].[warehouse] SET Items = Items%02d WHERE AccountID = '%s'",MySQL.szDatabase, WantSlot, gObj->AccountID);
	MySQL.Execute("UPDATE [%s].[dbo].[warehouse] SET UsedSlot = %d WHERE AccountID = '%s'",MySQL.szDatabase, WantSlot, gObj->AccountID);

	MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Ware] You sucsessfully change vault from %d to %d!", UsedSlot, WantSlot);
	return true;
}
									  
unsigned long __stdcall ShowGMOnline(int Index)
{
	for(int i = OBJECT_MIN; i <= OBJECT_MAX; i++)
	{  	 
		OBJECTSTRUCT *gObj = (OBJECTSTRUCT*)OBJECT_POINTER(i);	 
		if(gObj->Connected < 3) continue; 	
		if(!GMS.IsGMBD(gObj->Name)) continue;			   
		if(GMS.IsAdmin(gObj->Name) == 1)
			Chat.Message(1, Index, "[ADMIN] %s", gObj->Name);
		else if(GMS.IsAdmin(gObj->Name) == 2)
			Chat.Message(1, Index, "[GM] %s", gObj->Name);
		else if(GMS.IsAdmin(gObj->Name) == 0) continue;
		Sleep(1000);
	}
	return 1;
}			  

bool cChat::StatusCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsStatus, GMS.cStatus, 0, 0, 0, 0, 1, 1, "Status", "/status <name>", Msg))
		return true;

	char Target[11];
	sscanf(Msg,"%s",&Target);
	int Index = Util.GetPlayerIndex(Target);   

	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	MessageLog(0, cLog.c_Green, cLog.t_GM, gObj, "IP Address: %s",tObj->Ip_addr);
	MessageLog(0, cLog.c_Green, cLog.t_GM, gObj, "Account: %s | Character: %s",tObj->AccountID,tObj->Name);
	MessageLog(0, cLog.c_Green, cLog.t_GM, gObj, "Level: %d | Zen: %d",tObj->Level,tObj->Money);
	MessageLog(0, cLog.c_Green, cLog.t_GM, gObj, "Map: %s (%d)(%d,%d)",g_Happy.GetMapName(tObj->MapNumber),tObj->MapNumber,tObj->X,tObj->Y);

	if (AddTab[tObj->m_Index].IsMarried)
		MessageLog(0, cLog.c_Green, cLog.t_GM, gObj, "Married on %s | Resets: %d", AddTab[tObj->m_Index].MarryName, AddTab[tObj->m_Index].Resets);
	else																								 
		MessageLog(0, cLog.c_Green, cLog.t_GM, gObj, "Not married | Resets: %d", AddTab[tObj->m_Index].Resets);

	MessageLog(1, cLog.c_Green, cLog.t_GM, tObj, "[Status][GM] %s get your status!", gObj->Name);	  
	return true;
}

bool cChat::OnlineCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsOnline, GMS.NONE, 0, 0, 0, 0, 0, 0, "Online", "/online", Msg))
		return true;

	int totGMs = Util.GetOnlineGMCount();
	int totPlayers = Util.GetOnlineCount() - totGMs;
	MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[ONLINE]: %d Player(s).",totPlayers);
	DWORD ThreadID;
	HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShowGMOnline, (void*)gObj->m_Index, 0, &ThreadID);

	if ( hThread == 0 )
	{
		cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
		return true;
	}

	CloseHandle(hThread);
	return true;
}

bool cChat::ReloadCommand(LPOBJ g0bj, char *Msg)
{
	if(CheckCommand(g0bj, 1, GMS.cReload, 0, 0, 0, 0, 1, 0, "Reload", "/reload <number>", Msg))
		return true; 

		int NumberReload;
		sscanf(Msg,"%d",&NumberReload);
		switch (NumberReload)
		{
		case 0:
			{
				Config.LoadAll();
				MoveReq.MoveReqLoad();
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, g0bj, "[Reload] RMST Data & MoveReq Reloaded.");
				break;
			}
		case 1:
			{
				GameMonsterAllCloseAndReload();
#ifdef _GameServer_
				ReadMonsterAdd();
#endif
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, g0bj, "[Reload] Monsters Reloaded.");
				break;
			}
		case 2:
			{
				pShopDataLoad();
				g_PointShop.Init();	
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, g0bj, "[Reload] Shops & PCPointShop Reloaded.");
				break;
			}
		case 3:
			{
				if (Config.IsGMSystem)
				{
					GMS.LoadGMSystem();
					MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, g0bj, "[Reload] GMSystem Reloaded.");
				}
				else
					Message(1,g0bj->m_Index,"[Reload] GMSystem is disabled");
				break;
			}
		case 4:
			{
				cexDrop.LoadConfig();		// reload Drop config
#ifdef _GameServer_
				cexDropEvent.updateBC();	// reload BC drop config
				cexDropEvent.updateIT();	// reload IT drop config
				cexDropEvent.updateWW();	// reload WW drop config
				cexDropEvent.updateCC();	// reload CC drop config
#endif
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, g0bj, "[Reload] DropSystem`s Reloaded.");
				break;
			}
		case 5:
			{
				IP.LoadIpBlock();
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, g0bj, "[Reload] IPBlock Reloaded.");
				break;
			}
		case 6:
			{
				Config.LoadAll();
				GameMonsterAllCloseAndReload();
				pShopDataLoad();
				IP.LoadIpBlock();

				if (Config.IsGMSystem) GMS.LoadGMSystem();
				
				cexDrop.LoadConfig();		// reload Drop config
#ifdef _GameServer_
				ReadMonsterAdd();
				cexDropEvent.updateBC();	// reload BC drop config
				cexDropEvent.updateIT();	// reload IT drop config
				cexDropEvent.updateWW();	// reload WW drop config
				cexDropEvent.updateCC();	// reload CC drop config
#endif
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, g0bj, "[Reload] All Options Reloaded.");
				break;
			}
		default:
			{
				Message(1,g0bj->m_Index,"[Reload] Usage: /reload <Number>");
				break;
			}

		} 
	return true;
}

bool cChat::EvoCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsEvo, GMS.NONE, Config.Commands.EvoPriceZen,  Config.Commands.EvoPricePcPoint, Config.Commands.EvoPriceWCoin, Config.Commands.EvoLvlReq, 0, 0, "Evo", "/evo", Msg))
	 	return true;

	if(gObj->DbClass == 1 || gObj->DbClass == 17 || gObj->DbClass == 33 || gObj->DbClass == 48 || gObj->DbClass == 64 || gObj->DbClass == 81)
	{	
		TakeCommand(gObj, Config.Commands.EvoPriceZen,  Config.Commands.EvoPricePcPoint, Config.Commands.EvoPriceWCoin, "Evo");		
		gObj->DbClass += 2;
		BYTE btClass = (gObj->Class * 32) + 24;
		GCSendQuestPrize(gObj->m_Index, 204, btClass);
		MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj,"[Evo] Congratulations you've successfully pass third quest!");
    }
	else if(gObj->DbClass == 3 || gObj->DbClass == 19 || gObj->DbClass == 35 || gObj->DbClass == 50 || gObj->DbClass == 66 || gObj->DbClass == 83)
	{
		MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj,"[Evo] You have already finished third quest!");
    }
	else
	{
		MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj,"[Evo] You need to pass seccond quest!");
	}
	return true;
}

bool cChat::IsMarryCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.IsMarry, GMS.NONE, 0, 0, 0, 0, 1, 1, "Marry", "/ismarry <name>", Msg))
	return true;

	char Target[11];
	sscanf(Msg,"%s",&Target);
	int Index = Util.GetPlayerIndex(Target);   
			
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	if (AddTab[tObj->m_Index].IsMarried == 1)
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[Marry] %s is married!!!", tObj->Name);
		else
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[Marry] %s is not married!!!", tObj->Name);
	return true;
}
	
bool cChat::MarryOnlineCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.MarryOnline, GMS.NONE, 0, 0, 0, 0, 0, 0, "Marry", "/marryonline", Msg))
	return true;

	if(AddTab[gObj->m_Index].IsMarried == 0)
			{														   
				MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Marry] You are not married!");
				return true;
			}
	if(gObjGetIndex(AddTab[gObj->m_Index].MarryName) == -1)		
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[Marry]Your husband/wife is offline");
		else
				MessageLog(1, cLog.c_Blue, cLog.t_COMMANDS, gObj, "[Marry] Your husband/wife is now online!");
	return true;
}

bool cChat::MarryTraceCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Commands.MarryTrace, GMS.NONE, 0, 0, 0, Config.Commands.MarryTraceLvl, 0, 0, "Marry", "/marrytrace", Msg))
	return true;

		if(AddTab[gObj->m_Index].IsMarried == 0 || gObjGetIndex(AddTab[gObj->m_Index].MarryName) == -1)
			{														   
				MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Marry] You are not married or your marriage is offline!");
				return true;
			}
		int Index = gObjGetIndex(AddTab[gObj->m_Index].MarryName);
		OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);
		if (( tObj->MapNumber >= 11 && tObj->MapNumber <= 17) 
			|| tObj->MapNumber == 52  || (tObj->MapNumber >= 18 && tObj->MapNumber <= 23) 
			|| tObj->MapNumber == 53 ||  tObj->MapNumber == 9 || tObj->MapNumber == 32 || (tObj->MapNumber >= 45 && tObj->MapNumber <= 50))
		{
			MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Marry] Your marriage is inside event map, you cannot teleport");
			return false;
		}

	
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Marry] You successfully moved to %s!", tObj->Name);
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, tObj, "[Marry] %s successfully moved to you!", gObj->Name);
		gObjTeleport(gObj->m_Index, tObj->MapNumber, (int)tObj->X, (int)tObj->Y);	
		return true;
}

bool cChat::MarryDivorce(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, Config.Marry.IsDivorce, GMS.NONE, Config.Marry.DivorceCostZen, Config.Marry.DivorceCostPCPoint , Config.Marry.DivorceCostWCoin, Config.Commands.MarryTraceLvl, 0, 0, "Marry", "/marrytrace", Msg))
	return true;

	if(AddTab[gObj->m_Index].IsMarried == 0 )
	{														   
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Marry] You are not married!");
		return true;
	}
	int Index = gObjGetIndex(AddTab[gObj->m_Index].MarryName);
	OBJECTSTRUCT *tObj = (OBJECTSTRUCT*)OBJECT_POINTER(Index);

	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET IsMarried=0, MarryName = '' WHERE Name = '%s'",MySQL.szDatabase, gObj->Name);	
	MySQL.Execute("UPDATE [%s].[dbo].[Character] SET IsMarried=0, MarryName = '' WHERE Name = '%s'",MySQL.szDatabase, AddTab[gObj->m_Index].MarryName);	

	AddTab[gObj->m_Index].IsMarried = 0;
	if(gObjGetIndex(AddTab[gObj->m_Index].MarryName) != -1)
	AddTab[tObj->m_Index].IsMarried = 0;	

	TakeCommand(gObj, Config.Marry.DivorceCostZen,  Config.Marry.DivorceCostPCPoint, Config.Marry.DivorceCostWCoin, "Marry");
	MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Marry] You divorced with %s!", tObj->Name);
	if(gObjGetIndex(AddTab[gObj->m_Index].MarryName) != -1)
	MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, tObj, "[Marry] You divorced with %s!", gObj->Name);
	return true;
}


bool cChat::ZenDrop(LPOBJ gObj, char *Msg)
{
#ifdef _GameServer_
	if(CheckCommand(gObj, Config.Commands.IsZenDrop, GMS.cSetZen, 0, 0, 0, 0, 1, 0, "ZenDrop", "/ZenDrop <value>", Msg))
	return true;

	DWORD Value;
	sscanf(Msg,"%d",&Value);

	if(Value > 1000000000)
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[ZenDrop] Max Value is 1000000000");
		return false;
	}

	CMapClass__ZenDrop(gObj->MapNumber, (int)gObj->X, (int)gObj->Y, Value);
	MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[ZenDrop] %d Zen successfully Dropped", Value);
	return true;
#endif
#ifdef _GameServerCS_
	MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[ZenDrop] Command works only on Normal Server");
	return true;
#endif
}

bool cChat::AddMobCommand(LPOBJ gObj, char *Msg)
{
#ifdef _GameServer_
	if(cProtection.CheckFeature(cProtection.MonsterSpawn))
	{
		if(CheckCommand(gObj, Config.Commands.IsMobSpawn, GMS.cSetZen, 0, 0, 0, 0, 1, 0, "AddMob", "/mobadd <mobid> <count> <speed> <map> <x> <y>", Msg))
			return true;

		int Mob, Cnt = 1, Map = gObj->MapNumber, Speed = 30, X = (int)gObj->X, Y = (int)gObj->Y;
		sscanf(Msg, "%d %d %d %d %d %d", &Mob, &Cnt, &Speed, &Map, &X, &Y);

		FILE* AddMobFile;  
							 
		if((AddMobFile = fopen( "..\\RMST\\MonsterSpawn.ini", "a+")) == NULL)
		{						   
			MessageBox(NULL, "Cant Find MonsterSpawn.ini", "Error", 0);
		}
		else
		{				
			fprintf(AddMobFile, "\n%d %d %d %d %d %d", Mob, Cnt, Speed, Map, X, Y); 	 
			fclose(AddMobFile);
		}		

		for(int i = 0; i < Cnt; i++)
		{
			MonsterAddAndSpawn(Mob,Speed,Map,X,Y);
		}
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddMob] %d Mob Successfully spawned (Map: %d, X: %d, Y: %d, MobID: %d)", Cnt,Map,X,Y, Mob);
	}
	else
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[AddMob] This is Private module");
	}
#endif
	return true;
}

bool cChat::BuyVIPCommand(LPOBJ gObj, char *Msg)
{

	if(cProtection.CheckFeature(cProtection.VIPSystem))
	{			   		
		DWORD Hours = 1;
		char State[255];
		sscanf(Msg, "%s %d", &State, &Hours);

		if(AddTab[gObj->m_Index].IsVipInGame > 0 || AddTab[gObj->m_Index].IsVipServer > 0)
		{
			MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[BuyVIP] You have already bought VIP.");		
			return true;
		}

		if(!_strcmpi(State, Config.VIPServ.VIPState))
		{
			if(Config.VIPServ.Enabled)
			{
				if(CheckCommand(gObj, Config.VIPServ.EnabledCmd, GMS.NONE, Config.VIPServ.CostZen * Hours, Config.VIPServ.CostPCPoints * Hours, Config.VIPServ.CostWCoins * Hours, 0, 2, 0, "BuyVIP", "/buyvip <type> <hours>", Msg))
					return true;
				TakeCommand(gObj, Config.VIPServ.CostZen * Hours, Config.VIPServ.CostPCPoints * Hours, Config.VIPServ.CostWCoins * Hours, "BuyVIP");

				MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s=%s + %d WHERE memb___id='%s'",MySQL.szDatabase2,Config.VIPServ.ColumnDate,Config.VIPServ.ColumnDate, Hours*60, gObj->AccountID);	
				MySQL.Execute("UPDATE [%s].[dbo].[MEMB_INFO] SET %s = 1 WHERE memb___id='%s'",MySQL.szDatabase2,Config.VIPServ.Column, gObj->AccountID);	
				AddTab[gObj->m_Index].VipMinutesServer += Hours*60;
				AddTab[gObj->m_Index].IsVipServer = 1;

				MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[BuyVIPServ] Successfully bought %d Hour(s)", Hours);			
			}
			else	  			
			{
				MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[BuyVIP] Vip serv is disabled.");
				return true;
			}
		}
		else
		{
			int RealState = -1;
		   	for(int i = 1; i<= VIP.NumStates; i++)
			{
				if(!_strcmpi(State, VIP.VIPState[i].VIPName))
				{
					RealState = i;
					break;
				}
			}
			if(RealState == -1)
			{			 
				MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[VIP] There are no such vip status.");
				return true;
			}
			if(!VIP.VIPState[RealState].EnabledCmd)
			{			 
				MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[VIP] You can't buy %s vip status.", VIP.VIPState[RealState].VIPName);
				return true;
			}
			if(CheckCommand(gObj, VIP.VIPState[RealState].EnabledCmd, GMS.NONE, VIP.VIPState[RealState].CostZen * Hours, VIP.VIPState[RealState].CostPCPoints * Hours, VIP.VIPState[RealState].CostWCoins * Hours, 0, 2, 0, "BuyVIP", "/buyvip <state> <hours>", Msg))
				return true;

			TakeCommand(gObj, VIP.VIPState[RealState].CostZen * Hours, VIP.VIPState[RealState].CostPCPoints * Hours, VIP.VIPState[RealState].CostWCoins * Hours, "BuyVIP");

			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s=%s + %d WHERE Name='%s'",MySQL.szDatabase,VIP.ColumnDate,VIP.ColumnDate, Hours*60, gObj->Name);	
			MySQL.Execute("UPDATE [%s].[dbo].[Character] SET %s = %d WHERE Name='%s'",MySQL.szDatabase,VIP.Column, RealState, gObj->Name);	
			AddTab[gObj->m_Index].IsInGameVipMinutes += Hours*60;
			AddTab[gObj->m_Index].IsVipInGame = RealState;

			MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[BuyVIP] Successfully bought %s for %d Hour(s)", VIP.VIPState[RealState].VIPName, Hours);
				 
			DWORD ThreadID;
			HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)VipInGameServer, (void*)gObj->m_Index, 0, &ThreadID);

			if ( hThread == 0 )
			{
				cLog.ConsoleOutPut(0, cLog.c_Red,cLog.t_NULL,"CreateThread() failed with error %d", GetLastError());
				return true;
			}

			CloseHandle(hThread);
			MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[BuyVIP] Your VIP status starts right now!");
		}
	}
	else
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[BuyVIP] This is Private module");
	}
	return true;
}

bool cChat::CheckVIPCommand(LPOBJ gObj, char *Msg)
{

	if(cProtection.CheckFeature(cProtection.VIPSystem))
	{
		if(CheckCommand(gObj, VIP.Enabled, GMS.NONE, 0, 0, 0, 0, 0, 0, "CheckVip", "/checkvip", Msg))
		return true;
		if(AddTab[gObj->m_Index].VipMinutesServer > 0)
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[ServerVIP] You have %d server min left.",AddTab[gObj->m_Index].VipMinutesServer);
		else if(AddTab[gObj->m_Index].IsInGameVipMinutes > 0)
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[InGameVIP] You have %s vip for %d in game min.",VIP.VIPState[AddTab[gObj->m_Index].IsVipInGame].VIPName,AddTab[gObj->m_Index].IsInGameVipMinutes);
		else
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[VIP] You haven't bought VIP yet.");
	}
	else
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[CheckVip] This is Private module");
	}
	return true;
} 

bool cChat::FireworkCommand(LPOBJ gObj, char *Msg)
{
	if(CheckCommand(gObj, 1, GMS.cSkin, 0, 0, 0, 0, 0, 0, "Firework", "/firework", Msg))
		return true;

	int count = -1;
	int x = (int)gObj->X;
	int y = (int)gObj->Y;
	int map = gObj->MapNumber;
	sscanf(Msg, "%d %d %d", &count, &x, &y);

	if(x > 255 || y > 255 || x < 0 || y < 0)
	{	 								   
		MessageLog(1, cLog.c_Red, cLog.t_GM, gObj,"[Firework] Such coords doesn't exist!");	
		return true;
	}  	

	if(count <= 0 || count > 100)			
		cMarry.MarryFirework(x, y, map, gObj);
	else
	{	  
		PMSG_SERVERCMD ServerCmd;
		PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
		ServerCmd.CmdType = 0;		   
		ServerCmd.X = x;
		ServerCmd.Y = y;
		for (int i=0; i<count; i++)
		{
			MsgSendV2(gObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));	   
			DataSend(gObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
		}
	}


	MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Firework] [%d] Firework added to map: %d, x: %d, y: %d", count, map, x , y);
	return true;
}

bool cChat::GuildPost(LPOBJ gObj, char *Msg)
{ 
	//GuildStatus= GuildMaster 128, GuildMember 0, GuildAsistant 64, GuildBattleMaster 32.
	if(gObj->GuildStatus == 128 || gObj ->GuildStatus == 64)
		GDGuildNoticeSave(gObj->GuildName, Msg);
	else
		MessageLog(1, cLog.c_Red, cLog.t_Default, gObj,"You aren't guild master or assistant");	
	return true;
}

bool cChat::Move(LPOBJ gObj, char *Msg)
{
	char MapName[50];
	sscanf(Msg, "%s", MapName);

	int Index = -1;
	for(int i = 1; i <= MoveReq.Count; i++)
	{
		if(!_strcmpi(MapName, MoveReq.MoveReqInfo[i].MapName1) || !_strcmpi(MapName, MoveReq.MoveReqInfo[i].MapName2))
		{
			Index = i;
			break;
		}
	}
	if(Index != -1)
	{	 	
		DWORD ZenActual;
		ZenActual = (DWORD)gObj->Money;
		if(ZenActual < (DWORD)MoveReq.MoveReqInfo[Index].Zen)return true;
		DWORD ZenDec = ZenActual - (DWORD)MoveReq.MoveReqInfo[Index].Zen;			

		if(MoveReq.MoveReqInfo[Index].Vip != 0 && MoveReq.MoveReqInfo[Index].Vip != AddTab[gObj->m_Index].IsVipInGame && cProtection.CheckFeature(cProtection.VIPSystem))
		{	 		
			MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Move] Sorry but map is for V.I.P.");
			return true;
		} 

		if(gObj->m_PK_Level > 4)
		{
			MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Move] Fonomans can't move");
			return true;
		}

		gObj->Money = ZenDec;
		GCMoneySend (gObj->m_Index,ZenDec);
		gObjMoveGate(gObj->m_Index,MoveReq.MoveReqInfo[Index].Gate);   
		return true;
	}
	else
	{
		MessageLog(1, cLog.c_Red, cLog.t_COMMANDS, gObj, "[Move] Such map doesn't exists.");
		return true;
	}
}
bool cChat::License(LPOBJ gObj, char *Msg)
{
	if(cProtection.Local_Id != -1)
	{
		MessageLog(1, cLog.c_Red, cLog.t_Default, gObj,"Files are licensed to: %s", cProtection.CheckName());
		return true;
	}
	else
	{
		MessageLog(1, cLog.c_Red, cLog.t_Default, gObj,"Files are licensed to: Public");
		return true;
	}
		return false;
}