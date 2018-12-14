#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数

//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏变量
	m_lSiceCount=0;
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	m_cbQuanFeng = 0;
	m_cbQuanCounter = 1;
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	m_wReplaceUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//堆立信息
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 0;

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

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
	ZeroMemory( m_dwChiHuKind,sizeof(m_dwChiHuKind) );

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

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

//初始化
bool  CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(pIUnknownEx!=NULL);
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);
	if (m_pITableFrame==NULL)
		return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

		//开始模式//////////////////
	m_pITableFrame->SetStartMode(START_MODE_FULL_READY);
	return true;
}

//复位桌子
void  CTableFrameSink::RepositionSink()
{
	//游戏变量
	m_lSiceCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	ZeroMemory(m_bTrustee,sizeof(m_bTrustee));
	ZeroMemory(m_cbListenStatus,sizeof(m_cbListenStatus));
	ZeroMemory( m_cbHuaCardCount,sizeof(m_cbHuaCardCount) );
	m_wReplaceUser = INVALID_CHAIR;
	ZeroMemory( m_lGameScore,sizeof(m_lGameScore) );

	//出牌信息
	m_cbOutCardData=0;
	m_cbOutCardCount=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//堆立信息
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory(m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发牌信息
	m_cbSendCardData=0;
	m_cbSendCardCount=0;
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbRepertoryCard,sizeof(m_cbRepertoryCard));
	m_cbEndLeftCount = 0;

	//运行变量
	m_cbProvideCard=0;
	m_wResumeUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wProvideUser=INVALID_CHAIR;

	//状态变量
	m_bSendStatus=false;
	m_bGangStatus = false;
	ZeroMemory(m_bEnjoinChiHu,sizeof(m_bEnjoinChiHu));
	ZeroMemory(m_bEnjoinChiPeng,sizeof(m_bEnjoinChiPeng));

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
	ZeroMemory(m_dwChiHuKind,sizeof(m_dwChiHuKind));

	return;
}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	return true;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{
	//设置状态
	m_pITableFrame->SetGameStatus(GS_MJ_PLAY);

	//混乱扑克
	if( m_wBankerUser == INVALID_CHAIR )
		m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),MAKEWORD(rand()%6+1,rand()%6+1));
	else
		m_lSiceCount = MAKELONG(MAKEWORD(rand()%6+1,rand()%6+1),0);

	m_cbLeftCardCount=CountArray(m_cbRepertoryCard);
	m_GameLogic.RandCardData(m_cbRepertoryCard,CountArray(m_cbRepertoryCard));

	//test
	//BYTE byTest[] = {
	//	0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f,
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						
	//	0x21,0x22,0x23,0x24,0x25,0x26,0x22,0x27,
	//	
	//	0x29,0x31,0x31,0x11,

	//	0x21,0x12,0x13,0x14,0x15,0x16,0x21,0x22,0x23,
	//	0x24,0x25,0x26,0x31,

	//	0x28,0x29,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
	//	0x28,0x12,0x31,0x33,

	//	0x33,0x12,0x17,0x36,0x27,0x32,0x32,0x33,0x34,
	//	0x35,0x36,0x31,0x32,

	//	0x27,0x32,0x32,0x32,0x31,0x31,0x31,0x34,0x34,
	//	0x34,0x34,0x36,0x31,
	//};
	//CopyMemory( m_cbRepertoryCard,byTest,sizeof(byTest) );
	//m_wBankerUser = 0;
	//end test

	//分发扑克
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		m_cbLeftCardCount-=(MAX_COUNT-1);
		m_GameLogic.SwitchToCardIndex(&m_cbRepertoryCard[m_cbLeftCardCount],MAX_COUNT-1,m_cbCardIndex[i]);
	}

	if( m_wBankerUser == INVALID_CHAIR )
	{
		WORD wSice = (WORD)(m_lSiceCount>>16);
		m_wBankerUser = (HIBYTE(wSice)+LOBYTE(wSice))%GAME_PLAYER;
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//设置变量
	m_cbProvideCard=m_cbSendCardData;
	m_wProvideUser=m_wBankerUser;
	m_wCurrentUser=m_wBankerUser;

	//堆立信息
	WORD wSice = WORD(m_lSiceCount&0xffff);
	BYTE cbSiceTakeCount= HIBYTE(wSice)+LOBYTE(wSice);
	WORD wTakeChairID=(m_wBankerUser+cbSiceTakeCount-1)%GAME_PLAYER;
	while( cbSiceTakeCount*2 > HEAP_FULL_COUNT )
	{
		wTakeChairID = (wTakeChairID+1)%GAME_PLAYER;
		cbSiceTakeCount -= HEAP_FULL_COUNT/2;
	}
	m_wHeapTail = wTakeChairID;
	BYTE cbTakeCount=MAX_REPERTORY-m_cbLeftCardCount;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//计算数目
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceTakeCount)*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//提取扑克
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//完成判断
		if (cbTakeCount==0)
		{
			m_wHeapHead=wTakeChairID;
			break;
		}
		//切换索引
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//动作分析
	bool bAroseAction=false;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户索引
		WORD wUserIndex=(m_wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

		for( BYTE j = MAX_INDEX-MAX_HUA_CARD; j < MAX_INDEX; j++ )
		{
			if( m_cbCardIndex[wUserIndex][j] > 0 )
			{
				m_wReplaceUser = wUserIndex;
				break;
			}
		}

		if( m_wReplaceUser != INVALID_CHAIR ) break;
	}

	//庄家判断
	if ( m_wReplaceUser==INVALID_CHAIR )
	{
		//杠牌判断
		tagGangCardResult GangCardResult;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wBankerUser],NULL,0,GangCardResult);

		//胡牌判断
		CChiHuRight chr;
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]--;
		m_cbUserAction[m_wBankerUser]|=m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wBankerUser],NULL,0,m_cbSendCardData,chr);
		m_cbCardIndex[m_wBankerUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;
	}

	//构造数据
	CMD_S_GameStart GameStart;
	GameStart.lSiceCount=m_lSiceCount;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wReplaceUser = m_wReplaceUser;
	GameStart.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	GameStart.cbQuanFeng = m_cbQuanFeng;
	GameStart.wHeapHead = m_wHeapHead;
	GameStart.wHeapTail = m_wHeapTail;
	CopyMemory(GameStart.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//设置变量
		GameStart.cbUserAction=m_cbUserAction[i];

		ZeroMemory(GameStart.cbCardData,sizeof(GameStart.cbCardData));
		m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameStart.cbCardData);

		if( m_pITableFrame->GetTableUserItem(i)->IsAndroidUser() )
		{
			BYTE bIndex = 1;
			for( WORD j=0; j<GAME_PLAYER; j++ )
			{
				if( j == i ) continue;
				m_GameLogic.SwitchToCardData(m_cbCardIndex[j],&GameStart.cbCardData[MAX_COUNT*bIndex++]);
			}
		}

		//发送数据
		m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}

	return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//结束信息
			WORD wWinner = INVALID_CHAIR;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.dwChiHuKind[i]=m_dwChiHuKind[i];
				if( m_dwChiHuKind[i] == WIK_CHI_HU )
					wWinner = i;
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//如果不是流局
			if( wWinner != INVALID_CHAIR )
			{
				//权位过滤
				FiltrateRight( wWinner,m_ChiHuRight[wWinner] );
			}

			//统计积分
			if (m_wProvideUser!=INVALID_CHAIR)
			{
				//结束信息
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;

				m_ChiHuRight[wWinner].GetRightData( GameEnd.dwChiHuRight,MAX_RIGHT_COUNT );

				GameEnd.cbFanCount = CalScore( wWinner,GameEnd.lGameScore );
				GameEnd.cbHuaCardCount = m_cbHuaCardCount[wWinner];
			}
			else
			{
				//流局结束
				GameEnd.wProvideUser=m_wProvideUser;
				GameEnd.cbProvideCard=m_cbProvideCard;
			}
			
			//统计积分
			LONG lGameTax[GAME_PLAYER];
			ZeroMemory(lGameTax,sizeof(lGameTax));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//累加积分
				GameEnd.lGameScore[i]*=m_pGameServiceOption->lCellScore;
				//设置积分
				if( GAME_GENRE_SCORE != m_pGameServiceOption->wServerType )
				{
					if (GameEnd.lGameScore[i]>0L)
					{
						lGameTax[i] = (LONG)(GameEnd.lGameScore[i]*m_pGameServiceOption->cbRevenueRatio/1000L);
						GameEnd.lGameScore[i] -= lGameTax[i];
					}
				}
			}

			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;
				//写入积分
				////m_pITableFrame->WriteUserScore(i,GameEnd.lGameScore[i],lGameTax[i],ScoreKind);

				//发送信息
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

////////////////////////			
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			//统计积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				BYTE ScoreKind;
				if( GameEnd.lGameScore[i] > 0L ) ScoreKind = SCORE_TYPE_WIN;
				else if( GameEnd.lGameScore[i] < 0L ) ScoreKind = SCORE_TYPE_LOSE;
				else ScoreKind = SCORE_TYPE_DRAW;

				ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];
				ScoreInfoArray[i].lRevenue = lGameTax[i];
				ScoreInfoArray[i].cbType   = ScoreKind;
			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
///////////////////////

			//设置变量
			m_wBankerUser = (m_wBankerUser+GAME_PLAYER-1)%GAME_PLAYER;
			if( ++m_cbQuanCounter == 5 )
			{
				m_cbQuanCounter = 1;
				if( ++m_cbQuanFeng == 4 )
				{
					m_cbQuanFeng = 0;
				}
			}

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			return true;
		}
	case GER_DISMISS:		//游戏解散
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:	//网络中断
		{
			//变量定义
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));

			//设置变量
			GameEnd.wProvideUser=INVALID_CHAIR;

			//拷贝扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				GameEnd.cbCardCount[i]=m_GameLogic.SwitchToCardData(m_cbCardIndex[i],GameEnd.cbCardData[i]);
			}

			//积分
			GameEnd.lGameScore[wChairID] = -20L*m_pGameServiceOption->lCellScore;
			
			//通知消息
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("由于 [ %s ] 离开游戏，游戏结束"),pIServerUserItem->GetNickName());
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->GetTableUserItem(i);
				if (pISendUserItem!=NULL) 
					m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
			}

			WORD wIndex=0;
			do
			{
				IServerUserItem * pISendUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
				if (pISendUserItem==NULL)
					break;
				m_pITableFrame->SendGameMessage(pISendUserItem,szMessage,SMT_PROMPT);
			} while (true);

///////////////////////
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));

			//写入积分
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				ScoreInfoArray[i].lScore   = GameEnd.lGameScore[i];
				ScoreInfoArray[i].lRevenue = (i!=wChairID)?GameEnd.lGameTax:0;
				ScoreInfoArray[i].cbType   = (i==wChairID)?SCORE_TYPE_FLEE:SCORE_TYPE_WIN;
			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
//////////////////////

			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//写入积分
			////m_pITableFrame->WriteUserScore(wChairID,GameEnd.lGameScore[wChairID],0,SCORE_TYPE_FLEE);

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE,NULL,608);

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);
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
		{
			//变量定义
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			//游戏变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			StatusPlay.wReplaceUser = m_wReplaceUser;
			CopyMemory(StatusPlay.bTrustee,m_bTrustee,sizeof(m_bTrustee));
			StatusPlay.cbQuanFeng = m_cbQuanFeng;

			//状态变量
			StatusPlay.cbActionCard=m_cbProvideCard;
			StatusPlay.cbLeftCardCount=m_cbLeftCardCount;
			StatusPlay.cbActionMask=(m_bResponse[wChiarID]==false)?m_cbUserAction[wChiarID]:WIK_NULL;

			//听牌状态
			CopyMemory( StatusPlay.cbHearStatus,m_cbListenStatus,sizeof(m_cbListenStatus) );

			//历史记录
			StatusPlay.wOutCardUser=m_wOutCardUser;
			StatusPlay.cbOutCardData=m_cbOutCardData;
			CopyMemory(StatusPlay.cbDiscardCard,m_cbDiscardCard,sizeof(StatusPlay.cbDiscardCard));
			CopyMemory(StatusPlay.cbDiscardCount,m_cbDiscardCount,sizeof(StatusPlay.cbDiscardCount));

			//组合扑克
			CopyMemory(StatusPlay.WeaveItemArray,m_WeaveItemArray,sizeof(m_WeaveItemArray));
			CopyMemory(StatusPlay.cbWeaveCount,m_cbWeaveItemCount,sizeof(m_cbWeaveItemCount));

			//堆立信息
			StatusPlay.wHeapHead = m_wHeapHead;
			StatusPlay.wHeapTail = m_wHeapTail;
			CopyMemory(StatusPlay.cbHeapCardInfo,m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo));

			//扑克数据
			StatusPlay.cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex[wChiarID],StatusPlay.cbCardData);
			if( m_wReplaceUser == wChiarID ) StatusPlay.cbSendCardData = m_cbSendCardData;
			else StatusPlay.cbSendCardData=(m_wProvideUser==wChiarID)?m_cbProvideCard:0x00;

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
	switch (wSubCmdID)
	{
	case SUB_C_OUT_CARD:		//出牌消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OutCard));
			if (wDataSize!=sizeof(CMD_C_OutCard)) return false;

			//用户效验
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			CMD_C_OutCard * pOutCard=(CMD_C_OutCard *)pDataBuffer;
			return OnUserOutCard(pIServerUserItem->GetChairID(),pOutCard->cbCardData);
		}
	case SUB_C_OPERATE_CARD:	//操作消息
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_OperateCard));
			if (wDataSize!=sizeof(CMD_C_OperateCard)) return false;

			//用户效验
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//消息处理
			CMD_C_OperateCard * pOperateCard=(CMD_C_OperateCard *)pDataBuffer;
			return OnUserOperateCard(pIServerUserItem->GetChairID(),pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);
		}
	case SUB_C_REPLACE_CARD:	//用户补牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_ReplaceCard));
			if (wDataSize!=sizeof(CMD_C_ReplaceCard)) return false;

			//用户效验
			ASSERT(pIServerUserItem->GetUserStatus()==US_PLAYING);
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//变量定义
			CMD_C_ReplaceCard * pReplaceCard=(CMD_C_ReplaceCard *)pDataBuffer;

			//消息处理
			WORD wChairID=pIServerUserItem->GetChairID();
			return OnUserReplaceCard(pIServerUserItem->GetChairID(),pReplaceCard->cbCardData);
		}
	case SUB_C_TRUSTEE:
		{
			CMD_C_Trustee *pTrustee =(CMD_C_Trustee *)pDataBuffer;
			if(wDataSize != sizeof(CMD_C_Trustee)) return false;

			//用户效验
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();

			m_bTrustee[pIServerUserItem->GetChairID()]=pTrustee->bTrustee;
			CMD_S_Trustee Trustee;
			Trustee.bTrustee=pTrustee->bTrustee;
			Trustee.wChairID = pIServerUserItem->GetChairID();
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));

			return true;
		}
	case SUB_C_LISTEN:
		{
			//效验消息
			ASSERT(wDataSize==sizeof(CMD_C_Listen));
			if (wDataSize!=sizeof(CMD_C_Listen)) return false;

			//用户效验
			////tagServerUserData * pUserData=pIServerUserItem->GetUserData();
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			CMD_C_Listen *pListen = (CMD_C_Listen *)pDataBuffer;
			return OnUserListenCard(pIServerUserItem->GetChairID(),pListen->cbListen);
		}
	}

	return false;
}

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起来
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID, IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//庄家设置
	if (bLookonUser==false)
	{
		m_wBankerUser = INVALID_CHAIR;

		m_cbQuanCounter = 1;

		m_bTrustee[wChairID]=false;
		CMD_S_Trustee Trustee;
		Trustee.bTrustee=false;
		Trustee.wChairID = wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_TRUSTEE,&Trustee,sizeof(Trustee));
	}

	return true;
}

//用户听牌
bool CTableFrameSink::OnUserListenCard(WORD wChairID,BYTE cbListen)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//效验参数
	ASSERT((wChairID==m_wCurrentUser)&&(m_cbListenStatus[wChairID]==0));
	if ((wChairID!=m_wCurrentUser)||(m_cbListenStatus[wChairID]!=0)) return false;

	//设置变量
	m_cbListenStatus[wChairID] = cbListen;
	m_bEnjoinChiPeng[wChairID] = true;

	//构造数据
	CMD_S_Listen ListenCard;
	ListenCard.wChairId=wChairID;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LISTEN,&ListenCard,sizeof(ListenCard));

	return true;
}

//用户出牌
bool CTableFrameSink::OnUserOutCard(WORD wChairID, BYTE cbCardData)
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY) return true;

	//错误断言
	ASSERT(wChairID==m_wCurrentUser);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wCurrentUser) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false) return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置变量
	m_bSendStatus=true;
	m_bGangStatus = false;
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

	m_wProvideUser=wChairID;
	m_cbProvideCard=cbCardData;

	//用户切换
	m_wCurrentUser=(wChairID+m_wPlayerCount-1)%m_wPlayerCount;

	//响应判断
	bool bAroseAction=EstimateUserRespond(wChairID,cbCardData,EstimatKind_OutCard);

	//派发扑克
	if (bAroseAction==false) DispatchCardData(m_wCurrentUser);

	return true;
}

//用户操作
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard[3])
{
	//效验状态
	ASSERT(m_pITableFrame->GetGameStatus()==GS_MJ_PLAY);
	if (m_pITableFrame->GetGameStatus()!=GS_MJ_PLAY)
		return true;

	//效验用户 注意：当有机器人时有可能会发生此断言
	//ASSERT((wChairID==m_wCurrentUser)||(m_wCurrentUser==INVALID_CHAIR));
	if ((wChairID!=m_wCurrentUser)&&(m_wCurrentUser!=INVALID_CHAIR)) 
		return true;

	//被动动作
	if (m_wCurrentUser==INVALID_CHAIR)
	{
		//效验状态
		//ASSERT(m_bResponse[wChairID]==false);
		//ASSERT( m_cbUserAction[wChairID] != WIK_NULL );
		//ASSERT((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)!=0));

		//效验状态
		if (m_bResponse[wChairID]==true) return true;
		if( m_cbUserAction[wChairID] == WIK_NULL ) return true;
		if ((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)==0)) return true;

		//变量定义
		WORD wTargetUser=wChairID;
		BYTE cbTargetAction=cbOperateCode;

		//设置变量
		m_bResponse[wChairID]=true;
		m_cbPerformAction[wChairID]=cbOperateCode;
		if(cbOperateCard[0]==0)
			m_cbOperateCard[wChairID][0] = m_cbProvideCard;
		else CopyMemory( m_cbOperateCard[wChairID],cbOperateCard,sizeof(m_cbOperateCard[wChairID]) );

		//执行判断
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			//获取动作
			BYTE cbUserAction=(m_bResponse[i]==false)?m_cbUserAction[i]:m_cbPerformAction[i];

			//优先级别
			BYTE cbUserActionRank=m_GameLogic.GetUserActionRank(cbUserAction);
			BYTE cbTargetActionRank=m_GameLogic.GetUserActionRank(cbTargetAction);

			//动作判断
			if (cbUserActionRank>cbTargetActionRank)
			{
				wTargetUser=i;
				cbTargetAction=cbUserAction;
			}
		}
		if (m_bResponse[wTargetUser]==false) 
			return true;

		//吃胡等待
		if (cbTargetAction==WIK_CHI_HU)
		{
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if ((m_bResponse[i]==false)&&(m_cbUserAction[i]&WIK_CHI_HU))
					return true;
			}
		}

		//放弃操作
		if (cbTargetAction==WIK_NULL)
		{
			//用户状态
			ZeroMemory(m_bResponse,sizeof(m_bResponse));
			ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
			ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
			ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

			//发送扑克
			DispatchCardData(m_wResumeUser);

			return true;
		}

		//变量定义
		BYTE cbTargetCard=m_cbOperateCard[wTargetUser][0];

		//出牌变量
		m_cbOutCardData=0;
		m_bSendStatus=true;
		m_wOutCardUser=INVALID_CHAIR;

		//胡牌操作
		if (cbTargetAction==WIK_CHI_HU)
		{
			//结束信息
			m_cbChiHuCard=cbTargetCard;

			for (WORD i=(m_wProvideUser+m_wPlayerCount-1)%GAME_PLAYER;i!=m_wProvideUser;i = (i+m_wPlayerCount-1)%GAME_PLAYER)
			{
				//过虑判断
				if ((m_cbPerformAction[i]&WIK_CHI_HU)==0)
					continue;

				//胡牌判断
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[i];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[i];
				m_dwChiHuKind[i] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],pWeaveItem,cbWeaveItemCount,m_cbChiHuCard,m_ChiHuRight[i]);

				//插入扑克
				if (m_dwChiHuKind[i]!=WIK_NULL) 
				{
					m_cbCardIndex[i][m_GameLogic.SwitchToCardIndex(m_cbChiHuCard)]++;
					wTargetUser = i;
					break;
				}
			}

			//结束游戏
			ASSERT(m_dwChiHuKind[wTargetUser]!=WIK_NULL);
			OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

			return true;
		}

		//组合扑克
		ASSERT(m_cbWeaveItemCount[wTargetUser]<MAX_WEAVE);
		WORD wIndex=m_cbWeaveItemCount[wTargetUser]++;
		m_WeaveItemArray[wTargetUser][wIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wTargetUser][wIndex].cbCenterCard=cbTargetCard;
		m_WeaveItemArray[wTargetUser][wIndex].cbWeaveKind=cbTargetAction;
		m_WeaveItemArray[wTargetUser][wIndex].wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		m_WeaveItemArray[wTargetUser][wIndex].cbCardData[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = m_cbOperateCard[wTargetUser][1];
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = m_cbOperateCard[wTargetUser][2];
		}
		else
		{
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[1] = cbTargetCard;
			m_WeaveItemArray[wTargetUser][wIndex].cbCardData[2] = cbTargetCard;
			if( cbTargetAction & WIK_GANG )
				m_WeaveItemArray[wTargetUser][wIndex].cbCardData[3] = cbTargetCard;
		}

		//删除扑克
		switch (cbTargetAction)
		{
		case WIK_LEFT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_RIGHT:		//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_CENTER:	//上牌操作
			{
				//删除扑克
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],&m_cbOperateCard[wTargetUser][1],2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_PENG:		//碰牌操作
			{
				//删除扑克
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,2) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		case WIK_GANG:		//杠牌操作
			{
				//删除扑克,被动动作只存在放杠
				BYTE cbRemoveCard[]={cbTargetCard,cbTargetCard,cbTargetCard};
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wTargetUser],cbRemoveCard,CountArray(cbRemoveCard)) )
				{
					ASSERT( FALSE );
					return false;
				}

				break;
			}
		default:
			ASSERT( FALSE );
			return false;
		}

		//构造结果
		CMD_S_OperateResult OperateResult;
		ZeroMemory( &OperateResult,sizeof(OperateResult) );
		OperateResult.wOperateUser=wTargetUser;
		OperateResult.cbOperateCode=cbTargetAction;
		OperateResult.wProvideUser=(m_wProvideUser==INVALID_CHAIR)?wTargetUser:m_wProvideUser;
		OperateResult.cbOperateCard[0] = cbTargetCard;
		if( cbTargetAction&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			CopyMemory( &OperateResult.cbOperateCard[1],&m_cbOperateCard[wTargetUser][1],2*sizeof(BYTE) );
		else if( cbTargetAction&WIK_PENG )
		{
			OperateResult.cbOperateCard[1] = cbTargetCard;
			OperateResult.cbOperateCard[2] = cbTargetCard;
		}

		//发送消息
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

		//用户状态
		ZeroMemory(m_bResponse,sizeof(m_bResponse));
		ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
		ZeroMemory(m_cbOperateCard,sizeof(m_cbOperateCard));
		ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

		//设置用户
		m_wCurrentUser=wTargetUser;

		//杠牌处理
		if (cbTargetAction==WIK_GANG)
		{
			m_bGangStatus = true;
			DispatchCardData(wTargetUser,true);
		}

		return true;
	}

	//主动动作
	if (m_wCurrentUser==wChairID)
	{
		//效验操作
		ASSERT((cbOperateCode!=WIK_NULL)&&((m_cbUserAction[wChairID]&cbOperateCode)!=0));
		if ((cbOperateCode==WIK_NULL)||((m_cbUserAction[wChairID]&cbOperateCode)==0))
			return false;

		//扑克效验
		ASSERT((cbOperateCode==WIK_NULL)||(cbOperateCode==WIK_CHI_HU)||(m_GameLogic.IsValidCard(cbOperateCard[0])==true));
		if ((cbOperateCode!=WIK_NULL)&&(cbOperateCode!=WIK_CHI_HU)&&(m_GameLogic.IsValidCard(cbOperateCard[0])==false)) 
			return false;

		//设置变量
		m_bSendStatus=true;
		m_cbUserAction[m_wCurrentUser]=WIK_NULL;
		m_cbPerformAction[m_wCurrentUser]=WIK_NULL;

		bool bPublic=false;

		//执行动作
		switch (cbOperateCode)
		{
		case WIK_GANG:			//杠牌操作
			{
				//变量定义
				BYTE cbWeaveIndex=0xFF;
				BYTE cbCardIndex=m_GameLogic.SwitchToCardIndex(cbOperateCard[0]);

				//杠牌处理
				if (m_cbCardIndex[wChairID][cbCardIndex]==1)
				{
					//寻找组合
					for (BYTE i=0;i<m_cbWeaveItemCount[wChairID];i++)
					{
						BYTE cbWeaveKind=m_WeaveItemArray[wChairID][i].cbWeaveKind;
						BYTE cbCenterCard=m_WeaveItemArray[wChairID][i].cbCenterCard;
						if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
						{
							bPublic=true;
							cbWeaveIndex=i;
							break;
						}
					}

					//效验动作
					ASSERT(cbWeaveIndex!=0xFF);
					if (cbWeaveIndex==0xFF) return false;

					//组合扑克
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[3]=cbOperateCard[0];
				}
				else
				{
					//扑克效验
					ASSERT(m_cbCardIndex[wChairID][cbCardIndex]==4);
					if (m_cbCardIndex[wChairID][cbCardIndex]!=4) 
						return false;

					//设置变量
					bPublic=false;
					cbWeaveIndex=m_cbWeaveItemCount[wChairID]++;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbPublicCard=FALSE;
					m_WeaveItemArray[wChairID][cbWeaveIndex].wProvideUser=wChairID;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbWeaveKind=cbOperateCode;
					m_WeaveItemArray[wChairID][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
					for( BYTE j = 0; j < 4; j++ ) m_WeaveItemArray[wChairID][cbWeaveIndex].cbCardData[j] = cbOperateCard[0];
				}

				//删除扑克
				m_cbCardIndex[wChairID][cbCardIndex]=0;

				m_bGangStatus = true;

				//构造结果
				CMD_S_OperateResult OperateResult;
				ZeroMemory( &OperateResult,sizeof(OperateResult) );
				OperateResult.wOperateUser=wChairID;
				OperateResult.wProvideUser=wChairID;
				OperateResult.cbOperateCode=cbOperateCode;
				OperateResult.cbOperateCard[0]=cbOperateCard[0];

				//发送消息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPERATE_RESULT,&OperateResult,sizeof(OperateResult));

				//效验动作
				bool bAroseAction=false;
				if (bPublic==true) bAroseAction=EstimateUserRespond(wChairID,cbOperateCard[0],EstimatKind_GangCard);

				//发送扑克
				if (bAroseAction==false)
				{
					DispatchCardData(wChairID,true);
				}

				return true;
			}
		case WIK_CHI_HU:		//吃胡操作
			{
				//普通胡牌
				BYTE cbWeaveItemCount=m_cbWeaveItemCount[wChairID];
				tagWeaveItem * pWeaveItem=m_WeaveItemArray[wChairID];
				if( !m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],&m_cbProvideCard,1) )
				{
					ASSERT( FALSE );
					return false;
				}
				m_dwChiHuKind[wChairID] = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[wChairID],pWeaveItem,cbWeaveItemCount,m_cbProvideCard,
					m_ChiHuRight[wChairID]);
				m_cbCardIndex[wChairID][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;

				//结束信息
				m_cbChiHuCard=m_cbProvideCard;

				//结束游戏
				OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

				return true;
			}
		}

		return true;
	}

	return false;
}

//用户补牌
bool CTableFrameSink::OnUserReplaceCard(WORD wChairID, BYTE cbCardData)
{
	//错误断言
	ASSERT(wChairID==m_wReplaceUser);
	ASSERT(m_GameLogic.IsHuaCard(cbCardData)==true);
	ASSERT(m_GameLogic.IsValidCard(cbCardData)==true);

	//效验参数
	if (wChairID!=m_wReplaceUser) return false;
	if (m_GameLogic.IsHuaCard(cbCardData)==false) return false;
	if (m_GameLogic.IsValidCard(cbCardData)==false)  return false;

	//删除扑克
	if (m_GameLogic.RemoveCard(m_cbCardIndex[wChairID],cbCardData)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//状态变量
	m_bSendStatus=true;
	m_cbSendCardData=0x00;

	//花牌数量统计
	m_cbHuaCardCount[wChairID]++;

	//丢弃扑克
	m_cbDiscardCount[wChairID]++;
	m_cbDiscardCard[wChairID][m_cbDiscardCount[wChairID]-1]=cbCardData;

	//构造数据
	CMD_S_ReplaceCard ReplaceCard;
	ReplaceCard.wReplaceUser=wChairID;
	ReplaceCard.cbReplaceCard=cbCardData;

	//发送消息
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

	//派发扑克
	DispatchCardData(wChairID);

	return true;
}

//发送操作
bool CTableFrameSink::SendOperateNotify()
{
	//发送提示
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbUserAction[i]!=WIK_NULL)
		{
			//构造数据
			CMD_S_OperateNotify OperateNotify;
			OperateNotify.wResumeUser=m_wResumeUser;
			OperateNotify.cbActionCard=m_cbProvideCard;
			OperateNotify.cbActionMask=m_cbUserAction[i];

			//发送数据
			m_pITableFrame->SendTableData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
			m_pITableFrame->SendLookonData(i,SUB_S_OPERATE_NOTIFY,&OperateNotify,sizeof(OperateNotify));
		}
	}

	return true;
}

//派发扑克
bool CTableFrameSink::DispatchCardData(WORD wCurrentUser,bool bTail)
{
	//状态效验
	ASSERT(wCurrentUser!=INVALID_CHAIR);
	if (wCurrentUser==INVALID_CHAIR)
		return false;
	ASSERT( m_bSendStatus );
	if( m_bSendStatus == false ) return false;

	//丢弃扑克
	if ((m_wReplaceUser==INVALID_CHAIR)&&(m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		m_cbDiscardCount[m_wOutCardUser]++;
		m_cbDiscardCard[m_wOutCardUser][m_cbDiscardCount[m_wOutCardUser]-1]=m_cbOutCardData;
	}

	//荒庄结束
	if (m_cbLeftCardCount==m_cbEndLeftCount)
	{
		m_cbChiHuCard=0;
		m_wProvideUser=INVALID_CHAIR;
		OnEventGameConclude(m_wProvideUser,NULL,GER_NORMAL);

		return true;
	}

	//设置变量
	if( m_wReplaceUser == INVALID_CHAIR )
	{
		m_cbOutCardData=0;
		m_wCurrentUser=wCurrentUser;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//发送扑克
	m_cbSendCardCount++;
	m_cbSendCardData=m_cbRepertoryCard[--m_cbLeftCardCount];
	//加牌
	m_cbCardIndex[wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbSendCardData)]++;

	//补花判断
	if ((m_wReplaceUser!=INVALID_CHAIR)||(m_GameLogic.IsHuaCard(m_cbSendCardData)==true))
	{
		m_wReplaceUser=INVALID_CHAIR;

		//用户判断
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//用户索引
			WORD wUserIndex=(wCurrentUser+GAME_PLAYER-i)%GAME_PLAYER;

			//扑克判断
			for (BYTE j=MAX_INDEX-MAX_HUA_CARD;j<MAX_INDEX;j++)
			{
				if (m_cbCardIndex[wUserIndex][j]>0)
				{
					m_wReplaceUser=wUserIndex;
					break;
				}
			}

			if( m_wReplaceUser != INVALID_CHAIR ) break;
		}
	}

	if( m_wCurrentUser == wCurrentUser )
	{
		//设置变量
		m_wProvideUser=wCurrentUser;
		m_cbProvideCard=m_cbSendCardData;
	}

	if( m_wReplaceUser == INVALID_CHAIR )
	{
		if( !m_bEnjoinChiHu[m_wCurrentUser] )
		{
			//胡牌判断
			CChiHuRight chr;
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]--;
			m_cbUserAction[m_wCurrentUser] |= m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[m_wCurrentUser],m_WeaveItemArray[m_wCurrentUser],
				m_cbWeaveItemCount[m_wCurrentUser],m_cbProvideCard,chr);
			m_cbCardIndex[m_wCurrentUser][m_GameLogic.SwitchToCardIndex(m_cbProvideCard)]++;
		}

		//杠牌判断
		if ((m_bEnjoinChiPeng[m_wCurrentUser]==false)&&(m_cbLeftCardCount>m_cbEndLeftCount))
		{
			tagGangCardResult GangCardResult;
			m_cbUserAction[m_wCurrentUser]|=m_GameLogic.AnalyseGangCard(m_cbCardIndex[m_wCurrentUser],
				m_WeaveItemArray[m_wCurrentUser],m_cbWeaveItemCount[m_wCurrentUser],GangCardResult);
		}
	}

	//堆立信息
	ASSERT( m_wHeapHead != INVALID_CHAIR && m_wHeapTail != INVALID_CHAIR );
	if( !bTail )
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapHead][0]+m_cbHeapCardInfo[m_wHeapHead][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapHead=(m_wHeapHead+1)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapHead][0]++;
	}
	else
	{
		//切换索引
		BYTE cbHeapCount=m_cbHeapCardInfo[m_wHeapTail][0]+m_cbHeapCardInfo[m_wHeapTail][1];
		if (cbHeapCount==HEAP_FULL_COUNT) 
			m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapCardInfo);
		m_cbHeapCardInfo[m_wHeapTail][1]++;
	}

	//构造数据
	CMD_S_SendCard SendCard;
	SendCard.wSendCardUser = wCurrentUser;
	SendCard.wReplaceUser = m_wReplaceUser;
	SendCard.wCurrentUser= m_wReplaceUser==INVALID_CHAIR?m_wCurrentUser:INVALID_CHAIR;
	SendCard.bTail = bTail;
	SendCard.cbActionMask=m_wReplaceUser==INVALID_CHAIR?m_cbUserAction[m_wCurrentUser]:WIK_NULL;
	SendCard.cbCardData=(m_wCurrentUser==wCurrentUser)?m_cbProvideCard:m_cbSendCardData;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));

	return true;
}

//响应判断
bool CTableFrameSink::EstimateUserRespond(WORD wCenterUser, BYTE cbCenterCard, enEstimatKind EstimatKind)
{
	//变量定义
	bool bAroseAction=false;

	//用户状态
	ZeroMemory(m_bResponse,sizeof(m_bResponse));
	ZeroMemory(m_cbUserAction,sizeof(m_cbUserAction));
	ZeroMemory(m_cbPerformAction,sizeof(m_cbPerformAction));

	//动作判断
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//用户过滤
		if (wCenterUser==i) continue;

		//出牌类型
		if (EstimatKind==EstimatKind_OutCard)
		{
			//吃碰判断
			if (m_bEnjoinChiPeng[i]==false)
			{
				//碰牌判断
				m_cbUserAction[i]|=m_GameLogic.EstimatePengCard(m_cbCardIndex[i],cbCenterCard);

				//吃牌判断
				WORD wEatUser=(wCenterUser+m_wPlayerCount-1)%m_wPlayerCount;
				if (wEatUser==i)
					m_cbUserAction[i]|=m_GameLogic.EstimateEatCard(m_cbCardIndex[i],cbCenterCard);

				//杠牌判断
				if (m_cbLeftCardCount>m_cbEndLeftCount) 
				{
					m_cbUserAction[i]|=m_GameLogic.EstimateGangCard(m_cbCardIndex[i],cbCenterCard);
				}
			}
		}

		//胡牌判断
		if (m_bEnjoinChiHu[i]==false )
		{
			//吃胡判断
			CChiHuRight chr;
			BYTE cbWeaveCount=m_cbWeaveItemCount[i];
			BYTE cbAction = m_GameLogic.AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,chr);
			m_cbUserAction[i] |= cbAction;
		}

		//结果判断
		if (m_cbUserAction[i]!=WIK_NULL) 
			bAroseAction=true;
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

//算分
BYTE CTableFrameSink::CalScore( WORD wWinner, LONGLONG lScore[GAME_PLAYER] )
{
	//初始化
	ZeroMemory( lScore,sizeof(LONGLONG)*GAME_PLAYER );

	//胡牌番数
	WORD wFanShuo = m_GameLogic.GetChiHuActionRank( m_ChiHuRight[wWinner] );
	//花牌番数
	wFanShuo += m_cbHuaCardCount[wWinner];
	//花牌个数
	ASSERT( wFanShuo > 0 );
	if( wFanShuo == 0 ) return 0;

	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		if( i != wWinner )
		{
			//点炮另外两家不扣分
			if( wWinner != m_wProvideUser && i != m_wProvideUser ) continue;

			LONGLONG lGameScore = wFanShuo;
			lScore[i] = -lGameScore;
			lScore[wWinner] += lGameScore;
		}
	}

	return BYTE(wFanShuo);
}

//从系统翻牌获取钻牌索引
BYTE CTableFrameSink::GetMagicIndex( BYTE cbFanCard )
{
	BYTE cbZPaiIndex = m_GameLogic.SwitchToCardIndex(cbFanCard);
	if( cbZPaiIndex < 27 )
	{
		if( (cbZPaiIndex+1)%9 == 0 )
			cbZPaiIndex -= 8;
		else
			cbZPaiIndex++;
	}
	else
	{
		if( cbZPaiIndex + 1 == MAX_INDEX )
			cbZPaiIndex = 27;
		else
			cbZPaiIndex++;
	}
	return cbZPaiIndex;
}

//获取桌面牌数
BYTE CTableFrameSink::GetDiscardCount( BYTE cbCardData )
{
	BYTE cbCount = 0;
	//遍历丢弃牌
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		for( BYTE j = 0; j < m_cbDiscardCount[i]; j++ )
		{
			if( m_cbDiscardCard[i][j] == cbCardData )
				if( ++cbCount == 4 ) break;
		}
	}
	if( cbCount < 4 )
	{
		//检查杠牌
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			for( BYTE j = 0; j < m_cbWeaveItemCount[i]; j++ )
			{
				if( !m_WeaveItemArray[i][j].cbPublicCard ) continue;
				BYTE cbCardBuffer[4];
				BYTE cbCardCount = m_GameLogic.GetWeaveCard( m_WeaveItemArray[i][j].cbWeaveKind,
					m_WeaveItemArray[i][j].cbCenterCard,cbCardBuffer );
				for( BYTE k = 0; k < cbCardCount; k++ )
					if( cbCardBuffer[k] == cbCardData )
						if( ++cbCount == 4 ) break;
			}
		}
	}

	return cbCount;
}

//权位过滤
void CTableFrameSink::FiltrateRight( WORD wWinner, CChiHuRight &chr )
{
	/*	增加权位	*/

	//自摸
	if( wWinner == m_wProvideUser )
		chr |= CHR_ZI_MO;
	//圈风刻,门风刻
	for( BYTE i = 0; i < m_cbWeaveItemCount[wWinner]; i++ )
	{
		if( m_WeaveItemArray[wWinner][i].cbWeaveKind == WIK_PENG &&
			m_WeaveItemArray[wWinner][i].cbCenterCard >= 0x31 )
		{
			BYTE cbCenterCard = m_WeaveItemArray[wWinner][i].cbCenterCard;
			//圈风刻
			if( m_cbQuanFeng == 0 && cbCenterCard == 0x31 ||
				m_cbQuanFeng == 1 && cbCenterCard == 0x32 ||
				m_cbQuanFeng == 2 && cbCenterCard == 0x33 || 
				m_cbQuanFeng == 3 && cbCenterCard == 0x34 )
				chr |= CHR_QUAN_FENG_KE;
			//门风刻
			if( (m_wBankerUser+GAME_PLAYER-wWinner)%GAME_PLAYER == (cbCenterCard&MASK_VALUE)-1 )
				chr |= CHR_MEN_FENG_KE;
		}
	}
	for( BYTE i = 27; i < MAX_INDEX-MAX_HUA_CARD; i++ )
	{
		if( m_cbCardIndex[wWinner][i] == 3 )
		{
			BYTE cbCenterCard = m_GameLogic.SwitchToCardData(i);
			//圈风刻
			if( m_cbQuanFeng == 0 && cbCenterCard == 0x31 ||
				m_cbQuanFeng == 1 && cbCenterCard == 0x32 ||
				m_cbQuanFeng == 2 && cbCenterCard == 0x33 || 
				m_cbQuanFeng == 3 && cbCenterCard == 0x34 )
				chr |= CHR_QUAN_FENG_KE;
			//门风刻
			if( (m_wBankerUser+GAME_PLAYER-wWinner)%GAME_PLAYER == (cbCenterCard&MASK_VALUE)-1 )
				chr |= CHR_MEN_FENG_KE;
		}
	}
	//和绝张
	BYTE cbIndexProviderCard = m_GameLogic.SwitchToCardIndex(m_cbProvideCard);
	BYTE cbTableCount = 0;
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//吃碰
		for( BYTE j = 0; j < m_cbWeaveItemCount[i]; j++ )
		{
			if( m_WeaveItemArray[i][j].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG) )
			{
				for( BYTE k = 0; k < 3; k++ )
					if( m_cbProvideCard == m_WeaveItemArray[i][j].cbCardData[k] )
						cbTableCount++;
			}
		}
		//丢弃牌
		for( BYTE j = 0; j < m_cbDiscardCount[i]; j++ )
		{
			if( m_cbDiscardCard[i][j] == m_cbProvideCard )
				cbTableCount++;
		}
	}
	ASSERT( cbTableCount < 4 );
	if( cbTableCount == 3 )
		chr |= CHR_JUE_ZHANG;

	if( m_bGangStatus )
	{
		//抢杠和
		if( (chr&CHR_ZI_MO).IsEmpty() )
			chr |= CHR_QIANG_GANG;
		//杠上开花
		else chr |= CHR_GANG_KAI;
	}
	//海底捞月
	if( m_cbLeftCardCount==m_cbEndLeftCount && m_wProvideUser != wWinner )
		chr |= CHR_HAI_DI;
	//妙手回春
	if( m_cbLeftCardCount==m_cbEndLeftCount && m_wProvideUser == wWinner )
		chr |= CHR_MIAO_SHOU;

	/*	修改权位	*/

	//全求人 必须是胡别人牌
	if( !(chr&CHR_QUAN_QIU_REN).IsEmpty() && m_wProvideUser==wWinner )
		chr &= ~CHR_QUAN_QIU_REN;
	//不求人 必须自摸
	if( !(chr&CHR_BU_QIU_REN).IsEmpty() && m_wProvideUser!=wWinner )
		chr &= ~CHR_BU_QIU_REN;
	//门前清 必须是自摸
	if( !(chr&CHR_MEN_QI_QING).IsEmpty() && m_wProvideUser != wWinner )
		chr &= ~CHR_MEN_QI_QING;
	//无番和 和牌后，包括花牌，数不出任何番种分	!!!注:无番和一定要放在最后检查
	if( !(chr&CHR_WU_FAN).IsEmpty() )
	{
		//去除无番和
		if( m_cbHuaCardCount[wWinner] > 0 || !(chr&~CHR_WU_FAN).IsEmpty() )
		{
			chr &= ~CHR_WU_FAN;
		}
	}

	/*	调整权位	*/

	//大四喜	不计圈风刻、门风刻、三风刻、碰碰和
	if( !(chr&CHR_DA_SHI_XI).IsEmpty() )
	{
		chr &= ~CHR_QUAN_FENG_KE;
		chr &= ~CHR_MEN_FENG_KE;
		chr &= ~CHR_SAN_FENG_KE;
		chr &= ~CHR_PENG_PENG;
	}
	//大三元	不计箭刻
	if( !(chr&CHR_DA_SAN_YUAN).IsEmpty() )
		chr &= ~CHR_JIAN_KE;
	//绿一色	不计混一色
	if( !(chr&CHR_LU_YI_SE).IsEmpty() )
		chr &= ~CHR_HUN_YI_SE;
	//九莲宝灯	不计清一色
	if( !(chr&CHR_JIU_LIAN_DENG).IsEmpty() )
		chr &= ~CHR_QING_YI_SE;
	//四杠	不计三杠
	if( !(chr&CHR_SI_GANG).IsEmpty() )
		chr &= ~CHR_SAN_GANG;
	//连七对	不计清一色、不求人、单钓
	if( !(chr&CHR_LIAN_QI_DUI).IsEmpty() )
	{
		chr &= ~CHR_QING_YI_SE;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//十三幺	不计五门齐、不求人、单钓
	if( !(chr&CHR_SHI_SAN_YAO).IsEmpty() )
	{
		chr &= ~CHR_WU_MEN_QI;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//清幺九	不计碰碰和、同刻、无字
	if( !(chr&CHR_QING_YAO_JIU).IsEmpty() )
	{
		chr &= ~CHR_PENG_PENG;
		chr &= ~CHR_SAN_TONG_KE;
		chr &= ~CHR_SHUANG_TONG_KE;
		chr &= ~CHR_WU_ZI;
	}
	//小四喜	不计三风刻
	if( !(chr&CHR_XIAO_SHI_XI).IsEmpty() )
		chr &= ~CHR_SAN_FENG_KE;
	//小三元	不计箭刻
	if( !(chr&CHR_JIAN_KE).IsEmpty() )
		chr &= ~CHR_JIAN_KE;
	//字一色	不计碰碰和
	if( !(chr&CHR_ZI_YI_SE).IsEmpty() )
		chr &= ~CHR_PENG_PENG;
	//四暗刻	不计门前清、碰碰和、三暗刻、双暗刻
	if( !(chr&CHR_SI_AN_KE).IsEmpty() )
	{
		chr &= ~CHR_MEN_QI_QING;
		chr &= ~CHR_PENG_PENG;
		chr &= ~CHR_SAN_AN_KE;
		chr &= ~CHR_SHUANG_AN_KE;
	}
	//一色双龙会	不计平和、七对、清一色
	if( !(chr&CHR_YI_SE_SHUANG).IsEmpty() )
	{
		chr &= ~CHR_PING_HU;
		chr &= ~CHR_QI_DUI;
		chr &= ~CHR_QING_YI_SE;
	}
	//一色四同顺	不计一色三节高、一般高、四归一
	if( !(chr&CHR_YI_SHI_TONG).IsEmpty() )
	{
		chr &= ~CHR_YI_SE_JIE;
		chr &= ~CHR_YI_BAN_GAO;
		chr &= ~CHR_SHI_GUI_YI;
	}
	//一色四节高	不计一色三同顺、碰碰和
	if( !(chr&CHR_YI_SHI_JIE).IsEmpty() )
	{
		chr &= ~CHR_YI_SE_TONG;
		chr &= ~CHR_PENG_PENG;
	}
	//混幺九	不计碰碰和
	if( !(chr&CHR_HUN_YAO).IsEmpty() )
		chr &= ~CHR_PENG_PENG;
	//七对	不计不求人、单钓
	if( !(chr&CHR_QI_DUI).IsEmpty() )
	{
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//七星不靠	不计五门齐、不求人、单钓
	if( !(chr&CHR_QI_XING_BU_KAO).IsEmpty() )
	{
		chr &= ~CHR_WU_MEN_QI;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//全双刻	不计碰碰和、断幺
	if( !(chr&CHR_QUAN_SHUANG).IsEmpty() )
	{
		chr &= ~CHR_PENG_PENG;
		chr &= ~CHR_DUAN_YAO;
	}
	//清一色	不无字
	if( !(chr&CHR_QING_YI_SE).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//一色三同顺	不计一色三节高
	if( !(chr&CHR_YI_SE_TONG).IsEmpty() )
		chr &= ~CHR_YI_SE_JIE;
	//一色三节高	不计一色三同顺
	if( !(chr&CHR_YI_SE_JIE).IsEmpty() )
		chr &= ~CHR_YI_SE_TONG;
	//全大	不计无字
	if( !(chr&CHR_QUAN_DA).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//全中	不计断幺
	if( !(chr&CHR_QUAN_ZHONG).IsEmpty() )
		chr &= ~CHR_DUAN_YAO;
	//全小	不计无字
	if( !(chr&CHR_QUAN_XIAO).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//三色双龙会	不计喜相逢、老少副、无字、平和
	if( !(chr&CHR_SAN_SE_SHUANG).IsEmpty() )
	{
		chr &= ~CHR_XI_XIANG_FENG;
		chr &= ~CHR_LAO_SHAO_JIANG;
		chr &= ~CHR_WU_ZI;
		chr &= ~CHR_PING_HU;
	}
	//全带五	不计断幺
	if( !(chr&CHR_QUAN_DAI_WU).IsEmpty() )
		chr &= ~CHR_DUAN_YAO;
	//三同刻	不计双同刻
	if( !(chr&CHR_SAN_TONG_KE).IsEmpty() )
		chr &= ~CHR_SHUANG_TONG_KE;
	//三暗刻	不计双暗刻
	if( !(chr&CHR_SAN_AN_KE).IsEmpty() )
		chr &= ~CHR_SHUANG_AN_KE;
	//全不靠	不计五门齐、不求人、单钓
	if( !(chr&CHR_QUAN_BU_KAO).IsEmpty() )
	{
		chr &= ~CHR_WU_MEN_QI;
		chr &= ~CHR_BU_QIU_REN;
		chr &= ~CHR_DAN_DIAO_JIANG;
	}
	//大于五	不计无字
	if( !(chr&CHR_DA_WU).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//小于五	不计无字
	if( !(chr&CHR_XIAO_WU).IsEmpty() )
		chr &= ~CHR_WU_ZI;
	//推不倒	不计缺一门
	if( !(chr&CHR_TUI_BU_DAO).IsEmpty() )
		chr &= ~CHR_QUE_YI_MEN;
	//妙手回春	不计自摸
	if( !(chr&CHR_MIAO_SHOU).IsEmpty() )
		chr &= ~CHR_ZI_MO;
	//杠上开花	不计自摸
	if( !(chr&CHR_GANG_KAI).IsEmpty() )
		chr &= ~CHR_ZI_MO;
	//抢杠和	不计和绝张
	if( !(chr&CHR_QIANG_GANG).IsEmpty() )
		chr &= ~CHR_JUE_ZHANG;
	//全求人	不计单钓
	if( !(chr&CHR_QUAN_QIU_REN).IsEmpty() )
		chr &= ~CHR_DAN_DIAO_JIANG;
	//双暗杠	不计暗杠
	if( !(chr&CHR_SHUANG_AN_GANG).IsEmpty() )
		chr &= ~CHR_AN_GANG;
	//双箭刻	不计箭刻
	if( !(chr&CHR_SHUANG_JIAN_KE).IsEmpty() )
		chr &= ~CHR_JIAN_KE;
	//双明杠	不计明杠
	if( !(chr&CHR_SHUANG_MING_GANG).IsEmpty() )
		chr &= ~CHR_MING_GANG;
	//单吊将	不计边张、坎
	if( !(chr&CHR_DAN_DIAO_JIANG).IsEmpty() )
	{
		chr &= ~CHR_BIAN_ZHANG;
		chr &= ~CHR_KAN_ZHANG;
	}
	//边张	不计坎张
	if( !(chr&CHR_BIAN_ZHANG).IsEmpty() )
		chr &= ~CHR_KAN_ZHANG;
}


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
