#include "StdAfx.h"
#include ".\sendcard.h"

CSendCard::CSendCard(void) : m_strFilePath("")
{
}

CSendCard::~CSendCard(void)
{
}

//设置路径
void CSendCard::SetFilePath(CString strFilePath)
{
	if(!strFilePath.IsEmpty())
	{
		m_strFilePath = strFilePath;
	}
}

BYTE CSendCard::GetNum(char c)
{
	BYTE iNum = c - '0';
	return iNum;
}


//获得玩家手牌数据
int CSendCard::GetCardData(int iChairID, BYTE* pCard, int iNums)
{
	if(!m_strFilePath.IsEmpty())
	{
		::memset(pCard, 0, sizeof(BYTE)*iNums);
		TCHAR chCardData[1024]={0};
		CString strKeyValue=_T("");
		strKeyValue.Format(_T("User%d"), iChairID);
		DWORD uiReturn = GetPrivateProfileString(TEXT("CardData"), strKeyValue, TEXT("0"), chCardData, sizeof(chCardData), m_strFilePath);
		if (uiReturn>0)
		{
			if (1==uiReturn && '0'==chCardData[0])
			{
				return 0;
			}
			else
			{
				int iCountCard = 0;
				CString strCardData=_T(""), strCardValue=_T("");
				strCardData.Append(chCardData);
				strCardData.TrimLeft();
				strCardData.TrimRight();
				while(!strCardData.IsEmpty())
				{
					int iIndex = strCardData.Find(',');
					strCardValue="";
					if (-1==iIndex)
					{
						strCardValue = strCardData;
					}
					else
					{
						strCardValue = strCardData.Left(iIndex);
					}
					int iSize = strCardValue.GetLength();
					strCardData.Delete(0, iSize+1);
					if (!strCardValue.IsEmpty() && iSize>0)
					{
						strCardValue.TrimLeft();
						strCardValue.TrimRight();
						if (strCardValue.GetLength()>=4)
						{
							char chValue[64]={0};
							sprintf(chValue, "%s", strCardValue);
							BYTE byValue1 = GetNum(chValue[2]);
							BYTE byValue2 = GetNum(chValue[3]);
							BYTE byCardValue = (byValue1<<4)+byValue2;
							pCard[iCountCard++] = byCardValue;
							if (iCountCard>=iNums)
							{
								break;
							}
						}
					}
				}
				return iCountCard;
			}
		}
	}
	return 0;
}



//获得王牌
BYTE CSendCard::GetKingCard()
{
	if(!m_strFilePath.IsEmpty())
	{
		TCHAR chCardData[32]={0};
		DWORD uiReturn = GetPrivateProfileString(TEXT("KingCard"), TEXT("KingCard"), TEXT("0"), chCardData, sizeof(chCardData), m_strFilePath);
		if (4==uiReturn)
		{
			BYTE byValue1 = GetNum(chCardData[2]);
			BYTE byValue2 = GetNum(chCardData[3]);
			BYTE byCardValue = (byValue1<<4)+byValue2;
			return byCardValue;
		}
	}
	return -1;
}

//获得色子数
bool CSendCard::GetSZNums(int& iNum1, int& iNum2)
{
	iNum1 = 0;
	iNum2 = 0;
	if(!m_strFilePath.IsEmpty())
	{
		UINT uiReturn1 = GetPrivateProfileInt(TEXT("SZNums"), TEXT("Num1"), 0, m_strFilePath);
		UINT uiReturn2 = GetPrivateProfileInt(TEXT("SZNums"), TEXT("Num2"), 0, m_strFilePath);
		iNum1 = uiReturn1;
		iNum2 = uiReturn2;
	}
	return false;
}

//获得下次抓的牌
BYTE CSendCard::GetNextCard()
{
	if(!m_strFilePath.IsEmpty())
	{
		TCHAR chCardData[32]={0};
		DWORD uiReturn = GetPrivateProfileString(TEXT("NextCard"), TEXT("NextCard"), TEXT("0"), chCardData, sizeof(chCardData), m_strFilePath);
		if (4==uiReturn)
		{
			BYTE byValue1 = GetNum(chCardData[2]);
			BYTE byValue2 = GetNum(chCardData[3]);
			BYTE byCardValue = (byValue1<<4)+byValue2;
			return byCardValue;
		}
	}
	return -1;
}

//获得牌墙数据
int CSendCard::GetCardWallData(BYTE* pCard, int iNums)
{
	if(!m_strFilePath.IsEmpty())
	{
		::memset(pCard, 0, sizeof(BYTE)*iNums);
		TCHAR chCardData[2048]={0};
		DWORD uiReturn = GetPrivateProfileString(TEXT("CardWall"), TEXT("CardData"), TEXT("0"), chCardData, sizeof(chCardData), m_strFilePath);
		if (uiReturn>0)
		{
			if (1==uiReturn && '0'==chCardData[0])
			{
				return 0;
			}
			else
			{
				int iCountCard = 0;
				CString strCardData=_T(""), strCardValue=_T("");
				strCardData.Append(chCardData);
				strCardData.TrimLeft();
				strCardData.TrimRight();
				while(!strCardData.IsEmpty())
				{
					int iIndex = strCardData.Find(',');
					strCardValue="";
					if (-1==iIndex)
					{
						strCardValue = strCardData;
					}
					else
					{
						strCardValue = strCardData.Left(iIndex);
					}
					int iSize = strCardValue.GetLength();
					strCardData.Delete(0, iSize+1);
					if (!strCardValue.IsEmpty() && iSize>0)
					{
						strCardValue.TrimLeft();
						strCardValue.TrimRight();
						if (strCardValue.GetLength()>=4)
						{
							char chValue[64]={0};
							sprintf(chValue, "%s", strCardValue);
							BYTE byValue1 = GetNum(chValue[2]);
							BYTE byValue2 = GetNum(chValue[3]);
							BYTE byCardValue = (byValue1<<4)+byValue2;
							pCard[iCountCard++] = byCardValue;
							if (iCountCard>=iNums)
							{
								break;
							}
						}
					}
				}
				return iCountCard;
			}
		}
	}
	return 0;
}