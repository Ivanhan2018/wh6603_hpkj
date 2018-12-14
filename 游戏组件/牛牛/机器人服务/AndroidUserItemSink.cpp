#include "Stdafx.h"
#include "AndroidUserItemSink.h"

//////////////////////////////////////////////////////////////////////////

//辅助时间
#define TIME_LESS					2									//最少时间

//游戏时间
#define TIME_USER_CALL_BANKER		1									//叫庄定时器
#define TIME_USER_START_GAME		1									//开始定时器
#define TIME_USER_ADD_SCORE			1									//下注定时器
#define TIME_USER_OPEN_CARD			1									//摊牌定时器

//游戏时间
#define IDI_START_GAME			(100)			//开始时间
#define IDI_CALL_BANKER			(101)			//叫庄时间
#define IDI_USER_ADD_SCORE		(102)			//下注时间
#define IDI_OPEN_CARD			(103)			//开牌时间

//////////////////////////////////////////////////////////////////////////

//构造函数
CAndroidUserItemSink::CAndroidUserItemSink()
{
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	//接口变量
	m_pIAndroidUserItem=NULL;
	srand((unsigned)time(NULL));   
	
	return;
}

//析构函数
CAndroidUserItemSink::~CAndroidUserItemSink()
{
}

//接口查询
void * CAndroidUserItemSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IAndroidUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAndroidUserItemSink,Guid,dwQueryVer);
	return NULL;
}

//初始接口
bool CAndroidUserItemSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//查询接口
	m_pIAndroidUserItem=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IAndroidUserItem);
	if (m_pIAndroidUserItem==NULL) return false;

	return true;
}

//重置接口
bool CAndroidUserItemSink::RepositionSink()
{
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));

	return true;
}

//时间消息
bool CAndroidUserItemSink::OnEventTimer(UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始定时器
		{			
			//发送准备
			m_pIAndroidUserItem->SendUserReady(NULL,0);

			return true;
		}
	case IDI_CALL_BANKER:		//叫庄定时
		{
			//设置变量
			CMD_C_CallBanker CallBanker;
			CallBanker.bBanker = rand()%2;

			//发送信息
			m_pIAndroidUserItem->SendSocketData(SUB_C_CALL_BANKER,&CallBanker,sizeof(CallBanker));

			return true;
		}
	case IDI_USER_ADD_SCORE:	//加注定时
		{
			//可下注筹码
		//	LONGLONG lUserMaxScore[GAME_PLAYER];
		//	ZeroMemory(lUserMaxScore,sizeof(lUserMaxScore));
		//	LONGLONG lTemp=m_lTurnMaxScore;
		/*	for (WORD i=0;i<GAME_PLAYER;i++)
			{
				if ( lTemp > 0 )
					lUserMaxScore[i] = __max(lTemp/((i+1)),1L);
				else
					lUserMaxScore[i] = 1;
			}*/


			int num = rand()%10+5;
			
			SCORE m_score = m_lCellScore*num;

			if(m_score>m_lTurnMaxScore)
				m_score = m_lCellScore;

			//发送消息
			CMD_C_AddScore AddScore;
			AddScore.lScore=m_score;
			m_pIAndroidUserItem->SendSocketData(SUB_C_ADD_SCORE,&AddScore,sizeof(AddScore));

			return true;	
		}
	case IDI_OPEN_CARD:			//开牌定时
		{
			//发送消息
			CMD_C_OxCard OxCard;
			OxCard.bOX=(m_GameLogic.GetCardType(m_HandCardData,MAX_COUNT)>0)?TRUE:FALSE;
			m_pIAndroidUserItem->SendSocketData(SUB_C_OPEN_CARD,&OxCard,sizeof(OxCard));

			BankOperate();

			return true;	
		}

	}

	return false;
}

//游戏消息
bool CAndroidUserItemSink::OnEventGameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_CALL_BANKER:	//用户叫庄
		{
			//消息处理
			return OnSubCallBanker(pData,wDataSize);
		}
	case SUB_S_GAME_START:	//游戏开始
		{
			//消息处理
			return OnSubGameStart(pData,wDataSize);
		}
	case SUB_S_ADD_SCORE:	//用户下注
		{
			//消息处理
			return OnSubAddScore(pData,wDataSize);
		}
	case SUB_S_SEND_CARD:	//发牌消息
		{
			//消息处理
			return OnSubSendCard(pData,wDataSize);
		}
	case SUB_S_OPEN_CARD:	//用户摊牌
		{
			//消息处理
			return OnSubOpenCard(pData,wDataSize);
		}
	case SUB_S_PLAYER_EXIT:	//用户强退
		{
			//消息处理
			return OnSubPlayerExit(pData,wDataSize);
		}
	case SUB_S_GAME_END:	//游戏结束
		{
			//消息处理
			return OnSubGameEnd(pData,wDataSize);
		}
	
	}

	//错误断言
//	ASSERT(FALSE);

	return true;
}

//游戏消息
bool CAndroidUserItemSink::OnEventFrameMessage(WORD wSubCmdID, void * pData, WORD wDataSize)
{
	return true;
}

//场景消息
bool CAndroidUserItemSink::OnEventSceneMessage(BYTE cbGameStatus, bool bLookonOther, void * pData, WORD wDataSize)
{
	switch (cbGameStatus)
	{
	case GAME_STATUS_FREE:		//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;

			//消息处理
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pData;

			memcpy(m_szRoomName, pStatusFree->szGameRoomName, sizeof(m_szRoomName));
			
			m_lCellScore=pStatusFree->lCellScore;
			

			ReadConfigInformation();

			//开始时间
			UINT nElapse=rand()%(5)+3;
			m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

			return true;
		}
	case GS_TK_CALL:	// 叫庄状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusCall)) return false;
			CMD_S_StatusCall * pStatusCall=(CMD_S_StatusCall *)pData;

			memcpy(m_szRoomName, pStatusCall->szGameRoomName, sizeof(m_szRoomName));

			m_lCellScore=pStatusCall->lCellScore;
			
			ReadConfigInformation();

			//始叫用户
			if(pStatusCall->wCallBanker==m_pIAndroidUserItem->GetChairID())
			{
				//叫庄时间
				UINT nElapse=rand()%(2)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
			}

			return true;
		}
	case GS_TK_SCORE:	//下注状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusScore)) return false;
			CMD_S_StatusScore * pStatusScore=(CMD_S_StatusScore *)pData;

			memcpy(m_szRoomName, pStatusScore->szGameRoomName, sizeof(m_szRoomName));

			m_lCellScore=pStatusScore->lCellScore;

			ReadConfigInformation();

			//设置变量
			m_lTurnMaxScore=pStatusScore->lTurnMaxScore;
			WORD wMeChairId = m_pIAndroidUserItem->GetChairID();

			//设置筹码
			if (pStatusScore->lTurnMaxScore>0L && pStatusScore->lTableScore[wMeChairId]==0L)
			{
				//下注时间
				UINT nElapse=rand()%(2)+TIME_LESS;
				m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
			}

			return true;
		}
	case GS_TK_PLAYING:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pData;

			memcpy(m_szRoomName, pStatusPlay->szGameRoomName, sizeof(m_szRoomName));

			m_lCellScore=pStatusPlay->lCellScore;

			ReadConfigInformation();

			//设置变量
			m_lTurnMaxScore=pStatusPlay->lTurnMaxScore;
			WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
			CopyMemory(m_HandCardData,pStatusPlay->cbHandCardData[wMeChiarID],MAX_COUNT);

			//控件处理
			if(pStatusPlay->bOxCard[wMeChiarID]==0xff)
			{
				//开牌时间
				UINT nElapse=rand()%(6)+TIME_LESS*2;
				m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);
			}

			return true;
		}
	}

	ASSERT(FALSE);

	return false;
}

//用户进入
void CAndroidUserItemSink::OnEventUserEnter(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户离开
void CAndroidUserItemSink::OnEventUserLeave(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户积分
void CAndroidUserItemSink::OnEventUserScore(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户状态
void CAndroidUserItemSink::OnEventUserStatus(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户段位
void CAndroidUserItemSink::OnEventUserSegment(IAndroidUserItem * pIAndroidUserItem, bool bLookonUser)
{
	return;
}

//用户叫庄
bool CAndroidUserItemSink::OnSubCallBanker(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	if (wDataSize!=sizeof(CMD_S_CallBanker)) return false;
	CMD_S_CallBanker * pCallBanker=(CMD_S_CallBanker *)pBuffer;

	//始叫用户
	if(pCallBanker->wCallBanker==m_pIAndroidUserItem->GetChairID())
	{
		//叫庄时间
		UINT nElapse=rand()%(2)+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_CALL_BANKER,nElapse);
	}

	return true;
}

//游戏开始
bool CAndroidUserItemSink::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	//设置变量
	m_lTurnMaxScore=pGameStart->lTurnMaxScore;

	//设置筹码
	if (pGameStart->lTurnMaxScore>0)
	{
		//下注时间
		UINT nElapse=rand()%(2)+TIME_LESS;
		m_pIAndroidUserItem->SetGameTimer(IDI_USER_ADD_SCORE,nElapse);
	}

	return true;
}

//用户下注
bool CAndroidUserItemSink::OnSubAddScore(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_AddScore)) return false;
	CMD_S_AddScore * pAddScore=(CMD_S_AddScore *)pBuffer;

	return true;
}

//发牌消息
bool CAndroidUserItemSink::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{

	//效验数据
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//设置数据
	WORD wMeChiarID=m_pIAndroidUserItem->GetChairID();
	CopyMemory(m_HandCardData,pSendCard->cbCardData[wMeChiarID],sizeof(m_HandCardData));

	//开牌时间
	UINT nElapse=rand()%(6)+TIME_LESS*2;
	m_pIAndroidUserItem->SetGameTimer(IDI_OPEN_CARD,nElapse);

	return true;
}

//用户摊牌
bool CAndroidUserItemSink::OnSubOpenCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_Open_Card)) return false;
	CMD_S_Open_Card * pOpenCard=(CMD_S_Open_Card *)pBuffer;

	return true;
}

//用户强退
bool CAndroidUserItemSink::OnSubPlayerExit(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	if (wDataSize!=sizeof(CMD_S_PlayerExit)) return false;
	CMD_S_PlayerExit * pPlayerExit=(CMD_S_PlayerExit *)pBuffer;

	return true;
}

//游戏结束
bool CAndroidUserItemSink::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	BankOperate();

	//删除定时器
	m_pIAndroidUserItem->KillGameTimer(IDI_CALL_BANKER);
	m_pIAndroidUserItem->KillGameTimer(IDI_USER_ADD_SCORE);
	m_pIAndroidUserItem->KillGameTimer(IDI_OPEN_CARD);

	//开始时间
	UINT nElapse=rand()%(2)+TIME_LESS;
	m_pIAndroidUserItem->SetGameTimer(IDI_START_GAME,nElapse);

	//清理变量
	m_lTurnMaxScore = 0;
	ZeroMemory(m_HandCardData,sizeof(m_HandCardData));


	return true;
}

//银行操作
void CAndroidUserItemSink::BankOperate()
{
	//变量定义
	IServerUserItem *pUserItem = m_pIAndroidUserItem->GetMeUserItem();
	LONGLONG lRobotScore = pUserItem->GetUserScore();
	{
		//判断存取
		if (lRobotScore > m_lRobotScoreRange[1])
		{
			
			
			LONGLONG lSaveScore=0L;

			lSaveScore = LONGLONG(lRobotScore*m_nRobotBankStorageMul/100);
			if (lSaveScore > lRobotScore)  lSaveScore = lRobotScore;

			if (lSaveScore > 0)
				m_pIAndroidUserItem->PerformSaveScore(lSaveScore);

		/*	LONGLONG lRobotNewScore = pUserItem->GetUserScore();

			CString strInfo;
			strInfo.Format(TEXT("[%s] 执行存款：存款前金币(%I64d)，存款后金币(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

			NcaTextOut(strInfo);*/

		}
		else if (lRobotScore < m_lRobotScoreRange[0])
		{
			
			SCORE lScore=rand()%m_lRobotBankGetScoreBanker+m_lRobotBankGetScore;
			if (lScore > 0)
				m_pIAndroidUserItem->PerformTakeScore(lScore);

			
			/*LONGLONG lRobotNewScore = pUserItem->GetUserScore();

			CString strInfo;
			strInfo.Format(TEXT("[%s] 执行取款：取款前金币(%I64d)，取款后金币(%I64d)"),pUserItem->GetNickName(),lRobotScore,lRobotNewScore);

			NcaTextOut(strInfo);*/
					
		}
	}
}

//读取配置
void CAndroidUserItemSink::ReadConfigInformation()
{
	//设置文件名
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	_sntprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\OxServer.ini"), szPath);

	//分数限制
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMin"), _T("1000"), OutBuf, 255, szConfigFileName);
	_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[0]);

	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotScoreMax"), _T("10000"), OutBuf, 255, szConfigFileName);
	_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &m_lRobotScoreRange[1]);

	if (m_lRobotScoreRange[1] < m_lRobotScoreRange[0])	m_lRobotScoreRange[1] = m_lRobotScoreRange[0];

	//提款数额
	
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGet"), _T("200"), OutBuf, 255, szConfigFileName);
	_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScore);

	//提款数额 (庄家)
	ZeroMemory(OutBuf, sizeof(OutBuf));
	GetPrivateProfileString(m_szRoomName, TEXT("RobotBankGetBanker"), _T("30000000"), OutBuf, 255, szConfigFileName);
	_snwscanf(OutBuf, lstrlen(OutBuf), _T("%I64d"), &m_lRobotBankGetScoreBanker);

	//存款倍数
	m_nRobotBankStorageMul = GetPrivateProfileInt(m_szRoomName, _T("RobotBankStoMul"), 10, szConfigFileName);

	if (m_nRobotBankStorageMul<0||m_nRobotBankStorageMul>100) m_nRobotBankStorageMul =20;
}


//组件创建函数
DECLARE_CREATE_MODULE(AndroidUserItemSink);

//////////////////////////////////////////////////////////////////////////
