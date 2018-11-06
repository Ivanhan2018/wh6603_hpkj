#include "StdAfx.h"
#include "TableFrame.h"
#include "DataBasePacket.h"
#include "AttemperEngineSink.h"

//////////////////////////////////////////////////////////////////////////////////

//断线定义
#define IDI_OFF_LINE				(TIME_TABLE_SINK_RANGE+1)			//断线标识
#define TIME_OFF_LINE				30000L								//断线时间

//游戏记录
CGameScoreRecordArray				CTableFrame::m_GameScoreRecordBuffer;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrame::CTableFrame()
{
	//固有属性
	m_wTableID=0;
	m_wChairCount=0;
	m_cbStartMode=START_MODE_ALL_READY;
    m_wUserCount=0;//<by hxh>

	//标志变量
	m_bGameStarted=false;
	m_bDrawStarted=false;
	m_bTableStarted=false;
	ZeroMemory(m_bAllowLookon,sizeof(m_bAllowLookon));
	ZeroMemory(m_lFrozenedScore,sizeof(m_lFrozenedScore));

	//游戏变量
	m_lCellScore=0;
	m_cbGameStatus=GAME_STATUS_FREE;

	//动态属性
	m_dwTableOwnerID=0L;
	ZeroMemory(m_szEnterPassword,sizeof(m_szEnterPassword));

	//时间变量
	m_dwDrawStartTime=0L;
	ZeroMemory(&m_SystemTimeStart,sizeof(m_SystemTimeStart));

	//断线变量
	ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
	ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));

	//配置信息
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件接口
	m_pITimerEngine=NULL;
	m_pITableFrameSink=NULL;
	m_pIMainServiceFrame=NULL;
	m_pIAndroidUserManager=NULL;

	//扩张接口
	m_pITableUserAction=NULL;
	m_pITableUserRequest=NULL;

	//数据接口
	m_pIKernelDataBaseEngine=NULL;
	m_pIRecordDataBaseEngine=NULL;

	//用户数组
	ZeroMemory(m_TableUserItemArray,sizeof(m_TableUserItemArray));

	return;
}

//析构函数
CTableFrame::~CTableFrame()
{
	//释放对象
	SafeRelease(m_pITableFrameSink);

	return;
}

//接口查询
VOID * CTableFrame::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrame,Guid,dwQueryVer);
	return NULL;
}


//这里通知游戏开始
//开始游戏
bool CTableFrame::StartGame()
{
	//游戏状态
	ASSERT(m_bDrawStarted==false);
	if (m_bDrawStarted==true) return false;

	//保存变量
	bool bGameStarted=m_bGameStarted;
	bool bTableStarted=m_bTableStarted;

	//设置状态
	m_bGameStarted=true;
	m_bDrawStarted=true;
	m_bTableStarted=true;
	m_dwDrawStartTime=(DWORD)time(NULL);

	//开始时间
	GetLocalTime(&m_SystemTimeStart);
	m_dwDrawStartTime=(DWORD)time(NULL);

	//开始设置
	if (bGameStarted==false)
	{
		//状态变量
		ZeroMemory(m_wOffLineCount,sizeof(m_wOffLineCount));
		ZeroMemory(m_dwOffLineTime,sizeof(m_dwOffLineTime));

		//设置用户
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//获取用户
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			//设置用户
			if (pIServerUserItem!=NULL)
			{
				//设置状态
				BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
				if ((cbUserStatus!=US_OFFLINE)&&(cbUserStatus!=US_PLAYING)) pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,i);
			}
		}

		//发送状态
		if (bTableStarted!=m_bTableStarted) SendTableStatus();             
	}

	//通知事件
	ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink!=NULL) m_pITableFrameSink->OnEventGameStart();   //通知子游戏开始游戏

	return true;
}

//解散游戏
bool CTableFrame::DismissGame()
{
	//状态判断
	ASSERT(m_bTableStarted==true);
	if (m_bTableStarted==false) return false;

	//结束游戏
	if ((m_bGameStarted==true)&&(m_pITableFrameSink->OnEventGameConclude(INVALID_CHAIR,NULL,GER_DISMISS)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//设置状态
	if ((m_bGameStarted==false)&&(m_bTableStarted==true))
	{
		//设置变量
		m_bTableStarted=false;

		//发送状态
		SendTableStatus();
	}

	return false;
}

//结束游戏
bool CTableFrame::ConcludeGame(BYTE cbGameStatus,LPCTSTR szDrawInfo,WORD cbMcYes)
{
	//效验状态
	ASSERT(m_bGameStarted==true);
	if (m_bGameStarted==false) return false;

	//EWIN网络新增 新版本与旧版本分离
	if (cbMcYes!=608){
	
	
			//组件接口
			m_pITimerEngine=NULL;
			m_pITableFrameSink=NULL;
			m_pIMainServiceFrame=NULL;
			m_pIAndroidUserManager=NULL;

			//扩张接口
			m_pITableUserAction=NULL;
			m_pITableUserRequest=NULL;

			//数据接口
			m_pIKernelDataBaseEngine=NULL;
			m_pIRecordDataBaseEngine=NULL;
	
			return false;
	
	}  
	//保存变量
	bool bDrawStarted=m_bDrawStarted;

	//设置状态
	m_bDrawStarted=false;
	m_cbGameStatus=cbGameStatus;
	m_bGameStarted=(cbGameStatus>=GAME_STATUS_PLAY)?true:false;

	//游戏记录
	if (bDrawStarted==true)
	{
		if(CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule)==true)
		{
			//变量定义
			DBR_GR_GameScoreRecord GameScoreRecord;
			ZeroMemory(&GameScoreRecord,sizeof(GameScoreRecord));

			//设置变量
			GameScoreRecord.wTableID=m_wTableID;
			GameScoreRecord.dwPlayTimeCount=(bDrawStarted==true)?(DWORD)time(NULL)-m_dwDrawStartTime:0;

			//游戏时间
			GameScoreRecord.SystemTimeStart=m_SystemTimeStart;
			GetLocalTime(&GameScoreRecord.SystemTimeConclude);

			//游戏记录
			if(szDrawInfo==NULL)
				GameScoreRecord.szDrawInfo[0] = 0;
			else
				lstrcpyn(GameScoreRecord.szDrawInfo,szDrawInfo,sizeof(GameScoreRecord.szDrawInfo));

			//用户积分
			for (INT_PTR i=0;i<m_GameScoreRecordActive.GetCount();i++)
			{
				//获取对象
				ASSERT(m_GameScoreRecordActive[i]!=NULL);
				tagGameScoreRecord * pGameScoreRecord=m_GameScoreRecordActive[i];

				//用户数目
				if (pGameScoreRecord->cbAndroid==FALSE)
				{
					GameScoreRecord.wUserCount++;
				}
				else
				{
					GameScoreRecord.wAndroidCount++;
				}

				//奖牌统计
				GameScoreRecord.lUserMedal+=pGameScoreRecord->lUserMedal;

				//统计信息
				if (pGameScoreRecord->cbAndroid==FALSE)
				{
					GameScoreRecord.lWasteCount-=pGameScoreRecord->lScore-pGameScoreRecord->lRevenue;
					GameScoreRecord.lRevenueCount+=pGameScoreRecord->lRevenue;

					//成绩信息
					if (GameScoreRecord.wRecordCount<CountArray(GameScoreRecord.GameScoreRecord))
					{
						WORD wIndex=GameScoreRecord.wRecordCount++;
						CopyMemory(&GameScoreRecord.GameScoreRecord[wIndex],pGameScoreRecord,sizeof(tagGameScoreRecord));
					}
				}				
			}

			//投递数据
			WORD wHeadSize=sizeof(GameScoreRecord)-sizeof(GameScoreRecord.GameScoreRecord);
			WORD wDataSize=sizeof(GameScoreRecord.GameScoreRecord[0])*GameScoreRecord.wRecordCount;

			//EWIN网络，注意这里查看数据包有没有超过16376的长度
					//计算数据包长度，如果超出提示出错
					if((wHeadSize+wDataSize)>16375){

						//输出错误
						CTraceService::TraceString(TEXT("游戏结束：数据包长度超出最大值16376"),TraceLevel_Exception);

					}

			m_pIRecordDataBaseEngine->PostDataBaseRequest(DBR_GR_GAME_SCORE_RECORD,0,&GameScoreRecord,wHeadSize+wDataSize);
		}

		//清理记录
		if (m_GameScoreRecordActive.GetCount()>0L)
		{
			m_GameScoreRecordBuffer.Append(m_GameScoreRecordActive);
			m_GameScoreRecordActive.RemoveAll();
		}
	}

	//结束设置
	if (m_bGameStarted==false)
	{
		//变量定义
		bool bOffLineWait=false;

		//设置用户
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//获取用户
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);

			//用户处理
			if (pIServerUserItem!=NULL)
			{
				//解锁金币
				if (m_lFrozenedScore[i]!=0L)
				{
					pIServerUserItem->UnFrozenedUserScore(m_lFrozenedScore[i]);
					m_lFrozenedScore[i]=0L;
				}

				//设置状态
				if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
				{
					//断线处理
					bOffLineWait=true;
					PerformStandUpAction(pIServerUserItem);
				}
				else
				{
					//设置状态
					pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,i);

					//机器处理
					if (pIServerUserItem->IsAndroidUser()==true)
					{
						//查找机器
						CAttemperEngineSink * pAttemperEngineSink=(CAttemperEngineSink *)m_pIMainServiceFrame;
						tagBindParameter * pBindParameter=pAttemperEngineSink->GetBindParameter(pIServerUserItem->GetBindIndex());
						IAndroidUserItem * pIAndroidUserItem=m_pIAndroidUserManager->SearchAndroidUserItem(pIServerUserItem->GetUserID(),
							pBindParameter->dwSocketID);

						//机器处理
						if (pIAndroidUserItem!=NULL)
						{
							//获取时间
							SYSTEMTIME SystemTime;
							GetLocalTime(&SystemTime);
							DWORD dwTimeMask=(1L<<SystemTime.wHour);

							//获取属性
							tagAndroidService * pAndroidService=pIAndroidUserItem->GetAndroidService();
							tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

							//设置信息
							if(pAndroidService->dwResidualPlayDraw>0)
								--pAndroidService->dwResidualPlayDraw;

							//服务时间
							if (((pIServerUserItem->GetTableID()==GetTableID())&&(pAndroidParameter->dwServiceTime&dwTimeMask)==0L))
							{
								PerformStandUpAction(pIServerUserItem);
								continue;
							}

							//游戏时间
							DWORD dwCurrentTime=(DWORD)time(NULL);
							DWORD dwLogonTime=pIServerUserItem->GetLogonTime();
							DWORD dwReposeTime=pAndroidService->dwReposeTime;
							if ((dwLogonTime+dwReposeTime)<dwCurrentTime)
							{
								PerformStandUpAction(pIServerUserItem);
								continue;
							}

							//局数判断
							if ((pIServerUserItem->GetTableID()==GetTableID())&&(pAndroidService->dwResidualPlayDraw==0))
							{
								PerformStandUpAction(pIServerUserItem);
								continue;
							}
						}
					}
				}
			}
		}

		//删除时间
		if (bOffLineWait==true) KillGameTimer(IDI_OFF_LINE);
	}

	//重置桌子
	ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink!=NULL) m_pITableFrameSink->RepositionSink();

	//踢出检测
	if (m_bGameStarted==false)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//获取用户
			if (m_TableUserItemArray[i]==NULL) continue;
			IServerUserItem * pIServerUserItem=m_TableUserItemArray[i];

			//积分限制
			if ((SCORE_EQUAL(m_pGameServiceOption->lMinTableScore,0L)==false)&&(SCORE_LESS(pIServerUserItem->GetUserScore(),m_pGameServiceOption->lMinTableScore)))
			{
				//构造提示
				TCHAR szDescribe[128]=TEXT("");
				if (m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)
				{
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("您的游戏币少于 ") SCORE_STRING TEXT("，不能继续游戏！"),m_pGameServiceOption->lMinTableScore);
				}
				else
				{
					_sntprintf(szDescribe,CountArray(szDescribe),TEXT("您的游戏币少于 ") SCORE_STRING TEXT("，不能继续游戏！"),m_pGameServiceOption->lMinTableScore);
				}

				//发送消息
				SendGameMessage(pIServerUserItem,szDescribe,SMT_CHAT|SMT_CLOSE_GAME|SMT_EJECT);

				//用户起立
				PerformStandUpAction(pIServerUserItem);

				continue;
			}

			//关闭判断
			if ((CServerRule::IsForfendGameEnter(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
			{
				//发送消息
				LPCTSTR pszMessage=TEXT("由于系统维护，当前游戏桌子禁止用户继续游戏！");
				SendGameMessage(pIServerUserItem,pszMessage,SMT_EJECT|SMT_CHAT|SMT_CLOSE_GAME);

				//用户起立
				PerformStandUpAction(pIServerUserItem);

				continue;
			}
		}
	}

	return true;
}

//写入积分
bool CTableFrame::WriteUserScore(WORD wChairID, tagScoreInfo & ScoreInfo)
{
	//效验参数
	ASSERT((wChairID<m_wChairCount)&&(ScoreInfo.cbType!=SCORE_TYPE_NULL));
	if ((wChairID>=m_wChairCount)&&(ScoreInfo.cbType==SCORE_TYPE_NULL)) return false;

	//获取用户
	ASSERT(GetTableUserItem(wChairID)!=NULL);
	IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

	//写入积分
	if (pIServerUserItem!=NULL)
	{
		//游戏时间
		DWORD dwCurrentTime=(DWORD)time(NULL);
		DWORD dwPlayTimeCount=(m_bDrawStarted==true)?dwCurrentTime-m_dwDrawStartTime:0L;

		//奖牌计算
		SCORE lUserMemal=SCORE_LESS(ScoreInfo.lScore,0L)?-ScoreInfo.lScore:0;

		//写入积分
		pIServerUserItem->WriteUserScore(ScoreInfo.lScore,ScoreInfo.lRevenue,lUserMemal,ScoreInfo.cbType,dwPlayTimeCount);

		//游戏记录
		if (CServerRule::IsRecordGameScore(m_pGameServiceOption->dwServerRule)==true)
		{
			//变量定义
			tagGameScoreRecord * pGameScoreRecord=NULL;

			//查询库存
			if (m_GameScoreRecordBuffer.GetCount()>0L)
			{
				//获取对象
				INT_PTR nCount=m_GameScoreRecordBuffer.GetCount();
				pGameScoreRecord=m_GameScoreRecordBuffer[nCount-1];

				//删除对象
				m_GameScoreRecordBuffer.RemoveAt(nCount-1);
			}

			//创建对象
			if (pGameScoreRecord==NULL)
			{
				try
				{
					//创建对象
					pGameScoreRecord=new tagGameScoreRecord;
					if (pGameScoreRecord==NULL) throw TEXT("游戏记录对象创建失败");
				}
				catch (...)
				{
					ASSERT(FALSE);
				}
			}

			//记录数据
			if (pGameScoreRecord!=NULL)
			{
				//用户信息
				pGameScoreRecord->wChairID=wChairID;
				pGameScoreRecord->dwUserID=pIServerUserItem->GetUserID();
				pGameScoreRecord->cbAndroid=(pIServerUserItem->IsAndroidUser()?TRUE:FALSE);

				//成绩信息
				pGameScoreRecord->lScore=ScoreInfo.lScore;
				pGameScoreRecord->lGrade=ScoreInfo.lGrade;
				pGameScoreRecord->lRevenue=ScoreInfo.lRevenue;
				pGameScoreRecord->lUserMedal=lUserMemal;

				//附加信息
				pGameScoreRecord->dwPlayTimeCount=dwPlayTimeCount;

				//插入数据
				m_GameScoreRecordActive.Add(pGameScoreRecord);
			}
		}
	}
	else
	{
		//离开用户
		CTraceService::TraceString(TEXT("系统暂时未支持离开用户的补分操作处理！"),TraceLevel_Exception);

		return false;
	}

	return true;
}

//写入积分
bool CTableFrame::WriteTableScore(tagScoreInfo ScoreInfoArray[], WORD wScoreCount)
{
	//效验参数
	ASSERT(wScoreCount==m_wChairCount);
	if (wScoreCount!=m_wChairCount) return false;

	//写入分数
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (ScoreInfoArray[i].cbType!=SCORE_TYPE_NULL)
		{
			WriteUserScore(i,ScoreInfoArray[i]);
		}
	}

	return true;
}

//计算税收
SCORE CTableFrame::CalculateRevenue(WORD wChairID, SCORE lScore)
{
	//效验参数
	ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return 0L;

	//计算税收
	if (lScore>=REVENUE_BENCHMARK)
	{
		//获取用户
		ASSERT(GetTableUserItem(wChairID)!=NULL);
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);

		//计算税收
		SCORE lRevenue=lScore*m_pGameServiceOption->cbRevenueRatio/REVENUE_DENOMINATOR;

		//舍入处理
		lRevenue = Double_Round(lRevenue,2,1);

		return lRevenue;
	}

	return 0L;
}

//消费限额
SCORE CTableFrame::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	//用户效验
	ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	if (pIServerUserItem->GetTableID()!=m_wTableID) return 0L;

	//查询额度
	SCORE lTrusteeScore=pIServerUserItem->GetTrusteeScore();
	SCORE lMinEnterScore=m_pGameServiceOption->lMinTableScore;
	SCORE lUserConsumeQuota=m_pITableFrameSink->QueryConsumeQuota(pIServerUserItem);

	//效验额度
	ASSERT(SCORE_GREATER_EQUAL(lUserConsumeQuota,0L)&&SCORE_LESS_EQUAL(lUserConsumeQuota,pIServerUserItem->GetUserScore()-lMinEnterScore));
	if (SCORE_LESS(lUserConsumeQuota,0L)||SCORE_GREATER(lUserConsumeQuota,pIServerUserItem->GetUserScore()-lMinEnterScore)) return 0L;

	return lUserConsumeQuota+lTrusteeScore;
}

//寻找用户
IServerUserItem * CTableFrame::SearchUserItem(DWORD dwUserID)
{
	//变量定义
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//桌子用户
	for (WORD i=0;i<m_wChairCount;i++)
	{
		pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	}

	//旁观用户
	do
	{
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->GetUserID()==dwUserID)) return pIServerUserItem;
	} while (pIServerUserItem!=NULL);

	return NULL;
}

//游戏用户
IServerUserItem * CTableFrame::GetTableUserItem(WORD wChairID)
{
	//效验参数
	ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return NULL;

	//获取用户
	return m_TableUserItemArray[wChairID];
}

//旁观用户
IServerUserItem * CTableFrame::EnumLookonUserItem(WORD wEnumIndex)
{
	if (wEnumIndex>=m_LookonUserItemArray.GetCount()) return NULL;
	return m_LookonUserItemArray[wEnumIndex];
}

//设置时间
bool CTableFrame::SetGameTimer(DWORD dwTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM dwBindParameter)
{
	//效验参数
	ASSERT((dwTimerID>0)&&(dwTimerID<TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>=TIME_TABLE_MODULE_RANGE)) return false;

	//设置时间
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->SetTimer(dwEngineTimerID+dwTimerID,dwElapse,dwRepeat,dwBindParameter);

	return true;
}

//删除时间
bool CTableFrame::KillGameTimer(DWORD dwTimerID)
{
	//效验参数
	ASSERT((dwTimerID>0)&&(dwTimerID<=TIME_TABLE_MODULE_RANGE));
	if ((dwTimerID<=0)||(dwTimerID>TIME_TABLE_MODULE_RANGE)) return false;

	//删除时间
	DWORD dwEngineTimerID=IDI_TABLE_MODULE_START+m_wTableID*TIME_TABLE_MODULE_RANGE;
	if (m_pITimerEngine!=NULL) m_pITimerEngine->KillTimer(dwEngineTimerID+dwTimerID);

	return true;
}

//发送数据
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)
{
	//状态效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//发送数据
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

	return true;
}

//发送数据
bool CTableFrame::SendUserItemData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//状态效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)&&(pIServerUserItem->IsClientReady()==false)) return false;

	//发送数据
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

	return true;
}

//发送数据
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID)
{
	//用户群发
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//获取用户
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;

			//效验状态
			ASSERT(pIServerUserItem->IsClientReady()==true);
			if (pIServerUserItem->IsClientReady()==false) continue;

			//发送数据
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

		return true;
	}
	else
	{
		//获取用户
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) return false;

		//效验状态
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//发送数据
		m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);

		return true;
	}

	return false;
}

//发送数据
bool CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//用户群发
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//获取用户
			IServerUserItem * pIServerUserItem=GetTableUserItem(i);
			if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

			//发送数据
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}

		return true;
	}
	else
	{
		//获取用户
		IServerUserItem * pIServerUserItem=GetTableUserItem(wChairID);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

		CString strLog;
	    strLog.Format(L"SendTableData wChairID=%d",wChairID);
		OutputDebugStringW(strLog);

		//发送数据
		m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);

		return true;
	}

	return false;
}

//发送数据
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID)
{
	//变量定义
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//枚举用户
	do
	{
		//获取用户
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//效验状态
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//发送数据
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,NULL,0);
		}

	} while (true);

	return true;
}

//发送数据
bool CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//变量定义
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//枚举用户
	do
	{
		//获取用户
		pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//效验状态
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//发送数据
		if ((wChairID==INVALID_CHAIR)||(pIServerUserItem->GetChairID()==wChairID))
		{
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}

	} while (true);

	return true;
}

//发送消息
bool CTableFrame::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//变量定义
	WORD wEnumIndex=0;

	//发送消息
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

		//发送消息
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
	}

	//枚举用户
	do
	{
		//获取用户
		IServerUserItem * pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
		if (pIServerUserItem==NULL) break;

		//效验状态
		ASSERT(pIServerUserItem->IsClientReady()==true);
		if (pIServerUserItem->IsClientReady()==false) return false;

		//发送消息
		m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);

	} while (true);

	return true;
}

//房间消息
bool CTableFrame::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送消息
	m_pIMainServiceFrame->SendRoomMessage(pIServerUserItem,lpszMessage,wType);

	return true;
}

//游戏消息
bool CTableFrame::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送消息
	return m_pIMainServiceFrame->SendGameMessage(pIServerUserItem,lpszMessage,wType);
}

//发送场景
bool CTableFrame::SendGameScene(IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
{
	//用户效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->IsClientReady()==true));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) return false;

	//发送场景
	ASSERT(m_pIMainServiceFrame!=NULL);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_SCENE,pData,wDataSize);

	return true;
}

//断线事件
bool CTableFrame::OnEventUserOffLine(IServerUserItem * pIServerUserItem)
{
	//参数效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

 //   // add by hxh 20161102
 //   bool bRet=PerformStandUpAction(pIServerUserItem);
	//return bRet;

	//用户变量
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=m_TableUserItemArray[pUserInfo->wChairID];

	//用户属性
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//游戏用户
	if (cbUserStatus!=US_LOOKON)
	{
		//效验用户
		ASSERT(pIServerUserItem==GetTableUserItem(wChairID));
		if (pIServerUserItem!=GetTableUserItem(wChairID)) return false;

		//断线处理
		if (cbUserStatus==US_PLAYING)
		{
			//重断处理
			if (m_dwOffLineTime[wChairID]!=0L)
			{
				//用户设置
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);

				return true;
			}

			//事件通知
			if (m_pITableUserAction!=NULL)
			{
				m_pITableUserAction->OnActionUserOffLine(wChairID,pIServerUserItem);
			}

			//设置变量
			m_wOffLineCount[wChairID]++;
			m_dwOffLineTime[wChairID]=(DWORD)time(NULL);

			//断线通知
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);

			//时间设置
			WORD wOffLineCount=GetOffLineUserCount();
			if (wOffLineCount==1) SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE,1,wChairID);

			return true;
		}
	}

	//用户起立
	PerformStandUpAction(pIServerUserItem);

	//删除用户
	ASSERT(pIServerUserItem->GetUserStatus()==US_FREE);
	pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);

	return true;
}

//重进事件
bool CTableFrame::OnEventUserReConnect(IServerUserItem * pIServerUserItem)
{
	//参数效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//用户变量
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=m_TableUserItemArray[pUserInfo->wChairID];

	//用户属性
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//设置变量
	m_dwOffLineTime[wChairID]=0L;

	return true;
}

//时间事件
bool CTableFrame::OnEventTimer(DWORD dwTimerID, WPARAM dwBindParameter)
{
	//回调事件
	if ((dwTimerID>=0)&&(dwTimerID<TIME_TABLE_SINK_RANGE))
	{
		ASSERT(m_pITableFrameSink!=NULL);
		return m_pITableFrameSink->OnTimerMessage(dwTimerID,dwBindParameter);
	}

	//事件处理
	switch (dwTimerID)
	{
	case IDI_OFF_LINE:	//断线事件
		{
			//效验状态
			ASSERT(m_bGameStarted==true);
			if (m_bGameStarted==false) return false;

			//变量定义
			DWORD dwOffLineTime=0L;
			WORD wOffLineChair=INVALID_CHAIR;

			//寻找用户
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_dwOffLineTime[i]!=0L)&&((m_dwOffLineTime[i]<dwOffLineTime)||(wOffLineChair==INVALID_CHAIR)))
				{
					wOffLineChair=i;
					dwOffLineTime=m_dwOffLineTime[i];
				}
			}

			//位置判断
			ASSERT(wOffLineChair!=INVALID_CHAIR);
			if (wOffLineChair==INVALID_CHAIR) return false;

			//用户判断
			ASSERT(dwBindParameter<m_wChairCount);
			if (wOffLineChair!=(WORD)dwBindParameter)
			{
				//时间计算
				DWORD dwCurrentTime=(DWORD)time(NULL);
				DWORD dwLapseTime=dwCurrentTime-m_dwOffLineTime[wOffLineChair];

				//设置时间
				dwLapseTime=__min(dwLapseTime,TIME_OFF_LINE-2000L);
				SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE-dwLapseTime,1,wOffLineChair);

				return true;
			}

			//获取用户
			ASSERT(GetTableUserItem(wOffLineChair)!=NULL);
			IServerUserItem * pIServerUserItem=GetTableUserItem(wOffLineChair);

			//结束游戏
			if (pIServerUserItem!=NULL)
			{
				//设置变量
				m_dwOffLineTime[wOffLineChair]=0L;

				//用户起立
				PerformStandUpAction(pIServerUserItem);
			}

			//继续时间
			if (m_bGameStarted==true)
			{
				//变量定义
				DWORD dwOffLineTime=0L;
				WORD wOffLineChair=INVALID_CHAIR;

				//寻找用户
				for (WORD i=0;i<m_wChairCount;i++)
				{
					if ((m_dwOffLineTime[i]!=0L)&&((m_dwOffLineTime[i]<dwOffLineTime)||(wOffLineChair==INVALID_CHAIR)))
					{
						wOffLineChair=i;
						dwOffLineTime=m_dwOffLineTime[i];
					}
				}

				//设置时间
				if (wOffLineChair!=INVALID_CHAIR)
				{
					//时间计算
					DWORD dwCurrentTime=(DWORD)time(NULL);
					DWORD dwLapseTime=dwCurrentTime-m_dwOffLineTime[wOffLineChair];

					//设置时间
					dwLapseTime=__min(dwLapseTime,TIME_OFF_LINE-2000L);
					SetGameTimer(IDI_OFF_LINE,TIME_OFF_LINE-dwLapseTime,1,wOffLineChair);
				}
			}

			return true;
		}
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//游戏事件
bool CTableFrame::OnEventSocketGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(m_pITableFrameSink!=NULL);

	//消息处理
	return m_pITableFrameSink->OnGameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//框架事件
bool CTableFrame::OnEventSocketFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//游戏处理
	if (m_pITableFrameSink->OnFrameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem)==true) return true;

	////比赛处理
	//if(m_pIGameMatchSink!=NULL && m_pIGameMatchSink->OnFrameMessage(wSubCmdID,pData,wDataSize,pIServerUserItem)==true) return true;

	//默认处理
	switch (wSubCmdID)
	{
	case SUB_GF_GAME_OPTION:	//游戏配置
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GF_GameOption));
			if (wDataSize!=sizeof(CMD_GF_GameOption)) return false;

			//变量定义
			CMD_GF_GameOption * pGameOption=(CMD_GF_GameOption *)pData;

			//获取属性
			WORD wChairID=pIServerUserItem->GetChairID();
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//断线清理
			if ((cbUserStatus!=US_LOOKON)&&((m_dwOffLineTime[wChairID]!=0L)))
			{
				//设置变量
				m_dwOffLineTime[wChairID]=0L;

				//删除时间
				WORD wOffLineCount=GetOffLineUserCount();
				if (wOffLineCount==0) KillGameTimer(IDI_OFF_LINE);
			}

			//设置状态
			pIServerUserItem->SetClientReady(true);
			if (cbUserStatus!=US_LOOKON) m_bAllowLookon[wChairID]=pGameOption->cbAllowLookon?true:false;

			//发送状态
			CMD_GF_GameStatus GameStatus;
			GameStatus.cbGameStatus=m_cbGameStatus;
			GameStatus.cbAllowLookon=m_bAllowLookon[wChairID]?TRUE:FALSE;
			m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_GAME_STATUS,&GameStatus,sizeof(GameStatus));

			//事件通知
			if ((cbUserStatus==US_PLAYING)&&(m_pITableUserAction!=NULL))
			{
				m_pITableUserAction->OnActionUserConnect(wChairID,pIServerUserItem);
			}

			//发送场景
			bool bSendSecret=((cbUserStatus!=US_LOOKON)||(m_bAllowLookon[wChairID]==true));
			m_pITableFrameSink->OnEventSendGameScene(wChairID,pIServerUserItem,m_cbGameStatus,bSendSecret);

			//开始判断
			if ((cbUserStatus==US_READY)&&(EfficacyStartGame(wChairID)==true))
			{
				StartGame(); 
			}

			return true;
		}
	case SUB_GF_USER_READY:		//用户准备  //EWIN网络
		{
			//获取属性
			WORD wChairID=pIServerUserItem->GetChairID();
			BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

			//效验状态
			ASSERT(GetTableUserItem(wChairID)==pIServerUserItem);
			if (GetTableUserItem(wChairID)!=pIServerUserItem) return false;

			//效验状态
			ASSERT(cbUserStatus==US_SIT);
			if (cbUserStatus!=US_SIT) return true;

			//分组判断
			if ((m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW)!=0)
			{
				m_pIMainServiceFrame->InsertDistribute(pIServerUserItem);
				return true;
			}

			//事件通知
			if (m_pITableUserAction!=NULL)
			{
				m_pITableUserAction->OnActionUserOnReady(wChairID,pIServerUserItem,pData,wDataSize);
			}

			// add by hxh 20161031
#if 1
			//变量定义
			CMD_GR_UserStatus UserStatus;
			ZeroMemory(&UserStatus,sizeof(UserStatus));

			//构造数据
			UserStatus.dwUserID=pIServerUserItem->GetUserID();
			UserStatus.UserStatus.wTableID=pIServerUserItem->GetTableID();
			UserStatus.UserStatus.wChairID=pIServerUserItem->GetChairID();
			UserStatus.UserStatus.cbUserStatus=US_READY;

			//游戏用户
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)/*||(pIServerUserItem->IsClientReady()==false)*/) continue;

				//发送数据
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
			}
#endif

			//开始判断
            BYTE cbUserStatus1=US_READY;
			if (EfficacyStartGame(wChairID)==false)
			{
				pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
			}
			else
			{
				StartGame();
                cbUserStatus1=US_PLAYING;
			}

			// add by hxh 20161031
#if 0
            CMD_GR_UserStatus rsp;
			ZeroMemory(&rsp,sizeof(rsp));
			rsp.dwUserID=pIServerUserItem->GetUserID();
			rsp.UserStatus.wTableID=pIServerUserItem->GetTableID();
			rsp.UserStatus.wChairID=wChairID;
            rsp.UserStatus.cbUserStatus=cbUserStatus1;
            //m_pIMainServiceFrame->SendData(BG_ALL_CLIENT,MDM_GF_FRAME,SUB_GF_USER_READY,&rsp,sizeof(rsp));
			//游戏用户
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)/*||(pIServerUserItem->IsClientReady()==false)*/) continue;

				//发送数据
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_READY,&rsp,sizeof(rsp));
			}
#endif

			return true;
		}
	case SUB_GF_USER_CHAT:		//用户聊天
		{
			//变量定义
			CMD_GF_C_UserChat * pUserChat=(CMD_GF_C_UserChat *)pData;

			//效验参数
			ASSERT(wDataSize<=sizeof(CMD_GF_C_UserChat));
			ASSERT(wDataSize>=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)));
			ASSERT(wDataSize==(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

			//效验参数
			if (wDataSize>sizeof(CMD_GF_C_UserChat)) return false;
			if (wDataSize<(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
			if (wDataSize!=(sizeof(CMD_GF_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

			//目标用户
			if ((pUserChat->dwTargetUserID!=0)&&(SearchUserItem(pUserChat->dwTargetUserID)==NULL))
			{
				ASSERT(FALSE);
				return true;
			}

			//状态判断
			if ((CServerRule::IsForfendGameChat(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0L))
			{
				SendGameMessage(pIServerUserItem,TEXT("抱歉，当前游戏房间禁止游戏聊天！"),SMT_CHAT);
				return true;
			}

			//权限判断
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserRight())==false)
			{
				SendGameMessage(pIServerUserItem,TEXT("抱歉，您没有游戏聊天的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
				return true;
			}

			//构造消息
			CMD_GF_S_UserChat UserChat;
			ZeroMemory(&UserChat,sizeof(UserChat));

			//字符过滤
			lstrcpyn(UserChat.szChatString,pUserChat->szChatString,CountArray(UserChat.szChatString));

			//构造数据
			UserChat.dwChatColor=pUserChat->dwChatColor;
			UserChat.wChatLength=pUserChat->wChatLength;
			UserChat.dwTargetUserID=pUserChat->dwTargetUserID;
			UserChat.dwSendUserID=pIServerUserItem->GetUserID();
			UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

			//游戏用户
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

				//发送数据
				WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
				WORD wSendSize=wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]);
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);
			}

			//旁观用户
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//枚举用户
			do
			{
				//获取用户
				pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
				if (pIServerUserItem==NULL) break;

				//发送数据
				if (pIServerUserItem->IsClientReady()==true)
				{
					WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
					WORD wSendSize=wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]);
					m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserChat,wSendSize);
				}
			} while (true);

			return true;
		}
	case SUB_GF_USER_EXPRESSION:	//用户表情
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GF_C_UserExpression));
			if (wDataSize!=sizeof(CMD_GF_C_UserExpression)) return false;

			//变量定义
			CMD_GF_C_UserExpression * pUserExpression=(CMD_GF_C_UserExpression *)pData;

			//目标用户
			if ((pUserExpression->dwTargetUserID!=0)&&(SearchUserItem(pUserExpression->dwTargetUserID)==NULL))
			{
				ASSERT(FALSE);
				return true;
			}

			//状态判断
			if ((CServerRule::IsForfendGameChat(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0L))
			{
				SendGameMessage(pIServerUserItem,TEXT("抱歉，当前游戏房间禁止游戏聊天！"),SMT_CHAT);
				return true;
			}

			//权限判断
			if (CUserRight::CanRoomChat(pIServerUserItem->GetUserRight())==false)
			{
				SendGameMessage(pIServerUserItem,TEXT("抱歉，您没有游戏聊天的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
				return true;
			}

			//构造消息
			CMD_GR_S_UserExpression UserExpression;
			ZeroMemory(&UserExpression,sizeof(UserExpression));

			//构造数据
			UserExpression.wItemIndex=pUserExpression->wItemIndex;
			UserExpression.dwSendUserID=pIServerUserItem->GetUserID();
			UserExpression.dwTargetUserID=pUserExpression->dwTargetUserID;

			//游戏用户
			for (WORD i=0;i<m_wChairCount;i++)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=GetTableUserItem(i);
				if ((pIServerUserItem==NULL)||(pIServerUserItem->IsClientReady()==false)) continue;

				//发送数据
				m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));
			}

			//旁观用户
			WORD wEnumIndex=0;
			IServerUserItem * pIServerUserItem=NULL;

			//枚举用户
			do
			{
				//获取用户
				pIServerUserItem=EnumLookonUserItem(wEnumIndex++);
				if (pIServerUserItem==NULL) break;

				//发送数据
				if (pIServerUserItem->IsClientReady()==true)
				{
					m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_USER_CHAT,&UserExpression,sizeof(UserExpression));
				}
			} while (true);

			return true;
		}
	case SUB_GF_LOOKON_CONFIG:		//旁观配置
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_GF_LookonConfig));
			if (wDataSize<sizeof(CMD_GF_LookonConfig)) return false;

			//变量定义
			CMD_GF_LookonConfig * pLookonConfig=(CMD_GF_LookonConfig *)pData;

			//目标用户
			if ((pLookonConfig->dwUserID!=0)&&(SearchUserItem(pLookonConfig->dwUserID)==NULL))
			{
				ASSERT(FALSE);
				return true;
			}

			//用户效验
			ASSERT(pIServerUserItem->GetUserStatus()!=US_LOOKON);
			if (pIServerUserItem->GetUserStatus()==US_LOOKON) return false;

			//旁观处理
			if (pLookonConfig->dwUserID!=0L)
			{
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//获取用户
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetUserID()!=pLookonConfig->dwUserID) continue;
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//构造消息
					CMD_GF_LookonStatus LookonStatus;
					LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

					//发送消息
					ASSERT(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));

					break;
				}
			}
			else
			{
				//设置判断
				bool bAllowLookon=(pLookonConfig->cbAllowLookon==TRUE)?true:false;
				if (bAllowLookon==m_bAllowLookon[pIServerUserItem->GetChairID()]) return true;

				//设置变量
				m_bAllowLookon[pIServerUserItem->GetChairID()]=bAllowLookon;

				//构造消息
				CMD_GF_LookonStatus LookonStatus;
				LookonStatus.cbAllowLookon=pLookonConfig->cbAllowLookon;

				//发送消息
				for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
				{
					//获取用户
					IServerUserItem * pILookonUserItem=m_LookonUserItemArray[i];
					if (pILookonUserItem->GetChairID()!=pIServerUserItem->GetChairID()) continue;

					//发送消息
					ASSERT(m_pIMainServiceFrame!=NULL);
					m_pIMainServiceFrame->SendData(pILookonUserItem,MDM_GF_FRAME,SUB_GF_LOOKON_STATUS,&LookonStatus,sizeof(LookonStatus));
				}
			}

			return true;
		}
	}

	return false;
}

//获取空位
WORD CTableFrame::GetNullChairID()
{
	//椅子搜索
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_TableUserItemArray[i]==NULL)
		{
			return i;
		}
	}

	return INVALID_CHAIR;
}

//随机空位
WORD CTableFrame::GetRandNullChairID()
{
	//椅子搜索
	WORD wIndex = rand()%m_wChairCount;
	for (WORD i=wIndex;i<m_wChairCount+wIndex;i++)
	{
		if (m_TableUserItemArray[i%m_wChairCount]==NULL)
		{
			return i%m_wChairCount;
		}
	}

	return INVALID_CHAIR;
}

//机器数目
WORD CTableFrame::GetAndroidCount()
{
	//变量定义
	WORD wAndroidCount=0;

	//数目统计
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//用户接口
		IServerUserItem * pIServerUserItem = GetTableUserItem(i);
		if ((pIServerUserItem!=NULL) && (pIServerUserItem->IsAndroidUser()==true)) 
			++wAndroidCount;
	}

	return wAndroidCount;
}

//用户数目
WORD CTableFrame::GetSitUserCount()
{
	//变量定义
	WORD wUserCount=0;

	//数目统计
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (GetTableUserItem(i)!=NULL) wUserCount++;
	}

	return wUserCount;
}

//旁观数目
WORD CTableFrame::GetLookonUserCount()
{
	//获取数目
	INT_PTR nLookonCount=m_LookonUserItemArray.GetCount();

	return (WORD)(nLookonCount);
}

//断线数目
WORD CTableFrame::GetOffLineUserCount()
{
	//变量定义
	WORD wOffLineCount=0;

	//断线人数
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_dwOffLineTime[i]!=0L) wOffLineCount++;
	}

	return wOffLineCount;
}


//桌子状况
WORD CTableFrame::GetTableUserInfo(tagTableUserInfo & TableUserInfo)
{
	//设置变量
	ZeroMemory(&TableUserInfo,sizeof(TableUserInfo));

	//用户分析
	for (WORD i=0;i<m_pGameServiceAttrib->wChairCount;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//用户类型
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			TableUserInfo.wTableUserCount++;
		}
		else
		{
			TableUserInfo.wTableAndroidCount++;
		}

		//准备判断
		if (pIServerUserItem->GetUserStatus()==US_READY)
		{
			TableUserInfo.wTableReadyCount++;
		}
	}

	//最少数目
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:		//所有准备
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_PAIR_READY:		//配对开始
		{
			TableUserInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_TIME_CONTROL:	//时间控制
		{
			TableUserInfo.wMinUserCount=1;
			break;
		}
	default:						//默认模式
		{
			TableUserInfo.wMinUserCount=m_pGameServiceAttrib->wChairCount;
			break;
		}
	}

	return TableUserInfo.wTableAndroidCount+TableUserInfo.wTableUserCount;
}

//配置桌子
bool CTableFrame::InitializationFrame(WORD wTableID, tagTableFrameParameter & TableFrameParameter)
{
	//设置变量
	m_wTableID=wTableID;
	m_wChairCount=TableFrameParameter.pGameServiceAttrib->wChairCount;

	CString strLog;
	strLog.Format(L"CTableFrame::InitializationFrame m_wTableID=[%d],m_wChairCount=[%d]",m_wTableID,m_wChairCount);
	CTraceService::TraceString(strLog,TraceLevel_Normal);

	//配置参数
	m_pGameParameter=TableFrameParameter.pGameParameter;
	m_pGameServiceAttrib=TableFrameParameter.pGameServiceAttrib;
	m_pGameServiceOption=TableFrameParameter.pGameServiceOption;

	//组件接口
	m_pITimerEngine=TableFrameParameter.pITimerEngine;
	m_pIMainServiceFrame=TableFrameParameter.pIMainServiceFrame;
	m_pIAndroidUserManager=TableFrameParameter.pIAndroidUserManager;
	m_pIKernelDataBaseEngine=TableFrameParameter.pIKernelDataBaseEngine;
	m_pIRecordDataBaseEngine=TableFrameParameter.pIRecordDataBaseEngine;

	//创建桌子
	IGameServiceManager * pIGameServiceManager=TableFrameParameter.pIGameServiceManager;
	m_pITableFrameSink=(ITableFrameSink *)pIGameServiceManager->CreateTableFrameSink(IID_ITableFrameSink,VER_ITableFrameSink);

	//错误判断
	if (m_pITableFrameSink==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置桌子
	IUnknownEx * pITableFrame=QUERY_ME_INTERFACE(IUnknownEx);
	if (m_pITableFrameSink->Initialization(pITableFrame)==false) return false;

	//设置变量
	m_lCellScore=m_pGameServiceOption->lCellScore;

	//扩展接口
	m_pITableUserAction=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserAction);
	m_pITableUserRequest=QUERY_OBJECT_PTR_INTERFACE(m_pITableFrameSink,ITableUserRequest);

	return true;
}

//起立动作
bool CTableFrame::PerformStandUpAction(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	ASSERT(pIServerUserItem->GetChairID()<=m_wChairCount);

	//用户属性
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//游戏用户
	if ((m_bGameStarted==true)&&((cbUserStatus==US_PLAYING)||(cbUserStatus==US_OFFLINE)))
	{
		//结束游戏
		BYTE cbConcludeReason=(cbUserStatus==US_OFFLINE)?GER_NETWORK_ERROR:GER_USER_LEAVE;
		m_pITableFrameSink->OnEventGameConclude(wChairID,pIServerUserItem,cbConcludeReason);

		//离开判断
		if (m_TableUserItemArray[wChairID]!=pIServerUserItem) return true;
	}

	//设置变量
	if (pIServerUserItem==pITableUserItem)
	{
		//设置变量
		m_TableUserItemArray[wChairID]=NULL;

		//事件通知
		if (m_pITableUserAction!=NULL)
		{
			m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,false);
		}

		//用户状态
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus((cbUserStatus==US_OFFLINE)?US_NULL:US_FREE,INVALID_TABLE,INVALID_CHAIR);

		//变量定义
		bool bTableLocked=IsTableLocked();
		bool bTableStarted=IsTableStarted();
		WORD wTableUserCount=GetSitUserCount();

		//设置变量<by hxh>
		m_wUserCount=wTableUserCount;

		//桌子信息
		if (wTableUserCount==0)
		{
			m_dwTableOwnerID=0L;
			m_szEnterPassword[0]=0;
		}

		//结束桌子
		if ((m_bGameStarted==false)&&(m_bTableStarted==true))
		{
			if (m_cbStartMode==START_MODE_FULL_READY) m_bTableStarted=false;
			if (m_cbStartMode==START_MODE_PAIR_READY) m_bTableStarted=false;
			if ((m_cbStartMode==START_MODE_ALL_READY)&&(wTableUserCount<=1)) m_bTableStarted=false;
		}

		//开始判断
		if (EfficacyStartGame(INVALID_CHAIR)==true)
		{
			StartGame();
		}

		//发送状态
		if ((bTableLocked!=IsTableLocked())||(bTableStarted!=IsTableStarted()))
		{
			SendTableStatus();
		}

		return true;
	}
	else
	{
		//起立处理
		for (INT_PTR i=0;i<m_LookonUserItemArray.GetCount();i++)
		{
			if (pIServerUserItem==m_LookonUserItemArray[i])
			{
				//删除子项
				m_LookonUserItemArray.RemoveAt(i);

				//事件通知
				if (m_pITableUserAction!=NULL)
				{
					m_pITableUserAction->OnActionUserStandUp(wChairID,pIServerUserItem,true);
				}

				//用户状态
				pIServerUserItem->SetClientReady(false);
				pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);

				return true;
			}
		}

		//错误断言
		ASSERT(FALSE);
	}

	return true;
}

//旁观动作
bool CTableFrame::PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//变量定义
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//状态判断
	if ((CServerRule::IsForfendGameLookon(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("抱歉，当前游戏房间禁止用户旁观！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//游戏状态
	if ((m_bTableStarted==false)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("游戏还没有开始，不能旁观此游戏桌！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//椅子判断
	if ((pITableUserItem==NULL)&&(pIServerUserItem->GetMasterOrder()==0L))
	{
		SendRequestFailure(pIServerUserItem,TEXT("您所请求的位置没有游戏玩家，无法旁观此游戏桌"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//扩展效验
	if (m_pITableUserRequest!=NULL)
	{
		//变量定义
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//坐下效验
		if (m_pITableUserRequest->OnUserRequestLookon(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//发送信息
			SendRequestFailure(pIServerUserItem,RequestResult.szFailureReason,RequestResult.cbFailureCode,REQUEST_FAILURE_CMD_CONCLUDE);

			return false;
		}
	}

	//设置用户
	m_LookonUserItemArray.Add(pIServerUserItem);

	//用户状态
	pIServerUserItem->SetClientReady(false);
	pIServerUserItem->SetUserStatus(US_LOOKON,m_wTableID,wChairID);

	//事件通知
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,true);
	}

	return true;
}


//EWIN网络
//坐下动作
bool CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT((pIServerUserItem!=NULL)&&(wChairID<m_wChairCount));
	ASSERT((pIServerUserItem->GetTableID()==INVALID_TABLE)&&(pIServerUserItem->GetChairID()==INVALID_CHAIR));

	//变量定义
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	IServerUserItem * pITableUserItem=GetTableUserItem(wChairID);

	//积分变量
	SCORE lUserScore=pIServerUserItem->GetUserScore();
	SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
	SCORE lLessEnterScore=m_pITableFrameSink->QueryLessEnterScore(wChairID,pIServerUserItem);

	//状态判断
	if ((CServerRule::IsForfendGameEnter(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->GetMasterOrder()==0))
	{
		SendRequestFailure(pIServerUserItem,TEXT("抱歉，当前游戏桌子禁止用户进入！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//动态加入
	bool bDynamicJoin=true;
	if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

	//游戏状态
	if ((m_bTableStarted==true)&&(bDynamicJoin=false))
	{
		SendRequestFailure(pIServerUserItem,TEXT("游戏已经开始了，现在不能进入游戏桌！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
		return false;
	}

	//椅子判断
	if (pITableUserItem!=NULL)
	{
		//构造信息
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("椅子已经被 [ %s ] 捷足先登了，下次动作要快点了！"),pITableUserItem->GetNickName());
		CString str;
		str.Format(L"ttt 椅子被[%s]",pITableUserItem->GetNickName());
		OutputDebugString(str);

		//发送信息
		SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);

		return false;
	}

	//积分限制
	if (((SCORE_EQUAL(lMinTableScore,0L)==false)&&SCORE_LESS(lUserScore,lMinTableScore))||((SCORE_EQUAL(lLessEnterScore,0L)==false)&&SCORE_LESS(lUserScore,lLessEnterScore)))
	{
		//构造信息
		TCHAR szDescribe[128]=TEXT("");
		_sntprintf(szDescribe,CountArray(szDescribe),TEXT("加入游戏至少需要 ") SCORE_STRING TEXT(" 游戏币，您的游戏币不够，不能加入！"),__max(lMinTableScore,lLessEnterScore));

		//发送信息
		SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NOSCORE,REQUEST_FAILURE_CMD_CONCLUDE);

		return false;
	}

	//扩展效验
	if (m_pITableUserRequest!=NULL)
	{
		//变量定义
		tagRequestResult RequestResult;
		ZeroMemory(&RequestResult,sizeof(RequestResult));

		//坐下效验
		if (m_pITableUserRequest->OnUserRequestSitDown(wChairID,pIServerUserItem,RequestResult)==false)
		{
			//发送信息
			SendRequestFailure(pIServerUserItem,RequestResult.szFailureReason,RequestResult.cbFailureCode,REQUEST_FAILURE_CMD_CONCLUDE);

			return false;
		}
	}

	//设置变量
	m_TableUserItemArray[wChairID]=pIServerUserItem;

	//用户状态
	if (IsGameStarted()==false||(m_cbStartMode!=START_MODE_TIME_CONTROL))//by hxh 20161031
	{
		//if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)==false)
        if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)==false && (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH)==0)
		{
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);
		}
		else
		{
			pIServerUserItem->SetClientReady(false);
			pIServerUserItem->SetUserStatus(US_READY,m_wTableID,wChairID);
		}
	}
	else
	{
		pIServerUserItem->SetClientReady(false);
		pIServerUserItem->SetUserStatus(US_PLAYING,m_wTableID,wChairID);
	}

	//设置变量<by hxh>
	m_wUserCount=GetSitUserCount();

	//桌子信息<by hxh 20161101>
	if (GetSitUserCount()==1)
	{
		//状态变量
		bool bTableLocked=IsTableLocked();

		//设置变量
		m_dwTableOwnerID=pIServerUserItem->GetUserID();
		//lstrcpyn(m_szEnterPassword,pUserRule->szPassword,CountArray(m_szEnterPassword));

		//发送状态
		if (bTableLocked!=IsTableLocked()) SendTableStatus();
	}

	//事件通知
	if (m_pITableUserAction!=NULL)
	{
		m_pITableUserAction->OnActionUserSitDown(wChairID,pIServerUserItem,false);
	}

	return true;
}

//桌子状态
bool CTableFrame::SendTableStatus()
{
	//变量定义
	CMD_GR_TableStatus TableStatus;
	ZeroMemory(&TableStatus,sizeof(TableStatus));

	//构造数据
	TableStatus.wTableID=m_wTableID;
	TableStatus.TableStatus.cbTableLock=IsTableLocked()?TRUE:FALSE;
	TableStatus.TableStatus.cbPlayStatus=IsTableStarted()?TRUE:FALSE;

	//电脑数据
	//m_pIMainServiceFrame->SendData(BG_ALL_CLIENT,MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));//by hxh 20160622
    m_pIMainServiceFrame->SendData(BG_COMPUTER,MDM_GR_STATUS,SUB_GR_TABLE_STATUS,&TableStatus,sizeof(TableStatus));

	//手机数据

	return true;
}

//请求失败
bool CTableFrame::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, BYTE cbFailureCode,BYTE cbCommandCode)
{
	//变量定义
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//构造数据
	RequestFailure.cbFailureCode=cbFailureCode;
	RequestFailure.cbCommandCode=cbCommandCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//发送数据
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	m_pIMainServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//开始效验
bool CTableFrame::EfficacyStartGame(WORD wReadyChairID)
{
	//状态判断
	if (m_bGameStarted==true) return false;

	//模式过滤
	if (m_cbStartMode==START_MODE_TIME_CONTROL) return false;
	if (m_cbStartMode==START_MODE_MASTER_CONTROL) return false;

	//准备人数
	WORD wReadyUserCount=0;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		//获取用户
		IServerUserItem * pITableUserItem=GetTableUserItem(i);
		if (pITableUserItem==NULL) continue;

		//用户统计
		if (pITableUserItem!=NULL)
		{
			//状态判断
			if (pITableUserItem->IsClientReady()==false) return false;
			if ((wReadyChairID!=i)&&(pITableUserItem->GetUserStatus()!=US_READY)) return false;

			//用户计数
			wReadyUserCount++;
		}
	}

	//开始处理
	switch (m_cbStartMode)
	{
	case START_MODE_ALL_READY:			//所有准备
		{
			//数目判断
			if (wReadyUserCount>=2L) return true;

			return false;
		}
	case START_MODE_FULL_READY:			//满人开始
		{
			//人数判断
			if (wReadyUserCount==m_wChairCount) return true;

			return false;
		}
	case START_MODE_PAIR_READY:			//配对开始
		{
			//数目判断
			if (wReadyUserCount==m_wChairCount) return true;
			if ((wReadyUserCount<2L)||(wReadyUserCount%2)!=0) return false;

			//位置判断
			for (WORD i=0;i<m_wChairCount/2;i++)
			{
				//获取用户
				IServerUserItem * pICurrentUserItem=GetTableUserItem(i);
				IServerUserItem * pITowardsUserItem=GetTableUserItem(i+m_wChairCount/2);

				//位置过滤
				if ((pICurrentUserItem==NULL)&&(pITowardsUserItem!=NULL)) return false;
				if ((pICurrentUserItem!=NULL)&&(pITowardsUserItem==NULL)) return false;
			}

			return true;
		}
	default:
		{
			ASSERT(FALSE);
			return false;
		}
	}

	return false;
}


//地址效验
bool CTableFrame::EfficacyIPAddress(IServerUserItem * pIServerUserItem)
{
	return true;
}

//积分效验
bool CTableFrame::EfficacyScoreRule(IServerUserItem * pIServerUserItem)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
