#include "StdAfx.h"
#include "TableFrameSink.h"


//////////////////////////////////////////////////////////////////////////

//静态变量
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;			//游戏人数
const BYTE			CTableFrameSink::m_GameStartMode=START_MODE_ALL_READY;	//开始模式


SCORE				CTableFrameSink::m_lStockScore = 0.0;
LONGLONG			CTableFrameSink::m_lStorageOff = 0L;
BYTE				CTableFrameSink::StorageStyle=1;
//定时器 0~30
#define IDI_GAME_COMPAREEND					(1)									//结束定时器
#define IDI_GAME_OPENEND					(2)									//结束定时器

#define TIME_GAME_COMPAREEND				6000								//结束定时器
#define TIME_GAME_OPENEND					6000								//结束定时器

//////////////////////////////////////////////////////////////////////////
void TraceMessage(CString szinfo,WORD id);
//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrameSink::CTableFrameSink()
{
	//游戏记录
	m_GameRecord.Empty(); //清除对象
	ZeroMemory(m_szNickName,sizeof(m_szNickName));  //清除对象
	ZeroMemory(m_lAddScoreTimes,sizeof(m_lAddScoreTimes));  //清除对象
	//游戏变量
	m_bOperaCount=0;
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount=0L;
	m_bGameEnd=false;  //是否结束
	//控制信息
	m_dwCheatCount=0;
	m_dwCheatGameID=0;
	m_cbCheatType=CHEAT_TYPE_WIN;
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl = false;
	ZeroMemory(m_wFlashUser,sizeof(m_wFlashUser));

	//用户状态
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;
		m_wCompardUser[i].RemoveAll();
	}

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//下注信息
	m_lMaxCellScore=0L;
	m_lCellScore=SCORE_ZERO;
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));


	//组件变量
	m_pITableFrame=NULL;
	m_pGameServiceOption=NULL;
	m_pGameCustomRule=NULL;
	srand( (unsigned)time(NULL));
	return;
}

//析构函数
CTableFrameSink::~CTableFrameSink(void)
{

	if(!m_GameRecord.IsEmpty()) TraceMessage(m_GameRecord,m_wTableID);

}

//接口查询--检测相关信息版本
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
	if (m_pITableFrame==NULL) return false;

	//获取参数
	m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
	ASSERT(m_pGameServiceOption!=NULL);

	//自定规则
	ASSERT(m_pITableFrame->GetCustomRule()!=NULL);
	m_pGameCustomRule=(tagCustomRule *)m_pITableFrame->GetCustomRule();

		//开始模式
	m_pITableFrame->SetStartMode(START_MODE_ALL_READY);

		//下注变量
	m_lCellScore=Double_Round(m_pGameServiceOption->lCellScore,2,1);

	m_lCellScore_Long = (ceil( m_lCellScore* 100.00) / 100.00)*100;  //将小数转成整数100倍显示，结算时再除100

	m_wTableID=m_pITableFrame->GetTableID();


	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);

	//读取配置
	//TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(m_szFileName,CountArray(m_szFileName),TEXT("%s\\ZaJinHuaConfig.ini"),szPath);
	//开始存储数目
	TCHAR szServerName[MAX_PATH]=TEXT("");
	_sntprintf(szServerName,CountArray(szServerName),TEXT("%s"),m_pGameServiceOption->szServerName);
	//读取库存
	m_lStockScore=GetPrivateProfileInt(szServerName,TEXT("StorageStart"),0,m_szFileName);
	//读取衰减值
	m_lStorageDeduct=GetPrivateProfileInt(szServerName,TEXT("StorageDeduct"),10,m_szFileName);


	//设置最大衰减值
	if( m_lStorageDeduct < 0)
		m_lStorageDeduct = 0;
	if ( m_lStorageDeduct > 1000)
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
	
	
	//读取配置
	ReadConfigInformation(true);


	return true;
}

//复位桌子
void  CTableFrameSink::RepositionSink()
{	
	//游戏记录
	m_GameRecord.Empty();
	ZeroMemory(m_lAddScoreTimes,sizeof(m_lAddScoreTimes));

	//游戏变量
	m_bOperaCount=0;
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount=0L;
	ZeroMemory(m_wFlashUser,sizeof(m_wFlashUser));

	//用户状态
	ZeroMemory(&m_StGameEnd,sizeof(m_StGameEnd));
	ZeroMemory(m_cbPlayStatus,sizeof(m_cbPlayStatus));
	for(int i=0;i<m_wPlayerCount;i++)
	{
		m_bMingZhu[i]=false;	
		m_wCompardUser[i].RemoveAll();
	}

	//扑克变量
	ZeroMemory(m_cbHandCardData,sizeof(m_cbHandCardData));

	//下注信息
	m_lMaxCellScore=0L;										
	m_lCurrentTimes=0L;		
	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	ZeroMemory(m_lUserMaxScore,sizeof(m_lUserMaxScore));
	m_bWinnerCtrl=false;
	m_bAndroidWinningCtrl=false;

	return;
}

//开始模式
BYTE  CTableFrameSink::GetGameStartMode()
{
	return m_GameStartMode;
}

//游戏状态
bool  CTableFrameSink::IsUserPlaying(WORD wChairID)
{
	ASSERT(wChairID<m_wPlayerCount);
	return (m_cbPlayStatus[wChairID]==TRUE)?true:false;
}

//游戏开始
bool  CTableFrameSink::OnEventGameStart()
{

	//设置状态
	m_pITableFrame->SetGameStatus(GAME_STATUS_PLAY);

	m_bGameEnd=false;  //是否结束
	ZeroMemory(m_szNickName,sizeof(m_szNickName));
	//读取配置
	ReadConfigInformation(true);

	ZeroMemory(m_lTableScore,sizeof(m_lTableScore));
	//最大下注
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		//获取用户
		IServerUserItem *pIServerUser=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUser==NULL) continue;

		SCORE lUserScore=pIServerUser->GetUserScore();
		ASSERT(lUserScore >=m_pGameServiceOption->lCellScore);

		//设置变量
		m_cbPlayStatus[i]=TRUE;

		m_lUserMaxScore[i]=(ceil(lUserScore * 100.00) / 100.00)*100;

		_sntprintf(m_szNickName[i],CountArray(m_szNickName[i]),pIServerUser->GetNickName());

		m_GameRecord.AppendFormat(TEXT("P%ld,%s,%0.2f#"),i,m_szNickName[i],pIServerUser->GetUserScore());
	}

	//单元积分上限 20倍      //梦成网络
	m_lMaxCellScore=m_lCellScore_Long*20;
	
	//当前倍数
	m_lCurrentTimes=1;

	//混乱扑克
	m_GameLogic.RandCardList(m_cbHandCardData[0],sizeof(m_cbHandCardData)/sizeof(m_cbHandCardData[0][0]));

	
	//排列扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameLogic.SortCardList(m_cbHandCardData[i],MAX_COUNT);
	}

	//下方的是控制相关的
	////////////////////////////////////////////////////////
	//判断是否控制指定玩家
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
			if( wChairIDCtrl==INVALID_CHAIR)
			{
				m_bWinnerCtrl=false;
			}
		}
	}else{
		//控制次数少于0
		m_bWinnerCtrl=false;
	}

	//////////////////////////////////////////结束指定玩家控制

	//是否控制玩家
	if (m_bWinnerCtrl)  //指定控制优先第一
	{
		//m_cbCheatType胜利类型 1为胜利0为输
		ProbeJudge(wChairIDCtrl,m_cbCheatType);

	}
	else if (m_bAndroidWinningCtrl)//机器人赢率控制第二
	{
		
		AndroidWinningControl();  //机器人换牌机率

	}
	else  //库存控制优先级别排第三 也是最低
	{
		
		
		//机器人作弊
		AndroidCheck();
	}
	//////////////结束控制

	
	//用户设置
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_lTableScore[i]=m_lCellScore_Long;
			m_lAddScoreTimes[i]++;
		}
	}

	// 游戏记录
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			m_GameRecord.AppendFormat( TEXT("P%ld,Card:"),i);
			for(WORD j=0;j<CountArray(m_cbHandCardData[i]);j++)
				m_GameRecord.AppendFormat( TEXT("%s,"),TransformCardInfo(m_cbHandCardData[i][j]));
			m_GameRecord+=TEXT("#");
		}

	}

	//设置庄家
	if(m_wBankerUser==INVALID_CHAIR)m_wBankerUser=rand()%m_wPlayerCount;

	//庄家离开
	if(m_wBankerUser<m_wPlayerCount && m_cbPlayStatus[m_wBankerUser]==FALSE)m_wBankerUser=rand()%m_wPlayerCount;

	//确定庄家
	while(m_cbPlayStatus[m_wBankerUser]==FALSE)
	{
		m_wBankerUser=(m_wBankerUser+1)%m_wPlayerCount;
	}

	//当前用户
	m_wCurrentUser=(m_wBankerUser+1)%m_wPlayerCount;

	while(m_cbPlayStatus[m_wCurrentUser]==FALSE)
	{
		m_wCurrentUser=(m_wCurrentUser+1)%m_wPlayerCount;
	}




	//下方发送机器人信息需要添加，是否机器人胜利，当前库存，
	//构造数据
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));
	GameStart.lMaxScore=m_lMaxCellScore;
	GameStart.lCellScore=m_lCellScore_Long;
	GameStart.lCurrentTimes=m_lCurrentTimes;
	GameStart.wCurrentUser=m_wCurrentUser;
	GameStart.wBankerUser=m_wBankerUser;
	GameStart.wMaxTimes = m_pGameCustomRule->wMaxTimes;

	CopyMemory(GameStart.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));
	//发送数据
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE)
		{
			GameStart.lUserMaxScore=m_lUserMaxScore[i];
			m_pITableFrame->SendTableData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
		}
		m_pITableFrame->SendLookonData(i,SUB_S_GAME_START,&GameStart,sizeof(GameStart));
	}
	//向机器人发送用户牌
	AndroidSendUserCard();

	return true;
}

//游戏结束
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	switch (cbReason)
	{
	case GER_COMPARECARD:	//比牌结束
	case GER_NO_PLAYER:		//没有玩家
		{
			//梦成网络修正

			if(m_bGameEnd) return true;  //如果是结束奖态就直接返回不再执行
			
			m_bGameEnd=true;		//设置当前是结束奖态

			//游戏记录
			if(cbReason == GER_COMPARECARD)
				m_GameRecord+=TEXT("比牌结束:#"); //比牌结束
			else
				m_GameRecord+=TEXT("没有玩家:#");  //没有玩家
			
			 //桌面统计
			m_GameRecord+=TEXT("桌子下注统计:#");  //桌面统计
			for(WORD i = 0;i<GAME_PLAYER;i++)
			{
				if(m_lAddScoreTimes[i]>0L)
				{
					//m_GameRecord.AppendFormat( TEXT("%s,下注%I64d次,共%0.2f#"),m_szNickName[i],m_lAddScoreTimes[i],m_lTableScore[i]/100.00);
					m_GameRecord.AppendFormat( TEXT("P%d,下注%I64d次,共%0.2f#"),i,m_lAddScoreTimes[i],m_lTableScore[i]/100.00);
				}
			}

			////游戏结束处理
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd)); //清除消息
		
			//设置比牌用户为无效
			for (WORD i=0;i<m_wPlayerCount;i++){
				for (WORD j=0;j<4;j++)
				{
					//比牌用户数据清空
					GameEnd.wCompareUser[i][j]=INVALID_CHAIR;
				}
			}

			//唯一玩家
			WORD wWinner=0;  //胜利用户
			WORD wUserCount=0; //用户总数
			for (WORD i=0;i<m_wPlayerCount;i++)
			{	
				//正在游戏用户
				if(m_cbPlayStatus[i]==TRUE)
				{
					//用户总数加一
					wUserCount++;
					wWinner=i;
					if(GER_COMPARECARD==cbReason)ASSERT(m_wBankerUser==i);
					m_wBankerUser=i;
				}
			}

			//胜利者强退
			if(wUserCount==0 /*&& GER_COMPARECARD==cbReason*/)
			{
				//胜利用户设成庄家
				wWinner=m_wBankerUser;
			}

			//计算总注
			LONGLONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				//输分用户得分
				GameEnd.lGameScore[i]=-m_lTableScore[i];
				//胜利用户总得分
				lWinnerScore+=m_lTableScore[i];
			}

			//如果胜利得分少于100 
			//下方税收需要除100低于0.01的税收将不显示 低于0.01的税收统一收取0.01的税收
			if(lWinnerScore>100){

				//处理税收
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,lWinnerScore);
			
			}
			else
			{	//处理税收
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,100.00);
			}

			//胜利用户计算税收得分
			GameEnd.lGameScore[wWinner]=lWinnerScore-GameEnd.lGameTax;

			//复制扑克数据
			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//扑克数据
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				WORD wCount=0;
				while(m_wCompardUser[i].GetCount()>0)
				{
					GameEnd.wCompareUser[i][wCount++] = m_wCompardUser[i].GetAt(0);
					m_wCompardUser[i].RemoveAt(0);
				}
			}

			//设置结束
			GameEnd.wEndState=0;
			///////////////////////////////////////////////////////
			////计算得分
			//如果用户总数大于1,计算所有用户得分与税收
			if(wUserCount==1)
			{
				//修改积分
				tagScoreInfo ScoreInfoArray[GAME_PLAYER];
				//清除对象数据
				ZeroMemory(ScoreInfoArray,sizeof(ScoreInfoArray));
				for (WORD i=0;i<m_wPlayerCount;i++)
				{
					if(m_cbPlayStatus[i]==FALSE) continue;  //如果用户不是游戏中,跳过本些FOR 进入下一次FOR
					ScoreInfoArray[i].cbType =(GameEnd.lGameScore[i]>0)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;  //如果用户得分是大于O即为胜利否则就是输
					ScoreInfoArray[i].lRevenue=Double_Round(GameEnd.lGameTax/100.00,2,1);                //计算税收   除100是因为 这个扎金花游戏中全部都是按整数来进行的                        
					ScoreInfoArray[i].lScore=Double_Round(GameEnd.lGameScore[i]/100.00,2,1);			 //计算得分   除100是因为 这个扎金花游戏中全部都是按整数来进行的
					//游戏记录
					m_GameRecord.AppendFormat(TEXT("P%ld-:%0.2f,%0.2f#"),i,ScoreInfoArray[i].lScore ,ScoreInfoArray[i].lRevenue); //写入游戏记录 
				
				}
				//写分操作
				m_pITableFrame->WriteTableScore(ScoreInfoArray,CountArray(ScoreInfoArray));
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
			for (WORD i=0;i<m_wPlayerCount;i++)
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
						if(GameEnd.lGameScore[i]>0) RevenueScore = Double_Round(GameEnd.lGameTax/100.00,2,1);
						//计算库存衰减
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//系统库存值
						m_lStockScore-=(Double_Round(GameEnd.lGameScore[i]/100.00+RevenueScore,2,1)+lStorageDeduct); 
						//系统得分
						lSystemScore-=GameEnd.lGameScore[i]/100.00;

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
				strLog.Format(TEXT("A桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息

				//梦成网络添加
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//如果存在控制用户的话//发送游戏信息给控制号
				for(int i=0; i<m_wPlayerCount; i++)
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
				for(int i=0; i<m_wPlayerCount; i++)
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


			if(wChairID==GAME_PLAYER)
			{
				//结束数据
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_COMPAREEND,TIME_GAME_COMPAREEND,1,0);
			}
			else
			{
				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));


				TraceMessage(m_GameRecord,m_wTableID);  //输出记录

				//结束游戏
				m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);

			}

			//读取配置
			ReadConfigInformation(true);


			return true;
		}
	case GER_USER_LEAVE:		//用户强退
	case GER_NETWORK_ERROR:
		{
			if(m_bGameEnd)return true;

			//效验参数
			ASSERT(pIServerUserItem!=NULL);
			ASSERT(wChairID<m_wPlayerCount);

			//强退处理
			if(m_cbPlayStatus[wChairID]==TRUE)
				return OnUserGiveUp(wChairID,true);
			return true;
		}
	case GER_OPENCARD:			//开牌结束 //自动开牌执行这里的  
		{
			if(m_bGameEnd)return true;
			m_bGameEnd = true;
			
			m_GameRecord+=TEXT("开牌结束:#");
			
			 //桌面统计
			m_GameRecord+=TEXT("桌子下注统计:#");  //桌面统计
			for(WORD i = 0;i<GAME_PLAYER;i++)
			{
				if(m_lAddScoreTimes[i]>0)
				{
					m_GameRecord.AppendFormat( TEXT("P%ld,下注%I64d次,共%0.2f#"),i,m_lAddScoreTimes[i],m_lTableScore[i]/100.00);
				}
			}
			
			///////////////////////////////////////////////////////////////////
			//定义变量
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				for (WORD j=0;j<4;j++)
				{
					GameEnd.wCompareUser[i][j]=INVALID_CHAIR;
				}
			}
			//胜利玩家
			WORD wWinner=m_wBankerUser;

			//计算分数
			LONGLONG lWinnerScore=0L;
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{
				if(i==wWinner)continue;
				lWinnerScore+=m_lTableScore[i];
				GameEnd.lGameScore[i]=-m_lTableScore[i];
			}

			//处理税收
			//GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,lWinnerScore);
			//如果胜利得分少于100 
			//下方税收需要除100低于0.01的税收将不显示 低于0.01的税收统一收取0.01的税收
			if(lWinnerScore>100){

				//处理税收
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,lWinnerScore);
			
			}
			else
			{	//处理税收
				GameEnd.lGameTax=m_pITableFrame->CalculateRevenue(wWinner,100);
			}
			//胜利用户计算税收得分
			GameEnd.lGameScore[wWinner]=lWinnerScore-GameEnd.lGameTax;

			//开牌结束
			GameEnd.wEndState=1;
			//复制扑克数据
			CopyMemory(GameEnd.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));

			//扑克数据
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{				
				WORD wCount=0;
				while(m_wCompardUser[i].GetCount()>0)
				{
					GameEnd.wCompareUser[i][wCount++] = m_wCompardUser[i].GetAt(0);
					m_wCompardUser[i].RemoveAt(0);
				}
			}

			//修改积分
			tagScoreInfo ScoreInfo[m_wPlayerCount];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for (WORD i=0;i<m_wPlayerCount;i++) 
			{				
				if(i==wWinner || m_cbPlayStatus[i]==FALSE)continue;
				ScoreInfo[i].lScore=Double_Round(GameEnd.lGameScore[i]/100.00,2,1);
				ScoreInfo[i].cbType=SCORE_TYPE_LOSE;
				//游戏记录
				m_GameRecord.AppendFormat(TEXT("P%ld-:%0.2f,%0.2f#"),i,ScoreInfo[i].lScore, 0.00);
			
			}
			ScoreInfo[wWinner].lScore=Double_Round(GameEnd.lGameScore[wWinner]/100.00,2,1);
			ScoreInfo[wWinner].cbType=SCORE_TYPE_WIN;
			ScoreInfo[wWinner].lRevenue=Double_Round(GameEnd.lGameTax/100.00,2,1);
	
			//游戏记录
			m_GameRecord.AppendFormat(TEXT("P%ld-:%0.2f,%0.2f#"),wWinner,ScoreInfo[wWinner].lScore, ScoreInfo[wWinner].lRevenue);


			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));



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
			for (WORD i=0;i<m_wPlayerCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserIte=m_pITableFrame->GetTableUserItem(i);
				//如果用户不存在
				if (pIServerUserIte==NULL)continue;					
				//存在机器人才执行
				if(bUserAI)
				{
					//如果用户不是机器人
					if(!pIServerUserIte->IsAndroidUser())
					{
						//计算税收
						if(GameEnd.lGameScore[i]>0) RevenueScore = Double_Round(GameEnd.lGameTax/100.00,2,1);
						//计算库存衰减
						if(m_lStockScore>0.00) lStorageDeduct=Double_Round(m_lStockScore*(m_lStorageDeduct/1000.00),2,1);
						//系统库存值
						m_lStockScore-=(Double_Round(GameEnd.lGameScore[i]/100.00+RevenueScore,2,1)+lStorageDeduct); 
						//系统得分
						lSystemScore-=GameEnd.lGameScore[i]/100.00;
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
				strLog.Format(TEXT("B桌号：%d,当前库存：%0.2f,系统得分：%0.2f,税收：%0.2f,剩余库存：%0.2f,库存衰减：%0.2f"),m_pITableFrame->GetTableID()+1,lCurrentStorgeNum,lSystemScore,RevenueScore,m_lStockScore,lStorageDeduct);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息


				//梦成网络添加
				m_lStockLimit+=lSystemScore;

				CString szlStockLimit;
				szlStockLimit.Format(TEXT("%0.2f"),m_lStockLimit);
				//记录总输赢记录
				WritePrivateProfileString(m_pGameServiceOption->szServerName,TEXT("StorageLimit"),szlStockLimit,m_szFileName);


				///////////////////////////////////


				//如果存在控制用户的话//发送游戏信息给控制号
				for(int i=0; i<m_wPlayerCount; i++)
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
				for(int i=0; i<m_wPlayerCount; i++)
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

			if(wChairID==GAME_PLAYER)
			{
				//结束数据
				CopyMemory(&m_StGameEnd,&GameEnd,sizeof(m_StGameEnd));
				m_pITableFrame->SetGameTimer(IDI_GAME_OPENEND,TIME_GAME_OPENEND,1,0);
			}
			else
			{
				//发送信息
				m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));

				TraceMessage(m_GameRecord,m_wTableID);

				//结束游戏
				m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
			}

			ReadConfigInformation(true);
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
			StatusFree.lCellScore= m_lCellScore;
			StatusFree.wMaxTimes = m_pGameCustomRule->wMaxTimes;
				//房间名称
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));


			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));
		}
	case GAME_STATUS_PLAY:	//游戏状态
		{
			//构造数据
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay,sizeof(StatusPlay));

			//加注信息
			StatusPlay.lMaxCellScore=m_lMaxCellScore;
			StatusPlay.lCellScore=m_lCellScore_Long;
			StatusPlay.lCurrentTimes=m_lCurrentTimes;
			StatusPlay.lUserMaxScore=m_lUserMaxScore[wChiarID];
			StatusPlay.wMaxTimes = m_pGameCustomRule->wMaxTimes;

			//设置变量
			StatusPlay.wBankerUser=m_wBankerUser;
			StatusPlay.wCurrentUser=m_wCurrentUser;
			CopyMemory(StatusPlay.bMingZhu,m_bMingZhu,sizeof(m_bMingZhu));
			CopyMemory(StatusPlay.lTableScore,m_lTableScore,sizeof(m_lTableScore));
			CopyMemory(StatusPlay.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

			//当前状态
			StatusPlay.bCompareState=(m_lCompareCount<=0)?false:true;

			//设置扑克
			CopyMemory(&StatusPlay.cbHandCardData,&m_cbHandCardData[wChiarID],MAX_COUNT);

				//房间名称
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//发送场景
			return m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));
		}
	}

	CString strLog;
	strLog.Format(L"\ncbGameStatus = %d",cbGameStatus);
	OutputDebugString(strLog);

	//效验错误
	ASSERT(FALSE);

	return false;
}

//定时器事件 
bool  CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	//结束定时
	if(wTimerID==IDI_GAME_COMPAREEND || IDI_GAME_OPENEND==wTimerID)
	{
		//删除时间
		if(wTimerID==IDI_GAME_COMPAREEND)m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
		else m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

		ASSERT(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY);

		if(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY)
		{
			//发送信息
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));

			TraceMessage(m_GameRecord,m_wTableID);

			//结束游戏
			m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
		}
	}

	return false;
}

//游戏消息处理
bool  CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	try{
	switch (wSubCmdID)
	{
	case SUB_C_GIVE_UP:			//用户放弃
		{
			//效验消息
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			//状态判断
			ASSERT(m_cbPlayStatus[pIServerUserItem->GetChairID()]==TRUE);
			if (m_cbPlayStatus[pIServerUserItem->GetChairID()]==FALSE) return false;

			//消息处理
			return OnUserGiveUp(pIServerUserItem->GetChairID());
		}
	case SUB_C_LOOK_CARD:		//用户看牌
	case SUB_C_OPEN_CARD:		//用户开牌
		{
			//效验消息
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();
			//状态判断
			ASSERT(m_cbPlayStatus[wChairID]==TRUE);
			if (m_cbPlayStatus[wChairID]==FALSE) return false;

			//消息处理
			if(wSubCmdID==SUB_C_LOOK_CARD)return OnUserLookCard(wChairID);
			return OnUserOpenCard(wChairID);
		}
	case SUB_C_COMPARE_CARD:	//用户比牌
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_CompareCard));
			if (wDataSize!=sizeof(CMD_C_CompareCard)) return false;

			//变量定义
			CMD_C_CompareCard * pCompareCard=(CMD_C_CompareCard *)pData;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//参数效验
			ASSERT(wChairID==m_wCurrentUser);
			if(wChairID!=m_wCurrentUser)return false;

			//状态判断
			ASSERT(m_cbPlayStatus[wChairID]==TRUE && m_cbPlayStatus[pCompareCard->wCompareUser]==TRUE);
			if(m_cbPlayStatus[wChairID]==FALSE || m_cbPlayStatus[pCompareCard->wCompareUser]==FALSE)return false;

			//消息处理
			return OnUserCompareCard(wChairID,pCompareCard->wCompareUser);
		}
	case SUB_C_ADD_SCORE:		//用户加注
		{
			//效验数据
			ASSERT(wDataSize==sizeof(CMD_C_AddScore));
			if (wDataSize!=sizeof(CMD_C_AddScore)) return false;

			//变量定义
			CMD_C_AddScore * pAddScore=(CMD_C_AddScore *)pData;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//状态判断
			ASSERT(m_cbPlayStatus[wChairID]==TRUE);
			if (m_cbPlayStatus[wChairID]==FALSE) return false;

			//当前状态
			if(pAddScore->wState>0)m_lCompareCount=pAddScore->lScore;

			//消息处理
			return OnUserAddScore(wChairID,pAddScore->lScore,false,((pAddScore->wState>0)?true:false));
		}
	case SUB_C_WAIT_COMPARE:	//等待比牌
		{
			//效验消息
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//状态判断
			ASSERT(m_cbPlayStatus[wChairID]==TRUE);
			if (m_cbPlayStatus[wChairID]==FALSE) return false;

			//消息处理
			CMD_S_WaitCompare WaitCompare;
			WaitCompare.wCompareUser = wChairID;
			if(m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(WaitCompare)) &&
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_WAIT_COMPARE,&WaitCompare,sizeof(WaitCompare)) )
				return true;
		}
	case SUB_C_FINISH_FLASH:	//完成动画 
		{
												//效验消息
			ASSERT(wDataSize==0);
			if (wDataSize!=0) return false;

			//用户效验
			if (pIServerUserItem->GetUserStatus()!=US_PLAYING) return true;

			WORD wChairID = pIServerUserItem->GetChairID();

			//状态判断
			ASSERT(m_wFlashUser[wChairID]==TRUE);
			if (m_wFlashUser[wChairID]==FALSE) return true;

			//设置变量
			m_wFlashUser[wChairID] = FALSE;

			//结束游戏
			WORD i=0;
			for( i=0;i<GAME_PLAYER;i++)
			{
				if(m_wFlashUser[i] != FALSE)break;
			}	
			if(i==GAME_PLAYER)
			{
				//删除时间
				m_pITableFrame->KillGameTimer(IDI_GAME_COMPAREEND);
				m_pITableFrame->KillGameTimer(IDI_GAME_OPENEND);

				ASSERT(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY);
				//发送信息
				if(m_pITableFrame->GetGameStatus()==GAME_STATUS_PLAY)
				{
					m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));
					m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_END,&m_StGameEnd,sizeof(m_StGameEnd));

					TraceMessage(m_GameRecord,m_wTableID);

					//结束游戏
					
					m_pITableFrame->ConcludeGame(GAME_STATUS_FREE,m_GameRecord,608);
				}
			}
			return true;
		}
		
		case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
			return OnSubAmdinCommand(pIServerUserItem,pData);
		}

	}
	}catch(...){
		CString szerror ;
		szerror.Format(TEXT("%d，%d，%s"),wSubCmdID,wDataSize,TEXT("游戏消息错误！"));
		CTraceService::TraceString(szerror,TraceLevel_Exception);
	}
	return false;
}

//框架消息处理
bool  CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}



//用户坐下
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}

//用户起立
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	return true;
}
//放弃事件
bool CTableFrameSink::OnUserGiveUp(WORD wChairID,bool bExit)
{
	//设置数据
	m_cbPlayStatus[wChairID] = FALSE;


	//修改积分
	tagScoreInfo ScoreInfoArray;
	ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
	ScoreInfoArray.cbType =(bExit)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
	ScoreInfoArray.lRevenue=0;  
	ScoreInfoArray.lScore=-Double_Round(m_lTableScore[wChairID]/100.00,2,1);
	m_pITableFrame->WriteUserScore(wChairID,ScoreInfoArray);

	//游戏记录
	if(bExit)
		m_GameRecord.AppendFormat( TEXT("P%ld:离,%0.2f#"),wChairID,ScoreInfoArray.lScore);
	else
		m_GameRecord.AppendFormat( TEXT("P%ld:弃,%0.2f#"),wChairID,ScoreInfoArray.lScore);

	///////////////////////////////////////////////////
	//剩下的玩家有没有用户没有接直接开牌  4-8修改..
	//初始化指针
	//IServerUserItem *pIServerUserItem=NULL;

	//WORD wPlayer = 0;
	//WORD wChair = INVALID_CHAIR;
	////获取本桌机器人个数、玩家个数
	//for(WORD i=0;i<GAME_PLAYER;i++)
	//{
	//	pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
	//	if(pIServerUserItem == NULL) continue;

	//	if (wChair == INVALID_CHAIR && m_cbPlayStatus[i] == true)
	//	{
	//		wChair = pIServerUserItem->GetChairID();
	//	}

	//	if ((!pIServerUserItem->IsAndroidUser())&&m_cbPlayStatus[i] == true)
	//	{
	//		wPlayer++;
	//	}
	//}

	//if(0 == wPlayer)
	//{
	//	OnUserOpenCard(wChair);
	//	return true;
	//}

	//人数统计
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	//判断结束
	if (wPlayerCount>=2)
	{
		if (m_wCurrentUser==wChairID) OnUserAddScore(wChairID,0L,true,false);
		//发送消息
		CMD_S_GiveUp GiveUp;
		GiveUp.wGiveUpUser=wChairID;
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GIVE_UP,&GiveUp,sizeof(GiveUp));

	}
	else OnEventGameConclude(INVALID_CHAIR,NULL,GER_NO_PLAYER);

	return true;
}

//看牌事件
bool CTableFrameSink::OnUserLookCard(WORD wChairID)
{
	//状态效验
	ASSERT(m_wCurrentUser==wChairID);
	if (m_wCurrentUser!=wChairID) return false;

	//参数效验
	ASSERT(!m_bMingZhu[wChairID]);
	if (m_bMingZhu[wChairID]) return true;

	//设置参数
	m_bMingZhu[wChairID]=true;

	//构造数据
	CMD_S_LookCard LookCard;
	CopyMemory(LookCard.cbCardData,m_cbHandCardData[wChairID],sizeof(m_cbHandCardData[0]));
	LookCard.wLookCardUser=wChairID;

	m_GameRecord.AppendFormat( TEXT("P%ld:看牌#"),wChairID);

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_LOOK_CARD,&LookCard,sizeof(LookCard));

	return true;
}

//比牌事件
bool CTableFrameSink::OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID)
{
	//效验参数
	ASSERT(m_lCompareCount>0);
	if(!(m_lCompareCount>0))return false;
	ASSERT(m_wCurrentUser==m_wBankerUser || m_lTableScore[m_wCurrentUser]+m_lCompareCount>=m_lCellScore_Long);
	if(!(m_wCurrentUser==m_wBankerUser || m_lTableScore[m_wCurrentUser]+m_lCompareCount>=m_lCellScore_Long))return false;

	//游戏记录
	m_GameRecord.AppendFormat(TEXT("P%ld-P%ld比牌#"),wFirstChairID,wNextChairID);

	//比较大小
	WORD wChairID=m_GameLogic.CompareCard(m_cbHandCardData[wFirstChairID],m_cbHandCardData[wNextChairID],MAX_COUNT);

	//状态设置
	m_lCompareCount=0L;

	//胜利用户
	WORD wLostUser,wWinUser;
	if(wChairID==TRUE) 
	{
		wWinUser=wFirstChairID;
		wLostUser=wNextChairID;
	}
	else
	{
		wWinUser=wNextChairID;
		wLostUser=wFirstChairID;
	}

	//设置数据
	m_wCompardUser[wLostUser].Add(wWinUser);
	m_wCompardUser[wWinUser].Add(wLostUser);
	m_cbPlayStatus[wLostUser]=FALSE;

	//人数统计
	WORD wPlayerCount=0;
	for (WORD i=0;i<m_wPlayerCount;i++)
	{
		if (m_cbPlayStatus[i]==TRUE) wPlayerCount++;
	}

	///////////////////////////////////////////////////
	//剩下的玩家有没有用户没有接直接开牌  4-8修改..
	//初始化指针
	IServerUserItem *pIServerUserItem=NULL;

	WORD wPlayer = 0;
	WORD wChair = INVALID_CHAIR;
	//获取本桌机器人个数、玩家个数
	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		if (wChair == INVALID_CHAIR && US_PLAYING == pIServerUserItem->GetUserStatus())
		{
			wChair = pIServerUserItem->GetChairID();
		}

		if (pIServerUserItem!=NULL)
		{
			if (!pIServerUserItem->IsAndroidUser())
			{
				wPlayer++;
			}
		}
	}

	if(0 == wPlayer)
	{
		OnUserOpenCard(wChair);
		return true;
	}

	//继续游戏
	if(wPlayerCount>=2)
	{
		if(m_cbPlayStatus[wFirstChairID]==TRUE)
		{
			m_wCurrentUser=wFirstChairID;
		}
		else
		{
			//用户切换
			WORD wNextPlayer=INVALID_CHAIR;
			for (WORD i=1;i<m_wPlayerCount;i++)
			{
				//设置变量
				wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;

				//继续判断
				if (m_cbPlayStatus[wNextPlayer]==TRUE) break;
			}
			//设置用户
			m_wCurrentUser=wNextPlayer;
		}
	}
	else m_wCurrentUser=INVALID_CHAIR;

	//构造数据
	CMD_S_CompareCard CompareCard;
	CompareCard.wCurrentUser = m_wCurrentUser;
	CompareCard.wLostUser = wLostUser;
	CompareCard.wCompareUser[0] = wFirstChairID;
	CompareCard.wCompareUser[1] = wNextChairID;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_COMPARE_CARD,&CompareCard,sizeof(CompareCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_COMPARE_CARD,&CompareCard,sizeof(CompareCard));

	//修改积分
	tagScoreInfo ScoreInfoArray;
	ZeroMemory(&ScoreInfoArray,sizeof(ScoreInfoArray));
	ScoreInfoArray.cbType =SCORE_TYPE_LOSE;                                
	ScoreInfoArray.lScore=-(m_lTableScore[wLostUser]/100.00);

	m_pITableFrame->WriteUserScore(wLostUser,ScoreInfoArray);

	//游戏记录
	m_GameRecord.AppendFormat( TEXT("P%ld:%0.2f#"),wLostUser,ScoreInfoArray.lScore);

	CString strLog;
	strLog.Format(L"\nwPlayerCount = %d",wPlayerCount);
	OutputDebugString(strLog);

	//结束游戏
	if (wPlayerCount<2)
	{
		m_wBankerUser=wWinUser;
		m_wFlashUser[wNextChairID] = TRUE;
		m_wFlashUser[wFirstChairID] = TRUE;
		OnEventGameConclude(GAME_PLAYER,NULL,GER_COMPARECARD);
	}

	return true;
}

//开牌事件
bool CTableFrameSink::OnUserOpenCard(WORD wUserID,bool bSystemOpen)
{
	//效验参数
	ASSERT(m_lCompareCount>0);
	if(!(m_lCompareCount>0))return false;
	ASSERT(m_wCurrentUser==wUserID);
	if(m_wCurrentUser!=wUserID)return false;

	//记录信息
	if(bSystemOpen)
		m_GameRecord+= TEXT("系统开牌#");
	else
	{
		m_GameRecord.AppendFormat( TEXT("P%ld,开牌#"),wUserID);
	}

	//清理数据
	m_wCurrentUser=INVALID_CHAIR;
	m_lCompareCount = 0L;

	//保存扑克
	BYTE cbUserCardData[GAME_PLAYER][MAX_COUNT];
	CopyMemory(cbUserCardData,m_cbHandCardData,sizeof(cbUserCardData));

	//比牌玩家
	WORD wWinner=wUserID;

	//查找最大玩家
	for (WORD i=1;i<m_wPlayerCount;i++)
	{
		WORD w=(wUserID+i)%m_wPlayerCount;

		//用户过滤
		if (m_cbPlayStatus[w]==FALSE) continue;

		//对比扑克
		if (m_GameLogic.CompareCard(cbUserCardData[w],cbUserCardData[wWinner],MAX_COUNT)>=TRUE) 
		{
			wWinner=w;
		}
	}
	ASSERT(m_cbPlayStatus[wWinner]==TRUE);
	if(m_cbPlayStatus[wWinner]==FALSE)return false;

	//胜利玩家
	m_wBankerUser = wWinner;

	//构造数据
	CMD_S_OpenCard OpenCard;
	OpenCard.wWinner=wWinner;
	OpenCard.bSystem=bSystemOpen;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_OPEN_CARD,&OpenCard,sizeof(OpenCard));

	//结束游戏
	for(WORD i=0;i<GAME_PLAYER;i++)if(m_cbPlayStatus[i]==TRUE)m_wFlashUser[i] = TRUE;
	OnEventGameConclude(GAME_PLAYER,NULL,GER_OPENCARD);

	return true;
}

//加注事件
bool CTableFrameSink::OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareCard)
{
	if (bGiveUp==false)				//设置数据
	{
		//状态效验
		ASSERT(m_wCurrentUser==wChairID);
		if (m_wCurrentUser!=wChairID) return false;
		if(lScore<0) return false;
		//金币效验
		ASSERT((lScore+m_lTableScore[wChairID])<=m_lUserMaxScore[wChairID]);
		if ((lScore+m_lTableScore[wChairID])>m_lUserMaxScore[wChairID]) return false;

		//当前倍数					bGiveUp过滤了lScore为0
		LONGLONG lTimes=(m_bMingZhu[wChairID] || bCompareCard)?2:1;

		LONGLONG lTemp=lScore/m_lCellScore_Long/lTimes;
		ASSERT(m_lCurrentTimes<=lTemp && m_lCurrentTimes<=m_lMaxCellScore/m_lCellScore_Long);
		if(!(m_lCurrentTimes<=lTemp && m_lCurrentTimes<=m_lMaxCellScore/m_lCellScore_Long))return false;
		m_lCurrentTimes = lTemp;

		//用户注金
		m_lTableScore[wChairID]+=lScore;
		m_lAddScoreTimes[wChairID]++;

		//游戏记录
		//m_GameRecord.AppendFormat( TEXT("P%ld:加,%0.2f,总%0.2f#"),wChairID,lScore/100.00,m_lTableScore[wChairID]/100.00);
	}

	bool bSystemOpen = false;
	//设置用户
	if(!bCompareCard)
	{
		//用户切换
		WORD wNextPlayer=INVALID_CHAIR;
		for (WORD i=1;i<m_wPlayerCount;i++)
		{
			//设置变量
			wNextPlayer=(m_wCurrentUser+i)%m_wPlayerCount;
			
			//继续判断
			if (m_cbPlayStatus[wNextPlayer]==TRUE)
			{
				m_wCurrentUser=wNextPlayer;
				break;
			}
		}

		CString str;
		str.Format(TEXT("wMaxTimes = %d\n"),m_pGameCustomRule->wMaxTimes);
		if(m_pGameCustomRule->wMaxTimes>0)
		{
			if(wNextPlayer!=INVALID_CHAIR && m_lTableScore[wChairID]>=(m_lCellScore_Long*m_pGameCustomRule->wMaxTimes))
			{
				bSystemOpen= true;
			}
		}
	}	

	//构造数据
	CMD_S_AddScore AddScore;
	AddScore.lCurrentTimes=m_lCurrentTimes;
	AddScore.wCurrentUser=bSystemOpen?INVALID_CHAIR:m_wCurrentUser;
	AddScore.wAddScoreUser=wChairID;
	AddScore.lAddScoreCount=lScore;

	//判断状态
	AddScore.wCompareState=( bCompareCard )?TRUE:FALSE;

	//发送数据
	m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_ADD_SCORE,&AddScore,sizeof(AddScore));

	if(bSystemOpen)
	{
		m_lCompareCount = lScore;

		OnUserOpenCard(m_wCurrentUser,true);
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

//输出信息
void TraceMessage(CString szinfo,WORD id)
{
	if(!szinfo.IsEmpty())
		szinfo.Replace(TEXT("#"),TEXT("\r\n"));
	
	COleDateTime timeNow, dateNow;
	timeNow = COleDateTime::GetCurrentTime();     // 获取当前日期时间
	dateNow = COleDateTime::GetCurrentTime();     // 同样获取当前日期时间
	CString sTime = timeNow.Format(VAR_TIMEVALUEONLY);     // 获取当前时间
	CString sDate = dateNow.Format(VAR_DATEVALUEONLY);     // 获取当前日期

	TCHAR tchBuffer[MAX_PATH ]; 
	LPTSTR lpszCurDir;
	lpszCurDir = tchBuffer; 
	GetCurrentDirectory(MAX_PATH , lpszCurDir);//得到当前程序路径

	CString Filep = lpszCurDir;
	Filep.AppendFormat(TEXT("\\%s"),GAME_NAME);

	//创建文件夹	游戏分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ创建文件夹失败！")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\TableID_%d"),id);

	//创建文件夹	桌子分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ创建文件夹失败！")+Filep,TraceLevel_Exception);
		 }
	}
	Filep.AppendFormat(TEXT("\\%s"),sDate);
	
	//创建文件夹	日期分类
	if   (!PathIsDirectory(Filep)   )   
	{
		 if   (!CreateDirectory(Filep,   NULL   )   )   
		 {
			CTraceService::TraceString(TEXT("DZ创建文件夹失败！")+Filep,TraceLevel_Exception);
		 }
	}

	
	CString FileName=TEXT("");     // 文件名 = 日期_时间.txt
	FileName.AppendFormat(TEXT("%s\\%s.txt"),Filep,sTime);

	TCHAR pFilePath[MAX_PATH];
	LPCTSTR filepath = FileName;
	int FilePathLength = FileName.GetLength();     // 获得文件名长度，CFile不能用CString，只能用string做参数，要做转换
	int i=0;
	for( i=0; i < FilePathLength ; i++)
	{
		pFilePath[i] = FileName.GetAt(i);     // CString -> string
		if( (pFilePath[i] == TEXT(':')) && (i != 1))     // 剔除':'等不能作为文件名的符号,并保留根目录后的冒号
			pFilePath[i] = '-';
	}
	pFilePath[i] = '\0';     // 文件名结束

	CFile File;
	if ((File.Open(pFilePath,CFile::modeWrite)==FALSE)&&
		(File.Open(pFilePath,CFile::modeWrite|CFile::modeCreate)==FALSE))
	{
		ASSERT(FALSE);
		return;
	}

	File.SeekToEnd();
	File.Write(szinfo.GetBuffer(),szinfo.GetLength()*2);

	szinfo.ReleaseBuffer();
	File.Flush();
	File.Close();

	return;
}
//////////////////////////////////////////////////////////////////////////
//机器人专用：发送牌
void CTableFrameSink::AndroidSendUserCard()
{

	// 发送扑克
	CMD_S_SendCard pSendCard;
	ZeroMemory(&pSendCard,sizeof(pSendCard));
	CopyMemory(pSendCard.cbCardData,m_cbHandCardData,sizeof(m_cbHandCardData));
	//复制用户状态
	CopyMemory(pSendCard.cbPlayStatus,m_cbPlayStatus,sizeof(m_cbPlayStatus));

	//获取最大牌玩家
	//最大牌值玩家索引
	int iMaxIndex=0;
	

	for(int i=0; i<GAME_PLAYER;i++)
	{
		//用户过滤
		if (m_cbPlayStatus[i]==FALSE) continue;  //如果用户当前不是游戏奖态跳过本次FOR
		//获取本局最大的牌
		BYTE cbCompareResult=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		if(cbCompareResult==0) iMaxIndex=i;
	}


	for(WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);

		if(pIServerUserItem==NULL) continue;
		// 判断机器人否 
		if(pIServerUserItem->IsAndroidUser()||(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))==true)
		{

			pSendCard.IsWinUser=false;
			//发送最大牌给机器人;
			if(i==iMaxIndex) //赢家
				pSendCard.IsWinUser=true;
	
			m_pITableFrame->SendTableData(i,SUB_S_ANDROID_GET_CARD,&pSendCard,sizeof(pSendCard));
		}
	}
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
void CTableFrameSink::ProbeJudge(WORD wChairIDCtrl,BYTE cbCheatType)
{	
	//获取用户
	IServerUserItem * pIServerCtrlUserItem=m_pITableFrame->GetTableUserItem(wChairIDCtrl);
	if(pIServerCtrlUserItem==NULL) return; //用户不存在, 不需要控制返回
	if(pIServerCtrlUserItem->GetUserID()!=m_dwCheatGameID ) return; //控制ID不一致返回
	//最大牌值玩家索引
	int iMaxIndex=0;
	int iLostIndex=0;

	for(int i=0; i<GAME_PLAYER;i++)
	{
		//用户过滤
		if (m_cbPlayStatus[i]==FALSE) continue;  //如果用户当前不是游戏状态跳过本次FOR
		//获取本局最大的牌
		BYTE cbCompareResult=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		if(cbCompareResult==0) iMaxIndex=i;
	}

	//梦成网络修改8.9
	//////////////////////输家扑克 查找第二大的扑克给控制号
	if(iMaxIndex==0) iLostIndex=1;
	//对比玩家
	for (int i=0;i<GAME_PLAYER;i++)
	{
		//用户过滤
		if (m_cbPlayStatus[i]==FALSE) continue; //如果用户当前不是游戏状态跳过本次FOR
		if(iMaxIndex==i) continue; //跳出
		//对比扑克
		BYTE cbCompareResult=m_GameLogic.CompareCard(m_cbHandCardData[iLostIndex],m_cbHandCardData[i],MAX_COUNT);
		if(cbCompareResult==0) iLostIndex=i;
	}
	

	//临时变量
	BYTE cbTemp[MAX_COUNT]={0};
	//交换牌
	WORD wWinerUser=iMaxIndex;  //最大牌索引
	if(cbCheatType==CHEAT_TYPE_LOST) wWinerUser=iLostIndex; //最小牌索引

	if (wWinerUser!=wChairIDCtrl)    //如果当前用户不是控制号,则交换扑克数据 否则不需要交易
	{

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
	
	return;
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

	//最大牌值索引
	WORD iMaxIndex=INVALID_CHAIR;	

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

	//获取本局最大的牌
	for(int i=0; i<GAME_PLAYER;++i)
	{
		//用户过滤
		if (m_cbPlayStatus[i]==FALSE) continue;  //如果玩家没有在游戏，跳出

		if (iMaxIndex==INVALID_CHAIR)
		{
			iMaxIndex=i;  //初始化第一位用户
			continue;
		}

		//第一个比第二个大为真
		bool bFirstIsMax=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		//第二个比第一个大的话，把索引记录下来再比较，直接找出最大牌的玩家
		if(!bFirstIsMax) iMaxIndex=i;
	}

	//换牌变量
	int iTarget=0;//目标索引
	bool bChange = false;//是否要换牌

	//随机种子
	srand( (unsigned)time(NULL));
	//获取随机数
	WORD Ratio = rand()%(100); //产生0~100的数

	//iMaxindexitem获取
	pIServerUserItem=m_pITableFrame->GetTableUserItem(iMaxIndex);  //获取最大牌的用户
	if(pIServerUserItem == NULL) return false;   //如果用户不存在跳出
	bool ReturnValue=(m_AndroidWinningRatio>Ratio)?true:false; //如果配置比生成的数大
	//机器人赢
	if(ReturnValue)  //如果配置比生成的数大
	{
		if(pIServerUserItem->IsAndroidUser()==FALSE)//最大牌在真人手上
		{
			iTarget=iPostionAndroid[rand()%iAndroidCount];  //随机机器人目标换牌
			bChange = true;									//设置换牌变量为TRUE
		}
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
		CopyMemory(m_cbHandCardData[iTarget],cbTemp,MAX_COUNT);    //目标接收最大用户克扑
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

	//最大牌值索引
	WORD iMaxIndex=INVALID_CHAIR;	

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

	//获取本局最大的牌
	for(int i=0; i<GAME_PLAYER;++i)
	{
		//用户过滤
		if (m_cbPlayStatus[i]==FALSE) continue;  //如果玩家没有在游戏，跳出

		if (iMaxIndex==INVALID_CHAIR)
		{
			iMaxIndex=i;  //初始化第一位用户
			continue;
		}

		//第一个比第二个大为真
		bool bFirstIsMax=m_GameLogic.CompareCard(m_cbHandCardData[iMaxIndex],m_cbHandCardData[i],MAX_COUNT);
		//第二个比第一个大的话，把索引记录下来再比较，直接找出最大牌的玩家
		if(!bFirstIsMax) iMaxIndex=i;
	}

	//换牌变量
	int iTarget=0;//目标索引
	bool bChange = false;//是否要换牌

	//随机种子
	srand( (unsigned)time(NULL));
	//获取随机数
	WORD Ratio = rand()%(100); //产生0~99的数
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
	if(m_lStockScore>=m_lStorageMax)
	{	
		SCORE tmp_lStockScore = m_lStockScore;							 //原来库存
		SCORE m_StorageEating=m_lStockScore*(StorageEating/100.00);		 //系统吃分

		//减去吃掉的分
		m_lStockScore-=m_StorageEating;
		WinRatio=StorageMaxRatio;  //送分模式
		//送分模式
		StorageStyle=3;
		//输出库存值
		CString strLog;
		strLog.Format(TEXT("桌号：%d,当前库存：%0.2f,系统吃分：%0.2f,剩余库存：%0.2f"),m_pITableFrame->GetTableID()+1,tmp_lStockScore,m_StorageEating,m_lStockScore);
		CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息
	}
	
	//库存少于最小值,杀分模式
	if(m_lStockScore<=m_lStorageMin)
	{
		WinRatio=StorageMinRatio;
		StorageStyle=2;//杀分模式
	}

	//库存在最大最小之间,默认模式
	if(m_lStockScore > m_lStorageMin && m_lStockScore < m_lStorageMax)
	{
		WinRatio=StorageRatio;
		StorageStyle = 1;//默认模式
	}
	
	bool ReturnValue=(WinRatio>Ratio)?true:false; //计算机率
	//梦成网络添加
	
			    //输出当前模式
				CString strLog;
				strLog.Format(TEXT("桌号：%d,游戏模式：%d,当前输赢比率：%d (游戏模式1,正常,2,杀分,3,送分)"),m_pITableFrame->GetTableID()+1,StorageStyle,WinRatio);
				CTraceService::TraceString(strLog,TraceLevel_Info);  //输出信息

	//机器人赢
	if(ReturnValue)  //如果配置比生成的数大
	{
		if(pIServerUserItem->IsAndroidUser()==FALSE)//最大牌在真人手上
		{
			iTarget=iPostionAndroid[rand()%iAndroidCount];  //随机机器人目标换牌
			bChange = true;									//设置换牌变量为TRUE
		}
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
	if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false) return false;
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
			m_cbCheatType=AdminReq->cbCheatType;
			m_dwCheatCount=AdminReq->cbCheatCount;
			m_dwCheatGameID=AdminReq->dwGameID;
			
			

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

			m_dwCheatGameID1 = pIServerUserItem->GetUserID();

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



////////
//读取配置
void CTableFrameSink::ReadConfigInformation(bool bReadFresh)
{
	TCHAR OutBuf[255];
	//获取目录
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szPath),szPath);
	//读取配置
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ZaJinHuaConfig.ini"),szPath);
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


	//重置衰减值
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageDeduct"),TEXT("1"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageDeduct);

	//库存最大值
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageMax"),TEXT("0"),OutBuf,255,szFileName);
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%I64d"),&m_lStorageMax);

		//梦成网络添加
	m_lStorageMin=GetPrivateProfileInt(szServerName,TEXT("StorageMin"),10,m_szFileName);
	if( m_lStorageMin < 0 || m_lStorageMin > m_lStockScore ) m_lStorageMin = 0;
	m_lStorageMax=GetPrivateProfileInt(szServerName,TEXT("StorageMax"),100,m_szFileName);	
	StorageRatio=GetPrivateProfileInt(szServerName,TEXT("StorageRatio"),50,m_szFileName);
	StorageMinRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMinRatio"),50,m_szFileName);
	StorageMaxRatio=GetPrivateProfileInt(szServerName,TEXT("StorageMaxRatio"),50,m_szFileName);
	StorageEating=GetPrivateProfileInt(szServerName,TEXT("StorageEating"),50,m_szFileName);


	//系统总输赢分
	memset(OutBuf,0,255);
	GetPrivateProfileString(szServerName,TEXT("StorageLimit"),TEXT("0.0"),OutBuf,255,szFileName);
	
	//////////////////////////////////
	//梦成网络读取小数点数值
	CString TempValue;
	myscanf(OutBuf,mystrlen(OutBuf),TEXT("%s"),TempValue);
	m_lStockLimit=Mywtof(TempValue);
	/////////////////////////////////

	//梦成网络添加   //最新前台控制
	//控制玩家输赢
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

}

