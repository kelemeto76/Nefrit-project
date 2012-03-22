#include "Jewels.h"
#include "Stdafx.h"
#include "Main.h"
#include "ChatCommands.h"
#include "ConfigDefines.h"

Jewels g_Jewels;


Jewels::Jewels()
{

}

Jewels::~Jewels()
{

}

void Jewels::LoadConfig()
{ 
	FILE *Jewel;
	Jewel = fopen(RMSTJewel,"r");
	char zBuf[1024];
	bool flag = false;
	int i = 0;
	while (!feof(Jewel))
	{
		fgets(zBuf,1024,Jewel);

		if ( zBuf[0] == '/' && zBuf[1] == '/')
		{
			continue;
		}

		if ( zBuf[0] == '0' && !flag) 
		{
			flag = true;
			continue;
		}

		if ( zBuf == "end") break;

		if (flag)
		{
			sscanf(zBuf,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d",
				&ArrayJewelConf[i].id,&ArrayJewelConf[i].rate,&ArrayJewelConf[i].opt1,&ArrayJewelConf[i].opt2,&ArrayJewelConf[i].minopt1,
				&ArrayJewelConf[i].minopt2,&ArrayJewelConf[i].maxopt1,&ArrayJewelConf[i].maxopt2,&ArrayJewelConf[i].fail1,
				&ArrayJewelConf[i].fail2,&ArrayJewelConf[i].failopt1,&ArrayJewelConf[i].failopt2,&ArrayJewelConf[i].successfully1,
				&ArrayJewelConf[i].successfully2);
			i++;
		}
	}			   
	fclose(Jewel);
	AmountJewel = i-1;
}

void Jewels::IdentifyJewel(PMSG_USE_ITEM_RECV * lpmsg, int aIndex)
{
	OBJECTSTRUCT * gObj = (OBJECTSTRUCT*)OBJECT_POINTER(aIndex);
	CItem* lpItem = &gObj->pInventory[lpmsg->SourceSlot];
	CItem* ProtochItem = &gObj->pInventory[lpmsg->TargetSlot];

	for (int i=0; i<AmountJewel; i++)
	{
		if (lpItem->m_Type == ITEMGET(14,ArrayJewelConf[i].id))
		{
			if (Verification(ArrayJewelConf[i].opt1,ArrayJewelConf[i].opt2,ArrayJewelConf[i].minopt1,ArrayJewelConf[i].minopt2,
				ArrayJewelConf[i].maxopt1,ArrayJewelConf[i].maxopt2,ArrayJewelConf[i].successfully1,ArrayJewelConf[i].successfully2,
				ProtochItem))
			{
				RATE = rand()%100+1;
				if (RATE <= ArrayJewelConf[i].rate)
				{
					Chat.Message(1,gObj->m_Index,"[Jewel] Congratulation, Item Update is Successfully");
					this->Successfully(ArrayJewelConf[i].opt1,ArrayJewelConf[i].opt2,ArrayJewelConf[i].maxopt1,ArrayJewelConf[i].maxopt2,
						ArrayJewelConf[i].successfully1,ArrayJewelConf[i].successfully2,ProtochItem);
					this->DeleteItem(aIndex,lpmsg->SourceSlot);
					GCInventoryItemOneSend(aIndex, lpmsg->TargetSlot);
				}
				else
				{
					Chat.Message(1,gObj->m_Index,"[Jewel] Sorry, Item Update is Fail");
					this->Fail(ArrayJewelConf[i].opt1,ArrayJewelConf[i].opt2,ArrayJewelConf[i].fail1,ArrayJewelConf[i].fail2,
						ArrayJewelConf[i].failopt1,ArrayJewelConf[i].failopt2,ProtochItem);
					this->DeleteItem(aIndex,lpmsg->SourceSlot);
					GCInventoryItemOneSend(aIndex, lpmsg->TargetSlot);
				}
			}
			else
			{
				Chat.Message(1,gObj->m_Index,"[Jewel] Sorry, you can`t not use jewel on this item");
			}
		}
	}
}

bool Jewels::Verification(int Opt1, int Opt2,int MinOpt1, int MinOpt2, int MaxOpt1 , int MaxOpt2, int Success1, int Success2, CItem* ProtochItem)
{
	// Add Item Option
	if (Opt1 == 1 || Opt2 == 1)
	{
		if (Opt1 == 1 )
		{
			if (ProtochItem->m_Level + Success1 <= MaxOpt1 && ProtochItem->m_Level >= MinOpt1) return true;
			else return false;
		}
		if (Opt2 == 1)
		{
			if (ProtochItem->m_Level + Success2 <= MaxOpt2&& ProtochItem->m_Level >= MinOpt2) return true;
			else return false;
		}
	}
	// Luck Add
	if (Opt1 == 2 || Opt2 == 2)
	{
		if (ProtochItem->m_Option2 != 1)	return true;
		else return false;
	}
	// Skill Add
	if (Opt1 == 3 || Opt2 == 3)
	{
		if (ProtochItem->m_Type > ITEMGET(6,0)) return false;
		if (ProtochItem->m_Option1 != 1)	return true;
		else return false;
	}
	// Add Option
	if (Opt1 == 4 || Opt2 == 4)
	{
		if (Opt1 == 4 )
		{
			if (ProtochItem->m_Option3 + Success1 <= MaxOpt1 && ProtochItem->m_Option3 >= MinOpt1)	return true;
			else return false;
		}
		if (Opt2 == 4)
		{
			if (ProtochItem->m_Option3 + Success2 <= MaxOpt2 && ProtochItem->m_Option3 >= MinOpt2)	return true;
			else return false;
		}
	}
	// Excellent Opt
	if (Opt1 == 5 || Opt2 == 5)
	{
		if (ProtochItem->m_Type >= ITEMGET(12,0)) return false;
		int addexc;
		if (Opt1 == 5 )
		{
			addexc = this->AddExcOpt(ProtochItem->m_NewOption,MaxOpt1,Success1);
			if (addexc == -1 ) return false;
			return true;
		}
		if (Opt2 == 5)
		{
			addexc = this->AddExcOpt(ProtochItem->m_NewOption,MaxOpt2,Success2);
			if (addexc == -1 ) return false;
			return true;
		}
	}
	return false;
}

void Jewels::Successfully(int Opt1 , int Opt2 , int MaxOpt1 , int MaxOpt2, int Success1 , int Success2 , CItem* ProtochItem)
{
	// Add Item Option
	if (Opt1 == 1 || Opt2 == 1)
	{
		if (Opt1 == 1 )	ProtochItem->m_Level += Success1;
		if (Opt2 == 1)	ProtochItem->m_Level += Success2;
	}
	// Luck Add
	if (Opt1 == 2 || Opt2 == 2)	ProtochItem->m_Option2 = 1;

	// Skill Add
	if (Opt1 == 3 || Opt2 == 3)	ProtochItem->m_Option1 = 1;
	
	// Add Option
	if (Opt1 == 4 || Opt2 == 4)
	{
		if (Opt1 == 4 )	ProtochItem->m_Option3 += Success1;
		if (Opt2 == 4)	ProtochItem->m_Option3 += Success2;
	}

	// Exc Opt
	if (Opt1 == 5 || Opt2 == 5)
	{
		int addexc;
		if (Opt1 == 5 )
		{
			addexc = this->AddExcOpt(ProtochItem->m_NewOption,MaxOpt1,Success1);
			ProtochItem->m_NewOption = addexc;
		}
		if (Opt2 == 5)
		{
			addexc = this->AddExcOpt(ProtochItem->m_NewOption,MaxOpt2,Success2);
			ProtochItem->m_NewOption = addexc;
		}
	}
}

void Jewels::Fail(int Opt1, int Opt2 , int Fail1, int Fail2 , int FailOpt1, int FailOpt2, CItem* ProtochItem)
{
	// Down Item level
	if (Opt1 == 1 || Opt2 == 1)
	{
		if (Opt1 == 1)
		{
			if (Fail1 == 0) ProtochItem->m_Level = 0;
			if (Fail1 == 2) ProtochItem->m_Level -= FailOpt1;
		}
		if (Opt2 == 2)
		{
			if (Fail2 == 0) ProtochItem->m_Level = 0;
			if (Fail2 == 2) ProtochItem->m_Level -= FailOpt2;
		}
	}

	if (Opt1 == 4 || Opt2 == 4)
	{
		if (Opt1 == 4)
		{
			if (Fail1 == 0) ProtochItem->m_Option3 = 0;
			if (Fail1 == 2) ProtochItem->m_Option3 -= FailOpt1;
		}
		if (Opt2 == 4)
		{
			if (Fail2 == 0) ProtochItem->m_Option1 = 0;
			if (Fail2 == 2) ProtochItem->m_Option3 -= FailOpt2;
		}
	}
	if (Opt1 == 5 || Opt2 == 5)
	{
		if (Opt1 == 5)
		{
			if (Fail1 == 0) ProtochItem->m_NewOption = 0;
		}
		if (Opt2 == 5)
		{
			if (Fail2 == 0) ProtochItem->m_NewOption = 0;
		}
	}
}

int Jewels::AddExcOpt(int exc, int maxopt , int success)
{
	int arrayopt[6]={0,0,0,0,0,0};
	int num = 32;
	int amountopt = 0;
	int added = 0;

	//Узнаем какие опции на шмотке
	for (int i=5; i>=0; i--)
	{
		if ( exc >= num )
		{
			arrayopt[i] = 1;
			amountopt++;
			exc = exc - num;
		}
		num = num / 2;
	}

	if (maxopt <= amountopt)
		return -1;

	//Выбираем любую недостающую опцию
	int randomopt = rand()%(6-amountopt)+1;
	amountopt = 0;

	for (int i=0; i<6; i++)
	{
		if (!arrayopt[i])
		{ 
			amountopt++;
			if (amountopt == randomopt )
			{
				arrayopt[i] = 1;
				added++;
				if (success == added)	break;
			}
		}
		else
			continue;
	}

	num = 1; exc = 0;
	// Получаем код
	for (int i=0 ; i<6; i++)
	{
		if (arrayopt[i])
		{
			exc += num;
		}
		num = num * 2;
	}

	return exc;
}

void Jewels::DeleteItem(int aIndex, int Slot)
{
	gObjInventoryDeleteItem(aIndex,Slot);
	GCInventoryItemDeleteSend(aIndex,Slot,1);
}