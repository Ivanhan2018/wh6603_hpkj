#include "StdAfx.h"
#include "TableFrameSink.h"
//#include "..\..\开发库\Include\GlobalChannel.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
//const enStartMode	CTableFrameSink::m_GameStartMode=enStartMode_FullReady;	//开始模式

//扑克数据
BYTE const n_cbCardDataArray[27]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
		0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
		0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
};



//Open sendcard tool
#define			TOOL_SEND_CARD		

//////////////////////////////////////////////////////////////////////////

// 测试结算
void CTableFrameSink::TestGameEndFun(BYTE bSelect)//Test
{
	//static int i=0;
	//bSelect = i;
	//i++;
	//if (5==i)
	//	i=0;

	switch(bSelect)
	{
	case 0:	// 玩家自摸，自己有筋,别人没筋
		{			
			m_byGameEndType = 1;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[m_wProvideUser]=1;
			m_ChiHuResult[m_wProvideUser].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 1:	// 玩家自摸，自己无筋,别人有筋
		{
			m_byGameEndType = 1;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[(m_wProvideUser+1)%GAME_PLAYER]=10;
			m_ChiHuResult[m_wProvideUser].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 2:	// 玩家点泡，点炮人无筋,别人有筋
		{
			m_byGameEndType = 2;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[(m_wProvideUser+1)%GAME_PLAYER]=10;
			m_llMuscle[(m_wProvideUser+2)%GAME_PLAYER]=10;
			m_ChiHuResult[(m_wProvideUser+1)%GAME_PLAYER].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 3:	// 玩家点泡，点炮人有筋,别人无筋
		{
			m_byGameEndType = 2;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));
			m_llMuscle[(m_wProvideUser)%GAME_PLAYER]=10;
			m_ChiHuResult[(m_wProvideUser+1)%GAME_PLAYER].wChiHuKind=CHK_PING_HU;
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	case 4: // 通炮
		{
			m_byGameEndType = 2;
			ZeroMemory(m_llMuscle, sizeof(m_llMuscle));

			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				if (i==m_wProvideUser)
					continue;

				m_ChiHuResult[i].wChiHuKind=CHK_PING_HU;
			}
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			break;
		}
	}
}

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//m_pRoomLog = NULL;

	//游戏变量
	m_wSiceCount=MAKEWORD(1,1);
	//m_wMPIndex=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_llMuscle,sizeof(m_llMuscle));

	CString str;
	str.Format(_T("server: CTableFrameSink 37---%+I64d,%+I64d,%+I64d,%+I64d"), m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);


	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus=false;
	//ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	//ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(m_ChiHuResult,sizeof(m_ChiHuResult));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

	//杠上开花
	//memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));



	//osc update
	//memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	//memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	//m_iSaveGameOutCardCount = 0;
	//m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;

	m_bIsPlayDice=false;

	//::srand((unsigned int)time(NULL));
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询
void *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

// 通向游戏服务端的通道，可以传递任何简易数据或成员指针
bool CTableFrameSink::channel_to_game_service(int iMainCode, int iAssistantCode, void* pData, int iDataLen)
{
	//switch(iMainCode)
	//{
	//case CTGS_MAIN_LOG:	// 日志
	//	{
	//		if(CTGS_ASS_LOG_POINT==iAssistantCode)	// 日志组件的指针
	//		{
	//			if(pData)
	//			{
	//				//m_pRoomLog = (cnd_ILogMgr*)pData;

	//				// 由于每张桌子都会到此，但只能写一次
	//				static bool bArrow = true;
	//				if(bArrow/* && m_pRoomLog*/)
	//				{
	//					bArrow = !bArrow;

	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** 这是【转转麻将】游戏服务端日志");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** 此文件在启动服务时开始记录");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** 在停止服务或者挂掉时终止");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** 每条记录前面是记录时间");
	//					//m_pRoomLog->AddMsg(0, 0, TRUE, "***** 下面开始真正有意义的记录");
	//				}
	//			}
	//		}

	//		break;
	//	}
	//default:
	//	{
	//		break;
	//	}
	//}

	return true;
}

//初始化
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//开始模式//////////////////
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);//add by hxh 20170321
	return true;
}

//复位桌子
void  CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_wSiceCount=MAKEWORD(1,1);
	//m_wMPIndex=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_llMuscle,sizeof(m_llMuscle));

	CString str;
	str.Format(_T("server: RepositTableFrameSink 140---%+I64d,%+I64d,%+I64d,%+I64d"), m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus=false;
	//ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	//ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//杠上开花
	//memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));


	//osc update
	//memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	//memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	//m_iSaveGameOutCardCount = 0;
	//m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;
	return;
}

////开始模式
//enStartMode  CTableFrameSink::GetGameStartMode()
//{
//	return m_GameStartMode;
//}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}


//初始化数据
void CTableFrameSink::InitData()
{
	//游戏变量
	m_wSiceCount=MAKEWORD(1,1);
	//m_wMPIndex=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_llMuscle,sizeof(m_llMuscle));

	CString str;
	str.Format(_T("server: InitData 216---%+I64d,%+I64d,%+I64d,%+I64d"), m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus=false;
	//ZeroMemory(m_bHearStatus,sizeof(m_bHearStatus));
	//ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));
	ZeroMemory(m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

	//结束信息
	m_cbChiHuCard=0;
	m_wHaiDiUser=INVALID_CHAIR;
	ZeroMemory(&m_ChiHuResult,sizeof(m_ChiHuResult));

	//杠上开花
	//memset(m_tagUserGSKUResult, 0, sizeof(m_tagUserGSKUResult));

	//osc update
	//memset(m_bIsUserFirstOutCard, true, sizeof(m_bIsUserFirstOutCard));
	//memset(m_bUserFirstIsTingPai, 0, sizeof(m_bUserFirstIsTingPai));
	memset(m_byUserHuCardType, 0, sizeof(m_byUserHuCardType));
	//m_iSaveGameOutCardCount = 0;
	//m_iSaveGameSendCardCount = 0;
	m_byGameEndType = 0;

	//memset(m_cbKingCard, 0, sizeof(m_cbKingCard));
	memset(m_cbKingCount, 0, sizeof(m_cbKingCount));
	//m_cbMoCard = 0;
	m_wCurrentUser = INVALID_CHAIR;


	memset(m_byGangCard, 0, sizeof(m_byGangCard));
	memset(m_byGangCardNums, 0, sizeof(m_byGangCardNums));
	memset(m_byBuCard, 0, sizeof(m_byBuCard));
	memset(m_byBuCardNums, 0, sizeof(m_byBuCardNums));

	m_bIsPlayDice=true;

}
//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	InitData();

#ifdef TOOL_SEND_CARD
	TCHAR chDirWork[1024]={0};
	//设置工作目录
	GetModuleFileName(GetModuleHandle(NULL),chDirWork,sizeof(chDirWork));
	CString strDirWorkPath=_T("");
	strDirWorkPath.ReleaseBuffer();
	strDirWorkPath.Append(chDirWork);
	int nPos = 0; 
	nPos=strDirWorkPath.ReverseFind('\\'); 
	strDirWorkPath=strDirWorkPath.Left(nPos);
	strDirWorkPath.Append(_T("\\MJCard.ini"));
	m_SendCardTool.SetFilePath(strDirWorkPath);
#endif

	if (m_wBankerUser<0 || m_wBankerUser>=GAME_PLAYER)
	{
		m_wBankerUser = rand()%GAME_PLAYER;
	}

	//混乱扑克
	int iNum1=0,iNum2=0;
#ifdef TOOL_SEND_CARD
	m_SendCardTool.GetSZNums(iNum1, iNum2);
#endif
	if (0==iNum1 || 0==iNum2)
	{
		iNum1 = rand()%6+1;
		iNum2 = rand()%6+1;
	}
	m_wSiceCount=MAKEWORD(iNum1,iNum2);//随机骰子数
	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

#ifdef TOOL_SEND_CARD
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		BYTE byCardData[MAX_COUNT-1]={0};
		int iCardNums = m_SendCardTool.GetCardData(i, byCardData, MAX_COUNT-1);
		if (iCardNums==MAX_COUNT-1)
		{
			m_GameLogic.SwitchToCardIndex(byCardData,MAX_COUNT-1,m_cbCardIndex[i]);
		}
		else 
		{
			m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
		}
	}
#else
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}
#endif


#ifdef TOOL_SEND_CARD
	//改变牌墙数据
	BYTE byWallCard[MAX_REPERTORY]={0};
	int iCardNums = m_SendCardTool.GetCardWallData(byWallCard, MAX_REPERTORY);
	if (iCardNums>0)
	{
		::memcpy(&m_cbRepertoryCard[m_cbLeftCardCount-1], byWallCard, iCardNums);
	}
#endif

	//发送扑克
	BYTE byNextCard=0;
#ifdef TOOL_SEND_CARD
	byNextCard = m_SendCardTool.GetNextCard();
#endif
	--m_cbLeftCardCount;
	if (0==byNextCard || 255==byNextCard)
	{
		byNextCard = m_cbRepertoryCard[m_cbLeftCardCount];
	}
	m_cbSendCardCount++;
	m_cbSendCardData=byNextCard;
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//设置变量
	m_cbProvideCard=0;
	m_wProvideUser=INVALID_CHAIR;
	m_wCurrentUser=m_wBankerUser;

	CString str;
	str.Format(_T("server: OnEventGameStart() 538-- m_wCurrentUser %d"), m_wCurrentUser);
	OutputDebugString(str);

	//动作分析
	tagGangCardResult GangCardResult;
	BYTE byGangCard[MAX_INDEX]={0};
	BYTE byGangCardNums = 0;
	//bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbUserAction[i] = WIK_NULL;
		m_byUserHuCardType[i] = WIK_NULL;
		//m_bUserFirstIsTingPai[i] = false;
		//m_bIsUserFirstOutCard[i] = true; 

		//庄家判断
		if (i==m_wBankerUser)
		{
			memset(byGangCard, 0, sizeof(byGangCard));
			byGangCardNums = 0;
			//杠牌判断
			memset(&GangCardResult, 0, sizeof(tagGangCardResult));
			m_GameLogic.AnalyseGangCard(m_cbCardIndex[i],NULL,0,GangCardResult);
			//if (GangCardResult.cbCardCount>=1)
			//{
			//	m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, byGangCard, byGangCardNums);
			//}

			if (byGangCardNums>0)
			{
				m_cbUserAction[i] |= WIK_TING;
			}
			if (GangCardResult.cbCardCount>=1)
			{
				m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
			}

			//胡牌判断
			tagChiHuResult ChiHuResult;
			//BYTE cbReCardIndex[MAX_INDEX];
			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult);
			//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],NULL,0,0,0,ChiHuResult,m_cbKingCard,1,cbReCardIndex);
		}

		////状态设置
		//if ((bAroseAction==false)&&(i!=m_wBankerUser)&&(m_cbUserAction[i]!=WIK_NULL))
		//{
		//	bAroseAction=true;
		//	m_wResumeUser=m_wCurrentUser;
		//	m_wCurrentUser=INVALID_CHAIR;
		//}
	}

	//osc update
	m_wCurrentUser=m_wBankerUser;
	m_wProvideUser=m_wBankerUser;
	m_wResumeUser=m_wBankerUser;

	//构造数据
	CMD_S_GameStart GameStart;
	memset(&GameStart, 0, sizeof(CMD_S_GameStart));
	GameStart.wSiceCount=m_wSiceCount;
	//GameStart.wMPIndex=m_wMPIndex;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wCurrentUser=m_wCurrentUser;     
	CopyMemory(GameStart.bTrustee,m_bTrustee,sizeof(m_bTrustee));
	//GameStart.cbKingCard = m_cbKingCard[0];

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//设置变量
		memset(GameStart.byGangCard, 0, sizeof(GameStart.byGangCard));
		memset(GameStart.byBuCard, 0, sizeof(GameStart.byBuCard));
		GameStart.byGangCardNums = 0;
		GameStart.byBuCardNums = 0;
		GameStart.cbUserAction=m_cbUserAction[i];

		//初始化补杠值
		memset(m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		memset(m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;

		if (i == m_wBankerUser)
		{
			if (byGangCardNums>0 && byGangCardNums<=4)
			{
				memcpy(GameStart.byGangCard, byGangCard, sizeof(BYTE)*byGangCardNums);
				GameStart.byGangCardNums = byGangCardNums;

				memcpy(m_byGangCard[i], byGangCard, sizeof(BYTE)*byGangCardNums);
				m_byGangCardNums[i] = byGangCardNums;
			}
			if (GangCardResult.cbCardCount>=1)
			{
				memcpy(GameStart.byBuCard, GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
				GameStart.byBuCardNums = GangCardResult.cbCardCount;

				memcpy(m_byBuCard[i], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
				m_byBuCardNums[i] = GangCardResult.cbCardCount;
			}
		}

		//手牌
		memset(GameStart.cbCardData, 0, sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(CMD_S_GameStart));
	}

	return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	CString str;
	str.Format(_T("server: OnEventGameConclude 637---cbReason %d---%+I64d,%+I64d,%+I64d,%+I64d"), cbReason, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
	OutputDebugString(str);

	m_bIsPlayDice=false;
	::memset(m_bTrustee, 0, sizeof(m_bTrustee));
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;
			GameEnd.bIsHuangzhuang = false;

			//结束信息
			//GameEnd.bKingCard=m_cbKingCard[0];
			GameEnd.wProvideUser=wChairID;
			GameEnd.cbChiHuCard=m_cbChiHuCard;
			GameEnd.cbHaiDiCard=m_cbRepertoryCard[0];
			GameEnd.iChiHuType=m_byGameEndType;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				GameEnd.wChiHuKind[i]=m_ChiHuResult[i].wChiHuKind;  //胡牌类型
				GameEnd.wChiHuRight[i]=m_ChiHuResult[i].wChiHuRight; //胡牌权位
				GameEnd.cbKingCount[i]=m_ChiHuResult[i].m_bKingCount;  //王牌个数
			}

			CString str2;
			str2.Format(_T("server: OnEventGameConclude 678--m_byGameEndType %d,(%d,%d,%d,%d)"), m_byGameEndType,m_ChiHuResult[0].wChiHuKind,m_ChiHuResult[1].wChiHuKind,
				m_ChiHuResult[2].wChiHuKind,m_ChiHuResult[3].wChiHuKind);
			OutputDebugString(str2);

			//变量定义
			WORD wChiHuUserCount=0;

			//cbChiHuOrder 为胡牌玩家所有的番数

			//统计积分  自摸和抓炮，特殊牌型（起手牌）
			if (m_wProvideUser!=INVALID_CHAIR  &&  0!=m_byGameEndType &&  5!=m_byGameEndType)
			{
				OutputDebugString(_T("server: OnEventGameConclude : 有人胡牌"));
				//有人胡牌
				//自摸类型
				if ((1==m_byGameEndType || 3==m_byGameEndType)   &&   (m_ChiHuResult[m_wProvideUser].wChiHuKind!=CHK_NULL))
				{
					//自摸,除海底自摸
					OutputDebugString(_T("server: OnEventGameConclude : 有人胡牌 自摸"));
					////翻数计算
					//BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[m_wProvideUser]);
					//if(0==GameEnd.cbKingCount[m_wProvideUser] || 3==GameEnd.cbKingCount[m_wProvideUser])
					//{//三个王
					//	cbChiHuOrder+=1;
					//}
					//else if(4==GameEnd.cbKingCount[m_wProvideUser])
					//{//四个王
					//	cbChiHuOrder+=2;
					//}
					//if (m_bUserFirstIsTingPai[m_wProvideUser])
					//{//第一轮报听
					//	cbChiHuOrder+=1;
					//}
					//if (0==cbChiHuOrder) cbChiHuOrder=1;
					//cbChiHuOrder++;//算自摸

					ZeroMemory(GameEnd.iMuscle, sizeof(GameEnd.iMuscle));
					GameEnd.iMuscle[m_wProvideUser] = m_llMuscle[m_wProvideUser];
					// 自摸(其他人赔 两倍 + 自摸玩家的筋数目) * 底分
					GameEnd.cbTimes[m_wProvideUser] = (m_llMuscle[m_wProvideUser] + 2) * (GAME_PLAYER-1);
					GameEnd.lGameScore[m_wProvideUser] = lCellScore * GameEnd.cbTimes[m_wProvideUser];
					for (BYTE i=0; i<GAME_PLAYER; i++)
					{
						//赢家过滤
						if (i == m_wProvideUser)
						{
							continue;
						}

						// 其余3家给自摸底分*2倍的钱，（注：不是自摸玩家，之前拿的筋不再算分）
						GameEnd.cbTimes[i] = - (m_llMuscle[m_wProvideUser]+2);
						GameEnd.lGameScore[i] = lCellScore*GameEnd.cbTimes[i];
					}

					//for (WORD i=0;i<m_wPlayerCount;i++)
					//{
					////赢家过滤
					//if (i==m_wProvideUser) continue;

					//GameEnd.lGameScore[i]-=((i==m_wBankerUser) || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					//GameEnd.lGameScore[m_wProvideUser]+=(i==m_wBankerUser || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					////番数统计
					////GameEnd.cbTimes[i]=((i==m_wBankerUser) || (m_wProvideUser==m_wBankerUser))?(-2*cbChiHuOrder) : (-cbChiHuOrder);
					////GameEnd.cbTimes[m_wProvideUser]+=(i==m_wBankerUser || (m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder) : (cbChiHuOrder);
					//GameEnd.cbTimes[i]=GameEnd.lGameScore[i]/lCellScore;
					//GameEnd.cbTimes[m_wProvideUser]=(GameEnd.lGameScore[m_wProvideUser]/lCellScore);
					//}
					GameEnd.iChiHuKind[m_wProvideUser]=1;

					//胡牌数目
					wChiHuUserCount=1;

					//庄家设置
					m_wBankerUser=m_wProvideUser;

				}
				else if (2==m_byGameEndType || 4==m_byGameEndType) //捉炮类型
				{
					OutputDebugString(_T("server: OnEventGameConclude 763: 有人胡牌 放炮"));
					//放炮, m_wProvideUser为放炮的玩家
					//变量定义
					WORD wNextBankerUser=INVALID_CHAIR;
					LONGLONG llAllMuscle = 0; //所有赢钱玩家的筋
					GameEnd.iChiHuKind[m_wProvideUser]=3;

					ZeroMemory(GameEnd.iMuscle, sizeof(GameEnd.iMuscle));

					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						// 输家过滤
						if (m_ChiHuResult[i].wChiHuKind==CHK_NULL)
						{
							continue;
						}

						GameEnd.iMuscle[i] = m_llMuscle[i];
						// 加一番, 因为有人放炮 (实际番数=筋数目+放炮一番)
						GameEnd.cbTimes[i] = GameEnd.iMuscle[i] + 1;
						GameEnd.lGameScore[i] = lCellScore*GameEnd.cbTimes[i];
						GameEnd.iChiHuKind[i] = 2;

						// 赔钱玩家手上的筋数目 + 放炮一番
						llAllMuscle += GameEnd.iMuscle[i]+1;
					}

					// 放炮人需要赔的 筋 数目
					GameEnd.cbTimes[m_wProvideUser] = -llAllMuscle; //(几个人 被 放炮)
					GameEnd.lGameScore[m_wProvideUser] = lCellScore*GameEnd.cbTimes[m_wProvideUser];

					//循环累计
					//for (WORD i=0;i<m_wPlayerCount;i++)
					//{
					//	//输家过滤
					//	if (m_ChiHuResult[i].wChiHuKind==CHK_NULL) continue;

					//	//翻数计算
					//	BYTE cbChiHuOrder=m_GameLogic.GetChiHuActionRank(m_ChiHuResult[i]);
					//	if(0==GameEnd.cbKingCount[i] || 3==GameEnd.cbKingCount[i])
					//	{//三个王
					//		cbChiHuOrder+=1;
					//	}
					//	else if(4==GameEnd.cbKingCount[i])
					//	{//四个王
					//		cbChiHuOrder+=2;
					//	}
					//	if (m_bUserFirstIsTingPai[i])
					//	{//第一轮报听
					//		cbChiHuOrder+=1;
					//	}
					//	if (0==cbChiHuOrder) cbChiHuOrder=1;

					//	GameEnd.lGameScore[i]+=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))? (2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					//	GameEnd.lGameScore[m_wProvideUser]-=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder*lCellScore) : (cbChiHuOrder*lCellScore);
					//	//GameEnd.cbTimes[m_wProvideUser]=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))? (-2*cbChiHuOrder) : (-cbChiHuOrder);
					//	//GameEnd.cbTimes[i]=((i==m_wBankerUser)||(m_wProvideUser==m_wBankerUser))?(2*cbChiHuOrder) : (cbChiHuOrder);
					//	GameEnd.cbTimes[m_wProvideUser]=GameEnd.lGameScore[m_wProvideUser]/lCellScore;
					//	GameEnd.cbTimes[i]=(GameEnd.lGameScore[i]/lCellScore);
					//	//GameEnd.iChiHuKind[i]=2;
					//	//设置变量
					//	wNextBankerUser=i;
					//	wChiHuUserCount++;
					//}


					//庄家设置
					m_wBankerUser=(wChiHuUserCount>=2)?m_wProvideUser:wNextBankerUser;
					if ((m_wHaiDiUser!=INVALID_CHAIR)&&(wChiHuUserCount>1)) m_wBankerUser=m_wHaiDiUser;
				}

				//拷贝扑克
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
				}
			}
			else
			{
				OutputDebugString(_T("server: OnEventGameConclude : 荒庄处理"));
				//荒庄处理
				GameEnd.bIsHuangzhuang = true;
				GameEnd.cbChiHuCard=0;
				GameEnd.cbHaiDiCard=m_cbRepertoryCard[0];

				//设置庄家
				m_wBankerUser=(m_wHaiDiUser!=INVALID_CHAIR)?m_wHaiDiUser:m_wResumeUser;

				//拷贝扑克
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					GameEnd.cbCardCount[i]=0;
				}
			}

			//变量定义
			LONGLONG lRevenue=0L;  //服务费
			LONGLONG lUserScore[GAME_PLAYER]={0};
			ZeroMemory(lUserScore,sizeof(lUserScore));
			CopyMemory(lUserScore,GameEnd.lGameScore,sizeof(lUserScore));

			CString str1;
			str1.Format(_T("server: OnEventGameConclude 865--(%I64d,%I64d,%I64d,%I64d)"), GameEnd.lGameScore[0],GameEnd.lGameScore[1],
				GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
			OutputDebugString(str1);


			//写分
			for(int i=0;i<GAME_PLAYER;i++)
			{
				if(lUserScore[i]>0)
				{
					lRevenue=lUserScore[i]*(m_pGameServiceOption->cbRevenueRatio/100.0f);
					GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
				}
				else
				{
					lRevenue = 0;
				}

				GameEnd.lGameScore[i]=lUserScore[i]-lRevenue;
				GameEnd.iCellScore[i]=0;

				//写入积分
				LONGLONG lScore=GameEnd.lGameScore[i];
				BYTE ScoreKind;
				if(lScore>0L)
				{
					ScoreKind=SCORE_TYPE_WIN;
					GameEnd.iCellScore[i] = lScore*(0.01*m_pGameServiceOption->cbRevenueRatio);
				}

				else if(lScore==0L) 	
				{
					ScoreKind=SCORE_TYPE_DRAW;
				}
				else				
				{
					ScoreKind=SCORE_TYPE_LOSE;
				}

				CString str;
				str.Format(_T("server: OnEventGameConclude 899---%d,(%I64d,%I64d),%d"), i,GameEnd.lGameScore[i],lRevenue,ScoreKind);
				OutputDebugString(str);

				///m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,ScoreKind);

				////玩家任务判断
				//if (enScoreKind_Win==ScoreKind)
				//{
				//	OnUserTaskJudgement(i, 1);
				//}

			}

			CString str;
			str.Format(_T("server: OnEventGameConclude 909---%d,%d"), m_pGameServiceOption->cbRevenueRatio,m_wProvideUser);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 889---lGameScore 游戏积分,%+I64d,%+I64d,%+I64d,%+I64d"), GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 890---iCellScore 服务费,%+I64d,%+I64d,%+I64d,%+I64d"), GameEnd.iCellScore[0],GameEnd.iCellScore[1],GameEnd.iCellScore[2],GameEnd.iCellScore[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 891---iChiHuKind 胡牌类型,%d,%d,%d,%d"), GameEnd.iChiHuKind[0],GameEnd.iChiHuKind[1],GameEnd.iChiHuKind[2],GameEnd.iChiHuKind[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 892---cbTimes 番数,%d,%d,%d,%d"), GameEnd.cbTimes[0],GameEnd.cbTimes[1],GameEnd.cbTimes[2],GameEnd.cbTimes[3]);
			OutputDebugString(str);
			str.Format(_T("server: OnEventGameConclude 893---iMuscle 筋,%d,%d,%d,%d"), GameEnd.iMuscle[0],GameEnd.iMuscle[1],GameEnd.iMuscle[2],GameEnd.iMuscle[3]);
			OutputDebugString(str);

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(CMD_S_GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(CMD_S_GameEnd));
			//结束游戏
			///m_pITableFrame->ConcludeGame();
#if 1//def USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif

			if (m_wBankerUser>=GAME_PLAYER)
			{
				m_wBankerUser = INVALID_CHAIR;
			}
			return true;
		}
	case GER_USER_LEAVE:		//用户强退
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			GameEnd.bIsHuangzhuang = false;

			//变量定义
			LONGLONG lCellScore=m_pGameServiceOption->lCellScore;	//单元积分
			DOUBLE lRestrictScore=m_pGameServiceOption->lRestrictScore;	//输分限制
			LONGLONG Revenue = m_pGameServiceOption->cbRevenueRatio;	//税收比例
			LONGLONG lRevenue=0L;
			//lRevenue = (lRestrictScore/3)*(Revenue/100.0f);
			lRevenue = lCellScore;

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;

			//GameEnd.bKingCard=m_cbKingCard[0];

			//统计积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if (i==wChairID) 
				{
					GameEnd.lGameScore[i]=-lRestrictScore - lRevenue;
					GameEnd.IsUserLeft[i]=true;
				}
				else 
				{
					GameEnd.lGameScore[i]=lRestrictScore/3 - lRevenue;
					GameEnd.IsUserLeft[i]=false;
				}
			}


			CString str;
			str.Format(_T("server: OnEventGameConclude 949---%+I64d,%+I64d,%+I64d,%+I64d"), GameEnd.lGameScore[0],GameEnd.lGameScore[1],GameEnd.lGameScore[2],GameEnd.lGameScore[3]);
			OutputDebugString(str);

			////通知消息
			//TCHAR szMessage[128]=TEXT("");
			//_snprintf(szMessage,CountArray(szMessage),TEXT("由于 [ %s ] 离开游戏，游戏结束"),pIServerUserItem->GetAccounts());
			//for (WORD i=0;i<m_wPlayerCount;i++)
			//{
			//	IServerUserItem * pISendUserItem=m_pITableFrame->GetServerUserItem(i);
			//	if (pISendUserItem!=NULL) m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			//}
			//WORD wIndex=0;
			//do
			//{
			//	IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			//	if (pISendUserItem==NULL) break;
			//	m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_INFO);
			//} while (true);


			//修改积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				GameEnd.iCellScore[i]=0;
				if (wChairID==i)
				{
					//m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],0L,enScoreKind_Flee);
				}
				else
				{
					//m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lRevenue,enScoreKind_Win);
				}
				if (GameEnd.lGameScore[i]>0)
				{
					GameEnd.iCellScore[i] = GameEnd.lGameScore[i]*(0.01*m_pGameServiceOption->cbRevenueRatio);
				}
			}

			CString str1;
			str1.Format(_T("server: OnEventGameConclude 989-- 发送消息 SUB_S_GAME_END 强退"));
			OutputDebugString(str1);

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			///m_pITableFrame->ConcludeGame();
#if 1//def USE_GB
			m_pITableFrame->ConcludeGame(GS_MJ_FREE,NULL,608);
#else
			m_pITableFrame->ConcludeGame(GS_MJ_FREE/*,NULL,608*/);
#endif

			m_wBankerUser = INVALID_CHAIR;
			return true;
		}
	}

	return false;
}

//发送场景
bool  CTableFrameSink::OnEventSendGameScene(WORD wChiarID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//变量定义
			CMD_S_StatusFree StatusFree;
			memset(&StatusFree,0,sizeof(StatusFree));

			//构造数据
			StatusFree.wBankerUser=m_wBankerUser;
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;
			CopyMemory(StatusFree.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_MJ_PLAY:	//游戏状态
	case GS_MJ_HAI_DI:	//海底状态
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			bool bIsMeTingPai = false;
			//if (m_bIsUserFirstOutCard[wChiarID] && m_wOutCardUser==wChiarID)
			//{
			//	bool bIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChiarID], m_WeaveItemArray[wChiarID], m_cbWeaveItemCount[wChiarID], m_cbKingCard);
			//	if (bIsTing)
			//	{
			//		bIsMeTingPai = true;
			//	}
			//}
			StatusPlay.m_bTingCard=bIsMeTingPai;				

			//王牌
			//StatusPlay.byKindCard = m_cbKingCard[0];

			StatusPlay.bIsPlayDice=m_bIsPlayDice;

			//游戏变量
			StatusPlay.wSiceCount=m_wSiceCount;
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.bIsAlreadyOper = m_bResponse[wChiarID];

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			//StatusPlay.cbHearStatus=(m_bHearStatus[wChiarID]==true)?TRUE:FALSE;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);

			//杠补信息
			memcpy(StatusPlay.byGangCard, m_byGangCard[wChiarID], sizeof(BYTE)*m_byGangCardNums[wChiarID]);
			StatusPlay.byGangCardNums = m_byGangCardNums[wChiarID];
			memcpy(StatusPlay.byBuCard, m_byBuCard[wChiarID], sizeof(BYTE)*m_byBuCardNums[wChiarID]);
			StatusPlay.byBuCardNums = m_byBuCardNums[wChiarID];

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	CString strBuffer;
	tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
	strBuffer.Format(_T("server: CTableFrameSink::OnGameMessage() wSubCmdID=%d, wDataSize=%d, wChairID=%d"), wSubCmdID, wDataSize, pUserData->wChairID); 
	OutputDebugString(strBuffer);

	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pUserData->wChairID,pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	//case SUB_C_OPERATE_HAI_DI:	//海底操作
	//	{
	//		//效验消息
	//		ASSERT(wDataSize==sizeof(CMD_C_OperateHaiDi));
	//		if (wDataSize!=sizeof(CMD_C_OperateHaiDi)) return false;

	//		//用户效验
	//		tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	//		if (pUserData->cbUserStatus!=US_PLAY) return true;

	//		//消息处理
	//		CMD_C_OperateHaiDi * pOperateHaiDi=(CMD_C_OperateHaiDi *)pDataBuffer;
	//		return OnUserOperateHaiDi(pUserData->wChairID,pOperateHaiDi->cbTakeHaiDi);
	//	}
	case SUB_C_TRUSTEE:			//托管操作
		{
			//TestGameEndFun(0);return true;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo/*GetUserData*/();

			m_bTrustee[pUserData->wChairID]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pUserData->wChairID;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			return true;
		}
	case SUB_C_USERTINGCARD:	//听牌操作
		{
			if(wDataSize != sizeof(CMD_UserTingOperate)) return false;
			CMD_UserTingOperate *TingOperate =(CMD_UserTingOperate *)pDataBuffer;

			//用户效验
			//tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			return OnUserTingCard(pIServerUserItem->GetChairID(), TingOperate->bTingCard);
		}
	case SUB_C_SZSTART:
		{
			m_bIsPlayDice=false;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_C_SZSTART);
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_C_SZSTART);
		}
		break;
	default:
		break;
	}

	return false;
}

//用户听牌操作
bool CTableFrameSink::OnUserTingCard(WORD wChairID, bool bIsTing)
{
	//if (wChairID>=GAME_PLAYER || !m_bIsUserFirstOutCard[wChairID])
	{
		return false;
	}

	//bool bRetIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbKingCard);
	//if (bRetIsTing)
	//{
	//	m_bIsUserFirstOutCard[wChairID] = false;
	//	m_bUserFirstIsTingPai[wChairID] = bIsTing; //是不是第一次听牌.
	//	m_bHearStatus[wChairID] = bIsTing;

	//	if (bIsTing)
	//	{
	//		//告诉客户端用户某个玩家听牌了。
	//		CMD_S_GameTingCard userting;
	//		userting.wTingCardUser = wChairID;
	//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_USER_TINGCARD_RESULT,&userting,sizeof(CMD_S_GameTingCard));
	//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_USER_TINGCARD_RESULT,&userting,sizeof(CMD_S_GameTingCard));
	//	}
	//	//用户开始操作
	//	UserStartOperation();
	//}

	return true;
}

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	////庄家设置
	//if ((bLookonUser==false)&&(m_wBankerUser==INVALID_CHAIR))
	//{
	//	m_wBankerUser=pIServerUserItem->GetChairID();
	//}

	return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	////庄家设置
	//if ((bLookonUser==false)&&(wChairID==m_wBankerUser))
	//{
	//	m_wBankerUser=INVALID_CHAIR;

	//	for (WORD i=0;i<m_wPlayerCount;i++)
	//	{
	//		if ((i!=wChairID)&&(m_pITableFrame->GetServerUserItem(i)!=NULL))
	//		{
	//			m_wBankerUser=i;
	//			m_bTrustee[wChairID]=false;
	//			CMD_S_Trustee Trustee;
	//			Trustee.bTrustee=false;
	//			Trustee.wChairID = wChairID;
	//			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	//			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));


	//			break;
	//		}
	//	}
	//}

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) 
	{
		return true;
	}

	//效验参数
	if (wChairID!=m_wCurrentUser)
	{
		return false;
	}
	if (m_GameLogic.IsValidCard(cbCardData)==false)
	{
		return false;
	}

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置变量
	m_bIsPlayDice=false;
	m_bSendStatus=true;
	m_bGangStatus=false;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//出牌记录
	m_cbOutCardCount++;
	m_wOutCardUser=wChairID;
	m_cbOutCardData=cbCardData;

	//构造数据
	CMD_S_OutCard OutCard;
	OutCard.wOutCardUser=wChairID;
	OutCard.cbOutCardData=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OUT_CARD,&OutCard,sizeof(OutCard));

	//用户切换
	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//if (m_bIsUserFirstOutCard[wChairID])
	//{//是不是第一圈
	//bool bIsTing = m_GameLogic.IsCanTingCard(m_cbCardIndex[wChairID], m_WeaveItemArray[wChairID], m_cbWeaveItemCount[wChairID], m_cbKingCard);
	//if (bIsTing)
	//{
	//	CMD_S_GameTingCard tingcard;
	//	tingcard.wTingCardUser = wChairID;
	//	m_pITableFrame->SendTableData(wChairID,SUB_S_TING_CARD,&tingcard,sizeof(CMD_S_GameTingCard));
	//	return true;
	//}
	//else
	//{
	//m_bIsUserFirstOutCard[wChairID] = false;
	//}
	//}

	//用户开始操作
	UserStartOperation();

	return true;
}

//用户操作
void CTableFrameSink::UserStartOperation()
{
	//响应判断
	bool bAroseAction=EstimateUserRespond(m_wProvideUser,m_cbProvideCard,EstimatKind_OutCard);

	//抢杠设置
	//if (m_bGangStatus)
	//{
	//	for (WORD i=0;i<m_wPlayerCount;i++)
	//	{
	//		if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) break;
	//	}
	//	
	//	if (i==m_wPlayerCount) 
	//	{
	//		m_bGangStatus=false;
	//	}
	//}

	//派发扑克（打一张牌，其他玩家不能操作（如碰，补，胡）时发牌
	if (bAroseAction==false) 
	{
		m_bSendStatus = true;
		DispatchCardData(m_wCurrentUser, false);
	}
}

bool CTableFrameSink::OnWriteMuscleScore(WORD wWinChairID, WORD wLostChairID, int iMuscle, LONGLONG llResultGold[])
{
	ASSERT(0<=wWinChairID  &&  GAME_PLAYER>wWinChairID);
	ASSERT(0<=wLostChairID  &&  GAME_PLAYER>wLostChairID);
	if (0>wWinChairID || GAME_PLAYER<=wWinChairID || 0>wLostChairID || GAME_PLAYER<=wLostChairID)
	{
		CString strBuffer;
		strBuffer.Format(_T("Error: CTableFrameSink::OnWriteMuscleScore(WORD,WORD,int) wWinChairID=%d, wLostChairID=%d, iMuscle=%d"), 
			wWinChairID, wLostChairID, iMuscle);
		OutputDebugString(strBuffer);
		return false;
	}

	if (0 > iMuscle)
	{
		iMuscle *=  -1;
	}

	++m_llMuscle[wWinChairID];
	ZeroMemory(llResultGold, sizeof(LONGLONG)*4);
	llResultGold[wWinChairID] = iMuscle*m_pGameServiceOption->lCellScore*(1-0.01*m_pGameServiceOption->cbRevenueRatio);//扣税
	llResultGold[wLostChairID] = -(iMuscle*m_pGameServiceOption->lCellScore);

/*	m_pITableFrame->WriteUserScore(wWinChairID, llResultGold[wWinChairID], 0, enScoreKind_Present);
	m_pITableFrame->WriteUserScore(wLostChairID, llResultGold[wLostChairID], 0, enScoreKind_Present);*/		
	return true;
}

bool CTableFrameSink::OnWriteMuscleScore(WORD wWinChairID, int iMuscle, LONGLONG llResultGold[])
{
	ASSERT(0<=wWinChairID  &&  GAME_PLAYER>wWinChairID);
	if (0>wWinChairID || GAME_PLAYER<=wWinChairID)
	{
		CString strBuffer;
		strBuffer.Format(_T("Error: CTableFrameSink::OnWriteMuscleScore(WORD,int) wWinChairID=%d, iMuscle=%d"), wWinChairID, iMuscle);
		OutputDebugString(strBuffer);
		return false;
	}

	if (0 > iMuscle)
	{
		iMuscle *=  -1;
	}

	++m_llMuscle[wWinChairID];
	ZeroMemory(llResultGold, sizeof(LONGLONG)*4);
	llResultGold[wWinChairID] = iMuscle*m_pGameServiceOption->lCellScore*(1-0.01*m_pGameServiceOption->cbRevenueRatio);//扣税

	//m_pITableFrame->WriteUserScore(wWinChairID, iMuscle*m_pGameServiceOption->lCellScore, 0, enScoreKind_Present);

	for (WORD wID=0; wID<GAME_PLAYER; wID++)
	{
		if (wID == wWinChairID)
		{
			continue;
		}

		llResultGold[wID] = -(iMuscle/(GAME_PLAYER-1)*m_pGameServiceOption->lCellScore);
		//m_pITableFrame->WriteUserScore(wID, llResultGold[wID], 0, enScoreKind_Present);
	}

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//效验状态（所有的操作只能在游戏状态）
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//效验用户（wChairID==m_wCurrentUser表示自己操作，m_wCurrentUser==INVALID_CHAIR表示其他玩家操作）
	// 其他玩家能操作的条件，在EstimateUserRespond函数中 m_wCurrentUser设置INVALID_CHAIR
	// 不可能同时出现自己能操作，其他玩家能操作，规则是这样的。
	ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) return true;

	// 没有牌，游戏结束
	// 因为没有海底概念，所以抓最后一张牌的，如果玩家能胡，却放弃在这里游戏结束
	// 如果不能在发牌函数已经处理
	if (WIK_NULL==cbOperateCode  &&  0>=m_cbLeftCardCount)
	{
		m_byGameEndType = 0; //留局
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
		return true;
	}

	//取消杠
	//if (WIK_NULL==cbOperateCode && 0==cbOperateCard)
	//{
	//	m_tagUserGSKUResult[wChairID].bIsGSKU = false;
	//}

	if (INVALID_CHAIR == m_wCurrentUser) //被动动作（其他玩家唤醒操作，当前玩家不是我）
	{
		return OnOtherUserOperateCard(wChairID, cbOperateCode, cbOperateCard);
	}
	else if (wChairID == m_wCurrentUser) //主动动作（自己触发，当前玩家是我）
	{
		return OnCurUserOperateCard(wChairID, cbOperateCode, cbOperateCard);
	}

	return false;
}

/// 当前玩家操作
bool CTableFrameSink::OnCurUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	// 筋变动金币
	LONGLONG llReturnMuscleGold[GAME_PLAYER]={0};
	bool bBrightMuscleCanHu = false; //明筋时是否有玩家能胡牌

	//效验操作
	if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

	//扑克效验，检查出牌玩家打出牌的合法性
	if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard)==false))
	{
		return false;  
	}

	//设置变量
	m_bSendStatus=true;
	m_cbUserAction[wChairID]=WIK_NULL;
	m_cbPerformAction[wChairID]=WIK_NULL;

	//执行动作
	switch (cbOperateCode)
	{
	case WIK_FILL:			//补牌操作
	case WIK_GANG:			//杠牌操作
	//case WIK_TING:			//听牌操作
		{
			//变量定义
			//bool bAnGang = false;		//是不是暗杠
			BYTE cbWeaveIndex=0xFF;
			BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard);

			//杠牌处理，用户手中这张麻将只有一个（除组合牌），因此下面去组合牌堆查找
			if (m_cbCardIndex[wChairID][cbCardIndex]==1)
			{
				// 从碰的组合牌堆里寻找可以能否补
				for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
					if ((cbCenterCard==cbOperateCard)&&(cbWeaveKind==WIK_PENG))
					{
						cbWeaveIndex=i;
						break;
					}
				}

				//效验动作
				if (cbWeaveIndex==0xFF)
				{
					return false;
				}

				//组合扑克，补动作加入组合牌堆保存
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;

				// 明筋时其他玩家有胡，需要告知其他玩家是否需要胡，如果没人愿意胡，此时明筋才有效
				for (BYTE i=0; i<GAME_PLAYER; i++)
				{
					if (i == wChairID)
						continue;

					m_cbUserAction[i] = NULL;
					//吃胡判断
					tagChiHuResult ChiHuResult;
					BYTE cbWeaveCount=m_cbWeaveItemCount[i];
					m_cbUserAction[i]=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbOperateCard,0,ChiHuResult);
					if (WIK_NULL != m_cbUserAction[i])
					{
						bBrightMuscleCanHu = true;
					}
				}

				// 明筋时没有玩家能胡，明筋操作成功
				if ( !bBrightMuscleCanHu )
				{
					// 明筋
					OnWriteMuscleScore(wChairID, 3, llReturnMuscleGold);

					CString str;
					str.Format(_T("server: OnUserOperateCard 1641-%d-----%+I64d,%+I64d,%+I64d,%+I64d"), wChairID, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
					OutputDebugString(str);
				}
			}
			else // 自己不能碰自己的麻将，所以从手里的牌检查是否能补
			{//杠的情况
				//扑克效验
				ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
				if (m_cbCardIndex[wChairID][cbCardIndex]!=4)
				{
					return false;
				}

				//设置变量，组合扑克，补动作加入组合牌堆保存
				cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
				m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
				m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard;
				//bAnGang = true;

				// 暗筋
				OnWriteMuscleScore(wChairID, 6, llReturnMuscleGold);

				CString str;
				str.Format(_T("server: OnUserOperateCard 1670-%d-----%+I64d,%+I64d,%+I64d,%+I64d"), wChairID, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
				OutputDebugString(str);
			}

			//设置状态
			m_bGangStatus=true;
			//if (WIK_TING == cbOperateCode)
			//{//听牌
				//设置状态
				//m_bHearStatus[wChairID]=true;
				//m_bEnjoinChiPeng[wChairID]=true;
				//m_tagUserGSKUResult[wChairID].bIsGang = true;
				//if (m_bIsUserFirstOutCard[wChairID])
				//{
				//	m_bUserFirstIsTingPai[wChairID] = true; //可以翻倍
				//}
				//m_bIsUserFirstOutCard[wChairID] = false;
			//}

			//构造结果
			CMD_S_OperateResult OperateResult;
			OperateResult.wOperateUser=wChairID;
			OperateResult.wProvideUser=wChairID;
			OperateResult.cbOperateCode=cbOperateCode;
			OperateResult.cbOperateCard=cbOperateCard;
			OperateResult.bBringMuscleCanHu=bBrightMuscleCanHu;
			OperateResult.bMuscle = ((1==m_cbCardIndex[wChairID][cbCardIndex]) ? 2 : 1);
			CopyMemory(OperateResult.llMuscleGold, llReturnMuscleGold, sizeof(llReturnMuscleGold));

			//发送消息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

			//删除扑克，刚才补的那张麻将数目赋0，达到删除麻将作用
			m_cbCardIndex[wChairID][cbCardIndex]=0;

			// 明筋有玩家能胡，告诉能胡的胡家是否需要胡牌
			// 当前明筋数据暂不处理，保存在 m_OperateResult 中
			if ( bBrightMuscleCanHu )
			{
				m_wCurrentUser=INVALID_CHAIR;
				m_wResumeUser = wChairID;
				m_cbProvideCard = cbOperateCard;
				CopyMemory(&m_OperateResult, &OperateResult, sizeof(OperateResult));
				SendOperateNotify();
			}
			else
			{
				//发送扑克
				m_bSendStatus = true;
				DispatchCardData(wChairID, true);
			}

			return true;
		}
	case WIK_CHI_HU:		//吃胡操作
		{
			//吃牌权位
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) 
			//	m_byUserHuCardType[wChairID]|=CHR_TIAN;

			//if ((wChairID == m_wProvideUser) && m_tagUserGSKUResult[wChairID].bIsGang
			//	/*&& (m_iSaveGameOutCardCount==m_cbOutCardCount) 
			//	&& (m_iSaveGameSendCardCount==m_cbSendCardCount)*/)
			//{//杠上开花
			//	m_tagUserGSKUResult[wChairID].bIsGSKU = true;
			//	m_byUserHuCardType[wChairID] |= CHR_QIANG_GANG;
			//}

			//普通胡牌
			//BYTE cbReCardIndex[MAX_INDEX];
			BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
			tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
			m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,
				cbWeaveItemCount,0,m_byUserHuCardType[wChairID],m_ChiHuResult[wChairID]);
			//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,0,m_byUserHuCardType[wChairID],m_ChiHuResult[wChairID],m_cbKingCard,4,cbReCardIndex);

			//结束信息
			m_cbChiHuCard=m_cbProvideCard;

			//结束游戏
			m_byGameEndType = 1; //自摸
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}
	default: break;
	}

	return true;
}

/// 其他玩家操作
bool CTableFrameSink::OnOtherUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)
{
	//效验状态
	//ASSERT(m_bResponse[wChairID]==false);
	//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

	//效验状态
	// 每个玩家只能操作一次，用来记忆玩家是否操作过，来确认能操作的所有玩家都有做出响应
	if (m_bResponse[wChairID]==true)
		return true;

	// 玩家不是取消操作，但服务器记录他的操作是“没有操作”，此类玩家发过来的是非法数据
	if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) 
		return true;

	//变量定义
	WORD wTargetUser=wChairID;
	BYTE cbTargetAction=cbOperateCode;

	// 筋变动金币
	LONGLONG llReturnMuscleGold[GAME_PLAYER]={0};
	bool bBrightMuscleCanHu = false; //明筋时是否有玩家能胡牌

	//设置变量
	m_bResponse[wChairID]=true;
	m_cbPerformAction[wChairID]=cbOperateCode;
	// 玩家可以操作，但是传过来的数据非法，则用刚打出的牌替换，因为是这张牌触发的动作
	m_cbOperateCard[wChairID]=(0==cbOperateCard)?m_cbProvideCard:cbOperateCard;

	//bool bIsOtherGangAction = false;
	//执行判断（循环判断动作权限，权限最大的玩家，ID、权限分别保存在 wTargetUser，cbTargetAction）
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取玩家操作的动作或者 能 操作的动作
		BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

		//if ((cbUserAction&WIK_TING) > 0)
		//{
		//	bIsOtherGangAction = true;
		//}

		//优先级别
		BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
		BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

		//动作判断
		if (cbUserActionRank>cbTargetActionRank)
		{
			wTargetUser=i;//操作权限最大的玩家
			cbTargetAction=cbUserAction;
		}
	}

	// 权限最大的玩家还没操作
	if (m_bResponse[wTargetUser]==false) 
	{
		return true;
	}

	// 如果最大权限是胡，则判断能胡的所有玩家都操作了没, 还有没有操作的，继续等待
	if (WIK_CHI_HU == cbTargetAction)//吃胡等待
	{
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU)) 
				return true;
		}
	}
	else if (WIK_NULL == cbTargetAction) //虽然玩家有操作，但是都是放弃
	{
		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//初始化
		for(int i=0; i<m_wPlayerCount; i++)
		{
			m_cbUserAction[i] = WIK_NULL;
			::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
			m_byGangCardNums[i] = 0;
			::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
			m_byBuCardNums[i] = 0;
		}

		// 明筋时有玩家能胡，不过这个玩家没有胡，所以之前的明筋仍然有效
		if (m_OperateResult.bBringMuscleCanHu)
		{
			OnWriteMuscleScore(m_OperateResult.wOperateUser, 3, llReturnMuscleGold);

			CString str;
			str.Format(_T("server: OnUserOperateCard 1641-明筋能胡-终于扣金币了-%d-----%+I64d,%+I64d,%+I64d,%+I64d"), m_OperateResult.wOperateUser, m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
			OutputDebugString(str);

			m_OperateResult.bBringMuscleCanHu = false;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&m_OperateResult,sizeof(m_OperateResult));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&m_OperateResult,sizeof(m_OperateResult));
			ZeroMemory(&m_OperateResult, sizeof(m_OperateResult));
		}

		//发送扑克
		m_bSendStatus = true;
		DispatchCardData(m_wResumeUser, false);//by han 20120104
		return true;
	}

	//变量定义
	BYTE cbTargetCard=m_cbOperateCard[wTargetUser];
	//胡牌操作
	if (cbTargetAction==WIK_CHI_HU)
	{
		//结束信息
		m_cbChiHuCard=cbTargetCard;

		//胡牌判断，循环检查所有玩家是否能胡牌，如果有玩家能胡牌，则游戏结束（放炮）
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//BYTE bKingCount=0;
			//bKingCount=m_GameLogic.GetKingCardCount(m_cbCardIndex[i],NULL,0,m_cbKingCard,4);
			//if (0 != bKingCount)
			//{
			//	continue;
			//}
			//过虑判断
			if ((i==m_wProvideUser)||((m_cbPerformAction[i]&WIK_CHI_HU)==0))
				continue;

			//吃牌权位
			//WORD wChiHuRight=0;
			//if ((1==m_cbSendCardCount)&&(1==m_cbOutCardCount))
			//{
			//	m_byUserHuCardType[i]|=CHR_DI;
			//}
			if (m_OperateResult.bBringMuscleCanHu
				/*m_tagUserGSKUResult[m_wOutCardUser].bIsGang*/ 
				/*&& (m_iSaveGameOutCardCount==m_cbOutCardCount-1)
				&& (m_iSaveGameSendCardCount==m_cbSendCardCount)*/)
			{//杠上炮
				m_byUserHuCardType[i]|=CHR_QIANG_GANG;
			}
			ZeroMemory(&m_OperateResult, sizeof(m_OperateResult));
			//if (bIsOtherGangAction)
			//{//抢杠胡
			//	m_byUserHuCardType[i]|=CHR_QIANG_GANG;
			//}

			//普通胡牌
			if (0 != m_cbChiHuCard)
			{
				//胡牌判断
				//BYTE cbReCardIndex[MAX_INDEX];
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_byUserHuCardType[i],m_ChiHuResult[i]);
				//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_byUserHuCardType[i],m_ChiHuResult[i],m_cbKingCard,4,cbReCardIndex);

				//插入扑克
				if (CHK_NULL != m_ChiHuResult[i].wChiHuKind)
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
				}
			}
		}

		//结束游戏
		ASSERT(CHK_NULL != m_ChiHuResult[wTargetUser].wChiHuKind);
		m_byGameEndType = 2; //放炮--抢杠胡
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
		return true;
	}

	//出牌变量（上面胡已经处理，这里只处理碰，补等操作）
	m_cbOutCardData=0;
	m_bSendStatus=true;
	m_wOutCardUser=INVALID_CHAIR;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction)); 
	ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//组合扑克
	ASSERT(4 >= m_cbWeaveItemCount[wTargetUser]);
	if (4 < m_cbWeaveItemCount[wTargetUser])
		return true;

	WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
	m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
	m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
	m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
	m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;

	//删除扑克
	switch (cbTargetAction)
	{
		//case WIK_LEFT:		//上牌操作
		//	{
		//		//删除扑克
		//		BYTE cbRemoveCard[]={cbTargetCard+1,cbTargetCard+2};
		//		m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

		//		break;
		//	}
		//case WIK_RIGHT:		//上牌操作
		//	{
		//		//删除扑克
		//		BYTE cbRemoveCard[]={cbTargetCard-2,cbTargetCard-1};
		//		m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

		//		break;
		//	}
		//case WIK_CENTER:	//上牌操作
		//	{
		//		//删除扑克
		//		BYTE cbRemoveCard[]={cbTargetCard-1,cbTargetCard+1};
		//		m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

		//		break;
		//	}
	case WIK_PENG:		//碰牌操作
		{
			//删除扑克
			BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
			m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));

			break;
		}
	case WIK_FILL:		//补牌操作
	case WIK_GANG:		//杠牌操作
		//case WIK_TING:		//听牌操作
		{
			//删除扑克（庄家游戏刚开始抓了14张麻将，这个时候有牌补需要删除4张麻将）在OnEventGameStart()函数中实现
			if (1==m_cbSendCardCount  &&  0==m_cbOutCardData)
			{
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
			}
			else //其他情况补，手中只有三张
			{
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard));
			}

			// 放筋
			OnWriteMuscleScore(wTargetUser, m_WeaveItemArray[wTargetUser][wIndex].wProvideUser, 3, llReturnMuscleGold);

			CString str;
			str.Format(_T("server: OnUserOperateCard 1512-%d,%d-----%+I64d,%+I64d,%+I64d,%+I64d"), wTargetUser, m_WeaveItemArray[wTargetUser][wIndex].wProvideUser,
				m_llMuscle[0],m_llMuscle[1],m_llMuscle[2],m_llMuscle[3]);
			OutputDebugString(str);

			str.Format(_T("server: OnUserOperateCard 1749------%+I64d,%+I64d,%+I64d,%+I64d"), llReturnMuscleGold[0],llReturnMuscleGold[1],llReturnMuscleGold[2],llReturnMuscleGold[3]);
			OutputDebugString(str);
			break;
		}
	}

	//构造结果
	CMD_S_OperateResult OperateResult;
	OperateResult.wOperateUser=wTargetUser;
	OperateResult.cbOperateCard=cbTargetCard;
	OperateResult.cbOperateCode=cbTargetAction;
	OperateResult.bMuscle = ((WIK_FILL==cbTargetAction || WIK_GANG==cbTargetAction) ? 3 : 0);
	OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
	CopyMemory(OperateResult.llMuscleGold, llReturnMuscleGold, sizeof(llReturnMuscleGold));

	//设置状态
	if (WIK_GANG==cbTargetAction  ||  WIK_FILL==cbTargetAction/*  ||  WIK_TING==cbTargetAction*/)
	{
		m_bGangStatus=true;
	}
	//if (cbTargetAction==WIK_TING)
	//{
	//m_bHearStatus[wTargetUser]=true;
	//m_bEnjoinChiPeng[wTargetUser]=true;
	//m_tagUserGSKUResult[wTargetUser].bIsGang = true;

	//if (m_bIsUserFirstOutCard[wChairID])
	//{
	//	m_bUserFirstIsTingPai[wChairID] = true; //可以翻倍
	//}
	//m_bIsUserFirstOutCard[wChairID] = false;
	//}

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

	//BYTE byStartID = m_wProvideUser;
	//while (byStartID<m_wPlayerCount)
	//{
	//	//m_bIsUserFirstOutCard[byStartID] = false;
	//	if (byStartID==wTargetUser)
	//	{
	//		break;
	//	}
	//	byStartID = (byStartID+m_wPlayerCount-1)%m_wPlayerCount;
	//}

	//设置用户
	m_wCurrentUser=wTargetUser;
	//杠牌处理
	if (WIK_GANG==cbTargetAction  ||  WIK_FILL==cbTargetAction/*  ||  WIK_TING==cbTargetAction*/)
	{
		//给补（杠）、听牌的玩家发一张牌
		m_bSendStatus = true;
		DispatchCardData(wTargetUser, true);
	}
	return true;
}

////海底操作
//bool CTableFrameSink::OnUserOperateHaiDi(WORD wChairID, BYTE cbTakeHaiDi)
//{
//	//效验状态
//	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_HAI_DI);
//	if (m_pITableFrame->GetGameStatus()!=GS_MJ_HAI_DI) return true;
//
//	//效验用户
//	ASSERT(wChairID==m_wCurrentUser);
//	if (wChairID!=m_wCurrentUser) return false;
//
//	//海底处理
//	if (cbTakeHaiDi==1)
//	{
//		//胡牌判断
//		//BYTE cbReCardIndex[MAX_INDEX];
//		BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
//		tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
//		m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[wChairID]);
//		//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[wChairID],m_cbKingCard,4,cbReCardIndex);
//
//
//		//结果判断
//		if (m_ChiHuResult[wChairID].wChiHuKind==CHK_NULL)
//		{
//			//胡牌判断
//			bool bOtherChiHu=false;
//			for (WORD i=0;i<m_wPlayerCount;i++)
//			{
//				//忽略判断
//				if (i==wChairID) continue;
//
//				//胡牌判断
//				pWeaveItem=m_WeaveItemArray[i];
//				cbWeaveItemCount=m_cbWeaveItemCount[i];
//				m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[i]);
//				//m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,0,m_ChiHuResult[i],m_cbKingCard,4,cbReCardIndex);	
//
//				//结果处理
//				if (m_ChiHuResult[i].wChiHuKind!=CHK_NULL)
//				{
//					bOtherChiHu=true;
//					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
//					m_ChiHuResult[i].wChiHuRight |= CHR_HAI_DI;
//					m_ChiHuResult[i].wChiHuRight |= m_byUserHuCardType[i];
//				}
//			}
//
//			//结束游戏
//			if (bOtherChiHu==false)
//			{
//				//荒庄结束
//				m_cbChiHuCard=0;
//				m_cbLeftCardCount=0;
//				m_wHaiDiUser=wChairID;
//				m_wProvideUser=INVALID_CHAIR;
//				m_byGameEndType = 5; //海底荒庄
//				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//			}
//			else
//			{
//				//放炮结束
//				m_cbLeftCardCount=0;
//				m_wHaiDiUser=wChairID;
//				m_wProvideUser=wChairID;
//				m_cbChiHuCard=m_cbProvideCard;
//				m_byGameEndType = 4; //海底放炮
//				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//			}
//
//		}
//		else
//		{//玩家胡了
//
//			m_ChiHuResult[wChairID].wChiHuRight |= CHR_HAI_DI;
//			m_ChiHuResult[wChairID].wChiHuRight |= m_byUserHuCardType[wChairID];
//
//			//插入扑克
//			m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
//
//			//正常结束
//			m_cbLeftCardCount=0;
//			m_wHaiDiUser=wChairID;
//			m_wProvideUser=wChairID;
//			m_cbChiHuCard=m_cbProvideCard;
//			m_byGameEndType = 3; //海底自摸
//			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//		}
//	}
//	else
//	{
//		//设置用户
//		m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;
//
//		//荒庄判断
//		if (m_wCurrentUser==m_wResumeUser)
//		{
//			m_cbChiHuCard=0;
//			m_wCurrentUser=INVALID_CHAIR;
//			m_wProvideUser=INVALID_CHAIR;
//			m_byGameEndType = 0; //海底荒庄
//			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
//
//			return true;
//		}
//
//		//构造数据
//		CMD_S_OperateHaiDi OperateHaiDi;
//		OperateHaiDi.wCurrentUser=m_wCurrentUser;
//
//		//发送消息
//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//
//		return true;
//	}
//
//
//	return true;
//}

//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送可以操作的提示（碰，补，胡）
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			memset(&OperateNotify, 0, sizeof(CMD_S_OperateNotify));
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//if (m_bHearStatus[i])
			//{//听牌了
			//	if ((OperateNotify.cbActionMask&WIK_CHI_HU)>0)
			//	{
			//		m_byGangCardNums[i] = 0;
			//		m_byBuCardNums[i] = 0;
			//		memset(m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
			//		memset(m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
			//		m_cbUserAction[i] = OperateNotify.cbActionMask = WIK_CHI_HU;
			//	}
			//}

			if (m_byGangCardNums[i]>0 && m_byGangCardNums[i]<=4)
			{//杠、听
				memcpy(OperateNotify.byGangCard, m_byGangCard[i], sizeof(BYTE)*m_byGangCardNums[i]);
				OperateNotify.byGangCardNums = m_byGangCardNums[i];
			}
			if (m_byBuCardNums[i]>=1)
			{//补
				memcpy(OperateNotify.byBuCard, m_byBuCard[i], sizeof(BYTE)*m_byBuCardNums[i]);
				OperateNotify.byBuCardNums = m_byBuCardNums[i];
			}

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser, bool bSupplyCard)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR) 
	{
		return false;
	}

	//丢弃扑克
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	CString strBuffer;
	strBuffer.Format(_T("Server: CTableFrameSink::DispatchCardData 剩余牌数目:%d"),m_cbLeftCardCount);
	OutputDebugString(strBuffer);

//	//海底判断
//	if (m_cbLeftCardCount==1)
//	{
//		//设置变量
//		m_wResumeUser=wCurrentUser;
//		m_wCurrentUser=wCurrentUser;
//		m_wProvideUser=INVALID_CHAIR;
//
//#ifdef TOOL_SEND_CARD
//		m_cbProvideCard = m_SendCardTool.GetNextCard();
//#else
//		m_cbProvideCard=m_cbRepertoryCard[0];
//#endif
//
//		//设置状态
//		m_pITableFrame->SetGameStatus(GS_MJ_HAI_DI);
//
//		//构造数据
//		CMD_S_OperateHaiDi OperateHaiDi;
//		OperateHaiDi.wCurrentUser=wCurrentUser;
//
//		//发送消息
//		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_HAI_DI,&OperateHaiDi,sizeof(OperateHaiDi));
//
//		return true;
//	}

	//初始化
	for(int i=0; i<m_wPlayerCount; i++)
	{
		m_cbUserAction[i] = WIK_NULL;
		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;
	}

	//发牌处理
	m_cbUserAction[wCurrentUser] = WIK_NULL;
	tagGangCardResult GangCardResult;
	BYTE byGangCard[MAX_INDEX]={0};
	BYTE byGangCardNums = 0;
	memset(byGangCard, 0, sizeof(byGangCard));
	memset(&GangCardResult, 0, sizeof(tagGangCardResult));
	if (m_bSendStatus==true)
	{
		//发送扑克
		BYTE byNextCard=0;
#ifdef TOOL_SEND_CARD
		byNextCard = m_SendCardTool.GetNextCard();
#endif
		--m_cbLeftCardCount;
		if (0==byNextCard || 255==byNextCard)
		{
			byNextCard = m_cbRepertoryCard[m_cbLeftCardCount];
		}
		m_cbSendCardCount++;
		m_cbSendCardData=byNextCard;
		m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

		//设置变量
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;

		//if (m_tagUserGSKUResult[wCurrentUser].bIsGang)
		//{//用于杠上炮，保存出牌数、发牌数
		//	m_iSaveGameOutCardCount = m_cbOutCardCount;
		//	m_iSaveGameSendCardCount = m_cbSendCardCount;
		//}

		//杠牌判断
		if (/*!m_bHearStatus[wCurrentUser] && */m_cbLeftCardCount>1)
		{
			//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],
			//	m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);

			m_GameLogic.AnalyseGangCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],GangCardResult);
			//if (GangCardResult.cbCardCount>=1)
			//{
			//	m_GameLogic.EstimateTingCard(m_cbCardIndex[wCurrentUser], m_WeaveItemArray[wCurrentUser], m_cbWeaveItemCount[wCurrentUser], byGangCard, byGangCardNums, m_cbKingCard);
			//}

			//if (byGangCardNums>0)
			//{
			//	m_cbUserAction[wCurrentUser] |= WIK_TING;
			//}
			if (GangCardResult.cbCardCount>=1)
			{
				m_cbUserAction[wCurrentUser] |= WIK_FILL|WIK_GANG;
			}
		}

		//牌型权位
		WORD wChiHuRight=0;
		//if (m_bGangStatus) 
		//{
		//	wChiHuRight|=CHR_QIANG_GANG;
		//}

		//胡牌判断
		//BYTE cbReCardIndex[MAX_INDEX];;
		tagChiHuResult ChiHuResult;
		m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],
			0,wChiHuRight,ChiHuResult);
		//m_cbUserAction[wCurrentUser]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[wCurrentUser],m_WeaveItemArray[wCurrentUser],m_cbWeaveItemCount[wCurrentUser],
		//	                         0,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex);
		//if (m_bGangStatus && (WIK_CHI_HU&m_cbUserAction[wCurrentUser]))
		//{
		//	m_tagUserGSKUResult[wCurrentUser].bIsGang = true;
		//}

		// 已经是最后一张牌，且抓牌的这个玩家并不能因为这张牌改变战局，游戏只能留局咯！
		if (0>=m_cbLeftCardCount && !(WIK_CHI_HU&m_cbUserAction[wCurrentUser]))
		{
			m_byGameEndType = 0; //留局
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);
			return true;
		}
	}

	//设置变量
	m_cbOutCardData=0;
	m_wCurrentUser=wCurrentUser;
	m_wOutCardUser=INVALID_CHAIR;

	//构造数据
	CMD_S_SendCard SendCard;
	memset(&SendCard, 0, sizeof(CMD_S_SendCard));
	SendCard.wCurrentUser=wCurrentUser;
	SendCard.cbActionMask=m_cbUserAction[wCurrentUser];
	SendCard.cbCardData=(m_bSendStatus==true)?m_cbSendCardData:0x00;
	SendCard.bSupplyCard = bSupplyCard;

	if (byGangCardNums>0 && byGangCardNums<=4)
	{
		memcpy(SendCard.byGangCard, byGangCard, sizeof(BYTE)*byGangCardNums);
		SendCard.byGangCardNums = byGangCardNums;

		memcpy(m_byGangCard[wCurrentUser], byGangCard, sizeof(BYTE)*byGangCardNums);
		m_byGangCardNums[wCurrentUser] = byGangCardNums;
	}
	if (GangCardResult.cbCardCount>=1)
	{
		memcpy(SendCard.byBuCard, GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
		SendCard.byBuCardNums = GangCardResult.cbCardCount;

		memcpy(m_byBuCard[wCurrentUser], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
		m_byBuCardNums[wCurrentUser] = GangCardResult.cbCardCount;
	}

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	 //打一张牌，判断其他玩家是不是可以操作（碰，补，胡等）
	//变量定义
	bool bAroseAction=false;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//动作判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//初始化
		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
		m_byGangCardNums[i] = 0;
		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
		m_byBuCardNums[i] = 0;

		//用户过滤
		if (wCenterUser==i) continue;

		//出牌类型
		if (/*!m_bHearStatus[i] && */EstimatKind==EstimatKind_OutCard )
		{
			//吃碰判断
			//if (m_bEnjoinChiPeng[i]==false)
			{
				//碰牌判断
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//吃牌判断
				//WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				//if (wEatUser==i) m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);
			}

			//杠牌判断
			if (m_cbLeftCardCount>1) 
			{		
				BYTE byCardType = WIK_NULL;
				byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				if ((byCardType&(WIK_GANG|WIK_FILL))>0)
				{
					m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
					m_byBuCard[i][0] = cbCenterCard;
					m_byBuCardNums[i] = 1;
					//bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, cbCenterCard);
					//if (bIsTing)
					//{
					//	m_byGangCard[i][0] = cbCenterCard;
					//	m_byGangCardNums[i] = 1;
					//	m_cbUserAction[i] |= WIK_TING;
					//}
				}

				//m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
			}
		}

		//胡牌判断
		//if (m_bEnjoinChiHu[i]==false)
		{
			//牌型权位
			WORD wChiHuRight=0;
			//if (m_bGangStatus==true) wChiHuRight|=CHR_QIANG_GANG;
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==1)) wChiHuRight|=CHR_DI;
			//if ((m_cbSendCardCount==1)&&(m_cbOutCardCount==0)) wChiHuRight|=CHR_TIAN;

			//吃胡判断
			tagChiHuResult ChiHuResult;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			m_cbUserAction[i] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);

			//吃胡限制
			//if ((m_cbUserAction[i]&WIK_CHI_HU)!=0) m_bEnjoinChiHu[i]=true;
		}

		//结果判断
		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
	}

	//结果处理
	if (bAroseAction==true) 
	{
		//设置变量
		m_wProvideUser=wCenterUser;
		m_cbProvideCard=cbCenterCard;
		m_wResumeUser=m_wCurrentUser;
		m_wCurrentUser=INVALID_CHAIR;

		//发送提示
		SendOperateNotify();

		return true;
	}

	return false;
}

//响应判断
//bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
//{
//	//变量定义
//	bool bAroseAction=false;
//	
//
//	//用户状态
//	ZeroMemory(m_bResponse,sizeof(m_bResponse));
//	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
//	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));
//
//
//
//	//动作判断
//	for (WORD i=0;i<m_wPlayerCount;i++)
//	{
//
//		//初始化
//		::memset(&m_byGangCard[i], 0, sizeof(m_byGangCard[i]));
//		m_byGangCardNums[i] = 0;
//		::memset(&m_byBuCard[i], 0, sizeof(m_byBuCard[i]));
//		m_byBuCardNums[i] = 0;
//
//		//用户过滤
//		if (wCenterUser==i) continue;
//
//		//组合牌型的花色和数目
//		BYTE cbWeaveCount=m_cbWeaveItemCount[i];
//		//for (WORD j=0;j<cbWeaveCount;j++)
//		//{
//		//	BYTE cbCardColor[i][j]=m_WeaveItemArray[i][j].cbCenterCard&MASK_COLOR;
//		//}
//
//		//tagGangCardResult GangCardResult;
//		//BYTE bySaveUserShouPai[MAX_INDEX] = {0};
//		//BYTE byGangCard[MAX_INDEX]={0};
//		//BYTE byGangCardNums = 0;
//		////杠牌判断
//		//memset(&GangCardResult, 0, sizeof(tagGangCardResult));
//		//memcpy(bySaveUserShouPai, m_cbCardIndex[i], sizeof(BYTE)*MAX_INDEX);
//		//BYTE byCenterCardIndex = m_GameLogic.SwitchToCardIndex(cbCenterCard);
//		//if (byCenterCardIndex<MAX_INDEX)
//		//{
//		//	++bySaveUserShouPai[byCenterCardIndex];
//		//}
//		//m_GameLogic.AnalyseGangCard(bySaveUserShouPai,NULL,0,GangCardResult);
//		//if (GangCardResult.cbCardCount>=1)
//		//{
//		//	m_GameLogic.EstimateTingCard(bySaveUserShouPai, byGangCard, byGangCardNums, m_cbKingCard);
//		//}
//        		
//		//出牌类型
//		if (!m_bHearStatus[i] && EstimatKind==EstimatKind_OutCard )
//		{
//			if (cbWeaveCount == 0)
//			{//没有下槛
//				//吃碰判断
//				if (m_bEnjoinChiPeng[i]==false)
//				{//能吃能碰
//					//碰牌判断
//					m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//					//吃牌判断
//					WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//					if (wEatUser==i)
//					{//我是下家能吃
//						m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//					}
//				}
//				//杠牌判断
//				if (m_cbLeftCardCount>1) 
//				{
//					BYTE byCardType = WIK_NULL;
//					byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//					if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//					{
//						m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//						m_byBuCard[i][0] = cbCenterCard;
//						m_byBuCardNums[i] = 1;
//						bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], NULL, 0, cbCenterCard, m_cbKingCard);
//						if (bIsTing)
//						{
//							m_byGangCard[i][0] = cbCenterCard;
//							m_byGangCardNums[i] = 1;
//							m_cbUserAction[i] |= WIK_TING;
//						}
//					}
//				}
//			}
//			else if (cbWeaveCount == 1)
//			{//如果下槛了并且有一组牌.
//				BYTE cbCardColor=m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR;
//				if (((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) ))
//				{//吃牌情况
//					//吃碰判断
//					if (m_bEnjoinChiPeng[i]==false && (cbCenterCard&MASK_COLOR) == cbCardColor)
//					{
//						//碰牌判断
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						//吃牌判断
//						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//						if (wEatUser==i)
//						{
//							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//					}
//					//杠牌判断
//					if (m_cbLeftCardCount>1 && (cbCenterCard&MASK_COLOR) == cbCardColor) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//				}
//				else if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//				{//碰牌情况
//					//吃碰判断
//					if (m_bEnjoinChiPeng[i]==false)
//					{
//						//碰牌判断
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((cbCenterCard&MASK_COLOR) == cbCardColor)
//						{
//							//吃牌判断
//							WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//							if (wEatUser==i)
//							{
//								m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							}
//						}
//
//					}
//					//杠牌判断
//					if (m_cbLeftCardCount>1) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//				}
//
//			}
//			else if (cbWeaveCount == 2)
//			{//下槛二组
//				BYTE cbCardColor=m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR;
//                if(((m_WeaveItemArray[i][1].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][1].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][1].cbWeaveKind == WIK_RIGHT) ))
//                {//吃的情况下
//					//吃碰判断
//					if (m_bEnjoinChiPeng[i]==false && ((cbCenterCard&MASK_COLOR) == cbCardColor))
//					{
//						//碰牌判断
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						//吃牌判断
//						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//						if (wEatUser==i)
//						{
//							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//					}
//
//					//杠牌判断
//					if (m_cbLeftCardCount>1 && ((cbCenterCard&MASK_COLOR) == cbCardColor)) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//                }
//				else if (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//				{
//					//吃碰判断
//					if (m_bEnjoinChiPeng[i]==false)
//					{
//						if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							//碰牌判断
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//						else if(((cbCenterCard&MASK_COLOR) == cbCardColor) && ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) )) 
//						{
//							//碰牌判断
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//
//						if ((m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR) == (m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR))
//						{
//							//吃牌判断
//							if ((cbCenterCard&MASK_COLOR) == cbCardColor)
//							{
//								WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//								if (wEatUser==i)
//								{
//									m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//								}
//							}
//						}
//					}
//					//杠牌判断
//					if (m_cbLeftCardCount>1) 
//					{
//						if (m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//						else if(((cbCenterCard&MASK_COLOR) == cbCardColor) && ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][0].cbWeaveKind == WIK_RIGHT) )) 
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//					}
//				}
//			}
//
//			else if (cbWeaveCount == 3)
//			{//三组
//				BYTE cbCardColor=m_WeaveItemArray[i][2].cbCenterCard&MASK_COLOR;
//				if(((m_WeaveItemArray[i][2].cbWeaveKind == WIK_LEFT) || (m_WeaveItemArray[i][2].cbWeaveKind == WIK_CENTER ) || (m_WeaveItemArray[i][2].cbWeaveKind == WIK_RIGHT) ))
//				{
//					//吃碰判断
//					if (m_bEnjoinChiPeng[i]==false && ((cbCenterCard&MASK_COLOR) == cbCardColor))
//					{
//						//碰牌判断
//						m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						//吃牌判断
//						WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//						if (wEatUser==i)
//						{
//							m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//					}
//
//					//杠牌判断
//					if (m_cbLeftCardCount>1 && ((cbCenterCard&MASK_COLOR) == cbCardColor)) 
//					{
//						BYTE byCardType = WIK_NULL;
//						byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//						{
//							m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//							m_byBuCard[i][0] = cbCenterCard;
//							m_byBuCardNums[i] = 1;
//							bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//							if (bIsTing)
//							{
//								m_byGangCard[i][0] = cbCenterCard;
//								m_byGangCardNums[i] = 1;
//								m_cbUserAction[i] |= WIK_TING;
//							}
//						}
//					}
//				}
//				else if (m_WeaveItemArray[i][2].cbWeaveKind == WIK_PENG || ((m_WeaveItemArray[i][2].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//				{
//					//吃碰判断
//					if (m_bEnjoinChiPeng[i]==false)
//					{
//						bool bIsTongSe = false;
//						if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							//碰牌判断
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							//碰牌判断
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							//碰牌判断
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							//碰牌判断
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							bIsTongSe = true;
//						}
//						else if((cbCenterCard&MASK_COLOR) == cbCardColor)
//						{
//							//碰牌判断
//							m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//						}
//
//						if (bIsTongSe)
//						{
//							//吃牌判断
//							if ((((m_WeaveItemArray[i][0].cbCenterCard&MASK_COLOR)==(m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR) ) && 
//								((m_WeaveItemArray[i][1].cbCenterCard&MASK_COLOR)==(m_WeaveItemArray[i][2].cbCenterCard&MASK_COLOR) )) && 
//								(cbCenterCard&MASK_COLOR) == cbCardColor)
//							{//同花色
//								WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//								if (wEatUser==i)
//								{
//									m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//								}
//							}
//						}
//					}
//					//杠牌判断
//					if (m_cbLeftCardCount>1) 
//					{
//						bool bIsTongSe = false;
//						if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							bIsTongSe = true;
//						}
//						else if ((m_WeaveItemArray[i][0].cbWeaveKind == WIK_PENG) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && (m_WeaveItemArray[i][1].cbWeaveKind == WIK_PENG))
//						{
//							bIsTongSe = true;
//						}
//						else if (((m_WeaveItemArray[i][0].cbWeaveKind&(WIK_GANG|WIK_FILL))>0) && ((m_WeaveItemArray[i][1].cbWeaveKind&(WIK_GANG|WIK_FILL))>0))
//						{
//							bIsTongSe = true;
//						}
//
//						if (bIsTongSe)
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//						else if((cbCenterCard&MASK_COLOR) == cbCardColor)
//						{
//							BYTE byCardType = WIK_NULL;
//							byCardType = m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard,m_cbKingCard);
//							if ((byCardType&(WIK_GANG|WIK_FILL))>0)
//							{
//								m_cbUserAction[i] |= WIK_FILL|WIK_GANG;
//								m_byBuCard[i][0] = cbCenterCard;
//								m_byBuCardNums[i] = 1;
//								bool bIsTing = m_GameLogic.EstimateTingCard(m_cbCardIndex[i], m_WeaveItemArray[i], m_cbWeaveItemCount[i], cbCenterCard, m_cbKingCard);
//								if (bIsTing)
//								{
//									m_byGangCard[i][0] = cbCenterCard;
//									m_byGangCardNums[i] = 1;
//									m_cbUserAction[i] |= WIK_TING;
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//	
//        //胡牌判断
//		BYTE bKingCount=0;
//		bKingCount=m_GameLogic.GetKingCardCount(m_cbCardIndex[i],NULL,0,m_cbKingCard,4);
//		if (0==bKingCount)
//		{//放炮了，手中没有王牌才能判断胡牌.
//			//牌型权位
//			WORD wChiHuRight=0;
//			//吃胡判断
//			BYTE cbReCardIndex[MAX_INDEX]={0};
//			tagChiHuResult ChiHuResult;
//			//BYTE cbWeaveCount=m_cbWeaveItemCount[i];
//			//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex);
//			//有王牌只能自摸，只有手里无王牌才能接炮
//			//统计王牌数目
//			m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);
//			//m_cbUserAction[i]|=m_GameLogic.AnalyseChiHuLevity(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult,m_cbKingCard,4,cbReCardIndex, true);
//		}
//
//		//结果判断
//		if (m_cbUserAction[i]!=WIK_NULL) bAroseAction=true;
//	}
//
//	////检测下家手中的杠牌
//	//WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
//	//for (int k=0; k<m_wPlayerCount; k++)
//	//{
//	//	if (wEatUser==k && m_cbUserAction[k]!=WIK_NULL)
//	//	{
//	//		//动作分析
//	//		tagGangCardResult GangCardResult;
//	//		BYTE byGangCard[MAX_INDEX]={0};
//	//		BYTE byGangCardNums = 0;
//	//		//杠牌判断
//	//		memset(&GangCardResult, 0, sizeof(tagGangCardResult));
//	//		m_GameLogic.AnalyseGangCard(m_cbCardIndex[k],m_WeaveItemArray[k], m_cbWeaveItemCount[k],GangCardResult, m_cbKingCard);
//	//		if (GangCardResult.cbCardCount>=1)
//	//		{
//	//			m_GameLogic.EstimateTingCard(m_cbCardIndex[k], m_WeaveItemArray[k], m_cbWeaveItemCount[k], byGangCard, byGangCardNums, m_cbKingCard);
//	//		}
//	//		if (byGangCardNums>0)
//	//		{//添加杠
//	//			m_cbUserAction[k] |= WIK_TING;
//	//			memcpy(&m_byGangCard[k][m_byGangCardNums[k]], byGangCard, sizeof(BYTE)*byGangCardNums);
//	//			m_byGangCardNums[k] += byGangCardNums;
//	//		}
//	//		if (GangCardResult.cbCardCount>=1)
//	//		{//添加补
//	//			m_cbUserAction[k] |= WIK_FILL|WIK_GANG;
//	//			memcpy(&m_byBuCard[k][m_byBuCardNums[k]], GangCardResult.cbCardData, sizeof(BYTE)*GangCardResult.cbCardCount);
//	//			m_byBuCardNums[k] += GangCardResult.cbCardCount;
//	//		}
//
//	//		//结果判断
//	//		if (m_cbUserAction[k]!=WIK_NULL) bAroseAction=true;
//	//		break;
//	//	}
//	//}
//
//	//结果处理
//	if (bAroseAction==true) 
//	{
//		//设置变量
//		m_wProvideUser=wCenterUser;
//		m_cbProvideCard=cbCenterCard;
//		m_wResumeUser=m_wCurrentUser;
//		m_wCurrentUser=INVALID_CHAIR;
//
//		//发送提示
//		SendOperateNotify();
//
//		return true;
//	}
//
//	return false;
//}


//cp add 4.11
//==============================================================================
//任务ID
#define					Sparrow_TASK_ID_1			31001
#define					Sparrow_TASK_ID_2			31002
#define					Sparrow_TASK_ID_3			31003
#define					Sparrow_TASK_ID_4			31004
#define					Sparrow_TASK_ID_5			31005
#define					Sparrow_TASK_ID_6			31006
#define					Sparrow_TASK_ID_7			31007
#define					Sparrow_TASK_ID_8			31008
#define					Sparrow_TASK_ID_9			31009
#define					Sparrow_TASK_ID_10			31010
#define					Sparrow_TASK_ID_11			31011

//==============================================================================




//
////玩家任务判断
//void CTableFrameSink::OnUserTaskJudgement(WORD wChairID, BYTE btTag)
//{
//	//wChairID:玩家ID;  btTag:1:赢,0:输.
//	if (wChairID>=GAME_PLAYER || btTag>1)
//	{
//		return;
//	}
//	//首先获取wChairID玩家任务序号.
//	IServerUserItem *pUserItem = m_pITableFrame->GetServerUserItem( wChairID );
//	if (NULL == pUserItem)
//	{
//		return;
//	}
//	tagServerUserData *pUserData = pUserItem->GetUserData();
//	if (NULL == pUserData)
//	{
//		return;
//	}
//	//pUserData->task.dwID:任务ID
//	if (1==btTag)
//	{//wChairID赢了
//		bool bIsWin = false;
//		switch(pUserData->task.dwID)
//		{
//		case Sparrow_TASK_ID_1:	//任务1: 小胡
//			{
//				if ( (CHK_JI_HU == m_ChiHuResult[wChairID].wChiHuKind) || (CHK_PING_HU == m_ChiHuResult[wChairID].wChiHuKind) )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_2:	//任务2: 四喜
//			{   
//				if (( CHK_SIXI_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsSiXi(m_cbCardIndex[wChairID]) )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_3:	//任务3: 板板胡
//			{   
//                if (( CHK_BANBAN_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsBanBanHu(m_cbCardIndex[wChairID]) )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_4:	//任务4: 缺一色
//			{   
//				if (( CHK_QUEYISE_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsQueYiSe(m_cbCardIndex[wChairID]) )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_5:	//任务5: 六六顺
//			{   
//				if (( CHK_LIULIU_HU & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( true == m_GameLogic.IsLiuLiuShun(m_cbCardIndex[wChairID]) )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_6:	//任务6: 碰碰胡
//			{   
//				if (( CHK_PENG_PENG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( CHK_PENG_PENG == m_ChiHuResult[wChairID].wChiHuKind )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_7:	//任务7: 将将胡
//			{  		
//				if (( CHK_JIANG_JIANG & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_8:	//任务8: 七小对
//			{   
//				if (( CHK_QI_XIAO_DUI & m_ChiHuResult[wChairID].wChiHuKind ) > 0)
//				//if ( CHK_QI_XIAO_DUI == m_ChiHuResult[wChairID].wChiHuKind )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_9:	//任务9: 杠上开花
//			{   
//				if ( true == m_tagUserGSKUResult[wChairID].bIsGang && true == m_tagUserGSKUResult[wChairID].bIsGSKU)
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_10:	//任务10: 地胡
//			{   
//				if (( CHR_DI & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
//				//if ( CHR_DI == m_ChiHuResult[wChairID].wChiHuRight )
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		case Sparrow_TASK_ID_11:	//任务11: 天胡
//			{   
//				if (( CHR_TIAN & m_ChiHuResult[wChairID].wChiHuRight ) > 0)
//				//if ( CHR_TIAN == m_ChiHuResult[wChairID].wChiHuRight)
//				{//任务完成.
//					bIsWin = true;
//				}
//			}
//			break;
//		default:
//			break;
//		}
//		if (true == bIsWin)
//		{//任务完成.
//			//提交任务完成
//			m_pITableFrame->PefermTaskFinish(wChairID);
//		}
//	}
//}
//////////////////////////////////////////////////////////////////////////
////////////////
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	return 0;
}
SCORE CTableFrameSink::QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	return 0;
}
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	return false;
}
bool CTableFrameSink::OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	return false;
}
void CTableFrameSink::SetGameBaseScore(LONG lBaseScore)
{
	return;
}
bool CTableFrameSink::OnActionUserOnReady(WORD,IServerUserItem *,void *,WORD)
{
	return false;
}
//////////////////////////////////////////////////////////////////////////




