#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					2									//最少时间

//游戏时间
#define IDI_ANDROID_ITEM_SINK		100
#define IDI_START_GAME				(IDI_ANDROID_ITEM_SINK+0)			//开始定时器
//游戏时间
#define IDI_ADD						(IDI_ANDROID_ITEM_SINK+3)			//加注
#define IDI_SEE						(IDI_ANDROID_ITEM_SINK+4)			//看牌
#define IDI_FALLOW					(IDI_ANDROID_ITEM_SINK+5)			//跟注
#define IDI_COMPARE					(IDI_ANDROID_ITEM_SINK+6)			//比牌
#define IDI_GIVEUP					(IDI_ANDROID_ITEM_SINK+7)			//放弃
#define IDI_OPEN					(IDI_ANDROID_ITEM_SINK+8)			//开牌
#define IDI_COMPARE_OVER			(IDI_ANDROID_ITEM_SINK+9)			//比牌
//银行定时
#define IDI_BANK_OPERATE			(IDI_ANDROID_ITEM_SINK+10)			//银行定时

//时间标识
#define TIME_START_GAME				3									//开始定时器
#define TIME_USER_ADD_SCORE			4									//加注定时器
#define	TIME_USER_COMPARE_CARD		5									//比牌定时器
//////////////////////////////////////////////////////////////////////////
//辅助时间
#define TIME_LESS_TIME				2									//最少时间 
#define TIME_RAND_TIME				3									//随机时间 

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	//手上扑克
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(WORD i=0;i<GAME_PLAYER;i++)m_bMingZhu[i]=false;
	m_lMaxCellScore=0L;
	
	//接口变量
	m_pIAndroidUserItem=NULL;
	m_cbCardV = 0;
	m_bIsYz = false;
	m_bMinZhuIndex = 0;
	//随机种子
	srand( (unsigned)time(NULL));

	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void *  CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool  CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem)!=NULL);
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);

	//错误判断
	if (m_pIAndroidUserItem==NULL)
	{
		ASSERT(FALSE);
		return false;
	}
	return true;
}

//重置接口
bool  CAndroidUserItemSink::RepositionSink()
{
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	m_lMaxCellScore=0L;
	for(WORD i=0;i<GAME_PLAYER;i++)m_bMingZhu[i]=false;
	return true;
}

//时间消息
bool  CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始定时器
		{
			KillAllTimer();
			//发送准备
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_ADD:				//加注动作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_ADD);
			m_bFirstAdd=false;
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lMaxCellScore*2):m_lMaxCellScore;
			//发送消息
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=0;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			return true;
		}
	case IDI_SEE:				//看牌动作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_SEE);
			if(!m_bMingZhu[wMeChairID])
			{
				m_bMingZhu[wMeChairID]=true;
				m_pIAndroidUserItem->SendSocketData(SUB_C_LOOK_CARD,NULL,0);
			}
			return true;
		}
	case IDI_FALLOW:			//跟注动作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_FALLOW);
			m_bFirstAdd=false;
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*2):(m_lCurrentTimes*m_lCellScore);
			//发送消息
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=0;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			return true;
		}
	case IDI_COMPARE:			//比牌动作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE);
			LONGLONG lCurrentScore = m_lCurrentTimes*m_lCellScore*2;
			//发送消息
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=TRUE;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			return true;
		}
	case IDI_COMPARE_OVER:		//比牌消息
		{
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lMaxCellScore*2):m_lMaxCellScore;
			//发送消息
			CMD_C_AddScore AddScore;
			ZeroMemory(&AddScore,sizeof(AddScore));
			AddScore.wState=TRUE;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE_OVER);
			CMD_C_CompareCard CompareCard;
			ZeroMemory(&CompareCard,sizeof(CompareCard));
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if(i==wMeChairID)continue;
				if(m_cbPlayStatus[i]==TRUE)
				{
					CompareCard.wCompareUser=i;
					break;
				}
			}
			//发送消息
			m_pIAndroidUserItem->SendSocketData(SUB_C_COMPARE_CARD,&CompareCard,sizeof(CompareCard));
			return true;
		}
	case IDI_GIVEUP:			//放弃动作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_GIVEUP);
			m_pIAndroidUserItem->SendSocketData(SUB_C_GIVE_UP,NULL,0);
			return true;
		}
	case IDI_OPEN:				//开牌动作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_OPEN);
			LONGLONG lCurrentScore=(m_bMingZhu[wMeChairID])?(m_lCurrentTimes*m_lCellScore*4):(m_lCurrentTimes*m_lCellScore*2);
			//发送消息
			CMD_C_AddScore AddScore;
			AddScore.wState=2;
			AddScore.lScore=lCurrentScore;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));
			m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD,NULL,0);
			return true;
		}
	case IDI_BANK_OPERATE:		//银行操作
		{
			m_pIAndroidUserItem->KillGameTimer(IDI_BANK_OPERATE);  //删除时钟

			//变量定义
			IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();  //获取自已指针
			LONGLONG lRobotScore = pUserItem->GetUserScore();					//读取金币
			{

				//判断存取
				if (lRobotScore > m_lRobotScoreRange[1])   //金币大于最大值  存款
				{
					LONGLONG lSaveScore=0L;

					lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);  //计算百分比
					if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

					if (lSaveScore > 0)
						m_pIAndroidUserItem->PerformSaveScore(lSaveScore);			//存款

				}
				else if (lRobotScore < m_lRobotScoreRange[0])     //取款
				{

					//梦成网络修改
					//随机种子
					srand( (unsigned)time(NULL));
					
					SCORE lScore=m_lRobotBankGetScore+(rand()%(m_lRobotBankGetScoreBanker-m_lRobotBankGetScore));
					if (lScore > 0)
						m_pIAndroidUserItem->PerformTakeScore(lScore);
				}
			}
			return true;
		}
	

	}

	return false;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_ANDROID_GET_CARD:// 获取扑克
		{
			if(wDataSize != sizeof(CMD_S_SendCard)) return false;
			CMD_S_SendCard * p = (CMD_S_SendCard*)pData;
			//复制扑克数据
			CopyMemory(m_cbHandCardData,p->cbCardData,sizeof(p->cbCardData));
			//复制用户状态
			CopyMemory(m_cbPlayStatus,p->cbPlayStatus,sizeof(p->cbPlayStatus));

			//获取自已
			WORD MeChairID=m_pIAndroidUserItem->GetChairID();
			//开牌动作
			m_bMingZhu[MeChairID]=false;
			//////////////////////////////////////////////
			//最大牌形
			m_bBiggest=p->IsWinUser;

			m_bFirstAdd=true;


			//读取配置
			ReadConfigInformation();

			if(m_wCurrentUser==MeChairID)
			{
				m_bFirstAdd=true;
				//机器人所有操作
				Alloperations();
			}


			return true;
		}
	case SUB_S_GAME_START:		//游戏开始
		{
			//消息处理
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:		//用户下注
		{
			//消息处理
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_LOOK_CARD:		//看牌消息
		{
			//消息处理
			return OnSubLookCard(pData,wDataSize);
		}
	case SUB_S_COMPARE_CARD:	//比牌消息
		{
			//消息处理
			return OnSubCompareCard(pData,wDataSize);
		}
	case SUB_S_OPEN_CARD:		//开牌消息
		{
			//消息处理
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_GIVE_UP:			//用户放弃
		{
			//消息处理
			return OnSubGiveUp(pData,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:		//用户强退
		{
			//消息处理
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			//修改：2014-3-28
			m_bMingZhu[m_pIAndroidUserItem->GetChairID()]=false;

			//消息处理
			return OnSubGameEnd(pData,wDataSize);
		}
	case SUB_S_WAIT_COMPARE:	//等待比牌
		{

			//消息处理
			if (wDataSize!=sizeof(CMD_S_WaitCompare)) return false;
			CMD_S_WaitCompare * pWaitCompare=(CMD_S_WaitCompare *)pData;	
			ASSERT(pWaitCompare->wCompareUser==m_wCurrentUser);

			//重新定时
			if(m_pIAndroidUserItem->GetChairID()==m_wCurrentUser)
			{
				//比牌时间
				UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
				m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse);
			}

			return true;
		}
	}

	//错误断言
	//ASSERT(FALSE);

	return true;
}

//游戏消息
bool  CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool  CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, VOID * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			m_lCellScore = pStatusFree->lCellScore;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			//消息处理
			ReadConfigInformation();
			//开始时间
			UINT nElapse=rand()%(3)+TIME_LESS;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			m_lCellScore = pStatusPlay->lCellScore;
			m_lMaxCellScore = pStatusPlay->lMaxCellScore;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));
			
			ReadConfigInformation();
			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//用户进入
void  CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void  CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void  CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void  CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}



//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{


	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	//删除所有时钟
	KillAllTimer();

	m_bIsYz = false;
	//初始化
	m_bCompareState=0;
	m_lMaxCellScore=0;
	m_lUserAddScore=0;
	m_bMinZhuIndex=0;
	//牌数据
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));
	//明牌信息
	for(WORD i=0;i<GAME_PLAYER;i++)m_bMingZhu[i]=false;

	//数据信息
	m_lCellScore=pGameStart->lCellScore;		//单元下注	
	m_lCurrentTimes=pGameStart->lCurrentTimes;  //当前倍数
	m_lMaxCellScore=pGameStart->lMaxScore;		//单元上限
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_lUserMaxScore=pGameStart->lUserMaxScore;

	return true;
}

//用户放弃
bool CAndroidUserItemSink::OnSubGiveUp(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GiveUp)) return false;
	CMD_S_GiveUp * pGiveUp=(CMD_S_GiveUp *)pBuffer;
	KillAllTimer();
	//设置变量
	m_cbPlayStatus[pGiveUp->wGiveUpUser]=FALSE;

	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	IServerUserItem *pIServerUserItem=NULL;

	//玩家人数
	BYTE bCount = 0;
	BYTE bUserCount = 0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)
		{
			bCount++;
			IServerUserItem* pIUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
			if(pIUserItem == NULL) continue;
			if(!pIUserItem->IsAndroidUser())
			{
				bUserCount++;
			}
		}
	}

	if(bUserCount<=0 && bCount >= 2)
	{
		if(GaiLv(100)<40)
		{
			/*OnAllCompare();*/
			UINT nElapse=TIME_LESS_TIME;
			m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);

		}
		else
			Alloperations();
// 		UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
// 		m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse);
		return true;
	}
	//判断结束
	if(bCount>1)
	{
		WORD MeChairID=m_pIAndroidUserItem->GetChairID();
		if(m_wCurrentUser==MeChairID)
		{
			Alloperations();
		}
	}
	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{

	//变量定义
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_AddScore));
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	ASSERT(pAddScore->wCurrentUser < GAME_PLAYER);
	ASSERT(pAddScore->lCurrentTimes<=m_lMaxCellScore/m_lCellScore);
	
	KillAllTimer();
	//当前用户
	m_lCurrentTimes = pAddScore->lCurrentTimes;
	m_wCurrentUser=pAddScore->wCurrentUser;
	m_lTableScore[pAddScore->wAddScoreUser]+=pAddScore->lAddScoreCount;
	m_lUserAddScore+=pAddScore->lAddScoreCount;
	m_bCompareState=pAddScore->wCompareState;

	//设置时间
	if(pAddScore->wCompareState==FALSE && m_wCurrentUser==wMeChairID)
	{
		Alloperations();
	}
	else if(pAddScore->wCompareState==TRUE && m_wCurrentUser==wMeChairID)
	{
		//比牌
		OnAllCompare();
	}
	return true;
}

//用户看牌
bool CAndroidUserItemSink::OnSubLookCard(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	if (wDataSize!=sizeof(CMD_S_LookCard)) return false;
	KillAllTimer();
	CMD_S_LookCard * pLookCard=(CMD_S_LookCard *)pBuffer;
	m_bMingZhu[pLookCard->wLookCardUser] = true;   //看牌变量

	//判断扑克是否单张
	IsCanYa();

	//重新定时
	if(m_wCurrentUser==m_pIAndroidUserItem->GetChairID())
	{
		//下注时间
		Alloperations();
	}

	return true;
}

//用户比牌
bool CAndroidUserItemSink::OnSubCompareCard(const void * pBuffer, WORD wDataSize)
{
	
	//效验数据
	if (wDataSize!=sizeof(CMD_S_CompareCard)) return false;
	CMD_S_CompareCard * pCompareCard=(CMD_S_CompareCard *)pBuffer;
	KillAllTimer();
	//设置变量
	m_cbPlayStatus[pCompareCard->wLostUser]=FALSE;
	WORD wMeChairID=m_pIAndroidUserItem->GetChairID();

	//当前用户
	m_wCurrentUser=pCompareCard->wCurrentUser;

	//玩家人数
	BYTE bCount=0;
	BYTE bUserCount=0;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		if(m_cbPlayStatus[i]==TRUE)
		{
			IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);

			bCount++;
			if(pUserItem == NULL) continue;

			if(!pUserItem->IsAndroidUser())
				bUserCount++;
		}
	}

	//判断结束
	if(bCount>1)
	{
		//控件信息
		if(wMeChairID==m_wCurrentUser)
		{
			//所有操作
			if((bUserCount < 1)&&(GaiLv(100)<40))
			{
				//OnAllCompare();
				UINT nElapse=TIME_LESS_TIME;
				m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);
			}
			else
				Alloperations();
				
		}
	}

	return true;
}

//用户开牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_OpenCard)) return false;
	//效验数据
	KillAllTimer();
	return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	////效验数据
	//if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	//CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	////游戏信息
	//ASSERT(m_cbPlayStatus[pPlayerExit->wPlayerID]==TRUE);
	//m_cbPlayStatus[pPlayerExit->wPlayerID]=FALSE;


	//效验数据
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	//游戏信息
	ASSERT(m_cbPlayStatus[pPlayerExit->wPlayerID]==TRUE);
	m_cbPlayStatus[pPlayerExit->wPlayerID]=FALSE;
	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	////效验参数
	//if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	////开始按钮

	////下注时间
	//UINT nElapse=rand()%(TIME_START_GAME+3)+TIME_LESS*3;
	//m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);
	//下注时间
	KillAllTimer();
	//触发存取款判断

	m_pIAndroidUserItem->SetGameTimer(IDI_BANK_OPERATE, (rand()%2+1));
	UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	return true;
}
//////////////////////////////////////////////////////////////////////////
//全部操作
bool CAndroidUserItemSink::Alloperations()
{
	srand((unsigned)time(NULL));//设置随机数种子

	//获取自已
	WORD MeChairID=m_pIAndroidUserItem->GetChairID();
	
	//////////////默认
	m_bMinZhuIndex=0;	
	if(m_lUserAddScore<m_MinLostScore) m_bMinZhuIndex=m_MinLostIndex;   ////桌面下注没有达到最少下限
	if(m_lUserAddScore>m_MinLostScore && m_lUserAddScore<m_MaxLostScore) m_bMinZhuIndex=(rand()%m_MaxLostIndex)+m_MinLostIndex+15;   //在之间
	if(m_lUserAddScore>m_MaxLostScore) m_bMinZhuIndex=m_MaxLostIndex;   ////桌面下注达到最大上限

	CString strLog;
	strLog.Format(TEXT("开牌比率:%d,最小开牌比率:%d,最大开牌比率:%d,输分下限:%d"),m_bMinZhuIndex,m_MinLostIndex,m_MaxLostIndex,m_MinLostScore);
	CTraceService::TraceString(strLog,TraceLevel_Info);
	strLog.Format(TEXT("输分上线:%d,用户下注:%d"),m_MaxLostScore,m_lUserAddScore);
	CTraceService::TraceString(strLog,TraceLevel_Info);	

	if(!m_bMingZhu[MeChairID]) //没有看过牌的
	{
	
		if(GaiLv(m_bMinZhuIndex)&&m_bBiggest==true)  //百分之10看牌并且自已是最大的
		{
			//看牌
			UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
			m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse);
			return true;
		}

	}

	//金豆不足 无敌模式
	//////////////////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//开牌情况
	LONG lTemp=(m_bMingZhu[MeChairID])?6:5;   

	//m_lUserMaxScore用户金币-自已桌面下注 就是身上剩多少金币 少于 最大上限的5~6倍 
	//玩家游戏币不足
	if((m_lUserMaxScore-m_lTableScore[MeChairID]) < (m_lMaxCellScore*lTemp))   
	{
		//当前机器人是否最大如果是最大
		if(m_bBiggest)
		{
			//开牌  由于金币不足了,所有机器人选择开牌
			UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
			m_pIAndroidUserItem->SetGameTimer(IDI_OPEN,nElapse);
			return true;
		}
		else  //不是最大
		{
			////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			if(m_bIsYz)   //不是单牌类型
			{
				///////////////////////////////////////////////////////////////
				if(m_bMingZhu[MeChairID])  //已经看牌
				{
					if (GaiLv(40))  //百分之40
					{
						//开牌
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
					else
					{

						if(m_cbCardV>=CT_SHUN_ZI)
						{
							//开牌
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);

						}
						else
						{
							//放弃
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
							
						}
					}
				
				}
				else   ///////////////////////////////////这里能执行?m_bIsYz必需是看牌之后才为TRUE?!!什么逻辑
				{
					if (GaiLv(60))
					{
						//开牌
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
					else
					{
							//放弃
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);

						CString szStorageMin=TEXT("这里能执行?m_bIsYz必需是看牌之后才为TRUE?!!什么逻辑");
						//输出信息
						CTraceService::TraceString(szStorageMin,TraceLevel_Warning);


						return true;
					}
				} ////////////////////////////////////////////////////

				//////////////////////////////////////////////////////////////////////
			}  //////没有看过牌或者,牌是单张的情况
			else
			{
				//放弃
				if(m_bMingZhu[MeChairID])    //已经看牌  因为牌是单张类型所有非常大机率弃牌
				{
					if (GaiLv(90))  //百分之九十弃牌
					{
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						CString szStorageMin=TEXT("玩家游戏币不足,并且不是最大,而且牌是单张的牌放弃很正常");
						//输出信息
						CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
						return true;
					}
					else
					{				
						//开牌
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
				}
				else			//没有看过牌的
				{
					if (GaiLv(m_bMinZhuIndex))	//百分之五十弃牌  没有看牌就弃牌? 傻B吗! 放弃之前不是应该先看看牌吗?机器人!真2B 要改
					{
						//看牌
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse);
					}
					else
					{
						//开牌		没有看过牌,直接开牌   这里还可以! 
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
					}
				}

			}
			////^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			return true;
		}
	}
	//////////////////@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	//机器人智商
	if(m_bFirstAdd)//第一把
	{
		if(m_bBiggest)//赢 自已的牌最大
		{
			
			if(m_bMingZhu[MeChairID])  //看过牌的
			{
				if(GaiLv(80))
				{
					//跟注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}
				else
				{
					//加注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_ADD,nElapse);
					return true;
				}
			}
			else   //没有看过牌的
			{
				if(GaiLv(m_bMinZhuIndex)) 
				{
					//看牌
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//跟注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}
			}
		}
		else//输
		{			
			if(m_bMingZhu[MeChairID])  //看过牌
			{
			
				if(m_bIsYz)            //不是单张类形
				{
					switch(m_cbCardV)
					{
					case CT_SINGLE:
					case CT_SPECIAL:
						{
								//放弃
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);	
								return true;
						}
					case CT_DOUBLE:
					case CT_SHUN_ZI:
					case CT_JIN_HUA:
					case CT_SHUN_JIN:
					case CT_BAO_ZI:
						{
							//跟注
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
							return true;
						}
					}

					////////////////////////////如果都没有处理就跟注:
					//跟注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;

				}
				else
				{
					//牌很小
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);	
					return true;
				}

			}
			else   //这里是没有看过牌的
			{				
				if(GaiLv(m_bMinZhuIndex))   //看牌机率有点小!
				{
					//看牌
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//跟注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}			
			}
		}
	}
	else   //不是第一把下注 2 3 4 5  次下注了
	{

		///////////////////梦成网络重点
		if(m_bBiggest)//赢  
		{			
			if(m_bMingZhu[MeChairID])  //已看牌
			{
				BYTE bUserCount = 0;
				for(BYTE i = 0;i < GAME_PLAYER;i++)
				{
					if(m_cbPlayStatus[i] != true) continue;
					IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
					if(pUserItem == NULL) continue;
					if(!pUserItem->IsAndroidUser())
						bUserCount++;
				}

				if((bUserCount < 1)&&GaiLv(100) < 30)
				{
					//OnAllCompare();
					UINT nElapse=TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);

					return true;
				}
				if(m_bIsYz)        //如果不是单张
				{
					//////////////////////判断牌形
					switch(m_cbCardV)
					{
					case CT_DOUBLE:
						{
							if(m_lUserAddScore>m_MaxLostScore && GaiLv(55))
							{
								//比牌
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
								return true;

							}
							else
							{
								if(GaiLv(55))   //跟注
								{
									//跟注
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
									return true;

								}
								else
								{
									//比牌
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
									return true;
								}

							}

						}
					case CT_SHUN_ZI:
						{
							if(m_lUserAddScore>m_MaxLostScore && GaiLv(60))
							{
								//比牌
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
								return true;

							}
							else
							{
								if(GaiLv(20))
								{
									//加注
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_ADD,nElapse);
									return true;
									
								}
								else
								{
									//跟注
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
									return true;
				
								}		
							
							}
						
						}
					case CT_JIN_HUA:
					case CT_SHUN_JIN:
					case CT_BAO_ZI:
						{
						
							if(GaiLv(60))
							{
								//加注
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_ADD,nElapse);
								return true;
							
							}
							else
							{
								//跟注
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
								return true;

							}
	
						}
					}	


					////////////////////////////如果都没有处理就跟注:
					//跟注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
					
				}
				else   //单张牌,牌很小 
				{
					BYTE cbFirstValue=m_GameLogic.GetCardLogicValue(m_cbHandCardData[MeChairID][0]);
					if(cbFirstValue<11 && GaiLv(80)) 
					{
						//放弃,这里是单张牌的,而且没有达到最大上限所有弃牌,这里是杀分模式的
						//放弃
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;					
					}

					///////////////////////////////////////////////这里判断,当前下注总数,如果下注总数比较少就放弃,如果下注量比较大哪就比牌
					if(m_lUserAddScore>m_MaxLostScore)
					{     ////超过最大上限

						//比牌
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
						return true;

					}
					//放弃百分之80
					if(m_lUserAddScore<m_MinLostScore && GaiLv(60))
					{    
						//放弃,这里是单张牌的,而且没有达到最大上限所有弃牌,这里是杀分模式的
						//放弃
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;
					}
					else
					{
						//比牌或跟注
						if(m_lUserAddScore>m_MinLostScore && m_lUserAddScore<m_MaxLostScore && GaiLv(55))
						{    
							//跟注
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
							return true;
						}
						else
						{
							//比牌
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
							return true;
						}
					}				
				}

			}
			else   //没有看牌 赢
			{
				BYTE bUserCount = 0;
				for(BYTE i = 0;i < GAME_PLAYER;i++)
				{
					if(m_cbPlayStatus[i] != true) continue;
					IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
					if(pUserItem == NULL) continue;
					if(!pUserItem->IsAndroidUser())
						bUserCount++;
				}

				if((bUserCount < 1)&&GaiLv(100) < 30)
				{
					//OnAllCompare();
					UINT nElapse=TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);

					return true;
				}


				if(GaiLv(m_bMinZhuIndex))   //看牌机率有点小!
				{
					//看牌
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//跟注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}
			}

		}
		else//牌不是最大的,输
		{
			BYTE bUserCount = 0;
			for(BYTE i = 0;i < GAME_PLAYER;i++)
			{
				if(m_cbPlayStatus[i] != true) continue;
				IServerUserItem* pUserItem = m_pIAndroidUserItem->GetTableUserItem(i);
				if(pUserItem == NULL) continue;
				if(!pUserItem->IsAndroidUser())
					bUserCount++;
			}

			if((bUserCount < 1)&&GaiLv(100) < 60)
			{
				UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
				m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
				//OnAllCompare();
				return true;
			}

			if(m_bMingZhu[MeChairID]) //看过牌
			{			
				if(m_bIsYz)  //不是单张的类型 牌形比较大
				{

					//////////////////////判断牌形
					switch(m_cbCardV)
					{
					case CT_DOUBLE:
					case CT_SHUN_ZI:
						{
							///未达到最小上限跟注
							if(m_lUserAddScore<m_MinLostScore && GaiLv(50))
							{    
								//跟注
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
								return true;
							}
							else
							{
								if(m_lUserAddScore > m_MaxLostScore && GaiLv(30))//超过最大输分，30%的几率放弃
								{  
									//放弃
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
									return true;
								}
								else 
								{
									//超过最大输分 或者 在最大和最小之间有55%的几率比牌
									if((m_lUserAddScore > m_MinLostScore && m_lUserAddScore < m_MaxLostScore && GaiLv(55)) || m_lUserAddScore > m_MaxLostScore)
									{
										//比牌
										UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
										m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
										return true;
									}
									else
									{
										//跟注
										UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
										m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
										return true;
									}
								}

							}

						}
					case CT_JIN_HUA:
					case CT_SHUN_JIN:
					case CT_BAO_ZI:
						{
							//金币没有达到上限跟注
							if(m_lUserAddScore<m_MaxLostScore)
							{    

								//跟注
								UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
								m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
								return true;

							}
							else
							{
								///比牌或跟注
								if(m_lUserAddScore>m_MaxLostScore && GaiLv(80))
								{  

									//比牌
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
									return true;
								}
								else
								{
									//跟注
									UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
									m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
									return true;
								}
							}

						}
					}	

				}
				else   //单张牌的,直接放弃
				{
					BYTE cbFirstValue=m_GameLogic.GetCardLogicValue(m_cbHandCardData[MeChairID][0]);
					if(cbFirstValue<12) 
					{

						//放弃,这里是单张牌的,而且没有达到最大上限所有弃牌,这里是杀分模式的
						//放弃
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;		
					}


					///////////////////////////////////////////////这里判断,当前下注总数,如果下注总数比较少就放弃,如果下注量比较大哪就比牌
					if(m_lUserAddScore>m_MaxLostScore)
					{    
						////超过最大上限
						//比牌
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
						return true;
					}
					//放弃百分之80
					if(m_lUserAddScore<m_MinLostScore && GaiLv(80))
					{    

						//放弃,这里是单张牌的,而且没有达到最大上限所有弃牌,这里是杀分模式的
						//放弃
						UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
						m_pIAndroidUserItem->SetGameTimer(IDI_GIVEUP,nElapse);
						return true;
					}
					else
					{
						//放弃百分之50
						if(m_lUserAddScore>m_MinLostScore && m_lUserAddScore<m_MaxLostScore && GaiLv(25))
						{    
							//跟注
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
							return true;
						}
						else
						{
							////////判断单牌最大的是什么牌,Q以下就弃牌

							//比牌
							UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
							m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE,nElapse);
							return true;
						}
					}
				}
			}
			else   //没有看过牌的
			{
				if(GaiLv(m_bMinZhuIndex))   //看牌机率有点小!
				{
					//看牌
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_SEE,nElapse+2);
					return true;
				}
				else
				{
					//跟注
					UINT nElapse=rand()%(TIME_RAND_TIME)+TIME_LESS_TIME;
					m_pIAndroidUserItem->SetGameTimer(IDI_FALLOW,nElapse);
					return true;
				}

			}
		}
	}
	return true;
}
//比牌
bool CAndroidUserItemSink::OnAllCompare()
{
	m_bCompareState=0;
	//比牌
	UINT nElapse=TIME_LESS_TIME;
	m_pIAndroidUserItem->SetGameTimer(IDI_COMPARE_OVER,nElapse+1);
	return true;
}

bool CAndroidUserItemSink::GaiLv(BYTE bNum)
{
	if (rand()%100<bNum)
	{
		return true;
	}
	return false;
}
bool CAndroidUserItemSink::IsCanYa()
{
	//获取牌形
	m_cbCardV =m_GameLogic.GetCardType(m_cbHandCardData[m_pIAndroidUserItem->GetChairID()],3);
	//判断是否单张
	if(m_cbCardV>1&&m_cbCardV!=7) m_bIsYz = true;
	return m_bIsYz;
}

//清除全部计时器
void CAndroidUserItemSink::KillAllTimer()
{	
	m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE_OVER);
	m_pIAndroidUserItem->KillGameTimer(IDI_FALLOW);
	m_pIAndroidUserItem->KillGameTimer(IDI_START_GAME);
	m_pIAndroidUserItem->KillGameTimer(IDI_ADD);
	m_pIAndroidUserItem->KillGameTimer(IDI_SEE);
	m_pIAndroidUserItem->KillGameTimer(IDI_FALLOW);
	m_pIAndroidUserItem->KillGameTimer(IDI_COMPARE);
	m_pIAndroidUserItem->KillGameTimer(IDI_GIVEUP);
	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN);
}


//读取配置
void CAndroidUserItemSink::ReadConfigInformation()
{
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//读取配置
	TCHAR szConfigFileName[MAX_PATH]=TEXT("");
	_sntprintf(szConfigFileName,sizeof(szConfigFileName),TEXT("%s\\ZaJinHuaConfig.ini"),szPath);
	//分数限制
	m_lRobotScoreRange[0] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMin"), 50, szConfigFileName);
	m_lRobotScoreRange[1] = GetPrivateProfileInt(m_szRoomName, _T("RobotScoreMax"), 1000, szConfigFileName);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//提款数额
	m_lRobotBankGetScore = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGet"), 100, szConfigFileName);

	//提款数额 
	m_lRobotBankGetScoreBanker = GetPrivateProfileInt(m_szRoomName, _T("RobotBankGetBanker"), 500, szConfigFileName);

	//存款倍数
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 50, szConfigFileName);

	///看牌机率控制
	m_MinLostScore= GetPrivateProfileInt(m_szRoomName, _T("MinLostScore"), 0, szConfigFileName);							//输分下限 低于这个值,看牌机率比较少
	m_MaxLostScore= GetPrivateProfileInt(m_szRoomName, _T("MaxLostScore"), 0, szConfigFileName);							//输分上限 超过这个值,看牌机率非常大
	m_MinLostIndex= GetPrivateProfileInt(m_szRoomName, _T("MinLostIndex"), 0, szConfigFileName);							//输分下限 低于这个值,看牌机率比较少
	m_MaxLostIndex= GetPrivateProfileInt(m_szRoomName, _T("MaxLostIndex"), 0, szConfigFileName);							//输分上限 超过这个值,看牌机率非常大
}

//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
