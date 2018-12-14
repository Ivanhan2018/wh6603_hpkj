#include "StdAfx.h"
#include "TableFrameSink.h"

//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数

SCORE			CTableFrameSink::m_lStockScore = 0.0;
LONGLONG		CTableFrameSink::m_lStorageOff = 0L;
BYTE			CTableFrameSink::StorageStyle=1;
SCORE			CTableFrameSink::m_lAndroidWinAndLose = 0.0;
//////////////////////////////////////////////////////////////////////////

#define	IDI_SO_OPERATE							12							//代打定时器
#define	TIME_SO_OPERATE							80000						//代打定时器

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏记录
	m_GameRecord.Empty();
	ZeroMemory(m_szNickName,sizeof(m_szNickName));

	//游戏变量	
	m_lExitScore=0;	
	m_wBankerUser=INVALID_CHAIR;
	m_wFisrtCallUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	//用户状态
	ZeroMemory(m_cbDynamicJoin,sizeof(m_cbDynamicJoin));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_bOxCard[i]=0xff;

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//下注信息
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;

		//控制信息
	m_dwCheatCount=0;
	m_dwCheatGameID=0;
	m_cbCheatType=CHEAT_TYPE_WIN;
	
	m_lStockScore = 0.00;
	m_lAndroidWinAndLose=0.0;
	m_lStorageOff = 0L;
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;

	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{
}

//接口查询--检测相关信息版本
VOID *  CTableFrameSink::QueryInterface(const IID & Guid, DWORD dwQueryVer)
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
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//开始模式
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//读取配置
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\OxServer.ini"),szPath);
	//开始存储数目
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

		//读取库存
		///////////////////读取小数转换 梦成网络修正
		TCHAR OutBuf[255];
		memset(OutBuf,0,255);
		if(m_lStockScore > -0.0000001 && m_lStockScore < 0.000000001)
		{
			GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,m_szFileName);

			CString TempValue;
			myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
			m_lStockScore=Mywtof(TempValue);
			m_lAndroidWinAndLose = m_lStockScore*2;
		}
		///////////////////////////////

	//读取衰减值
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),10,m_szFileName);

	//设置最大衰减值
	if( m_lStorageDeduct < 0 || m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000 )
		m_lStorageDeduct = 1000;

	//梦成网络添加
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
		if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);
	//梦成网络添加
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;

	ReadConfigInformation(true);
	return true;
}

//复位桌子
VOID  CTableFrameSink::RepositionSink()
{
		//游戏记录
	m_GameRecord.Empty();

	//游戏变量
	m_lExitScore=0;	
	m_wCurrentUser=INVALID_CHAIR;

	//用户状态
	ZeroMemory(m_cbDynamicJoin,sizeof(m_cbDynamicJoin));
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	ZeroMemory(m_bCallStatus,sizeof(m_bCallStatus));
	for(BYTE i=0;i<m_wPlayerCount;i++)m_bOxCard[i]=0xff;

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//下注信息
	ZeroMemory(m_lTurnMaxScore,sizeof(m_lTurnMaxScore));

	return;
}


//用户坐下
bool  CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false) m_HistoryScore.OnEventUserEnter(pIServerUserItem->GetChairID());

	if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
		m_cbDynamicJoin[pIServerUserItem->GetChairID()]=true;

	return true;
}

//用户起立
bool  CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//历史积分
	if (bLookonUser==false) 
	{
		m_HistoryScore.OnEventUserLeave(pIServerUserItem->GetChairID());
		m_cbDynamicJoin[pIServerUserItem->GetChairID()]=false;
		ZeroMemory(m_szNickName[pIServerUserItem->GetChairID()],sizeof(m_szNickName[pIServerUserItem->GetChairID()]));
	}

	return true;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{

	//读取配置
	ReadConfigInformation(true);
	//设置状态
	m_pITableFrame->SetGameStatus(GS_TK_CALL);

	//用户状态
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);

		if(pIServerUser==NULL)
		{
			m_cbPlayStatus[i]=FALSE;
		}
		else
		{
			m_cbPlayStatus[i]=TRUE;
			//复制用户昵称
			_sntprintf(m_szNickName[i],CountArray(m_szNickName[i]),TEXT("%s"),pIServerUser->GetNickName());
		}
	}

	//首局随机始叫
	if(m_wFisrtCallUser==INVALID_CHAIR)
	{
		m_wFisrtCallUser=rand()%m_wPlayerCount;  //随机用户
	}
	else
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	//始叫用户
	while(m_cbPlayStatus[m_wFisrtCallUser]!=TRUE)
	{
		m_wFisrtCallUser=(m_wFisrtCallUser+1)%m_wPlayerCount;
	}

	////叫庄用户
	//当前用户
	m_wCurrentUser=m_wFisrtCallUser;

	//设置变量
	CMD_S_CallBanker CallBanker;
	CallBanker.wCallBanker=m_wCurrentUser;
	CallBanker.bFirstTimes=true;

	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]!=TRUE) continue;
		m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));



	m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);


	BYTE bTempArray[GAME_PLAYER*MAX_COUNT];
	m_GameLogic.RandCardList(bTempArray,sizeof(bTempArray));

	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);	
		if(pIServerUser==NULL)continue;
		//派发扑克
		CopyMemory(m_cbHandCardData[i],&bTempArray[i*MAX_COUNT],MAX_COUNT);
	}


			
	//下方的是控制相关的
	////////////////////////////////////////////////////////
	//判断是否控制指定玩家
	//WORD wChairIDCtrl = INVALID_CHAIR;

	//if(m_dwCheatCount>0)  //控制次数
	//{
	//	m_bWinnerCtrl = true;  //如果控制次数大于0，指定玩家输赢控制开启

	//	if(m_dwCheatGameID==0)  //如果玩家ID等于0 指定玩家输赢控制关闭
	//	{
	//		m_bWinnerCtrl=false;  
	//	}
	//	else   //玩家ID大于0
	//	{
	//		//返回ID
	//		wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);  //获取玩家椅子号

	//		//指定玩家不在
	//		if(wChairIDCtrl==INVALID_CHAIR)
	//		{
	//			m_bWinnerCtrl=false;
	//		}
	//	}
	//}else{
	//	//控制次数少于0
	//	m_bWinnerCtrl=false;
	//}





//////////////////////////////////////////结束指定玩家控制

	////是否控制玩家
	//if (m_bWinnerCtrl)  //指定控制优先第一
	//{
	//	OutputDebugStringA("hhh 111");
	//	//m_cbCheatType胜利类型 1为胜利0为输
	//	ProbeJudge(wChairIDCtrl,m_cbCheatType);

	//}
	//else if (m_bAndroidWinningCtrl)//机器人赢率控制第二
	//{
	//	OutputDebugStringA("hhh 222");
	//	//m_bCtrlThisTime = true;
	//	AndroidWinningControl();  //机器人换牌机率
	//	

	//}
	//else  //库存控制优先级别排第三 也是最低
	//{
	//	OutputDebugStringA("hhh 333");
	//	//m_bCtrlThisTime = false;
	//	
	//	//机器人作弊
	//	AndroidCheck();
	//	
	//}



	//////////////结束控制
	return true;
}

//游戏结束
bool  CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_NORMAL:		//常规结束
		{
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			WORD m_wWinTimes[GAME_PLAYER],m_wWinCount[GAME_PLAYER];
			ZeroMemory(m_wWinCount,sizeof(m_wWinCount));
			ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

			//保存扑克
			BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
			CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

			//庄家倍数
			ASSERT(m_bOxCard[m_wBankerUser]<2);
			if(m_bOxCard[m_wBankerUser]==TRUE)
				m_wWinTimes[m_wBankerUser]=m_GameLogic.GetTimes(cbUserCardData[m_wBankerUser],MAX_COUNT);
			else m_wWinTimes[m_wBankerUser]=1;

			//对比玩家
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;

				ASSERT(m_bOxCard[i]<2);

				//对比扑克
				if (m_GameLogic.CompareCard(cbUserCardData[i],cbUserCardData[m_wBankerUser],MAX_COUNT,m_bOxCard[i],m_bOxCard[m_wBankerUser])) 
				{
					m_wWinCount[i]++;
					//获取倍数
					if(m_bOxCard[i]==TRUE)
						m_wWinTimes[i]=m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT);
					else m_wWinTimes[i]=1;
				}
				else
				{
					m_wWinCount[m_wBankerUser]++;
				}
			}

			//统计得分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;

				WORD j=i;
				if(m_wWinCount[j]>0)	//闲家胜利
				{
					GameEnd.lGameScore[j]=Double_Round(m_lTableScore[j]*m_wWinTimes[j],2,1);
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[j];
					m_lTableScore[j]=0.;
				}
				else					//庄家胜利
				{
					GameEnd.lGameScore[j]=Double_Round(-m_lTableScore[j]*m_wWinTimes[m_wBankerUser],2,1);
					GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[j];
					m_lTableScore[j]=0.;
				}
			}

			//闲家强退分数	
			GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

			//离开用户
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_lTableScore[i]>COMPARE_ZERO)GameEnd.lGameScore[i]=Double_Round(-m_lTableScore[i],2,1);
			}

			//修改积分
			tagScoreInfo ScoreInfoArray[GAME_PLAYER];
			ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				//税收
				if(( GAME_GENRE_SCORE & m_pGameServiceOption->wServerType )==0)
				{
					if (GameEnd.lGameScore[i]>COMPARE_ZERO)
					{
						GameEnd.lGameTax[i] =  m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]);
						GameEnd.lGameScore[i] -=GameEnd.lGameTax[i] ;
					}
				}
				ScoreInfoArray[i].lRevenue = GameEnd.lGameTax[i] ;
				ScoreInfoArray[i].lScore=GameEnd.lGameScore[i]; 
				ScoreInfoArray[i].cbType = (GameEnd.lGameScore[i]>COMPARE_ZERO)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				
				//游戏记录
				m_GameRecord.AppendFormat(TEXT("%s得分:%0.2f,%0.2f#"),m_szNickName[i],ScoreInfoArray[i].lScore, ScoreInfoArray[i].lRevenue);

				m_HistoryScore.OnEventUserScore(i,GameEnd.lGameScore[i]);
			}

			m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

			//发送信息
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE) continue;
				m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			//统计结束写分操作结束
			/////////////////////////////////////////////////////////
			////统计库存信息 只有存在机器人并且存在真实用户的情况下才会计算
			bool  bUserAI = false;  //是否存在机器人
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//存在机器人
					bUserAI = true;
					//跳出FOR
					break;

				}
			}
			//统计信息与计算库存
			SCORE lSystemScore=0.0;    //玩家得分
			SCORE lCurrentStorgeNum = m_lStockScore; //当前库存值
			SCORE RevenueScore = 0.0;	 //税收
			SCORE lStorageDeduct=0.0;	//库存衰减
			//库存统计 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//如果用户不存在
				if (pIServerUserIte==NULL)continue;					
				//存在机器人才执行
				if(bUserAI){
					//如果用户不是机器人
					if(!pIServerUserIte->IsAndroidUser())
					{
						//计算税收
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//计算库存衰减
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//系统库存值
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
						//系统得分
						lSystemScore-=ScoreInfoArray[i].lScore;
						m_lAndroidWinAndLose += abs(ScoreInfoArray[i].lScore);
					}
				}
			}

			////////////////////////////////////////////////////////////

			//判断是否存在真实玩家
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//存在真实用户
					bUser = true;
					//跳出FOR
					break;

				}
			}

			//判断是否存在真实玩家
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息


				//梦成网络添加
				m_lStockLimit+=lSystemScore;
				
				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//如果存在控制用户的话//发送游戏信息给控制号
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue;	//如果用户不存在
					if(pUserItem->IsAndroidUser()) continue;  //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //发送消息窗口
				}
			}

			//////////////////////////////////////////////////////
			//扣除库存
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//如果扣减数不等于0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("人为操作库存减少%I64d，剩余库存：%0.2f"),m_lStorageOff, m_lStockScore);
				//输出信息
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//设置完之后把该值变成0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//发送控制号窗口提示
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue; //如果用户不存在
					if(pUserItem->IsAndroidUser()) continue; //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //发送消息窗口

				}
			}

			//库存数据处理结束

			/////////////////////////////////////////////////
			

			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case  GER_NETWORK_ERROR:
		{
			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount && (m_cbPlayStatus[wChairID]==TRUE||m_cbDynamicJoin[wChairID]==FALSE));

			if(m_cbPlayStatus[wChairID]==FALSE||m_cbDynamicJoin[wChairID]==TRUE) return true;
			//设置状态
			m_cbPlayStatus[wChairID]=FALSE;
			m_cbDynamicJoin[wChairID]=FALSE;

			m_GameRecord.AppendFormat(TEXT("%s离开#"),m_szNickName[wChairID]);

			//定义变量
			CMD_S_PlayerExit PlayerExit;
			PlayerExit.wPlayerID=wChairID;

			//发送信息
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(i==wChairID || (m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)) continue;
				m_pITableFrame->SendTableData(i,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));
			}
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_PLAYER_EXIT,&PlayerExit,sizeof(PlayerExit));

			WORD m_wWinTimes[GAME_PLAYER];
			if (m_pITableFrame->GetGameStatus()>GS_TK_CALL)   //游戏已经开始
			{
				//定义保存金币变量
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));

				if (wChairID==m_wBankerUser)	//庄家强退
				{
					//定义变量
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));
					ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

					BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
					CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

					//得分倍数
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;
						m_wWinTimes[i]=(m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING)?(1):(m_GameLogic.GetTimes(cbUserCardData[i],MAX_COUNT));
					}

					//统计得分 已下或没下
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || m_cbPlayStatus[i]==FALSE)continue;
						GameEnd.lGameScore[i]=Double_Round(m_lTableScore[i]*m_wWinTimes[i],2,1);
						GameEnd.lGameScore[m_wBankerUser]-=GameEnd.lGameScore[i];
						m_lTableScore[i]=SCORE_ZERO;
					}

					//闲家强退分数 
					GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;

					//离开用户
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>0.001)GameEnd.lGameScore[i]=Double_Round(-m_lTableScore[i],2,1);
					}


					//积分税收
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(( GAME_GENRE_SCORE & m_pGameServiceOption->wServerType )==0)
						{
							if (GameEnd.lGameScore[i]>0.001)
							{
								GameEnd.lGameTax[i] = m_pITableFrame->CalculateRevenue(i,GameEnd.lGameScore[i]); 
								GameEnd.lGameScore[i] -=GameEnd.lGameTax[i] ;
							}
						}
					}

				

					//发送信息
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(i==m_wBankerUser || (m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE))continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					
					//修改积分
					tagScoreInfo ScoreInfo[GAME_PLAYER];
					ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbPlayStatus[i]!=TRUE && i!=m_wBankerUser) continue;
						ScoreInfo[i].lRevenue=GameEnd.lGameTax[i];
						ScoreInfo[i].lScore=GameEnd.lGameScore[i];
		
						if(GameEnd.lGameScore[i]>0.001)
							ScoreInfo[i].cbType=SCORE_TYPE_WIN;
						else
							ScoreInfo[i].cbType=SCORE_TYPE_LOSE;
		
						//游戏记录
						m_GameRecord.AppendFormat(TEXT("%s得分:%0.2f,%0.2f#"),m_szNickName[i],ScoreInfo[i].lScore, ScoreInfo[i].lRevenue);
					}
					
					m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

					
					//结束游戏
					m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
				}
				else						//闲家强退
				{

					//已经下注
					if (m_lTableScore[wChairID]>COMPARE_ZERO)
					{
						ZeroMemory(m_wWinTimes,sizeof(m_wWinTimes));

						//用户扑克
						BYTE cbUserCardData[MAX_COUNT];
						CopyMemory(cbUserCardData,m_cbHandCardData[m_wBankerUser],MAX_COUNT);

						//用户倍数
						m_wWinTimes[m_wBankerUser]=(m_pITableFrame->GetGameStatus()==GS_TK_SCORE)?(1):(m_GameLogic.GetTimes(cbUserCardData,MAX_COUNT));
					
						//修改积分
						SCORE lScore=Double_Round(-m_lTableScore[wChairID]*m_wWinTimes[m_wBankerUser],2,1);
						m_lExitScore+=(-lScore);
						m_lTableScore[wChairID]=(-lScore);

						ScoreInfoArray[wChairID].lScore = lScore;
						ScoreInfoArray[wChairID].cbType = SCORE_TYPE_LOSE;
						//游戏记录
						m_GameRecord.AppendFormat(TEXT("%s得分:%0.2f,%0.2f#"),m_szNickName[wChairID],ScoreInfoArray[wChairID].lScore, ScoreInfoArray[wChairID].lRevenue);

					}

					//玩家人数
					WORD wUserCount=0;
					for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE)wUserCount++;

					//////////////////////////////////////////////////////////////////
					//结束游戏
					if(wUserCount==1)
					{
						//定义变量
						CMD_S_GameEnd GameEnd;
						ZeroMemory(&GameEnd,sizeof(GameEnd));
						ASSERT(m_lExitScore>-COMPARE_ZERO); 

						//统计得分
						GameEnd.lGameScore[m_wBankerUser]+=m_lExitScore;
						if (GameEnd.lGameScore[m_wBankerUser]>COMPARE_ZERO && (GAME_GENRE_GOLD &m_pGameServiceOption->wServerType)!=0 )
						{
							GameEnd.lGameTax[m_wBankerUser] = m_pITableFrame->CalculateRevenue(m_wBankerUser,GameEnd.lGameScore[m_wBankerUser]); 
							GameEnd.lGameScore[m_wBankerUser] -=GameEnd.lGameTax[m_wBankerUser] ;
						}

						//离开用户
						for (WORD i=0;i<m_wPlayerCount;i++)
						{
							if(m_lTableScore[i]>0.001)GameEnd.lGameScore[i]=Double_Round(-m_lTableScore[i],2,1);
						}

						//发送信息
						m_pITableFrame->SendTableData(m_wBankerUser,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
						m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

						WORD Zero=0;
						for ( Zero=0;Zero<m_wPlayerCount;Zero++)
						{
							if(m_lTableScore[Zero]>COMPARE_ZERO||m_lTableScore[Zero]<-COMPARE_ZERO)
								break;
						}
						if(Zero!=m_wPlayerCount)
						{
							if(m_wBankerUser!=INVALID_CHAIR)
							{
								ScoreInfoArray[m_wBankerUser].lScore =GameEnd.lGameScore[m_wBankerUser];
								ScoreInfoArray[m_wBankerUser].lRevenue = GameEnd.lGameTax[m_wBankerUser];
								ScoreInfoArray[m_wBankerUser].cbType =SCORE_TYPE_WIN;
								m_GameRecord.AppendFormat(TEXT("得分:%0.2f,%0.2f#"),m_szNickName[m_wBankerUser],ScoreInfoArray[m_wBankerUser].lScore, ScoreInfoArray[m_wBankerUser].lRevenue);
							
							}

						}
						m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));

						//结束游戏
						m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);	
					}
					else if	(m_pITableFrame->GetGameStatus()==GS_TK_SCORE )
					{
						if( m_lTableScore[wChairID]<COMPARE_ZERO &&m_lTableScore[wChairID]>-COMPARE_ZERO)
							OnUserAddScore(wChairID,SCORE_ZERO);
						else
							m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
					}
					else if (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING && m_bOxCard[wChairID]==0xff)
					{
						OnUserOpenCard(wChairID,0);
						m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
					}
					//////////////////////////////////////////////////////////////////

				}

			

			//统计结束写分操作结束
			/////////////////////////////////////////////////////////
			////统计库存信息 只有存在机器人并且存在真实用户的情况下才会计算
			bool  bUserAI = false;  //是否存在机器人
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (pIServerUserItemTemp->IsAndroidUser()){
					//存在机器人
					bUserAI = true;
					//跳出FOR
					break;

				}
			}
			//统计信息与计算库存
			SCORE lSystemScore=0.0;    //玩家得分
			SCORE lCurrentStorgeNum = m_lStockScore; //当前库存值
			SCORE RevenueScore = 0.0;	 //税收
			SCORE lStorageDeduct=0.0;	//库存衰减
			//库存统计 
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//如果用户不存在
				if (pIServerUserIte==NULL)continue;					
				//存在机器人才执行
				if(bUserAI){
					//如果用户不是机器人
					if(!pIServerUserIte->IsAndroidUser())
					{
						//计算税收
						if(ScoreInfoArray[i].lScore>0) RevenueScore = Double_Round(ScoreInfoArray[i].lRevenue,2,1);
						//计算库存衰减
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//系统库存值
						m_lStockScore-=(Double_Round(ScoreInfoArray[i].lScore,2,1)+lStorageDeduct); 
						//系统得分
						lSystemScore-=ScoreInfoArray[i].lScore;
						m_lAndroidWinAndLose += abs(ScoreInfoArray[i].lScore);
					}
				}
			}

			////////////////////////////////////////////////////////////

			//判断是否存在真实玩家
			bool bUser = false;
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				IServerUserItem * pIServerUserItemTemp=m_pITableFrame->GetTableUserItem(i);
				if (pIServerUserItemTemp==NULL) continue;
				if (!pIServerUserItemTemp->IsAndroidUser()){
					//存在真实用户
					bUser = true;
					//跳出FOR
					break;

				}
			}

			//判断是否存在真实玩家
			if (bUser)
			{
				
				CString strLog;
				strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息


				//梦成网络添加
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//如果存在控制用户的话//发送游戏信息给控制号
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue;	//如果用户不存在
					if(pUserItem->IsAndroidUser()) continue;  //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue;  //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,strLog,SMT_EJECT);					   //发送消息窗口
				}
			}

			//////////////////////////////////////////////////////
			//扣除库存
			m_lStorageOff=GetPrivateProfileInt(m_pGameServiceOption->szServerName,_T("StorageOff"),0,m_szFileName);
			//如果扣减数不等于0
			if (m_lStorageOff != 0)
			{
				m_lStockScore -= m_lStorageOff;
				TCHAR szStorageMin[1024]=TEXT("");
				_sntprintf(szStorageMin,CountArray(szStorageMin),TEXT("人为操作库存减少%I64d，剩余库存：%0.2f"),m_lStorageOff, m_lStockScore);
				//输出信息
				CTraceService::TraceString(szStorageMin,TraceLevel_Warning);
				//设置完之后把该值变成0
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageOff"),TEXT("0"),m_szFileName);

				//发送控制号窗口提示
				for(int i=0; i<GAME_PLAYER; i++)
				{
					IServerUserItem* pUserItem=m_pITableFrame->GetTableUserItem(i); //获取用户帐号
					if(pUserItem==NULL) continue; //如果用户不存在
					if(pUserItem->IsAndroidUser()) continue; //如果是机器人
					if((CUserRight::IsGameCheatUser(pUserItem->GetUserRight()))==false) continue; //判断用户控制权限
					m_pITableFrame->SendGameMessage(pUserItem,szStorageMin,SMT_EJECT);  //发送消息窗口

				}
			}

			//库存数据处理结束

			/////////////////////////////////////////////////
		




			}
			else    //游戏还没有开始
			{
				//玩家人数
				WORD wUserCount=0;
				for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE) wUserCount++;

				//结束游戏
				if(wUserCount==1)
				{
					//定义变量
					CMD_S_GameEnd GameEnd;
					ZeroMemory(&GameEnd,sizeof(GameEnd));

					//发送信息
					for (WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE) continue;
						m_pITableFrame->SendTableData(i,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					}
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					//结束游戏

					m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);		
				}
				else if(m_wCurrentUser==wChairID) OnUserCallBanker(wChairID,0);
			}

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
	case GAME_STATUS_FREE:		//空闲状态
		{
			//构造数据
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));

			//设置变量
			StatusFree.lCellScore=m_pGameServiceOption->lCellScore;

			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusFree.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusFree.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GS_TK_CALL:	//叫庄状态
		{
			//构造数据
			CMD_S_StatusCall StatusCall;
			ZeroMemory(&StatusCall,sizeof(StatusCall));

			StatusCall.lCellScore=m_pGameServiceOption->lCellScore;
			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusCall.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusCall.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//设置变量
			StatusCall.wCallBanker=m_wCurrentUser;
			StatusCall.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusCall.cbPlayStatus,m_cbPlayStatus,sizeof(StatusCall.cbPlayStatus));

			//房间名称
			CopyMemory(StatusCall.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusCall.szGameRoomName));


			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusCall,sizeof(StatusCall));
		}
	case GS_TK_SCORE:	//下注状态
		{
			//构造数据
			CMD_S_StatusScore StatusScore;
			memset(&StatusScore,0,sizeof(StatusScore));

			StatusScore.lCellScore=m_pGameServiceOption->lCellScore;
			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusScore.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusScore.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//加注信息
			StatusScore.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusScore.wBankerUser=m_wBankerUser;
			StatusScore.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusScore.cbPlayStatus,m_cbPlayStatus,sizeof(StatusScore.cbPlayStatus));

			//设置积分
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				StatusScore.lTableScore[i]=m_lTableScore[i];
			}

				//房间名称
			CopyMemory(StatusScore.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusScore.szGameRoomName));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusScore,sizeof(StatusScore));
		}
	case GS_TK_PLAYING:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			memset(&StatusPlay,0,sizeof(StatusPlay));

			StatusPlay.lCellScore=m_pGameServiceOption->lCellScore;
			//历史积分
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				tagHistoryScore * pHistoryScore=m_HistoryScore.GetHistoryScore(i);
				StatusPlay.lTurnScore[i]=pHistoryScore->lTurnScore;
				StatusPlay.lCollectScore[i]=pHistoryScore->lCollectScore;
			}

			//设置信息
			StatusPlay.lTurnMaxScore=m_lTurnMaxScore[wChiarID];
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.cbDynamicJoin=m_cbDynamicJoin[wChiarID];
			CopyMemory(StatusPlay.bOxCard,m_bOxCard,sizeof(StatusPlay.bOxCard));
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(StatusPlay.cbPlayStatus));

			//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));


			//设置扑克
			for (WORD i=0;i< m_wPlayerCount;i++)
			{
				if(m_cbPlayStatus[i]==FALSE)continue;
				WORD j= i;
				StatusPlay.lTableScore[j]=m_lTableScore[j];
				CopyMemory(StatusPlay.cbHandCardData[j],m_cbHandCardData[j],MAX_COUNT);
			}

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}
	//效验错误
	ASSERT(FALSE);

	return false;
}

//定时器事件
bool  CTableFrameSink::OnTimerMessage(DWORD dwTimerID, WPARAM wBindParam)
{
	switch(dwTimerID)
	{
	case IDI_SO_OPERATE:
		{
			//删除时间
			m_pITableFrame->KillGameTimer(IDI_SO_OPERATE);

			//游戏状态
			switch( m_pITableFrame->GetGameStatus())
			{
			case GS_TK_CALL:			//用户叫庄
				{
					OnUserCallBanker(m_wCurrentUser, 0);
					break;
				}
			case GS_TK_SCORE:			//下注操作
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_lTableScore[i]>COMPARE_ZERO || m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)continue;
						OnUserAddScore(i,m_lTurnMaxScore[i]/8);
					}

					break;
				}
			case GS_TK_PLAYING:			//用户开牌
				{
					for(WORD i=0;i<m_wPlayerCount;i++)
					{
						if(m_bOxCard[i]<2 || m_cbPlayStatus[i]==FALSE)continue;
						OnUserOpenCard(i, 0);
					}

					break;
				}
			default:
				{
					break;
				}
			}

			if(m_pITableFrame->GetGameStatus()!=GS_TK_FREE)
				m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
			return true;
		}
	}

	return false;
}

//游戏消息处理 
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	bool bResult=false;
	switch (wSubCmdID)
	{
	case SUB_C_CALL_BANKER:			//用户叫庄
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CallBanker));
			if (wDataSize!=sizeof(CMD_C_CallBanker)) return false;

			//变量定义
			CMD_C_CallBanker * pCallBanker=(CMD_C_CallBanker *)pDataBuffer;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;


			//消息处理
			bResult=OnUserCallBanker(pUserData->wChairID,pCallBanker->bBanker);
			break;
		}
	case SUB_C_ADD_SCORE:			//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//变量定义
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pDataBuffer;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			bResult=OnUserAddScore(pUserData->wChairID,pAddScore->lScore);
			break;
		}
	case SUB_C_OPEN_CARD:			//用户摊牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_OxCard));
			if (wDataSize!=sizeof(CMD_C_OxCard)) return false;

			//变量定义
			CMD_C_OxCard * pOxCard=(CMD_C_OxCard *)pDataBuffer;

			//用户效验
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//消息处理
			bResult=OnUserOpenCard(pUserData->wChairID,pOxCard->bOX);
			break;
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			//if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
			return OnSubAmdinCommand(pIServerUserItem,pDataBuffer);
		}
	}

	//操作定时器
	if(bResult)
	{
		m_pITableFrame->SetGameTimer(IDI_SO_OPERATE,TIME_SO_OPERATE,1,0);
		return true;
	}

	return false;
}

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}


//叫庄事件
bool CTableFrameSink::OnUserCallBanker(WORD wChairID, BYTE bBanker)
{
	//状态效验
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_CALL);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_CALL) return true;
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return true;

	//设置变量
	m_bCallStatus[wChairID]=TRUE;

	//叫庄人数
	WORD wCallUserCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==TRUE && m_bCallStatus[i]==TRUE) wCallUserCount++;
		else if(m_cbPlayStatus[i]!=TRUE) wCallUserCount++;
	}

	//下注开始
	if(bBanker==TRUE || wCallUserCount==m_wPlayerCount)
	{
		//始叫用户
		m_wBankerUser=wChairID;
		m_wCurrentUser=INVALID_CHAIR;

		//过滤最后一个叫庄用户强退情况
		while(m_cbPlayStatus[m_wBankerUser]==FALSE)
		{
			m_wBankerUser=(m_wBankerUser+1)%GAME_PLAYER;
		}

		//设置状态
		m_pITableFrame->SetGameStatus(GS_TK_SCORE);

		//庄家积分
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wBankerUser);
		SCORE lBankerScore=pIServerUserItem->GetUserScore();

		//玩家人数
		WORD wUserCount=0;
		for (WORD i=0;i<m_wPlayerCount;i++)if(m_cbPlayStatus[i]==TRUE )wUserCount++;

		//最大下注
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]!=TRUE || i==m_wBankerUser)continue;

			//获取用户
			pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

			//获取积分
			SCORE lScore=pIServerUserItem->GetUserScore();

			//金币检验
			ASSERT(lScore>=m_pGameServiceOption->lCellScore);

			//下注变量 客户要求
			m_lTurnMaxScore[i]=__min(lBankerScore/(wUserCount-1)/5,lScore/5);
		}

		//设置变量
		//m_wOperaCount=0;
		CMD_S_GameStart GameStart;
		GameStart.wBankerUser=m_wBankerUser;
		GameStart.lTurnMaxScore=0;

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			GameStart.lTurnMaxScore=m_lTurnMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	else		 //用户叫庄
	{
		//查找下个玩家
		do{
			m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
		}while(m_cbPlayStatus[m_wCurrentUser]==FALSE);
		
			
		

		//设置变量
		CMD_S_CallBanker CallBanker;
		CallBanker.wCallBanker=m_wCurrentUser;
		CallBanker.bFirstTimes=false;

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CALL_BANKER,&CallBanker,sizeof(CallBanker));
	}

	//改为确定庄家之后再控制输赢

	WORD wChairIDCtrl = INVALID_CHAIR;

	if(m_dwCheatCount>0)  //控制次数
	{
		m_bWinnerCtrl = true;  //如果控制次数大于0，指定玩家输赢控制开启

		if(m_dwCheatGameID==0)  //如果玩家ID等于0 指定玩家输赢控制关闭
		{
			m_bWinnerCtrl=false;  
		}
		else   //玩家ID大于0
		{
			//返回ID
			wChairIDCtrl = GetChairIdByGameId(m_dwCheatGameID);  //获取玩家椅子号

			//指定玩家不在
			if(wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}else{
		//控制次数少于0
		m_bWinnerCtrl=false;
	}




	//是否控制玩家
	if (m_bWinnerCtrl)  //指定控制优先第一
	{
		OutputDebugStringA("hhh 111");
		//m_cbCheatType胜利类型 1为胜利0为输
		ProbeJudge(wChairIDCtrl,m_cbCheatType);

	}
	else if (m_bAndroidWinningCtrl)//机器人赢率控制第二
	{
		OutputDebugStringA("hhh 222");
		//m_bCtrlThisTime = true;
		AndroidWinningControl();  //机器人换牌机率


	}
	else  //库存控制优先级别排第三 也是最低
	{
		OutputDebugStringA("hhh 333");
		//m_bCtrlThisTime = false;

		//机器人作弊
		AndroidCheck();

	}




	return true;
}

//加注事件
bool CTableFrameSink::OnUserAddScore(WORD wChairID, SCORE lScore)
{
	//状态效验
	ASSERT(m_pITableFrame->GetGameStatus()==GS_TK_SCORE);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_SCORE) return true;

	SCORE lAddScore = lScore;
	//金币效验
	if(m_cbPlayStatus[wChairID]==TRUE)
	{
		ASSERT(lAddScore>COMPARE_ZERO && lAddScore<=m_lTurnMaxScore[wChairID]);
		if (lAddScore<COMPARE_ZERO)
			lAddScore=m_pGameServiceOption->lCellScore;
		if(lAddScore-m_lTurnMaxScore[wChairID]>0.001)
			lAddScore=m_lTurnMaxScore[wChairID];
	}
	else //没下注玩家强退
	{
		ASSERT(lAddScore<COMPARE_ZERO&&lAddScore>-COMPARE_ZERO);
		if (lAddScore>COMPARE_ZERO||lAddScore<-COMPARE_ZERO) return false;
	}

	if(lAddScore>COMPARE_ZERO)
	{
		//下注金币
		m_lTableScore[wChairID]=lAddScore;

		//构造数据
		CMD_S_AddScore AddScore;
		AddScore.wAddScoreUser=wChairID;
		AddScore.lAddScoreCount=m_lTableScore[wChairID];

		//发送数据
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
		
		//游戏记录
		m_GameRecord.AppendFormat(TEXT("%s,加,%0.2f#"),m_szNickName[wChairID],lAddScore);
	}

	//下注人数
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_lTableScore[i]>COMPARE_ZERO && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE || i==m_wBankerUser)bUserCount++;
	}

	//闲家全到
	if(bUserCount==m_wPlayerCount)
	{
		//设置状态
		m_pITableFrame->SetGameStatus(GS_TK_PLAYING);

		//构造数据
		CMD_S_SendCard SendCard;
		ZeroMemory(SendCard.cbCardData,sizeof(SendCard.cbCardData));

		//发送扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE)continue;
			
			// 游戏记录
			m_GameRecord.AppendFormat( TEXT("%s-Card:"),m_szNickName[i] );

			//派发扑克
			CopyMemory(SendCard.cbCardData[i],m_cbHandCardData[i],MAX_COUNT);

			for(BYTE j = 0;j<CountArray(m_cbHandCardData[i]);j++)
			{
				m_GameRecord.AppendFormat( TEXT("%s,"),	TransformCardInfo(m_cbHandCardData[i][j]) );
			}
			m_GameRecord+=TEXT("#");
		}
		

		//发送数据
		for (WORD i=0;i< m_wPlayerCount;i++)
		{
			if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
			m_pITableFrame->SendTableData(i,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
		}
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_SEND_CARD,&SendCard,sizeof(SendCard));
	}

	return true;
}



//摊牌事件
bool CTableFrameSink::OnUserOpenCard(WORD wChairID, BYTE bOx)
{
	//状态效验
	ASSERT (m_pITableFrame->GetGameStatus()==GS_TK_PLAYING);
	if (m_pITableFrame->GetGameStatus()!=GS_TK_PLAYING) return true;
	if (m_bOxCard[wChairID]!=0xff) return true;

	//效验数据
	ASSERT(bOx<=TRUE);
	if(bOx>TRUE)return false;

	//效验数据
	if(bOx)
	{
		ASSERT(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0);
		if(!(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>0))return false;
	}
	else if(m_cbPlayStatus[wChairID]==TRUE)
	{
		if(m_GameLogic.GetCardType(m_cbHandCardData[wChairID],MAX_COUNT)>=OX_THREE_SAME)bOx=TRUE;
	}

	//牛牛数据
	m_bOxCard[wChairID] = bOx;

	//摊牌人数
	BYTE bUserCount=0;
	for(WORD i=0;i<m_wPlayerCount;i++)
	{
		if(m_bOxCard[i]<2 && m_cbPlayStatus[i]==TRUE)bUserCount++;
		else if(m_cbPlayStatus[i]==FALSE)bUserCount++;
	}

	 //构造变量
	CMD_S_Open_Card OpenCard;
	ZeroMemory(&OpenCard,sizeof(OpenCard));

	//设置变量
	OpenCard.bOpen=bOx;
	OpenCard.wPlayerID=wChairID;

	//发送数据
	for (WORD i=0;i< m_wPlayerCount;i++)
	{
		if(m_cbPlayStatus[i]==FALSE&&m_cbDynamicJoin[i]==FALSE)continue;
		
		m_pITableFrame->SendTableData(i,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	}
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));	

	//结束游戏
	if(bUserCount == m_wPlayerCount)
	{
		return OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);
	}

	return true;
}




//牌转文字
CString CTableFrameSink::TransformCardInfo( BYTE cbCardData ) 
{
	CString str = TEXT("");

	BYTE cbCardValue=cbCardData&LOGIC_MASK_VALUE;
	switch(cbCardValue)
	{
		case 0x01:
		{
			str += TEXT("A");
			break;
		}
		case 0x02:
		case 0x03:
		case 0x04:
		case 0x05:
		case 0x06:
		case 0x07:
		case 0x08:
		case 0x09:
			{
				str.Format( TEXT("%d"),cbCardValue);
				break;
			}
		case 0x0A:
			{
				str += TEXT("10");
				break;
			}
		case 0x0B:
			{
				str += TEXT("J");
				break;
			}
		case 0x0C:
			{
				str += TEXT("Q");
				break;
			}
		case 0x0D:
			{
				str += TEXT("K");
				break;
			}
		default:
			ASSERT(FALSE);
	}
	
	BYTE cbCardColor = (cbCardData&LOGIC_MASK_COLOR);
	switch( cbCardColor )
	{
	case 0x00:
		str += TEXT("方块");
		break;
	case 0x10:
		str += TEXT("梅花");
		break;
	case 0x20:
		str += TEXT("红桃");
		break;
	case 0x30:
		str += TEXT("黑桃");
		break;
	default:
		ASSERT(FALSE);
	}

	return str;
}

//根据用户Id获取其所坐的椅子Id
WORD CTableFrameSink::GetChairIdByGameId(DWORD dwGameId)
{
	WORD wChairID;
	for (wChairID=0; wChairID<GAME_PLAYER; wChairID++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue; //用户不存在

		if( pIServerUserItem->GetUserID() == dwGameId)
		{
			return wChairID;
		}
	}

	return INVALID_CHAIR;
}



//控制指定玩家输赢
bool CTableFrameSink::ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType)
{	
	//获取用户
	IServerUserItem * pIServerCtrlUserItem=m_pITableFrame->GetTableUserItem(wChairIDCtrl);
	if(pIServerCtrlUserItem==NULL) return false; //用户不存在, 不需要控制返回
	if(pIServerCtrlUserItem->GetUserID()!=m_dwCheatGameID ) return false; //控制ID不一致返回

		//控制控制控制控制控制控制控制
		//梦成网络添加
		//启动控制
		//////////////////////////////////////////////////////////////////////////
		BOOL bOxCard[GAME_PLAYER]={0};
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT]={0};
		//复制扑克数据
		CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
		for(int i=0; i<m_wPlayerCount; i++)
		{
			//判断有没有人牛牛
			bOxCard[i]=FALSE;
			if(m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>=0) bOxCard[i]=TRUE;
		}

		//排列扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}



		//最大牌值玩家索引
		int iMaxIndex=0;
		int iLostIndex=0;

		for(int i=0; i<GAME_PLAYER;i++)
		{
			//用户过滤
			if (m_cbPlayStatus[i]==FALSE) continue;  //如果用户当前不是游戏奖态跳过本次FOR
			//获取本局最大的牌
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iMaxIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iMaxIndex],bOxCard[i]);
			if(!cbCompareResult) iMaxIndex=i;
		}

			//对比玩家
		for (int i=0;i<GAME_PLAYER;i++)
		{
			//用户过滤
			if (m_cbPlayStatus[i]==FALSE) continue; //如果用户当前不是游戏奖态跳过本次FOR
			//对比扑克
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iLostIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iLostIndex],bOxCard[i]);
			if(cbCompareResult) iLostIndex=i;
		}

			//////////////////////////////////////////
			
			WORD wWinerUser=iMaxIndex;  //最大牌索引
			if(cbCheatType==CHEAT_TYPE_LOST) wWinerUser=iLostIndex; //最小牌索引

				//换牌
			if (wWinerUser!=wChairIDCtrl)    //如果当前用户不是控制号,则交换扑克数据 否则不需要交易
			{
					
					BYTE cbTemp[MAX_COUNT]={0};
					CopyMemory(cbTemp,m_cbHandCardData[wWinerUser],MAX_COUNT);   //将当前搜索到的牌存放到临时变量
					CopyMemory(m_cbHandCardData[wWinerUser],m_cbHandCardData[wChairIDCtrl],MAX_COUNT);  //将控制号的牌与当前的牌交换
					CopyMemory(m_cbHandCardData[wChairIDCtrl],cbTemp,MAX_COUNT);  //将临时的扑克数据与控制号交易
			}


		    ////////////////////////////
			////控制信息初始化
			if(m_dwCheatCount>0) m_dwCheatCount--;
			if(m_dwCheatCount==0) {m_dwCheatGameID=0;}
			m_bWinnerCtrl = false;
			///////////////////////////

			//梦成网络添加
			//写入配置文件
			CString TempValue;
			
			//写入控制总数
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
		
			//梦成网络添加
	
			return true;
}



//////机器人输赢比率，低于指定玩家控制，和库存互斥
bool CTableFrameSink::AndroidWinningControl()
{
	//机器人个数
	int iAndroidCount=0;
	//真人个数
	int iPlayerCount=0;

	//机器人换牌目标索引
	int iPostionAndroid[GAME_PLAYER]={0};
	//真人换牌目标索引
	int iPostionPlayer[GAME_PLAYER]={0};
	
	//初始化指针
	IServerUserItem *pIServerUserItem=NULL;

	//获取本桌机器人个数、玩家个数
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				//记录索引
				iPostionAndroid[iAndroidCount]=i;
				iAndroidCount++;
			}
			else
			{
				//记录索引
				iPostionPlayer[iPlayerCount]=i;
				iPlayerCount++;
			}
		}
	}

	//没有机器人返回
	if(iAndroidCount==0) return false;
	//没有真人返回
	if (iPlayerCount==0) return false;

	//////////////////////////////////
//启动控制
		//////////////////////////////////////////////////////////////////////////
		BOOL bOxCard[GAME_PLAYER]={0};
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT]={0};
		//复制扑克数据
		CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
		for(int i=0; i<m_wPlayerCount; i++)
		{
			//判断有没有人牛牛
			bOxCard[i]=FALSE;
			if(m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>=0) bOxCard[i]=TRUE;
		}

		//排列扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}



		//最大牌值玩家索引
		int iMaxIndex=0;
		int iLostIndex=0;

		for(int i=0; i<GAME_PLAYER;i++)
		{
			//用户过滤
			if (m_cbPlayStatus[i]==FALSE) continue;  //如果用户当前不是游戏奖态跳过本次FOR
			//获取本局最大的牌
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iMaxIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iMaxIndex],bOxCard[i]);
			if(!cbCompareResult) iMaxIndex=i;
		}

			//对比玩家
		for (int i=0;i<GAME_PLAYER;i++)
		{
			//用户过滤
			if (m_cbPlayStatus[i]==FALSE) continue; //如果用户当前不是游戏奖态跳过本次FOR
			//对比扑克
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iLostIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iLostIndex],bOxCard[i]);
			if(cbCompareResult) iLostIndex=i;
		}

		bool bAndroidBanker = false;
		IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wBankerUser);
		if(pBankerUser == NULL) return false;
		bAndroidBanker = pBankerUser->IsAndroidUser();


	//换牌变量
	int iTarget=0;//目标索引
	bool bChange = false;//是否要换牌

	//随机种子
	srand( (unsigned)time(NULL));
	//获取随机数
	DOUBLE fAndroidWinPer = (float)(m_lStockScore/m_lAndroidWinAndLose) ;
	//获取随机数
	WORD Ratio =fAndroidWinPer*100; //产生0~100的数


	CString str;
	str.Format(L"nnn Ratio：%d",Ratio);
	OutputDebugString(str);

	//iMaxindexitem获取
	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);  //获取最大牌的用户
	if(pIServerUserItem == NULL) return false;   //如果用户不存在跳出
	bool ReturnValue=(m_AndroidWinningRatio>Ratio)?true:false; //如果配置比生成的数大
	//机器人赢
	if(ReturnValue)  //如果配置比生成的数大
	{
//		if(pIServerUserItem->IsAndroidUser()==FALSE)//最大牌在真人手上
		{
			if(bAndroidBanker)
			{
				iTarget=m_wBankerUser;//iPostionAndroid[rand()%iAndroidCount];  //随机机器人目标换牌
				bChange = true;									//设置换牌变量为TRUE
			}
			else// if(iMaxIndex == m_wBankerUser)
			{
				//临时变量
				BYTE cbTemp[MAX_COUNT]={0};

				CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //最大牌的玩家扑克
				CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //目标扑克用户
				CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //目标接收最大用户扑克

			}
		}
// 		else
// 		{
// 			if(bAndroidBanker)
// 			{
// 
// 			}
// 				//临时变量
// 			BYTE cbTemp[MAX_COUNT]={0};
// 
// 			CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //最大牌的玩家扑克
// 			CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //目标扑克用户
// 			CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //目标接收最大用户扑克
// 
// 		}
	}
	//玩家赢
	else
	{
		if(pIServerUserItem->IsAndroidUser()==TRUE)//最大牌在机器人手上
		{
			iTarget=iPostionPlayer[rand()%iPlayerCount];
			bChange = true;
		}
	}

	//换牌
	if (bChange)
	{
		//临时变量
		BYTE cbTemp[MAX_COUNT]={0};
		//交换牌
		CopyMemory(cbTemp,m_cbHandCardData[iMaxIndex],MAX_COUNT);  //最大牌的玩家扑克
		CopyMemory(m_cbHandCardData[iMaxIndex],m_cbHandCardData[iTarget],MAX_COUNT);  //目标扑克用户
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);    //目标接收最大用户扑克

	}

	return true;
}





//库存控制 梦成网络修正
bool CTableFrameSink::AndroidCheck()
{
	//机器人个数
	int iAndroidCount=0;
	//真人个数
	int iPlayerCount=0;

	//机器人换牌目标索引
	int iPostionAndroid[GAME_PLAYER]={0};
	//真人换牌目标索引
	int iPostionPlayer[GAME_PLAYER]={0};

	//初始化指针
	IServerUserItem *pIServerUserItem=NULL;

	//获取本桌机器人个数、玩家个数
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem!=NULL)
		{
			if (pIServerUserItem->IsAndroidUser())
			{
				//记录索引
				iPostionAndroid[iAndroidCount]=i;
				iAndroidCount++;
			}
			else
			{
				//记录索引
				iPostionPlayer[iPlayerCount]=i;
				iPlayerCount++;
			}
		}
	}
	OutputDebugStringA("hhh 56");
	//没有机器人返回
	if(iAndroidCount==0) return false;
	//没有真人返回
	if (iPlayerCount==0) return false;

		OutputDebugStringA("hhh 61");
	bool bAndroidBanker = false;
	IServerUserItem *pBankerUser = m_pITableFrame->GetTableUserItem(m_wBankerUser);
	if(pBankerUser == NULL) return false;
	bAndroidBanker = pBankerUser->IsAndroidUser();
		//////////////////////////////////
		OutputDebugStringA("hhh 62");
	//启动控制
		//////////////////////////////////////////////////////////////////////////
		BOOL bOxCard[GAME_PLAYER]={0};
		BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT]={0};
		//复制扑克数据
		CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));
		for(int i=0; i<m_wPlayerCount; i++)
		{
			//判断有没有人牛牛
			bOxCard[i]=FALSE;
			if(m_GameLogic.GetCardType(cbUserCardData[i],MAX_COUNT)>=0) bOxCard[i]=TRUE;
		}

		//排列扑克
		for (WORD i=0;i<m_wPlayerCount;i++)
		{
			m_GameLogic.SortCardList(cbUserCardData[i],MAX_COUNT);
		}

	OutputDebugStringA("hhh 63");

		//最大牌值玩家索引
		int iMaxIndex=0;
		int iLostIndex=0;

		for(int i=0; i<GAME_PLAYER;i++)
		{
			//用户过滤
			if (m_cbPlayStatus[i]==FALSE) continue;  //如果用户当前不是游戏奖态跳过本次FOR
			//获取本局最大的牌
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iMaxIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iMaxIndex],bOxCard[i]);
			if(!cbCompareResult) iMaxIndex=i;
		}

			//对比玩家
		for (int i=0;i<GAME_PLAYER;i++)
		{
			//用户过滤
			if (m_cbPlayStatus[i]==FALSE) continue; //如果用户当前不是游戏奖态跳过本次FOR
			//对比扑克
			bool cbCompareResult=m_GameLogic.CompareCard(cbUserCardData[iLostIndex],cbUserCardData[i],MAX_COUNT,bOxCard[iLostIndex],bOxCard[i]);
			if(cbCompareResult) iLostIndex=i;
		}



	//换牌变量
	int iTarget=0;//目标索引
	bool bChange = false;//是否要换牌

	//随机种子
	srand( (unsigned)time(NULL));
	DOUBLE fAndroidWinPer = (float)(m_lStockScore/m_lAndroidWinAndLose) ;
	//获取随机数
	WORD Ratio =fAndroidWinPer*100; //产生0~100的数


	//iMaxindexitem获取
	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);  //获取最大牌的用户
	if(pIServerUserItem == NULL) return false;   //如果用户不存在跳出
	

	//梦成网络添加
	WORD WinRatio=100;
	if(StorageStyle==1) WinRatio = StorageRatio;            //定义当前输赢比例 //默认比例
	if(StorageStyle==2) WinRatio = StorageMinRatio;         //定义当前输赢比例 //杀分
	if(StorageStyle==3) WinRatio = StorageMaxRatio;         //定义当前输赢比例 //送分
	//计算是杀分还是送分
	//吃掉库存
	if(m_lStockScore>=m_lStorageMax && StorageStyle!=3)
	{
		
			SCORE tmp_lStockScore = m_lStockScore;							 //原来库存
			SCORE m_StorageEating=m_lStockScore*(StorageEating/100.00);		 //系统吃分
			
			//减去吃掉的分
			m_lStockScore-=m_StorageEating;
			WinRatio=StorageMaxRatio;  //送分模式
						//送分模式
			StorageStyle=3;

	}
	
	//库存少于最小值,杀分模式
	if(m_lStockScore<=m_lStorageMin /*&& StorageStyle!=2*/)
	{
		WinRatio=StorageMinRatio;
		StorageStyle=2;//杀分模式
	}
	
				 //输出当前模式
				CString strLog;
				strLog.Format(TEXT("桌号：%d,游戏模式：%d,当前输赢比率：%d (游戏模式1,正常,2,杀分,3,送分)"),m_pITableFrame->GetTableID()+1,StorageStyle,WinRatio);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息



				CString str;
				str.Format(L"hhh 随机值Ratio：%d 系统的库存m_lStockScore为：%lf 机器人赢的概率为WinRatio:%d",Ratio,m_lStockScore,WinRatio);
				OutputDebugString(str);

	bool ReturnValue=(WinRatio>Ratio)?true:false; //计算机率
	//梦成网络添加

	CString str1;
	str1.Format(L"hhh 计算出来的胜率为ReturnValue：%d",ReturnValue);
	OutputDebugString(str1);

	//机器人赢
	if(ReturnValue)  //如果配置比生成的数大
	{
//		if(pIServerUserItem->IsAndroidUser()==FALSE)//最大牌在真人手上
		{
			if(bAndroidBanker)
			{
				iTarget=m_wBankerUser;//iPostionAndroid[rand()%iAndroidCount];  //随机机器人目标换牌
				bChange = true;									//设置换牌变量为TRUE
			}
			else// if(iMaxIndex == m_wBankerUser)
			{
				//临时变量
				BYTE cbTemp[MAX_COUNT]={0};

				CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //最大牌的玩家扑克
				CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //目标扑克用户
				CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //目标接收最大用户扑克

			}
		}
// 		else
// 		{
// 				//临时变量
// 				BYTE cbTemp[MAX_COUNT]={0};
// 
// 				CopyMemory(cbTemp,m_cbHandCardData[m_wBankerUser],MAX_COUNT);  //最大牌的玩家扑克
// 				CopyMemory(m_cbHandCardData[m_wBankerUser],m_cbHandCardData[iLostIndex],MAX_COUNT);  //目标扑克用户
// 				CopyMemory(m_cbHandCardData[iLostIndex],cbTemp,MAX_COUNT);    //目标接收最大用户扑克
// 
// 		}
	}
	//玩家赢
	else
	{
		if(pIServerUserItem->IsAndroidUser()==TRUE)//最大牌在机器人手上
		{
			iTarget=iPostionPlayer[rand()%iPlayerCount];
			bChange = true;
		}
	}

	//换牌
	if (bChange)
	{
		//临时变量
		BYTE cbTemp[MAX_COUNT]={0};
		//交换牌
		CopyMemory(cbTemp,m_cbHandCardData[iMaxIndex],MAX_COUNT);  //最大牌的玩家扑克
		CopyMemory(m_cbHandCardData[iMaxIndex],m_cbHandCardData[iTarget],MAX_COUNT);  //目标扑克用户
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);    //目标接收最大用户扑克
	}



return true;


}



bool CTableFrameSink::OnSubAmdinCommand(IServerUserItem*pIServerUserItem,const void*pDataBuffer)
{
	//如果不具有管理员权限 则返回错误
	//if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
	const CMD_C_AdminReq* AdminReq=static_cast<const CMD_C_AdminReq*>(pDataBuffer);
	switch(AdminReq->cbReqType)
	{
	case RQ_OPTION_CANCLE:	//取消
		{
			if (m_dwCheatGameID!=0)
			{
				//限制提示
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("已经成功将GameID为:%d的作弊权限取消"),m_dwCheatGameID);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
				m_dwCheatGameID=0;
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("当前没有设置作弊用户!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_QUERYING:			//查询
		{
			if (m_dwCheatGameID!=0)
			{
				TCHAR szType[][32]={TEXT("输"),TEXT("赢")};

				if(m_cbCheatType!=0 && m_cbCheatType!=1) break;

				//限制提示
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("当前作弊GameID：%d，剩余控制次数：%d局，控制类型：%s\n"),
					m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
				m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			}
			else
			{
				m_pITableFrame->SendGameMessage(pIServerUserItem,TEXT("当前没有设置作弊用户!"),SMT_CHAT);
			}
			break;
		}
	case RQ_OPTION_SETING:		//设置
		{
			m_dwCheatGameID=AdminReq->dwGameID;
			m_dwCheatCount=AdminReq->cbCheatCount;
			m_cbCheatType=AdminReq->cbCheatType;




			//梦成网络添加
			//写入配置文件
			CString TempValue;
			//写入控制类型
			TempValue.Format(TEXT("%d"),m_cbCheatType);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatType"),TempValue,m_szFileName);
			//写入控制总数
			TempValue.Format(TEXT("%d"),m_dwCheatCount);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("CheatCount"),TempValue,m_szFileName);
			//写入控制用户ID
			TempValue.Format(TEXT("%d"),m_dwCheatGameID);
			WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("WinnerUserID"),TempValue,m_szFileName);
			//梦成网络添加


			TCHAR szType[][32]={TEXT("输"),TEXT("赢")};

			//限制提示
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("成功为GameID:%d设置作弊权限,控制：%d次，类型：%s"),m_dwCheatGameID,m_dwCheatCount,szType[m_cbCheatType]);
			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);
			break;
		}
	}
	return true;
}






//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\OxServer.ini"),szPath);
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);

	//是否读取每盘刷新配置变量
	if (bReadFresh)
	{
		//每盘刷新
		BYTE cbRefreshCfg = GetPrivateProfileInt(szServerName, TEXT("Refresh"), 0, szFileName);
		m_bRefreshCfg = cbRefreshCfg?true:false;

		//是否刷新库存
		BYTE cbRefreshStorageCfg = GetPrivateProfileInt(szServerName, TEXT("RefreshStorage"), 0, szFileName);
		m_bRefreshStorageCfg = cbRefreshStorageCfg?true:false;

	}

	//库存设置
	if(m_bRefreshStorageCfg)
	{
		//重置库存
		
		memset(OutBuf,0,255);
		GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,szFileName);
			
		CString TempValue;
		myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
		m_lStockScore=Mywtof(TempValue);
	}
	else
	{
		if(m_lStockScore <= 0.00001 && m_lStockScore >= -0.00001)
		{
			memset(OutBuf,0,255);
			GetPrivateProfileString(szServerName,TEXT("StorageStart"),TEXT("0.0"),OutBuf,255,szFileName);

			CString TempValue;
			myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
			m_lStockScore=Mywtof(TempValue);

		}
	}


	//重置衰减值
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);

// 	memset(OutBuf,0,255);
// 	GetPrivateProfileString(szServerName,TEXT("StorageMin"),TEXT("10"),OutBuf,255,szFileName);
// 	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMin);
// 	if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
// 
// 	memset(OutBuf,0,255);
// 	GetPrivateProfileString(szServerName,TEXT("StorageMax"),TEXT("100"),OutBuf,255,szFileName);
// 	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMax);
		//梦成网络添加
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
//	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageEating"),TEXT("50"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&StorageEating);


	//系统总输赢分
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0.0"),OutBuf,255,szFileName);
	
	//////////////////////////////////
	//梦成网络读取小数点数值
	CString TempValue;
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
	m_lStockLimit=Mywtof(TempValue);
	/////////////////////////////////

	CString str;
	str.Format(L"hhh 获取到的系统输赢为:%lf",m_lStockLimit);
	OutputDebugString(str);

	//梦成网络添加

	////2012-08-28
	//控制玩家输赢
	BYTE cbWinnerCtrl = GetPrivateProfileInt(szServerName, TEXT("WinnerCtrl"), 0, szFileName);
	m_bWinnerCtrl = cbWinnerCtrl?true:false;
	//需要胜出的玩家UserID
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("WinnerUserID"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%lu"),&m_dwCheatGameID);
	//读取胜利类型
	m_cbCheatType = GetPrivateProfileInt(szServerName, TEXT("CheatType"), 1, szFileName);
	//读取控制次数
	m_dwCheatCount = GetPrivateProfileInt(szServerName, TEXT("CheatCount"), 0, szFileName);
	/////////////////



	//////获取机器人输赢
	BYTE cbAndroidWinningCtrl = GetPrivateProfileInt(szServerName, TEXT("AndroidWinningCtrl"), 0, szFileName);
	m_bAndroidWinningCtrl = cbAndroidWinningCtrl?true:false;

	//////获取机器人输赢比率
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("AndroidWinningRatio"),TEXT("0"),OutBuf,60,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_AndroidWinningRatio);
	if (m_AndroidWinningRatio<0 || m_AndroidWinningRatio >100)
	{
		m_AndroidWinningRatio = 60;
	}

	CString strLog;
	strLog.Format(TEXT("桌号：%d,起始库存：%0.2f,库存最大小值:%I64d  %I64d,机器人赢比例:%I64d"),m_pITableFrame->GetTableID()+1,m_lStockScore,m_lStorageMax,m_lStorageMin,m_AndroidWinningRatio);
	CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息


	

}