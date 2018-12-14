#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//扑克数据
BYTE const CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
};

//扑克数据
BYTE const m_cbCardDataArray[27]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,				    	//索子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,					    //同子
};
//
//W: 0， 1， 2， 3， 4， 5， 6， 7， 8
//S: 9, 10, 11, 12, 13, 14, 15, 16, 17
//T: 18, 19, 20, 21, 22, 23, 24, 25, 26
//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//混乱准备
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)]={0};
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	//工作目录
/*	TCHAR m_szWorkDirectory[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),m_szWorkDirectory,sizeof(m_szWorkDirectory));
	int nModuleLen=lstrlen(m_szWorkDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE"));
	m_szWorkDirectory[nModuleLen-nProcessLen]=0;
	SetCurrentDirectory(m_szWorkDirectory);

	//读取扑克
	TCHAR szFile[MAX_PATH]=TEXT(""),szItem[20];
	_sntprintf(szFile,CountArray(szFile),TEXT("%s\\CardInfo.INI"),m_szWorkDirectory);

	for (BYTE i=0;i<CountArray(m_cbCardDataArray);i++)
	{
		TCHAR szRead[64]=TEXT("");
		_sntprintf(szItem,CountArray(szItem),TEXT("Item%d"),i+1);
		GetPrivateProfileString(TEXT("CardData"),szItem,TEXT(""),szRead,sizeof(szRead),szFile);
		cbCardData[i]=atoi(szRead);
		if (cbCardData[i]==0) break;
	}
	if (i==CountArray(m_cbCardDataArray)) return;*/

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//删除扑克
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	ASSERT(FALSE);

	return false;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//删除扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//删除扑克
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//错误断言
			ASSERT(FALSE);

			//还原删除
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//检验数据
	ASSERT(cbCardCount<=14);
	ASSERT(cbRemoveCount<=cbCardCount);

	//定义变量
	BYTE cbDeleteCount=0,cbTempCardData[14]={0};
	if (cbCardCount>CountArray(cbTempCardData)) return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//置零扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//成功判断
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//清理扑克
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=7)&&(cbColor==3)));
}

//扑克数目
BYTE CGameLogic::GetCardCount(BYTE cbCardIndex[MAX_INDEX])
{
	//数目统计
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//获取组合
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard+1;
			cbCardBuffer[1]=cbCenterCard+2;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_RIGHT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//碰牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_FILL:		//补牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	case WIK_TING:		//杠牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	case WIK_GANG:		//杠牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//动作等级
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//胡牌等级
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//杠牌等级
	if (cbUserAction&(WIK_FILL|WIK_GANG|WIK_TING)) { return 3; }

	//碰牌等级
	if (cbUserAction&WIK_PENG) { return 2; }

	//上牌等级
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//胡牌等级
BYTE CGameLogic::GetChiHuActionRank(tagChiHuResult & ChiHuResult)
{
	//变量定义
	BYTE cbChiHuOrder=1;
	WORD wChiHuRight=ChiHuResult.wChiHuRight;
	WORD wChiHuKind=ChiHuResult.wChiHuKind;
	
	WORD byHuKind[3] = {CHK_PENG_PENG, CHK_JIANG_JIANG, CHK_QI_XIAO_DUI};
	for (int i=0; i<3; i++)
	{
		if (wChiHuKind&byHuKind[i])
		{
			cbChiHuOrder++;
		}
	}
	WORD byHuRight[7] = {CHR_DI, CHR_TIAN, CHR_HAI_DI, CHR_QIANG_GANG, CHR_QING_YI_SE, CHR_MEN_QIAN_QING, CHR_QUAN_QIU_REN};
	for (int k=0; k<7; k++)
	{
		if (wChiHuRight&byHuRight[k])
		{
			cbChiHuOrder++;
		}
	}

	return cbChiHuOrder;
}

////吃胡判断(起手小胡)
//BYTE CGameLogic::EstimateChiHu(BYTE cbCardIndex[MAX_INDEX])
//{
//	//变量定义
//	BYTE cbActionMask=WIK_NULL;
//
//	//特殊胡牌
//	if (IsSiXi(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//	else if (IsQueYiSe(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//	else if (IsBanBanHu(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//	else if (IsLiuLiuShun(cbCardIndex)==true) cbActionMask|=WIK_CHI_HU;
//
//	return cbActionMask;
//}

////吃牌判断
//BYTE CGameLogic::EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbKingCard[])
//{
//	//参数效验
//	ASSERT(IsValidCard(cbCurrentCard));
//
//	//过滤判断
//	if (cbCurrentCard>=0x31 || cbCurrentCard == cbKingCard[0]) return WIK_NULL;
//
//	//变量定义
//	BYTE cbExcursion[3]={0,1,2};
//	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};
//
//	//构造麻将
//	BYTE cbCardIndexTemp[MAX_INDEX]={0};
//	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(BYTE)*MAX_INDEX);
//
//	cbCardIndexTemp[SwitchToCardIndex(cbKingCard[0])] = 0;
//
//	//吃牌判断
//	BYTE cbEatKind=0,cbFirstIndex=0;
//	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
//	for (BYTE i=0;i<CountArray(cbItemKind);i++)
//	{
//		BYTE cbValueIndex=cbCurrentIndex%9;
//		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
//		{
//			//吃牌判断
//			cbFirstIndex=cbCurrentIndex-cbExcursion[i];			
//			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndexTemp[cbFirstIndex]==0))
//				continue;
//			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndexTemp[cbFirstIndex+1]==0))
//				continue;
//			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndexTemp[cbFirstIndex+2]==0)) 
//				continue;
//
//		    //设置类型
//			cbEatKind|=cbItemKind[i];
//		}
//	}
//
//	return cbEatKind;
//}

//碰牌判断
BYTE CGameLogic::EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard/*, BYTE cbKingCard[]*/)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	////过滤判断
	//if (cbCurrentCard == cbKingCard[0]) return WIK_NULL;

	//碰牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}


////听牌判断
//BYTE CGameLogic::EstimateTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE IsGangData[MAX_INDEX],BYTE &GangCount)
//{
//	GangCount = 0;
//	for (int i=0; i<MAX_INDEX; i++)
//	{
//		if (4 == cbCardIndex[i])
//		{
//			BYTE bySaveCardIndex[MAX_INDEX] = {0};
//			memset(bySaveCardIndex, 0, sizeof(bySaveCardIndex));
//			memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);
//
//			BYTE byWeaveItemCount = cbItemCount;
//			tagWeaveItem weaveItemTemp[5];
//			memset(weaveItemTemp, 0, sizeof(tagWeaveItem)*5);
//			if (byWeaveItemCount>0 && byWeaveItemCount<4)
//			{
//				memcpy(weaveItemTemp, WeaveItem, sizeof(tagWeaveItem)*byWeaveItemCount);
//			}
//			weaveItemTemp[byWeaveItemCount].cbCenterCard = SwitchToCardData(i);
//			weaveItemTemp[byWeaveItemCount].cbWeaveKind = WIK_FILL;
//			weaveItemTemp[byWeaveItemCount].wProvideUser = 0;
//			weaveItemTemp[byWeaveItemCount].cbPublicCard = false;
//			++byWeaveItemCount;
//
//			bySaveCardIndex[i] = 0;
//
//			bool IsTingPai=IsCanTingCard(bySaveCardIndex,weaveItemTemp,byWeaveItemCount);
//			if (IsTingPai)
//			{
//				IsGangData[GangCount++] = SwitchToCardData(i);
//			}
//		}
//	}
//
//	for(BYTE i=0;i<cbItemCount;i++)
//	{
//		BYTE cbCardData=WeaveItem[i].cbCenterCard;
//		if(0!=cbCardData && WeaveItem[i].cbWeaveKind==WIK_PENG)
//		{
//			for (int k=0; k<MAX_INDEX; k++)
//			{
//				if (cbCardIndex[k]>0)
//				{
//					BYTE byTempOutCard = SwitchToCardData(k);
//					if (byTempOutCard == cbCardData)
//					{
//						BYTE bySaveCardIndex[MAX_INDEX] = {0};
//						memset(bySaveCardIndex, 0, sizeof(bySaveCardIndex));
//						memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);
//
//						BYTE byWeaveItemCount = cbItemCount;
//						tagWeaveItem weaveItemTemp[5];
//						memset(weaveItemTemp, 0, sizeof(tagWeaveItem)*5);
//						if (byWeaveItemCount>0 && byWeaveItemCount<4)
//						{
//							memcpy(weaveItemTemp, WeaveItem, sizeof(tagWeaveItem)*byWeaveItemCount);
//						}
//						weaveItemTemp[i].cbCenterCard = cbCardData;
//						weaveItemTemp[i].cbWeaveKind = WIK_FILL;
//						weaveItemTemp[i].wProvideUser = 0;
//						weaveItemTemp[i].cbPublicCard = false;
//
//						bySaveCardIndex[k] = 0;
//
//						bool IsTingPai=IsCanTingCard(bySaveCardIndex,weaveItemTemp,byWeaveItemCount);
//						if (IsTingPai)
//						{
//							IsGangData[GangCount++] = cbCardData;
//						}
//						return 0;
//					}
//				}
//			}
//		}
//	}
//
//
//	return 0;
//}
//
////听牌判断
//bool CGameLogic::EstimateTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE byOutCard)
//{
//	//参数效验
//	if (!IsValidCard(byOutCard))
//	{
//		return false;
//	}
//
//	for (int i=0; i<MAX_INDEX; i++)
//	{
//		BYTE byTempOutCard = SwitchToCardData(i);
//		if ((0!=byTempOutCard) && (3==cbCardIndex[i]) && (byTempOutCard==byOutCard))
//		{
//			BYTE bySaveCardIndex[MAX_INDEX] = {0};
//			memset(bySaveCardIndex, 0, sizeof(bySaveCardIndex));
//			memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);
//
//			BYTE byWeaveItemCount = cbItemCount;
//			tagWeaveItem weaveItemTemp[5];
//			memset(weaveItemTemp, 0, sizeof(tagWeaveItem)*5);
//			if (byWeaveItemCount>0 && byWeaveItemCount<4)
//			{
//				memcpy(weaveItemTemp, WeaveItem, sizeof(tagWeaveItem)*byWeaveItemCount);
//			}
//			weaveItemTemp[byWeaveItemCount].cbCenterCard = byTempOutCard;
//			weaveItemTemp[byWeaveItemCount].cbWeaveKind = WIK_FILL;
//			weaveItemTemp[byWeaveItemCount].wProvideUser = 0;
//			weaveItemTemp[byWeaveItemCount].cbPublicCard = false;
//			++byWeaveItemCount;
//
//			bySaveCardIndex[i] = 0;
//
//			bool IsTingPai=IsCanTingCard(bySaveCardIndex,weaveItemTemp,byWeaveItemCount);
//			return IsTingPai;
//		}
//	}
//
//	return false;
//}


//杠牌判断
BYTE CGameLogic::EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard/*, BYTE cbKingCard[]*/)
{
	//参数效验
	if (!IsValidCard(cbCurrentCard))
	{
		return WIK_NULL;
	}

	BYTE byType = WIK_NULL;
	//过滤判断
	//if (cbCurrentCard == cbKingCard[0]) return WIK_NULL;

	BYTE byCardIndex = SwitchToCardIndex(cbCurrentCard);
	if (byCardIndex<MAX_INDEX)
	{
		//杠牌判断
		if (3 == cbCardIndex[byCardIndex])
		{
			byType |= (WIK_GANG|WIK_FILL);
		}
	}
	return byType;
}

//杠牌分析
BYTE CGameLogic::AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult/*, BYTE cbKingCard[]*/)
{
	//设置变量
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	BYTE bySaveCardIndex[MAX_INDEX] = {0};
	memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);

	//BYTE byKindCardIndex = SwitchToCardIndex(cbKingCard[0]);
	//if (byKindCardIndex<MAX_INDEX)
	//{//王牌不能杠
	//	bySaveCardIndex[byKindCardIndex] = 0;
	//}
	
	//手上杠牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (bySaveCardIndex[i]==4)
		{
			cbActionMask|=(WIK_GANG|WIK_FILL);
			GangCardResult.cbCardData[GangCardResult.cbCardCount]=(WIK_GANG|WIK_FILL);
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//组合杠牌
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if (bySaveCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=(WIK_GANG|WIK_FILL);
				GangCardResult.cbCardData[GangCardResult.cbCardCount]=(WIK_GANG|WIK_FILL);
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}


//吃胡分析(无王牌)
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, 
								  BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult)
{
	for (BYTE i=0; i<cbWeaveCount; i++)
	{
		if ((WeaveItem[i].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)
		{
			return WIK_NULL;
		}
	}
	//变量定义
	WORD wChiHuKind=CHK_NULL;
	static CAnalyseItemArray AnalyseItemArray;

	//设置变量
	AnalyseItemArray.RemoveAll();
	ZeroMemory(&ChiHuResult,sizeof(ChiHuResult));

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX]={0};
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入扑克
	if (cbCurrentCard!=0) cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//权位处理
	//if ((cbCurrentCard!=0)&&(cbWeaveCount==4)) wChiHuRight|=CHR_QUAN_QIU_REN;
	//if (IsQingYiSe(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuRight|=CHR_QING_YI_SE;
	//if (IsMenQianQing(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuRight|=CHR_MEN_QIAN_QING;

	//牌型判断
	//if (IsQiXiaoDui(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_QI_XIAO_DUI;
	//if (IsJiangJiangHu(cbCardIndexTemp,WeaveItem,cbWeaveCount)==true) wChiHuKind|=CHK_JIANG_JIANG;

	//分析扑克
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//胡牌分析
	if (AnalyseItemArray.GetCount()>0)
	{
		//眼牌需求
		//bool bNeedSymbol=((wChiHuRight&0xFF00)==0);
		//bool bMenQing=IsMenQianQing(cbCardIndexTemp,WeaveItem,cbWeaveCount);

		//牌型分析
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//变量定义
			bool bLianCard=false,bPengCard=false;
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			//牌眼类型
			BYTE cbEyeValue=pAnalyseItem->cbCardEye&MASK_VALUE;
		//	bool bSymbolEye=((cbEyeValue==2)||(cbEyeValue==5)||(cbEyeValue==8));

			//牌型分析
			for (BYTE j=0;j<CountArray(pAnalyseItem->cbWeaveKind);j++)
			{
				BYTE cbWeaveKind=pAnalyseItem->cbWeaveKind[j];
				bPengCard=((cbWeaveKind&(WIK_GANG|WIK_FILL|WIK_PENG))!=0)?true:bPengCard;
				bLianCard=((cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))!=0)?true:bLianCard;
			}

			//牌型判断
			ASSERT((bLianCard==true)||(bPengCard==true));

			//碰碰牌型
		   if ((bLianCard==false)&&(bPengCard==true)) 
			{
				wChiHuKind|=CHK_PENG_PENG;
			}
			else if (bLianCard  && bPengCard )
			{
				wChiHuKind|=CHK_JI_HU;
			}
			else if (bLianCard  && !bPengCard )
			{
				wChiHuKind|=CHK_PING_HU;
			}
			
		}
	}
	
	if (wChiHuKind!=CHK_NULL) 
	{
		ChiHuResult.wChiHuKind=wChiHuKind;
		ChiHuResult.wChiHuRight=wChiHuRight;
		return WIK_CHI_HU;
	}
	//结果判断
	//if (wChiHuKind!=CHK_NULL)
	//{
	//	if ( cbWeaveCount!=0 && ( (wChiHuKind==CHK_JIANG_JIANG && wChiHuRight==CHK_NULL) || (wChiHuKind==CHK_JI_HU && wChiHuRight==CHK_NULL) || (wChiHuKind==CHK_PING_HU && wChiHuRight==CHK_NULL) ) )
	//	{
	//		//设置结果
	//		return WIK_NULL;
	//	}
	//	else
	//	{
	//		if (cbWeaveCount!=0)
	//		{//下槛了
	//			WORD byTempHuType = 0;
	//			WORD wHuCardType[3] = {CHK_JI_HU, CHK_PING_HU, CHK_PENG_PENG};
	//			for (int i=0; i<3; i++)
	//			{
	//				if ((wChiHuKind&wHuCardType[i]) > 0)
	//				{
	//					byTempHuType |= wHuCardType[i];
	//				}
	//			}
	//			wChiHuKind = byTempHuType;
	//			if (0 == wChiHuKind)
	//			{
	//				//设置结果
	//				return WIK_NULL;
	//			}
	//		}
	//		//设置结果
	//		ChiHuResult.wChiHuKind=wChiHuKind;
	//		ChiHuResult.wChiHuRight=wChiHuRight;

	//		return WIK_CHI_HU;
	//	}

	//}

	return WIK_NULL;
}

BYTE CGameLogic::AnalyseGangPengCountCard(BYTE cbCardIndex[MAX_INDEX], BYTE byKind, BYTE byCardData, BYTE byCanSelectCardData[4], BYTE &byCount)
{
	BYTE bySaveCardIndex[MAX_INDEX] = {0};
	memcpy(bySaveCardIndex, cbCardIndex, sizeof(BYTE)*MAX_INDEX);
	BYTE byTmpCardData = 255;
	byCount = 0;
	ZeroMemory(byCanSelectCardData, sizeof(BYTE)*4);

	for (BYTE i=0; i<MAX_INDEX; i++)
	{
		byTmpCardData = SwitchToCardData(i);
		if (4 == bySaveCardIndex[i]   ||   (3==bySaveCardIndex[i] && byCardData==byTmpCardData))
		{
			byCanSelectCardData[byCount++] = byTmpCardData;
		}
	}

	//调用这个函数肯定有一组结果
	ASSERT(0 < byCount);
	return byCanSelectCardData[0];
}

////王牌判断
//bool CGameLogic::IsKingCard(BYTE cbCardData,BYTE bKingCard[],BYTE bKingCount)
//{
//	for(BYTE i=0;i<bKingCount;i++)
//	{
//		if(bKingCard[i]==0)
//			continue;
//
//		if(cbCardData==bKingCard[i])
//			return true;
//
//	}
//	return false;
//}

////王牌数目
//BYTE CGameLogic::GetKingCardCount(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,BYTE bKingCard[],BYTE bKingCount)
//{
//	BYTE bReturnKingCount=0;
//
//	for(BYTE i=0;i<cbItemCount;i++)
//	{
//		BYTE cbCardData=WeaveItem[i].cbCenterCard;
//
//		if(WeaveItem[i].cbWeaveKind==WIK_PENG)
//		{
//
//			if(IsKingCard(cbCardData,bKingCard,bKingCount))	//王牌已经碰牌	
//			{
//				bReturnKingCount=3;
//				return bReturnKingCount;
//			}
//		}
//		else if(WeaveItem[i].cbWeaveKind!=WIK_GANG)
//		{
//			BYTE bCardData[3]={0,0,0};//吃牌数据
//			bool bEateType=false;	  //吃牌判断
//			switch(WeaveItem[i].cbWeaveKind)
//			{
//			case WIK_LEFT://左吃
//				{
//					bCardData[0]=cbCardData;
//					bCardData[1]=cbCardData+0x01;
//					bCardData[2]=cbCardData+0x02;
//					bEateType=true;				
//					break;
//				}
//			case WIK_CENTER://中吃
//				{
//					bCardData[0]=cbCardData-0x01;
//					bCardData[1]=cbCardData;
//					bCardData[2]=cbCardData+0x01;
//					bEateType=true;
//					break;
//				}
//			case WIK_RIGHT://右吃
//				{
//					bCardData[0]=cbCardData-0x02;
//					bCardData[1]=cbCardData-0x01;
//					bCardData[2]=cbCardData;
//					bEateType=true;
//					break;
//				}
//			}
//
//			if(bEateType)
//			{
//				for(BYTE j=0;j<3;j++)
//				{
//					//吃牌中有王牌
//					if(IsKingCard(bCardData[j],bKingCard,bKingCount))bReturnKingCount++;					
//				}
//			}
//
//		}
//
//		//if(bReturnKingCount>2)return bReturnKingCount;
//	}
//
//	for(BYTE i=0;i<MAX_INDEX;i++)
//	{
//		if(cbCardIndex[i]>0)
//		{
//			BYTE cbCard=SwitchToCardData(i);
//
//			//手中牌有王牌
//			if(IsKingCard(cbCard,bKingCard,bKingCount))
//				bReturnKingCount+=cbCardIndex[i];
//
//			//if(bReturnKingCount>2)return bReturnKingCount;
//		}
//	}
//
//	return bReturnKingCount;
//}

//分析扑克
bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//计算数目
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) cbCardCount+=cbCardIndex[i];

	//效验数目
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0)) return false;

	//变量定义
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[MAX_COUNT-2];
	ZeroMemory(KindItem,sizeof(KindItem));

	//需求判断
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==4);

	//单吊判断
	if (cbLessKindItem==0)
	{
		//效验参数
		ASSERT((cbCardCount==2)&&(cbWeaveCount==4));

		//牌眼判断
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			if (cbCardIndex[i]==2)
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//设置结果
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);

				//插入结果
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//拆分分析
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX;i++)
		{
			//同牌判断
			if (cbCardIndex[i]>=3)
			{
				KindItem[cbKindItemCount].cbCenterCard=i;
				KindItem[cbKindItemCount].cbCardIndex[0]=i;
				KindItem[cbKindItemCount].cbCardIndex[1]=i;
				KindItem[cbKindItemCount].cbCardIndex[2]=i;
				KindItem[cbKindItemCount++].cbWeaveKind=WIK_PENG;
			}

			//连牌判断
			if ((i<(MAX_INDEX-2))&&(cbCardIndex[i]>0)&&((i%9)<7))
			{
				for (BYTE j=1;j<=cbCardIndex[i];j++)
				{
					if ((cbCardIndex[i+1]>=j)&&(cbCardIndex[i+2]>=j))
					{
						KindItem[cbKindItemCount].cbCenterCard=i;
						KindItem[cbKindItemCount].cbCardIndex[0]=i;
						KindItem[cbKindItemCount].cbCardIndex[1]=i+1;
						KindItem[cbKindItemCount].cbCardIndex[2]=i+2;
						KindItem[cbKindItemCount++].cbWeaveKind=WIK_LEFT;
					}
				}
			}
		}
	}

	//组合分析
	if (cbKindItemCount>=cbLessKindItem)
	{
		//变量定义
		BYTE cbCardIndexTemp[MAX_INDEX]={0};
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//变量定义
		BYTE cbIndex[4]={0,1,2,3};
		tagKindItem * pKindItem[4];
		ZeroMemory(&pKindItem,sizeof(pKindItem));

		//开始组合
		do
		{
			//设置变量
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			for (BYTE i=0;i<cbLessKindItem;i++) pKindItem[i]=&KindItem[cbIndex[i]];

			//数量判断
			bool bEnoughCard=true;
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//存在判断
				BYTE cbCardIndex=pKindItem[i/3]->cbCardIndex[i%3];
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					bEnoughCard=false;
					break;
				}
				else cbCardIndexTemp[cbCardIndex]--;
			}

			//胡牌判断
			if (bEnoughCard==true)
			{
				//牌眼判断
				BYTE cbCardEye=0;
				for (BYTE i=0;i<MAX_INDEX;i++)
				{
					if (cbCardIndexTemp[i]==2)
					{
						cbCardEye=SwitchToCardData(i);
						break;
					}
				}

				//组合类型
				if (cbCardEye!=0)
				{
					//变量定义
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//设置组合
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
					}

					//设置牌型
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
					}

					//设置牌眼
					AnalyseItem.cbCardEye=cbCardEye;

					//插入结果
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//设置索引
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				int i=0;
				for (i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						int cbNewIndex=cbIndex[i-1];
						for (int j=(i-1);j<cbLessKindItem;j++) cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0) break;
			}
			else cbIndex[cbLessKindItem-1]++;

		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}


//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);
	return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
}


//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//转换扑克
	BYTE cbPosition=0;
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//转换扑克
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//门子：清一色牌，七小对牌，豪华对牌，将将胡牌

////清一色牌
//bool CGameLogic::IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
//{
//	//胡牌判断
//	BYTE cbCardColor=0xFF;
//	for (BYTE i=0;i<MAX_INDEX;i++)
//	{
//		if (cbCardIndex[i]!=0)
//		{
//			//花色判断
//			if (cbCardColor!=0xFF) return false;
//
//			//设置花色
//			cbCardColor=(SwitchToCardData(i)&MASK_COLOR);
//
//			//设置索引
//			i=(i/9+1)*9;
//		}
//	}
//
//	//组合判断
//	for (BYTE i=0;i<cbItemCount;i++)
//	{
//		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
//		if ((cbCenterCard&MASK_COLOR)!=cbCardColor) return false;
//	}
//
//	return true;
//}
//
////七小对牌
//bool CGameLogic::IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
//{
//	//组合判断
//	if (cbWeaveCount!=0) return false;
//
//	//扑克判断
//	for (BYTE i=0;i<MAX_INDEX;i++)
//	{
//		BYTE cbCardCount=cbCardIndex[i];
//		if ((cbCardCount!=0)&&(cbCardCount!=2)&&(cbCardCount!=4)) return false;
//	}
//
//	return true;
//}
//
////门前清牌
//bool CGameLogic::IsMenQianQing(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
//{
//	//组合判断
//	if (cbWeaveCount!=0)
//	{
//		return false;
//	}
//
//	return true;
//
//}

////豪华对牌
//bool CGameLogic::IsHaoHuaDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount)
//{
//	//变量定义
//	bool bFourCard=false;
//
//	//组合判断
//	for (BYTE i=0;i<cbItemCount;i++)
//	{
//		//杆补判断
//		if (WeaveItem[i].cbWeaveKind!=WIK_FILL) return false;
//		if (WeaveItem[i].cbWeaveKind!=WIK_GANG) return false;
//
//		//设置变量
//		bFourCard=true;
//	}
//
//	//扑克判断
//	for (BYTE i=0;i<MAX_INDEX;i++)
//	{
//		//四牌判断
//		if (cbCardIndex[i]==4)
//		{
//			bFourCard=true;
//			continue;
//		}
//
//		//对牌判断
//		if ((cbCardIndex[i]!=0)&&(cbCardIndex[i]!=2)) return false;
//	}
//
//	//结果判断
//	if (bFourCard==false) return false;
//
//	return true;
//}

////将将胡牌
//bool CGameLogic::IsJiangJiangHu(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
//{
//	//组合判断
//	for (BYTE i=0;i<cbWeaveCount;i++)
//	{
//		//类型判断
//		BYTE cbWeaveKind=WeaveItem[i].cbWeaveKind;
//		if ((cbWeaveKind!=WIK_PENG)&&(cbWeaveKind!=WIK_GANG)&&(cbWeaveKind!=WIK_FILL)) return false;
//
//		//数值判断
//		BYTE cbCenterValue=(WeaveItem[i].cbCenterCard&MASK_VALUE);
//		if ((cbCenterValue!=2)&&(cbCenterValue!=5)&&(cbCenterValue!=8)) return false;
//	}
//
//	//扑克判断
//	for (BYTE i=0;i<MAX_INDEX;i++)
//	{
//		if ((i%3!=1)&&(cbCardIndex[i]!=0)) return false;
//	}
//
//	return true;
//}

////判断是否可以听牌
////输入：手上牌，组合牌，组合数
//bool CGameLogic::IsCanTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
//{
//	m_cbHuCardIndex.clear();
//    //听牌判断
//	BYTE chCanHu = WIK_NULL;
//	BYTE byHuNums = 0;
//
//	//判断胡牌
//	BYTE  cbCardDataTemp[27]=
//	{
//		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					//万子
//		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,			    	//索子
//		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,			    	//同子
//	};
//
//	for (int j=0;j<27;j++)
//	{
//		//构造麻将
//		BYTE cbCardIndexTemp[MAX_INDEX]={0};
//		CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(BYTE)*MAX_INDEX);
//
//		bool bIsHave = false;
//		for (int i=0; i<MAX_INDEX; i++)
//		{
//			if (4==cbCardIndexTemp[i])
//			{
//				bIsHave = false;
//				BYTE byCard = SwitchToCardData(i);
//				if (byCard == cbCardDataTemp[j])
//				{
//					bIsHave = true;
//					break;
//				}
//			}
//		}
//		if (bIsHave)
//		{
//			continue;
//		}
//
//		tagChiHuResult ChiHuResult;
//		BYTE cbReCardIndex[MAX_INDEX];
//		BYTE cbCurrentCard=cbCardDataTemp[j];
//		chCanHu  = AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,0,ChiHuResult);
//		if (WIK_CHI_HU == chCanHu)
//		{
//			m_cbHuCardIndex.push_back(cbCurrentCard);
//			++byHuNums;
//			//break;
//		}
//	}
//
//	//听牌判断
//	if(/*WIK_CHI_HU == chCanHu*/byHuNums>0)
//	{
//		return true;
//	}
//	else
//	{
//		return false;
//	}
//
//}
const char * CGameLogic::GetMJCardName(unsigned char nCardID)
{
	bool bLegal=IsValidCard(nCardID);
	//ASSERT(bLegal && "GetMJCardName的输入参数有误!");
	if(bLegal)
	{
		static const char * szCardName[4][15]={
			{"一万","二万","三万","四万","五万","六万","七万","八万","九万","","","","","",""},
			{"一索","二索","三索","四索","五索","六索","七索","八索","九索","","","","","",""},
			{"一筒","二筒","三筒","四筒","五筒","六筒","七筒","八筒","九筒","","","","","",""},
			{"东风","南风","西风","北风","红中","发财","白板","春","夏","秋","冬","梅","兰","竹","菊"},
		};
		unsigned char nColorIndex=(nCardID & 0xf0)>>4;
		unsigned char nValueIndex=(nCardID & 0x0f)-1;
		return szCardName[nColorIndex][nValueIndex];
	}
	else
		return "";
}

int CGameLogic::GetHuPaiData(BYTE cbCardData[], BYTE cbCardCount)
{
	BYTE byHuNums = m_cbHuCardIndex.size();
	if (byHuNums>0)
	{
		for (int i=0; i<byHuNums&&i<8; ++i)
		{
			if (i < cbCardCount)
			{
				cbCardData[i] = m_cbHuCardIndex[i];
			}
		}
	}
	if (byHuNums>=8)
	{
		byHuNums=8;
	}
	return byHuNums;
}

//////////////////////////////////////////////////////////////////////////
