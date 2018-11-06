#include "StdAfx.h"
#include "ServiceUnits.h"
#include "ControlPacket.h"
#include "AttemperEngineSink.h"
#include "TraceLog.h"
//////////////////////////////////////////////////////////////////////////////////

//系统时间
#define IDI_LOAD_ANDROID_USER		(IDI_MAIN_MODULE_START+1)			//机器信息
#define IDI_REPORT_SERVER_INFO		(IDI_MAIN_MODULE_START+2)			//房间信息
#define IDI_CONNECT_CORRESPOND		(IDI_MAIN_MODULE_START+3)			//连接时间
#define IDI_DISTRIBUTE_ANDROID		(IDI_MAIN_MODULE_START+4)			//分配机器
#define IDI_DEFINITE_TIME_GROUP     (IDI_MAIN_MODULE_START+5)           //定时分组
//----add by hxh 20160627
#if 1
//#define IDI_GAME_SERVICE_PULSE		(IDI_MAIN_MODULE_START+4)			//服务脉冲
//#define IDI_DISTRIBUTE_ANDROID		(IDI_MAIN_MODULE_START+5)			//分配机器
#define IDI_DBCORRESPOND_NOTIFY		(IDI_MAIN_MODULE_START+6)			//缓存通知
#define IDI_LOAD_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+7)			//系统消息
#define IDI_SEND_SYSTEM_MESSAGE		(IDI_MAIN_MODULE_START+8)			//系统消息
#define IDI_LOAD_SENSITIVE_WORD		(IDI_MAIN_MODULE_START+9)			//加载敏感词
#define IDI_DISTRIBUTE_USER		    (IDI_MAIN_MODULE_START+10)			//分配用户
#define IDI_LOAD_INI				(IDI_MAIN_MODULE_START+11)			//读取配置
#endif

//时间定义
#define TIME_LOAD_ANDROID_USER		300/*600L*/								//加载机器
#define TIME_DEFINITE_TIME_GROUP    30L                                 //分组间隔
//----add by hxh 20160627
#if 1
#define TIME_DISTRIBUTE_ANDROID		8L									//分配用户
#define TIME_REPORT_SERVER_INFO		30L									//上报时间
#define TIME_DBCORRESPOND_NOTIFY	3L									//缓存通知时间
#define TIME_LOAD_SYSTEM_MESSAGE	3600L								//系统消息时间
#define TIME_SEND_SYSTEM_MESSAGE	10L								    //系统消息时间
#define TIME_LOAD_SENSITIVE_WORD	5L									//加载敏感词时间
#endif

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CAttemperEngineSink::CAttemperEngineSink()
{
	//状态变量
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//绑定数据
	m_pNormalParameter=NULL;
	m_pAndroidParameter=NULL;

	//状态变量
	m_pInitParameter=NULL;
	m_pGameParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pIAttemperEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;
	m_pIGameServiceManager=NULL;

	//数据引擎
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;

	//配置数据
	ZeroMemory(&m_DataConfigColumn,sizeof(m_DataConfigColumn));
	
	//新增服务端口和服务地址在配置文件中读取 zrh

	//ZeroMemory(szServerPort,sizeof(szServerPort));
	ZeroMemory(szServerAddr,sizeof(szServerAddr));

	//读取配置文件
	//工作目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szFileName[MAX_PATH]=TEXT("");
	_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\ServerInfo.ini"),szDirectory);

	//读取配置
	CWHIniData IniData;
	IniData.SetIniFilePath(szFileName);

	TCHAR szKeyAddr[20];
	ZeroMemory(&szKeyAddr,sizeof(szKeyAddr));

	_sntprintf(szKeyAddr,CountArray(szKeyAddr),_T("ServerAddr"));
	//读取配置
	IniData.ReadEncryptString(TEXT("GameServerInfo"),szKeyAddr,NULL,szServerAddr,CountArray(szServerAddr));
	
	return;
}

//析构函数
CAttemperEngineSink::~CAttemperEngineSink()
{
	//删除数据
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//删除桌子
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	return;
}

//接口查询
VOID * CAttemperEngineSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IMainServiceFrame,Guid,dwQueryVer);
	QUERYINTERFACE(IAttemperEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE(IServerUserItemSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IAttemperEngineSink,Guid,dwQueryVer);
	return NULL;
}

//启动事件
bool CAttemperEngineSink::OnAttemperEngineStart(IUnknownEx * pIUnknownEx)
{
	//绑定信息
	m_pAndroidParameter=new tagBindParameter[MAX_ANDROID];
	ZeroMemory(m_pAndroidParameter,sizeof(tagBindParameter)*MAX_ANDROID);

	//绑定信息
	m_pNormalParameter=new tagBindParameter[m_pGameServiceOption->wMaxPlayer];
	ZeroMemory(m_pNormalParameter,sizeof(tagBindParameter)*m_pGameServiceOption->wMaxPlayer);

	//配置机器
	if (InitAndroidUser()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//配置桌子
	if (InitTableFrameArray()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//设置接口
	if (m_ServerUserManager.SetServerUserItemSink(QUERY_ME_INTERFACE(IServerUserItemSink))==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//启动机器
	if (m_AndroidUserManager.StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//调整分组参数
	RectifyDistributeParameter();

	//设置规则
	m_DistributeManage.SetDistributeRule(m_pGameServiceOption->cbDistributeRule);

	//设置时间
	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_ANDROID,TIME_DISTRIBUTE_ANDROID*1000L,TIMES_INFINITY,NULL);	
	m_pITimerEngine->SetTimer(IDI_LOAD_ANDROID_USER,TIME_LOAD_ANDROID_USER*1000L,TIMES_INFINITY,NULL);
    m_pITimerEngine->SetTimer(IDI_DEFINITE_TIME_GROUP,m_pGameServiceOption->wDistributeTimeSpace*1000L,TIMES_INFINITY,NULL);	
	m_pITimerEngine->SetTimer(IDI_DISTRIBUTE_ANDROID,TIME_DISTRIBUTE_ANDROID*1000L,TIMES_INFINITY,NULL);

//----add by hxh 20160627
#if 0
#ifdef _DEBUG
	m_pITimerEngine->SetTimer(IDI_LOAD_SYSTEM_MESSAGE,15*1000L,TIMES_INFINITY,NULL);
	m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,5*1000L,TIMES_INFINITY,NULL);
#else
	m_pITimerEngine->SetTimer(IDI_LOAD_SYSTEM_MESSAGE,TIME_LOAD_SYSTEM_MESSAGE*1000L,TIMES_INFINITY,NULL);
	m_pITimerEngine->SetTimer(IDI_SEND_SYSTEM_MESSAGE,TIME_SEND_SYSTEM_MESSAGE*1000L,TIMES_INFINITY,NULL);
#endif
#endif

	return true;
}

//停止事件
bool CAttemperEngineSink::OnAttemperEngineConclude(IUnknownEx * pIUnknownEx)
{
	//状态变量
	m_bCollectUser=false;
	m_bNeekCorrespond=true;

	//配置信息
	m_pInitParameter=NULL;
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//组件变量
	m_pITimerEngine=NULL;
	m_pITCPSocketService=NULL;
	m_pITCPNetworkEngine=NULL;

	//数据引擎
	m_pIRecordDataBaseEngine=NULL;
	m_pIKernelDataBaseEngine=NULL;

	//绑定数据
	SafeDeleteArray(m_pNormalParameter);
	SafeDeleteArray(m_pAndroidParameter);

	//删除桌子
	for (INT_PTR i=0;i<m_TableFrameArray.GetCount();i++)
	{
		SafeRelease(m_TableFrameArray[i]);
	}

	//删除用户
	m_TableFrameArray.RemoveAll();
	m_DistributeManage.RemoveAll();
	m_ServerUserManager.DeleteUserItem();
	m_ServerListManager.ResetServerList();

	//停止服务
	m_AndroidUserManager.ConcludeService();

	return true;
}

//控制事件
bool CAttemperEngineSink::OnEventControl(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case CT_CONNECT_CORRESPOND:		//连接协调
		{
			//发起连接
			tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
			m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//构造提示
			TCHAR szString[512]=TEXT("");
			_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

			//提示消息
			CTraceService::TraceString(szString,TraceLevel_Normal);

			return true;
		}
	case CT_LOAD_SERVICE_CONFIG:	//加载配置
		{
			//加载配置
			m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_PARAMETER,0L,NULL,NULL);

			//加载列表
			m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_GAME_COLUMN,0L,NULL,0L);

			//加载机器
			m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_ANDROID_USER,0L,NULL,0L);

			return true;
		}
	}

	return false;
}

//调度事件
bool CAttemperEngineSink::OnEventAttemperData(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	return false;
}

//时间事件
bool CAttemperEngineSink::OnEventTimer(DWORD dwTimerID, WPARAM wBindParam)
{
	//调度时间
	if ((dwTimerID>=IDI_MAIN_MODULE_START)&&(dwTimerID<=IDI_MAIN_MODULE_FINISH))
	{
		//时间处理
		switch (dwTimerID)
		{
		case IDI_LOAD_ANDROID_USER:		//加载机器
			{
				//加载机器
				m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOAD_ANDROID_USER,0L,NULL,0L);

				return true;
			}
		case IDI_REPORT_SERVER_INFO:	//房间信息
			{
				//变量定义
				CMD_CS_C_ServerOnLine ServerOnLine;
				ZeroMemory(&ServerOnLine,sizeof(ServerOnLine));

				//设置变量
				ServerOnLine.dwOnLineCount=m_ServerUserManager.GetUserItemCount();

				//发送数据
				m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_ONLINE,&ServerOnLine,sizeof(ServerOnLine));

				return true;
			}
		case IDI_CONNECT_CORRESPOND:	//连接协调
			{
				//发起连接
				tagAddressInfo * pCorrespondAddress=&m_pInitParameter->m_CorrespondAddress;
				m_pITCPSocketService->Connect(pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//构造提示
				TCHAR szString[512]=TEXT("");
				_sntprintf(szString,CountArray(szString),TEXT("正在连接协调服务器 [ %s:%d ]"),pCorrespondAddress->szAddress,m_pInitParameter->m_wCorrespondPort);

				//提示消息
				CTraceService::TraceString(szString,TraceLevel_Normal);

				return true;
			}
		case IDI_DISTRIBUTE_ANDROID:	//分配机器
			{

				//动作处理
				if (m_AndroidUserManager.GetAndroidCount()>0)
				{
					//变量定义
					bool bAllowAndroidAttend=CServerRule::IsAllowAndroidAttend(m_pGameServiceOption->dwServerRule);
					bool bAllowAndroidSimulate=CServerRule::IsAllowAndroidSimulate(m_pGameServiceOption->dwServerRule);

					//陪打处理
					if (bAllowAndroidAttend==true)
					{
						//被动状态
						tagAndroidUserInfo AndroidPassivity;
						m_AndroidUserManager.GetAndroidUserInfo(AndroidPassivity,ANDROID_PASSIVITY);

						//被动处理
						if (AndroidPassivity.wFreeUserCount>0)
						{
							if ((m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW)!=0)
							{
								//变量定义
								WORD wMinUserCount = __max(m_pGameServiceOption->wMinDistributeUser,m_pGameServiceOption->wMinGameUser);
								WORD wAndoridUserCount = m_DistributeManage.GetAndroidCount();
								INT  nNeedAndroidCount = wMinUserCount-wAndoridUserCount-1;

								//坐下处理
								if(nNeedAndroidCount>0)
								{
									WORD wIndex = rand()%(__max(AndroidPassivity.wFreeUserCount,1));
									for (WORD j=wIndex;j<AndroidPassivity.wFreeUserCount+wIndex;++j)
									{
										//变量定义
										WORD wAndroidIndex=j%AndroidPassivity.wFreeUserCount;
										IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[wAndroidIndex];
										if(PerformUserSitDown(pIAndroidUserItem->GetMeUserItem(),INVALID_TABLE,INVALID_CHAIR)==true)
										{
											CString strLog;
											strLog.Format(L"Android 4 [%s]进入游戏",pIAndroidUserItem->GetMeUserItem()->GetNickName());
											CTraceService::TraceString(strLog,TraceLevel_Normal);

											if(--nNeedAndroidCount==0) break;
										};
									}

                                    if(nNeedAndroidCount==0) return true;
								}								
							}
							else
							{
								//坐下处理
								WORD wIndex = rand()%(__max(AndroidPassivity.wFreeUserCount,1));
								for (WORD j=wIndex;j<AndroidPassivity.wFreeUserCount+wIndex;++j)
								{
									//变量定义
									WORD wAndroidIndex=j%AndroidPassivity.wFreeUserCount;
									IAndroidUserItem * pIAndroidUserItem=AndroidPassivity.pIAndroidUserFree[wAndroidIndex];
									if(PerformUserSitDown(pIAndroidUserItem->GetMeUserItem(),INVALID_TABLE,INVALID_CHAIR))
									{
										CString strLog;
										strLog.Format(L"Android 1 [%s]进入游戏",pIAndroidUserItem->GetMeUserItem()->GetNickName());
										CTraceService::TraceString(strLog,TraceLevel_Normal);

										return true;
									}
								}
							}							
						}
					}

					//陪打处理
					if (bAllowAndroidAttend==true)
					{
						//主动状态
						tagAndroidUserInfo AndroidInitiative;
						m_AndroidUserManager.GetAndroidUserInfo(AndroidInitiative,ANDROID_INITIATIVE);

						//主动处理
						if (AndroidInitiative.wFreeUserCount>0)
						{
							if ((m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW)!=0)
							{
								//变量定义
								WORD wMinUserCount = __max(m_pGameServiceOption->wMinDistributeUser,m_pGameServiceOption->wMinGameUser);
								WORD wRealUserCount = m_DistributeManage.GetRealCount();
								WORD wAndoridUserCount = m_DistributeManage.GetAndroidCount();
								INT  nNeedAndroidCount = wMinUserCount-wRealUserCount-wAndoridUserCount-1;

								//坐下处理
								if(nNeedAndroidCount>0)
								{
									WORD wIndex = rand()%(__max(AndroidInitiative.wFreeUserCount,1));
									for (WORD j=wIndex;j<AndroidInitiative.wFreeUserCount+wIndex;++j)
									{
										//变量定义
										WORD wAndroidIndex=j%AndroidInitiative.wFreeUserCount;
										IAndroidUserItem * pIAndroidUserItem=AndroidInitiative.pIAndroidUserFree[wAndroidIndex];
										if(PerformUserSitDown(pIAndroidUserItem->GetMeUserItem(),INVALID_TABLE,INVALID_CHAIR)==true)
										{
											CString strLog;
											strLog.Format(L"Android 3 [%s]进入游戏",pIAndroidUserItem->GetMeUserItem()->GetNickName());
											CTraceService::TraceString(strLog,TraceLevel_Normal);

											if(--nNeedAndroidCount==0) break;
										};
									}

									if(nNeedAndroidCount==0) return true;
								}								
							}
							else
							{
								//坐下处理
								WORD wIndex = rand()%(__max(AndroidInitiative.wFreeUserCount,1));
								for (WORD j=wIndex;j<AndroidInitiative.wFreeUserCount+wIndex;++j)
								{
									//变量定义
									WORD wAndroidIndex=j%AndroidInitiative.wFreeUserCount;
									IAndroidUserItem * pIAndroidUserItem=AndroidInitiative.pIAndroidUserFree[wAndroidIndex];
									if(PerformUserSitDown(pIAndroidUserItem->GetMeUserItem(),INVALID_TABLE,INVALID_CHAIR))
									{
										CString strLog;
										strLog.Format(L"Android 2 [%s]进入游戏",pIAndroidUserItem->GetMeUserItem()->GetNickName());
										CTraceService::TraceString(strLog,TraceLevel_Normal);

										return true;
									}
								}
							}							
						}
					}

					//起立处理
					INT_PTR nIndex = rand()%(__max(m_pGameServiceOption->wTableCount,1));
					for (INT_PTR i=nIndex;i<m_pGameServiceOption->wTableCount+nIndex;++i)
					{
						//获取桌子
						INT_PTR nTableIndex=i%m_pGameServiceOption->wTableCount;
						CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
						if ((pTableFrame->IsGameStarted()==true)&&(bAllowAndroidSimulate==false)) continue;

						//桌子状况
						tagTableUserInfo TableUserInfo;
						WORD wUserSitCount=pTableFrame->GetTableUserInfo(TableUserInfo);

						//用户过虑
						if (TableUserInfo.wTableAndroidCount==0) continue;
						if ((TableUserInfo.wTableUserCount>0)&&(bAllowAndroidAttend==true)) continue;

						//起立处理
						for (WORD j=0;j<pTableFrame->GetChairCount();j++)
						{
							//获取用户
							IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(j);
							if (pIServerUserItem==NULL) continue;

							//玩家起立
							if ((pIServerUserItem->IsAndroidUser()==true)&&(PerformUserStandUp(pIServerUserItem,TRUE)==true))
							{
								return true;
							}
						}
					}
				}

				return  true;
		}
		case IDI_DEFINITE_TIME_GROUP:   //定时分组   //EWIN网络
			{
				//自动分组
				PerformDistribute();

				return true;
			}

	    //----add by hxh 20160627 begin
		//case IDI_LOAD_SYSTEM_MESSAGE: //系统消息
		//	{
		//		//清除消息数据
		//		ClearSystemMessageData();

		//		//加载消息
		//		m_pIDBCorrespondManager->PostDataBaseRequest(0L,DBR_GR_LOAD_SYSTEM_MESSAGE,0L,NULL,0L);
		//		return true;
		//	}
		//case IDI_SEND_SYSTEM_MESSAGE: //系统消息
		//	{
		//		//数量判断
		//		if(m_SystemMessageList.GetCount()==0) return true;

		//		//时效判断
		//		DWORD dwCurrTime = (DWORD)time(NULL);
		//		POSITION pos = m_SystemMessageList.GetHeadPosition();
		//		while(pos != NULL)
		//		{
		//			//获取消息
		//			POSITION tempPos = pos;
		//			tagSystemMessage *pTagSystemMessage = m_SystemMessageList.GetNext(pos);

		//			//时效判断
		//			if(pTagSystemMessage->SystemMessage.tConcludeTime < dwCurrTime)
		//			{
		//				m_SystemMessageList.RemoveAt(tempPos);
		//				delete pTagSystemMessage;
		//				continue;
		//			}

		//			//间隔判断
		//			if(pTagSystemMessage->dwLastTime+pTagSystemMessage->SystemMessage.dwTimeRate < dwCurrTime)
		//			{
		//				//更新数据
		//				pTagSystemMessage->dwLastTime=dwCurrTime;

		//				//构造消息
		//				CMD_GR_SendMessage SendMessage = {};
		//				SendMessage.cbAllRoom = (pTagSystemMessage->SystemMessage.cbMessageID==100)?TRUE:FALSE;
		//				SendMessage.cbGame = (pTagSystemMessage->SystemMessage.cbMessageType==1)?TRUE:FALSE;
		//				SendMessage.cbRoom = (pTagSystemMessage->SystemMessage.cbMessageType==2)?TRUE:FALSE;
		//				if(pTagSystemMessage->SystemMessage.cbMessageType==3)
		//				{
		//					SendMessage.cbGame = TRUE;
		//					SendMessage.cbRoom = TRUE;
		//				}
		//				lstrcpyn(SendMessage.szSystemMessage,pTagSystemMessage->SystemMessage.szSystemMessage,CountArray(SendMessage.szSystemMessage));
		//				SendMessage.wChatLength = lstrlen(SendMessage.szSystemMessage)+1;

		//				//发送消息
		//				WORD wSendSize = sizeof(SendMessage)-sizeof(SendMessage.szSystemMessage)+CountStringBuffer(SendMessage.szSystemMessage);
		//				SendSystemMessage(&SendMessage,wSendSize);
		//			}
		//		}


		//		return true;
		//	}
           //----add by hxh 20160627 end
		}
	}

	//机器时器
	if ((dwTimerID>=IDI_REBOT_MODULE_START)&&(dwTimerID<=IDI_REBOT_MODULE_FINISH))
	{
		//时间处理
		m_AndroidUserManager.OnEventTimerPulse(dwTimerID,wBindParam);

		return true;
	}	

	//桌子时间
	if ((dwTimerID>=IDI_TABLE_MODULE_START)&&(dwTimerID<=IDI_TABLE_MODULE_FINISH))
	{
		//桌子号码
		DWORD dwTableTimerID=dwTimerID-IDI_TABLE_MODULE_START;
		WORD wTableID=(WORD)(dwTableTimerID/TIME_TABLE_MODULE_RANGE);

		//时间效验
		if (wTableID>=(WORD)m_TableFrameArray.GetCount()) 
		{
			ASSERT(FALSE);
			return false;
		}

		//时间通知
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		return pTableFrame->OnEventTimer(dwTableTimerID%TIME_TABLE_MODULE_RANGE,wBindParam);
	}	

	return false;
}

//数据库事件
bool CAttemperEngineSink::OnEventDataBase(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	switch (wRequestID)
	{
	case DBO_GR_LOGON_SUCCESS:			//登录成功
		{
			return OnDBLogonSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_LOGON_FAILURE:			//登录失败
		{
			return OnDBLogonFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_PARAMETER:			//游戏参数
		{
			return OnDBGameParameter(dwContextID,pData,wDataSize);
		}
	case DBO_GR_GAME_COLUMN_INFO:		//列表信息
		{
			return OnDBGameColumnInfo(dwContextID,pData,wDataSize);
		}
	case DBR_GR_GAME_ANDROID_INFO:		//机器信息
		{
			//AfxMessageBox(_T("1"));
			return OnDBGameAndroidInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_INFO:		//银行信息
		{
			return OnDBUserInsureInfo(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_SUCCESS:	//银行成功
		{
			return OnDBUserInsureSuccess(dwContextID,pData,wDataSize);
		}
	case DBO_GR_USER_INSURE_FAILURE:	//银行失败
		{
			return OnDBUserInsureFailure(dwContextID,pData,wDataSize);
		}
	case DBO_GR_QUERY_IP_DESCRIBE:     //查询IP描述
		{
			return OnDBQueryIpDescribeSuccess(dwContextID,pData,wDataSize);
		}
	}

	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPSocketShut(WORD wServiceID, BYTE cbShutReason)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//设置变量
		m_bCollectUser=false;

		//删除时间
		m_pITimerEngine->KillTimer(IDI_REPORT_SERVER_INFO);

		//重连判断
		if (m_bNeekCorrespond==true)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("与协调服务器的连接关闭了，%ld 秒后将重新连接"),m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);
		}

		return true;
	}

	return false;
}

//连接事件
bool CAttemperEngineSink::OnEventTCPSocketLink(WORD wServiceID, INT nErrorCode)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		//错误判断
		if (nErrorCode!=0)
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("协调服务器连接失败 [ %ld ]，%ld 秒后将重新连接"),
				nErrorCode,m_pInitParameter->m_wConnectTime);

			//提示消息
			CTraceService::TraceString(szDescribe,TraceLevel_Warning);

			//设置时间
			ASSERT(m_pITimerEngine!=NULL);
			m_pITimerEngine->SetTimer(IDI_CONNECT_CORRESPOND,m_pInitParameter->m_wConnectTime*1000L,1,0);

			return false;
		}

		//提示消息
		CTraceService::TraceString(TEXT("正在发送游戏房间注册信息..."),TraceLevel_Normal);

		//变量定义
		CMD_CS_C_RegisterServer RegisterServer;
		ZeroMemory(&RegisterServer,sizeof(RegisterServer));

		//服务端口
		CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
		RegisterServer.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();
		//RegisterServer.wServerPort=_ttoi(szServerPort[]);//新增从配置文件获取

		//构造数据
		RegisterServer.wKindID=m_pGameServiceOption->wKindID;
		RegisterServer.wSortID=m_pGameServiceOption->wSortID;
		RegisterServer.wServerID=m_pGameServiceOption->wServerID;
		RegisterServer.lServerScore=m_pGameServiceOption->lCellScore;
		RegisterServer.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		RegisterServer.lMinServerScore=__max(m_pGameServiceOption->lMinTableScore,m_pGameServiceOption->lMinEnterScore);
		

		//EWIN网络密码房间
		RegisterServer.wServerKind=m_pGameServiceOption->wServerKind;
		lstrcpyn(RegisterServer.szServerPassWork,m_pGameServiceOption->szServerPassWork,CountArray(m_pGameServiceOption->szServerPassWork));

		
		lstrcpyn(RegisterServer.szServerName,m_pGameServiceOption->szServerName,CountArray(RegisterServer.szServerName));
		//lstrcpyn(RegisterServer.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(RegisterServer.szServerAddr));
		lstrcpyn(RegisterServer.szServerAddr,szServerAddr,CountArray(szServerAddr));//新增从配置文件读取

		//发送数据
		ASSERT(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_REGISTER,SUB_CS_C_REGISTER_SERVER,&RegisterServer,sizeof(RegisterServer));

		//设置定时器
		ASSERT(m_pITimerEngine!=NULL);
		m_pITimerEngine->SetTimer(IDI_REPORT_SERVER_INFO,TIME_REPORT_SERVER_INFO*1000L,TIMES_INFINITY,0);

		return true;
	}

	return true;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPSocketRead(WORD wServiceID, TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//协调连接
	if (wServiceID==NETWORK_CORRESPOND)
	{
		switch (Command.wMainCmdID)
		{
		case MDM_CS_REGISTER:		//注册服务
			{
				return OnTCPSocketMainRegister(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_SERVICE_INFO:	//服务信息
			{
				return OnTCPSocketMainServiceInfo(Command.wSubCmdID,pData,wDataSize);
			}
		case MDM_CS_USER_COLLECT:	//用户汇总
			{
				return OnTCPSocketMainUserCollect(Command.wSubCmdID,pData,wDataSize);
			}
		}
	}

	//错误断言
	ASSERT(FALSE);

	return true;
}

//应答事件
bool CAttemperEngineSink::OnEventTCPNetworkBind(DWORD dwClientAddr, DWORD dwSocketID)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//设置变量
	if (pBindParameter!=NULL)
	{
		pBindParameter->dwSocketID=dwSocketID;
		pBindParameter->dwClientAddr=dwClientAddr;
		pBindParameter->dwActiveTime=(DWORD)time(NULL);

		return true;
	}

	//错误断言
	ASSERT(FALSE);

	return false;
}

//关闭事件
bool CAttemperEngineSink::OnEventTCPNetworkShut(DWORD dwClientAddr, DWORD dwActiveTime, DWORD dwSocketID)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwSocketID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//获取用户
	IServerUserItem * pIServerUserItem=pBindParameter->pIServerUserItem;

	//用户处理
	if (pIServerUserItem!=NULL)
	{
#ifdef USE_RS_PRINT
		CString strLog;
		strLog.Format(L"CAttemperEngineSink::OnEventTCPNetworkShut dwSocketID=[%d],dwUserID=[%d]",dwSocketID,pIServerUserItem->GetUserID());
		CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif

		//解锁用户<add by hxh 20161117>
#if 0
		//变量定义
		DBR_GR_DeleteUserLocker DeleteUserLocker;
		ZeroMemory(&DeleteUserLocker,sizeof(DeleteUserLocker));

		//用户信息
		DeleteUserLocker.dwUserID=pIServerUserItem->GetUserID();

		//投递请求
		m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_DELETE_USER_LOCKER,0L,&DeleteUserLocker,sizeof(DeleteUserLocker));
#endif

		//删除分组
		m_DistributeManage.RemoveDistributeNode(pIServerUserItem);

		//变量定义
		WORD wTableID=pIServerUserItem->GetTableID();
        WORD wChairID=pIServerUserItem->GetChairID();

		//断线处理<正常从游戏退出是65535，刷不出信息退出是0>
		if (wTableID!=INVALID_TABLE)
		{
			//解除绑定
			pIServerUserItem->DetachBindStatus();

			//断线通知
			ASSERT(wTableID<m_pGameServiceOption->wTableCount);
			m_TableFrameArray[wTableID]->OnEventUserOffLine(pIServerUserItem);
		}
		else pIServerUserItem->SetUserStatus(US_NULL,INVALID_TABLE,INVALID_CHAIR);
	}

	//清除信息
	ZeroMemory(pBindParameter,sizeof(tagBindParameter));

	return false;
}

//读取事件
bool CAttemperEngineSink::OnEventTCPNetworkRead(TCP_Command Command, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (Command.wMainCmdID)
	{
	case MDM_GR_USER:		//用户命令
		{
			return OnTCPNetworkMainUser(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GR_LOGON:		//登录命令
		{
			return OnTCPNetworkMainLogon(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_GAME:		//游戏命令
		{
			return OnTCPNetworkMainGame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GF_FRAME:		//框架命令
		{
			return OnTCPNetworkMainFrame(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GR_INSURE:		//银行命令
		{
			return OnTCPNetworkMainInsure(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	case MDM_GR_MANAGE:		//管理命令
		{
			return OnTCPNetworkMainManage(Command.wSubCmdID,pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//插入分配
bool CAttemperEngineSink::InsertDistribute(IServerUserItem * pIServerUserItem,bool bCheckPlazaType)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//状态判断
	ASSERT(pIServerUserItem->GetUserStatus()<US_PLAYING);
	if (pIServerUserItem->GetUserStatus()>=US_PLAYING) return false;

	//检查类型
	if(bCheckPlazaType==true)
	{
		if(pIServerUserItem->GetPlazeType()==PLAZE_TYPE_MULTI)
		{
			//变量定义
			tagBindParameter * pBindParameter=GetBindParameter(pIServerUserItem->GetBindIndex());
			ASSERT(pBindParameter!=NULL);
			if(pBindParameter==NULL) return true;

			//投递请求
			m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_QUERY_IP_DESCRIBE,pBindParameter->dwSocketID,NULL,0);

			return true;
		}
	}

	//变量定义
	tagDistributeInfo DistributeInfo;
	ZeroMemory(&DistributeInfo,sizeof(DistributeInfo));

	//设置变量
	DistributeInfo.pIServerUserItem=pIServerUserItem;
	DistributeInfo.wLastTableID=pIServerUserItem->GetTableID();  //获取桌子
	DistributeInfo.dwClientAddr=pIServerUserItem->GetClientAddr();
	DistributeInfo.pPertainNode=NULL;

	//加入数组
	if(m_DistributeManage.InsertDistributeNode(DistributeInfo)==false)
	{
		return pIServerUserItem->IsAndroidUser()?false:true; 
	}

	//用户起立
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		m_TableFrameArray[wTableID]->PerformStandUpAction(pIServerUserItem);
	}

	//分配判断
	bool bDistribute=false;
	if (m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_IMMEDIATE) bDistribute=true;
	if ((m_pGameServiceOption->wMaxDistributeUser!=0)&&(m_DistributeManage.GetCount()>=m_pGameServiceOption->wMaxDistributeUser)) bDistribute=true;

	//执行分组
	if (bDistribute==true)
	{
		PerformDistribute();
	}

	return true;
}

//房间消息
bool CAttemperEngineSink::SendRoomMessage(LPCTSTR lpszMessage, WORD wType)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	m_AndroidUserManager.SendDataToClient(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_ALL_CLIENT);//by hxh 20160627

	return true;
}

//游戏消息
bool CAttemperEngineSink::SendGameMessage(LPCTSTR lpszMessage, WORD wType)
{
	//变量定义
	CMD_CM_SystemMessage SystemMessage;
	ZeroMemory(&SystemMessage,sizeof(SystemMessage));

	//构造数据
	SystemMessage.wType=wType;
	SystemMessage.wLength=lstrlen(lpszMessage)+1;
	lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

	//数据属性
	WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
	WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

	//发送数据
	m_AndroidUserManager.SendDataToClient(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
	m_pITCPNetworkEngine->SendDataBatch(MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize,BG_ALL_CLIENT);//by hxh 20160627

	return true;
}

//房间消息
bool CAttemperEngineSink::SendRoomMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//变量定义
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//构造数据
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//变量定义
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//数据属性
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//发送数据
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_CM_SYSTEM,SUB_CM_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//游戏消息
bool CAttemperEngineSink::SendGameMessage(IServerUserItem * pIServerUserItem, LPCTSTR lpszMessage, WORD wType)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if ((pIServerUserItem->GetBindIndex()!=INVALID_WORD)&&(pIServerUserItem->IsClientReady()==true))
	{
		//变量定义
		CMD_CM_SystemMessage SystemMessage;
		ZeroMemory(&SystemMessage,sizeof(SystemMessage));

		//构造数据
		SystemMessage.wType=wType;
		SystemMessage.wLength=lstrlen(lpszMessage)+1;
		lstrcpyn(SystemMessage.szString,lpszMessage,CountArray(SystemMessage.szString));

		//变量定义
		WORD dwUserIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(dwUserIndex);

		//数据属性
		WORD wHeadSize=sizeof(SystemMessage)-sizeof(SystemMessage.szString);
		WORD wSendSize=wHeadSize+CountStringBuffer(SystemMessage.szString);

		//发送数据
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,MDM_GF_FRAME,SUB_GF_SYSTEM_MESSAGE,&SystemMessage,wSendSize);
		}

		return true;
	}

	return false;
}

//发送数据
bool CAttemperEngineSink::SendData(BYTE cbSendMask, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//机器数据
	if ((cbSendMask&BG_COMPUTER)!=0)
	{
		m_AndroidUserManager.SendDataToClient(wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	//电脑数据
	m_pITCPNetworkEngine->SendDataBatch(wMainCmdID,wSubCmdID,pData,wDataSize,cbSendMask);

	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//发送数据
	if (LOWORD(dwSocketID)!=INVALID_WORD)
	{
		if (LOWORD(dwSocketID)>=INDEX_ANDROID)
		{
			//机器用户
			m_AndroidUserManager.SendDataToClient(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else 
		{
			//网络用户
			m_pITCPNetworkEngine->SendData(dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
	}

	return true;
}

//发送数据
bool CAttemperEngineSink::SendData(IServerUserItem * pIServerUserItem, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送数据
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			//机器用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_AndroidUserManager.SendDataToClient(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}
		else
		{
			//常规用户
			WORD wBindIndex=pIServerUserItem->GetBindIndex();
			tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
			m_pITCPNetworkEngine->SendData(pBindParameter->dwSocketID,wMainCmdID,wSubCmdID,pData,wDataSize);
		}

		return true;
	}

	return false;
}

//用户积分
bool CAttemperEngineSink::OnEventUserItemScore(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_UserScore UserScore;
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();

	//构造数据
	UserScore.dwUserID=pUserInfo->dwUserID;
	UserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	UserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	UserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	UserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	UserScore.UserScore.dwExperience=pUserInfo->dwExperience;

	//构造积分
	UserScore.UserScore.lScore=pUserInfo->lScore;
	UserScore.UserScore.lInsure=pUserInfo->lInsure;
	UserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	UserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//发送数据
	//SendData(BG_ALL_CLIENT,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));//by hxh 20160622
	  SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_SCORE,&UserScore,sizeof(UserScore));

	//变量定义
	CMD_GR_MobileUserScore MobileUserScore;

	//构造数据
	MobileUserScore.dwUserID=pUserInfo->dwUserID;
	MobileUserScore.UserScore.dwWinCount=pUserInfo->dwWinCount;
	MobileUserScore.UserScore.dwLostCount=pUserInfo->dwLostCount;
	MobileUserScore.UserScore.dwDrawCount=pUserInfo->dwDrawCount;
	MobileUserScore.UserScore.dwFleeCount=pUserInfo->dwFleeCount;
	MobileUserScore.UserScore.dwExperience=pUserInfo->dwExperience;

	//构造积分
	MobileUserScore.UserScore.lScore=pUserInfo->lScore;
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetTrusteeScore();
	MobileUserScore.UserScore.lScore+=pIServerUserItem->GetFrozenedScore();

	//发送数据
	SendDataBatchToMobileUser(pIServerUserItem->GetTableID(),MDM_GR_USER,SUB_GR_USER_SCORE,&MobileUserScore,sizeof(MobileUserScore));

	//即时写分
	if ((CServerRule::IsImmediateWriteScore(m_pGameServiceOption->dwServerRule)==true)&&(pIServerUserItem->IsVariation()==true))
	{
		//变量定义
		DBR_GR_WriteGameScore WriteGameScore;
		ZeroMemory(&WriteGameScore,sizeof(WriteGameScore));

		//用户信息
		WriteGameScore.dwUserID=pIServerUserItem->GetUserID();
		WriteGameScore.dwDBQuestID=pIServerUserItem->GetDBQuestID();
		WriteGameScore.dwInoutIndex=pIServerUserItem->GetInoutIndex();
		WriteGameScore.dwClientAddr=pIServerUserItem->GetClientAddr();

		//提取积分
		pIServerUserItem->DistillVariation(WriteGameScore.VariationInfo);

		//投递请求
		m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_WRITE_GAME_SCORE,0L,&WriteGameScore,sizeof(WriteGameScore));
	}

	return true;
}

//用户状态<by hxh 20160624>
#if 0
bool CAttemperEngineSink::OnEventUserItemStatus(IServerUserItem * pIServerUserItem)
#else
bool CAttemperEngineSink::OnEventUserItemStatus(IServerUserItem * pIServerUserItem, WORD wOldTableID, WORD wOldChairID)
#endif
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_UserStatus UserStatus;
	ZeroMemory(&UserStatus,sizeof(UserStatus));

	//构造数据
	UserStatus.dwUserID=pIServerUserItem->GetUserID();
	UserStatus.UserStatus.wTableID=pIServerUserItem->GetTableID();
	UserStatus.UserStatus.wChairID=pIServerUserItem->GetChairID();
	UserStatus.UserStatus.cbUserStatus=pIServerUserItem->GetUserStatus();

	//发送数据
	//SendData(BG_ALL_CLIENT,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));//by hxh 20160622
    SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));
	WORD wTableID=wOldTableID;
	if(pIServerUserItem->GetUserStatus()>=US_SIT/*||pIServerUserItem->GetUserStatus()==US_PLAYING*/)
	{
	    wTableID=pIServerUserItem->GetTableID();
	}
	SendDataBatchToMobileUser(wTableID,MDM_GR_USER,SUB_GR_USER_STATUS,&UserStatus,sizeof(UserStatus));

	//离开判断
	if (pIServerUserItem->GetUserStatus()==US_NULL)
	{
		//获取绑定
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

		//绑带处理
		if (pBindParameter!=NULL)
		{
			//绑定处理
			if (pBindParameter->pIServerUserItem==pIServerUserItem)
			{
				pBindParameter->pIServerUserItem=NULL;
			}

			//中断网络
			if (pBindParameter->dwSocketID!=0L)
			{
				if (LOWORD(pBindParameter->dwSocketID)>=INDEX_ANDROID)
				{
					m_AndroidUserManager.DeleteAndroidUserItem(pBindParameter->dwSocketID);
				}
				else
				{
					m_pITCPNetworkEngine->ShutDownSocket(pBindParameter->dwSocketID);
				}
			}
		}

		//离开处理
		OnEventUserLogout(pIServerUserItem,0L);
	}

	return true;
}

//用户信息
bool CAttemperEngineSink::OnEventUserItemSpreadInfo(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_UserSpreadInfo UserSpreadInfo;
	ZeroMemory(&UserSpreadInfo,sizeof(UserSpreadInfo));

	//构造数据
	UserSpreadInfo.dwUserID=pIServerUserItem->GetUserID();
	CopyMemory(UserSpreadInfo.UserSpreadInfo.szIpAddrDescribe,pIServerUserItem->GetUserInfo()->szAddrDescribe,sizeof(UserSpreadInfo.UserSpreadInfo.szIpAddrDescribe));

	//发送数据
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_SPREADINFO,&UserSpreadInfo,sizeof(UserSpreadInfo));//by hxh 20160622

	return true;

}
//EWIN网络,这里发送所有用户的用户信息,,包含机器人
//登录成功
bool CAttemperEngineSink::OnDBLogonSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	WORD wBindIndex=LOWORD(dwContextID);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
	DBO_GR_LogonSuccess * pDBOLogonSuccess=(DBO_GR_LogonSuccess *)pData;

	//废弃判断
	if ((pBindParameter->pIServerUserItem!=NULL)||(pBindParameter->dwSocketID!=dwContextID))
	{
		//错误断言
		ASSERT(FALSE);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_NORMAL);

		return true;
	}

	//变量定义
	bool bAndroidUser=(wBindIndex>=INDEX_ANDROID);
	bool bMobileUser=(pBindParameter->cbClientKind==CLIENT_KIND_MOBILE);

	//切换判断
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pDBOLogonSuccess->dwUserID);
	if (pIServerUserItem!=NULL)
	{
		//切换用户
		SwitchUserItemConnect(pIServerUserItem,pDBOLogonSuccess->szMachineID,wBindIndex);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_USER_IMPACT);

		return true;
	}

	//维护判断
	if ((CServerRule::IsForfendRoomEnter(m_pGameServiceOption->dwServerRule)==true)&&(pDBOLogonSuccess->cbMasterOrder==0))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，由于系统维护的原因，当前游戏房间禁止用户进入！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SYSTEM);

		return true;
	}
	
	
	//查找机器
	if (bAndroidUser==true)
	{
		//查找机器
		DWORD dwUserID=pDBOLogonSuccess->dwUserID;
		IAndroidUserItem * pIAndroidUserItem=m_AndroidUserManager.SearchAndroidUserItem(dwUserID,dwContextID);

		//修改积分
		if (pIAndroidUserItem!=NULL)
		{
			//获取配置
			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();

			//调整积分
			if ((SCORE_EQUAL(pAndroidParameter->lMinTakeScore,0L)==false)&&(SCORE_EQUAL(pAndroidParameter->lMaxTakeScore,0L)==false))
			{
				//变量定义
				LONGLONG lMinTakeScore=(LONGLONG)pAndroidParameter->lMinTakeScore;
				LONGLONG lMaxTakeScore=(LONGLONG)__max(pAndroidParameter->lMaxTakeScore,pAndroidParameter->lMinTakeScore);

				//调整积分
				if (SCORE_GREATER(lMaxTakeScore-lMinTakeScore,0L))
				{
					pDBOLogonSuccess->lScore=(SCORE)(lMinTakeScore+rand()%(lMaxTakeScore-lMinTakeScore));
				}
				else
				{
					pDBOLogonSuccess->lScore=(SCORE)lMaxTakeScore;
				}
			}
		}
	}

	//最低分数
	if ((SCORE_EQUAL(m_pGameServiceOption->lMinEnterScore,0L)==false)&&SCORE_LESS(pDBOLogonSuccess->lScore,m_pGameServiceOption->lMinEnterScore))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，您的游戏币低于当前游戏房间的最低进入游戏币条件，不能进入当前游戏房间！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//最高分数
	if ((SCORE_EQUAL(m_pGameServiceOption->lMaxEnterScore,0L)==false)&&SCORE_GREATER(pDBOLogonSuccess->lScore,m_pGameServiceOption->lMaxEnterScore))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，您的游戏币高于当前游戏房间的最高进入游戏币条件，不能进入当前游戏房间！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_CONDITIONS);

		return true;
	}

	//满人判断
	WORD wMaxPlayer=m_pGameServiceOption->wMaxPlayer;
	DWORD dwOnlineCount=m_ServerUserManager.GetUserItemCount();
	if ((pDBOLogonSuccess->cbMemberOrder==0)&&(pDBOLogonSuccess->cbMasterOrder==0)&&(dwOnlineCount>(DWORD)(wMaxPlayer-RESERVE_USER_COUNT)))
	{
		//发送失败
		SendLogonFailure(TEXT("抱歉，由于此房间已经人满，普通玩家不能继续进入了！"),0,pBindParameter->dwSocketID);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		return true;
	}

	//用户变量
	tagUserInfo UserInfo;
	tagUserInfoPlus UserInfoPlus;
	ZeroMemory(&UserInfo,sizeof(UserInfo));
	ZeroMemory(&UserInfoPlus,sizeof(UserInfoPlus));

	//属性资料
	UserInfo.wFaceID=pDBOLogonSuccess->wFaceID;
	UserInfo.dwUserID=pDBOLogonSuccess->dwUserID;
	lstrcpyn(UserInfo.szNickName,pDBOLogonSuccess->szNickName,CountArray(UserInfo.szNickName));

	//机器人IP地址 EWIN网络
	//查找机器
	if (bAndroidUser==true)
	{
		//查找机器
		DWORD dwUserID=pDBOLogonSuccess->dwUserID;
		IAndroidUserItem * pIAndroidUserItem=m_AndroidUserManager.SearchAndroidUserItem(dwUserID,dwContextID);

		//判断是否不存在机器人
		if (pIAndroidUserItem!=NULL)
		{

			//读取配置
			TCHAR szWorkDir[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));
			//构造路径
			TCHAR szIniFile[MAX_PATH]=TEXT("");
			_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\ServerConfig.ini"),szWorkDir);
			//读取数据
			UINT nValue=GetPrivateProfileInt(TEXT("Android"),TEXT("IsAndroidIP"),0,szIniFile);
			
			if(nValue==1){
			
					//读取个数
					int Rint=rand()%GetPrivateProfileInt(TEXT("Android"),TEXT("AndroidCount"),1,szIniFile);

					TCHAR szItemName[8]=TEXT("");
					TCHAR szAddrDescribe[12]=TEXT("");
					_sntprintf(szItemName,CountArray(szItemName),TEXT("Ip%d"),Rint);
					//读取地址信息
					GetPrivateProfileString(TEXT("Android"),szItemName,TEXT(""),szAddrDescribe,CountArray(szAddrDescribe),szIniFile);
					

					if(szItemName[0]!=0){
			
						//读取配置记录
						lstrcpyn(UserInfo.szAddrDescribe,szAddrDescribe,CountArray(szAddrDescribe));
					
					}else{

					//读取数据库
					//获取机器人IP地址
					tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();
					lstrcpyn(UserInfo.szAddrDescribe,pAndroidParameter->szAddrDescribe,CountArray(pAndroidParameter->szAddrDescribe));
					
					}
			

			}else{
			
			//读取数据库
			//获取机器人IP地址
			tagAndroidParameter * pAndroidParameter=pIAndroidUserItem->GetAndroidParameter();
			lstrcpyn(UserInfo.szAddrDescribe,pAndroidParameter->szAddrDescribe,CountArray(pAndroidParameter->szAddrDescribe));

			}
				
		}

	}else{
	
			/////控制号随机地址显示
 			if(CUserRight::IsGameCheatUser(pDBOLogonSuccess->dwUserRight))
 			{
 			
 				//读取配置
 				TCHAR szWorkDir[MAX_PATH]=TEXT("");
 				CWHService::GetWorkDirectory(szWorkDir,CountArray(szWorkDir));
 				//构造路径
 				TCHAR szIniFile[MAX_PATH]=TEXT("");
 				_sntprintf(szIniFile,CountArray(szIniFile),TEXT("%s\\ServerConfig.ini"),szWorkDir);
 				//读取数据
 				UINT nValue=GetPrivateProfileInt(TEXT("Android"),TEXT("IsAndroidIP"),0,szIniFile);
 			
 				if(nValue==1)
				{
 			
 					//读取个数
 					int Rint=rand()%GetPrivateProfileInt(TEXT("Android"),TEXT("AndroidCount"),1,szIniFile);
 
 					TCHAR szItemName[8]=TEXT("");
 					TCHAR szAddrDescribe[12]=TEXT("");
 					_sntprintf(szItemName,CountArray(szItemName),TEXT("Ip%d"),Rint);
 					//读取地址信息
 					GetPrivateProfileString(TEXT("Android"),szItemName,TEXT(""),szAddrDescribe,CountArray(szAddrDescribe),szIniFile);
					CTraceService::TraceString(szAddrDescribe,TraceLevel_Normal);
 
 					if(szItemName[0]!=0)
					{
 						
 						//读取配置记录
 						lstrcpyn(UserInfo.szAddrDescribe,szAddrDescribe,CountArray(szAddrDescribe));
 					
 					}
					else
					{
 
 						//真人IP地址
 						//不是机器人
 						lstrcpyn(UserInfo.szAddrDescribe,pDBOLogonSuccess->szAddrDescribe,CountArray(pDBOLogonSuccess->szAddrDescribe));
 				
 					}
 
 				}
				else
				{
 						//真人IP地址
 						//不是机器人
 						lstrcpyn(UserInfo.szAddrDescribe,pDBOLogonSuccess->szAddrDescribe,CountArray(pDBOLogonSuccess->szAddrDescribe));
 				
 				}
 			
 			}
			else
			{
					
			//普通玩家地址
			//真人IP地址
			//不是机器人
			lstrcpyn(UserInfo.szAddrDescribe,pDBOLogonSuccess->szAddrDescribe,CountArray(pDBOLogonSuccess->szAddrDescribe));

			}
	
	

	}

	//用户资料
	UserInfo.cbGender=pDBOLogonSuccess->cbGender;
	UserInfo.cbMemberOrder=pDBOLogonSuccess->cbMemberOrder;
	UserInfo.cbMasterOrder=pDBOLogonSuccess->cbMasterOrder;

	//状态设置
	UserInfo.cbUserStatus=US_FREE;
	UserInfo.wTableID=INVALID_TABLE;
	UserInfo.wChairID=INVALID_CHAIR;

	//积分信息
	UserInfo.lScore=pDBOLogonSuccess->lScore;
	UserInfo.lInsure=pDBOLogonSuccess->lInsure;
	UserInfo.lUserMedal=pDBOLogonSuccess->lUserMedal;
	UserInfo.dwWinCount=pDBOLogonSuccess->dwWinCount;
	UserInfo.dwLostCount=pDBOLogonSuccess->dwLostCount;
	UserInfo.dwDrawCount=pDBOLogonSuccess->dwDrawCount;
	UserInfo.dwFleeCount=pDBOLogonSuccess->dwFleeCount;
	UserInfo.dwExperience=pDBOLogonSuccess->dwExperience;

	//登录信息
	UserInfoPlus.dwLogonTime=(DWORD)time(NULL);
	UserInfoPlus.dwInoutIndex=pDBOLogonSuccess->dwInoutIndex;

	//用户权限
	UserInfoPlus.dwUserRight=pDBOLogonSuccess->dwUserRight;
	UserInfoPlus.dwMasterRight=pDBOLogonSuccess->dwMasterRight;

	//辅助变量
	UserInfoPlus.bMobileUser=bMobileUser;
	UserInfoPlus.bAndroidUser=bAndroidUser;
	UserInfoPlus.cbPlazaType = pDBOLogonSuccess->cbPlazaType;
	UserInfoPlus.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	lstrcpyn(UserInfoPlus.szPassword,pDBOLogonSuccess->szPassword,CountArray(UserInfoPlus.szPassword));

	//连接信息
	UserInfoPlus.wBindIndex=wBindIndex;
	UserInfoPlus.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(UserInfoPlus.szMachineID,pDBOLogonSuccess->szMachineID,CountArray(UserInfoPlus.szMachineID));


	//激活用户
	m_ServerUserManager.InsertUserItem(&pIServerUserItem,UserInfo,UserInfoPlus);

	//错误判断
	if (pIServerUserItem==NULL)
	{
		//错误断言
		ASSERT(FALSE);

		//解除锁定
		PerformUnlockScore(pDBOLogonSuccess->dwUserID,pDBOLogonSuccess->dwInoutIndex,LER_SERVER_FULL);

		//断开用户
		if (bAndroidUser==true)
		{
			m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
		}
		else
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
		}

		return true;
	}

	//设置用户
	pBindParameter->pIServerUserItem=pIServerUserItem;
#ifdef USE_RS_PRINT
	CString strLog;
	if(pIServerUserItem->IsAndroidUser())
	{
		strLog.Format(L"CAttemperEngineSink::OnDBLogonSuccess Android [%s]登陆成功",pIServerUserItem->GetNickName());
	}
	else
	{
		strLog.Format(L"CAttemperEngineSink::OnDBLogonSuccess User [%s]登陆成功",pIServerUserItem->GetNickName());
	}
	CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif
	//登录事件
	OnEventUserLogon(pIServerUserItem);

	//汇总用户
	if (m_bCollectUser==true)
	{
		//变量定义
		CMD_CS_C_UserEnter UserEnter;
		ZeroMemory(&UserEnter,sizeof(UserEnter));

		//设置变量
		UserEnter.dwUserID=pIServerUserItem->GetUserID();
		lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

		//辅助信息
		UserEnter.cbGender=pIServerUserItem->GetGender();
		UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
		UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();

		//发送消息
		ASSERT(m_pITCPSocketService!=NULL);
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));
	}

	return true;
}

//登录失败
bool CAttemperEngineSink::OnDBLogonFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem!=NULL)) return true;

	//发送错误
	DBO_GR_LogonFailure * pLogonFailure=(DBO_GR_LogonFailure *)pData;
	SendLogonFailure(pLogonFailure->szDescribeString,pLogonFailure->lResultCode,dwContextID);

	//断开连接
	if (LOWORD(dwContextID)>=INDEX_ANDROID)
	{
		m_AndroidUserManager.DeleteAndroidUserItem(dwContextID);
	}
	else
	{
		m_pITCPNetworkEngine->ShutDownSocket(dwContextID);
	}

	return true;
}

//配置信息
bool CAttemperEngineSink::OnDBGameParameter(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(DBO_GR_GameParameter));
	if (wDataSize!=sizeof(DBO_GR_GameParameter)) return false;

	//变量定义
	DBO_GR_GameParameter * pGameParameter=(DBO_GR_GameParameter *)pData;

	//设置变量
	m_pGameParameter->wRevenueRate=pGameParameter->wRevenueRate;

	//版本信息
	m_pGameParameter->dwClientVersion=pGameParameter->dwClientVersion;
	m_pGameParameter->dwServerVersion=pGameParameter->dwServerVersion;

	return true;
}

//列表信息
bool CAttemperEngineSink::OnDBGameColumnInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GameColumnInfo * pGameColumnInfo=(DBO_GR_GameColumnInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameColumnInfo)-sizeof(pGameColumnInfo->ColumnItemInfo);

	//效验参数
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0])))) return false;

	//获取状态
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	enServiceStatus ServiceStatus=pServiceUnits->GetServiceStatus();

	//数据处理
	if (pGameColumnInfo->cbColumnCount==0)
	{
		//默认列表
	}
	else
	{
		//拷贝数据
		m_DataConfigColumn.cbColumnCount=pGameColumnInfo->cbColumnCount;
		CopyMemory(m_DataConfigColumn.ColumnItem,pGameColumnInfo->ColumnItemInfo,pGameColumnInfo->cbColumnCount*sizeof(pGameColumnInfo->ColumnItemInfo[0]));
	}

	//错误通知
	if (ServiceStatus!=ServiceStatus_Service)
	{
		//事件通知
		CP_ControlResult ControlResult;
		ControlResult.cbSuccess=ER_SUCCESS;
		SendUIControlPacket(UI_SERVICE_CONFIG_RESULT,&ControlResult,sizeof(ControlResult));
	}

	return true;
}

//机器信息
bool CAttemperEngineSink::OnDBGameAndroidInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//变量定义
	DBO_GR_GameAndroidInfo * pGameAndroidInfo=(DBO_GR_GameAndroidInfo *)pData;
	WORD wHeadSize=sizeof(DBO_GR_GameAndroidInfo)-sizeof(pGameAndroidInfo->AndroidParameter);

	//效验参数
	ASSERT((wDataSize>=wHeadSize)&&(wDataSize==(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0]))));
	if ((wDataSize<wHeadSize)||(wDataSize!=(wHeadSize+pGameAndroidInfo->wAndroidCount*sizeof(pGameAndroidInfo->AndroidParameter[0])))) return false;

	//设置机器
	if (pGameAndroidInfo->lResultCode==DB_SUCCESS)
	{
		m_AndroidUserManager.SetAndroidStock(pGameAndroidInfo->AndroidParameter,pGameAndroidInfo->wAndroidCount);
	}

	return true;
}

//银行信息
bool CAttemperEngineSink::OnDBUserInsureInfo(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserInsureInfo * pUserInsureInfo=(DBO_GR_UserInsureInfo *)pData;

	//变量定义
	CMD_GR_S_UserInsureInfo UserInsureInfo;
	ZeroMemory(&UserInsureInfo,sizeof(UserInsureInfo));

	//构造数据
	UserInsureInfo.wServerID=pUserInsureInfo->wServerID;
	UserInsureInfo.lUserInsure=pUserInsureInfo->lUserInsure;
	UserInsureInfo.lUserScore+=pIServerUserItem->GetUserScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetTrusteeScore();
	UserInsureInfo.lUserScore+=pIServerUserItem->GetFrozenedScore();

	//发送数据
	m_pITCPNetworkEngine->SendData(dwContextID,MDM_GR_INSURE,SUB_GR_USER_INSURE_INFO,&UserInsureInfo,sizeof(UserInsureInfo));

	return true;
}

//银行成功
bool CAttemperEngineSink::OnDBUserInsureSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	ASSERT(GetBindUserItem(LOWORD(dwContextID))!=NULL);
	IServerUserItem * pIServerUserItem=GetBindUserItem(LOWORD(dwContextID));

	//变量定义
	DBO_GR_UserInsureSuccess * pUserInsureSuccess=(DBO_GR_UserInsureSuccess *)pData;

	//变量定义
	SCORE lFrozenedScore=pUserInsureSuccess->lFrozenedScore;
	SCORE lInsureRevenue=pUserInsureSuccess->lInsureRevenue;
	SCORE lVariationScore=pUserInsureSuccess->lVariationScore;
	SCORE lVariationInsure=pUserInsureSuccess->lVariationInsure;

	//解冻积分
	if ((lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(lFrozenedScore)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	//银行同步
	tagUserInfo * pUserInfo = pIServerUserItem->GetUserInfo();
	ASSERT( pUserInfo!=NULL );
	if(pUserInfo->lInsure!=pUserInsureSuccess->lSourceInsure)
	{
		pUserInfo->lInsure = pUserInsureSuccess->lSourceInsure;
	}

	//银行操作
	if (pIServerUserItem->ModifyUserInsure(lVariationScore,lVariationInsure,lInsureRevenue)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	CMD_GR_S_UserInsureSuccess UserInsureSuccess;
	ZeroMemory(&UserInsureSuccess,sizeof(UserInsureSuccess));

	//构造变量
	UserInsureSuccess.lUserScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	UserInsureSuccess.lUserInsure=pUserInsureSuccess->lSourceInsure+pUserInsureSuccess->lVariationInsure;
	lstrcpyn(UserInsureSuccess.szDescribeString,pUserInsureSuccess->szDescribeString,CountArray(UserInsureSuccess.szDescribeString));

	//发送数据
	WORD wDescribe=CountStringBuffer(UserInsureSuccess.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureSuccess)-sizeof(UserInsureSuccess.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_SUCCESS,&UserInsureSuccess,wHeadSize+wDescribe);

	return true;
}

//操作失败
bool CAttemperEngineSink::OnDBUserInsureFailure(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	WORD wBindIndex=LOWORD(dwContextID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//发送错误
	DBO_GR_UserInsureFailure * pUserInsureFailure=(DBO_GR_UserInsureFailure *)pData;
	SendInsureFailure(pIServerUserItem,pUserInsureFailure->szDescribeString,pUserInsureFailure->lResultCode);

	//解冻积分
	if ((pUserInsureFailure->lFrozenedScore>0L)&&(pIServerUserItem->UnFrozenedUserScore(pUserInsureFailure->lFrozenedScore)==false))
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//查询成功
bool CAttemperEngineSink::OnDBQueryIpDescribeSuccess(DWORD dwContextID, VOID * pData, WORD wDataSize)
{
	//判断在线
	tagBindParameter * pBindParameter=GetBindParameter(LOWORD(dwContextID));
	if ((pBindParameter->dwSocketID!=dwContextID)||(pBindParameter->pIServerUserItem==NULL)) return true;

	//获取用户
	WORD wBindIndex=LOWORD(dwContextID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//提取数据
	DBR_GR_QueryIpDescribeSuccess * pQueryIpDescribeSuccess = (DBR_GR_QueryIpDescribeSuccess *)pData;
	ASSERT(pQueryIpDescribeSuccess!=NULL);
	if(pQueryIpDescribeSuccess==NULL) return false;

	//设置变量
	pIServerUserItem->SetIpAddrDescribe(pQueryIpDescribeSuccess->szAddrDescribe);

	//插入分组
	InsertDistribute(pIServerUserItem,false);

	return true;
}

//注册事件
bool CAttemperEngineSink::OnTCPSocketMainRegister(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_REGISTER_FAILURE:		//注册失败
		{
			//变量定义
			CMD_CS_S_RegisterFailure * pRegisterFailure=(CMD_CS_S_RegisterFailure *)pData;

			//效验参数
			ASSERT(wDataSize>=(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString)));
			if (wDataSize<(sizeof(CMD_CS_S_RegisterFailure)-sizeof(pRegisterFailure->szDescribeString))) return false;

			//关闭处理
			m_bNeekCorrespond=false;
			m_pITCPSocketService->CloseSocket();

			//显示消息
			if (lstrlen(pRegisterFailure->szDescribeString)>0)
			{
				CTraceService::TraceString(pRegisterFailure->szDescribeString,TraceLevel_Exception);
			}

			//事件通知
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_FAILURE;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//列表事件
bool CAttemperEngineSink::OnTCPSocketMainServiceInfo(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_SERVER_INFO:		//房间信息
		{
			//废弃列表
			m_ServerListManager.DisuseServerItem();

			return true;
		}
	case SUB_CS_S_SERVER_ONLINE:	//房间人数
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerOnLine));
			if (wDataSize!=sizeof(CMD_CS_S_ServerOnLine)) return false;

			//变量定义
			CMD_CS_S_ServerOnLine * pServerOnLine=(CMD_CS_S_ServerOnLine *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerOnLine->wServerID);

			//设置人数
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.dwOnLineCount=pServerOnLine->dwOnLineCount;
			}

			return true;
		}
	case SUB_CS_S_SERVER_INSERT:	//房间插入
		{
			//效验参数
			ASSERT(wDataSize%sizeof(tagGameServer)==0);
			if (wDataSize%sizeof(tagGameServer)!=0) return false;

			//变量定义
			WORD wItemCount=wDataSize/sizeof(tagGameServer);
			tagGameServer * pGameServer=(tagGameServer *)pData;

			//更新数据
			for (WORD i=0;i<wItemCount;i++)
			{
				m_ServerListManager.InsertGameServer(pGameServer++);
			}

			return true;
		}
	case SUB_CS_S_SERVER_MODIFY:	//房间修改
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerModify));
			if (wDataSize!=sizeof(CMD_CS_S_ServerModify)) return false;

			//变量定义
			CMD_CS_S_ServerModify * pServerModify=(CMD_CS_S_ServerModify *)pData;

			//查找房间
			CGameServerItem * pGameServerItem=m_ServerListManager.SearchGameServer(pServerModify->wServerID);

			//设置房间
			if (pGameServerItem!=NULL)
			{
				pGameServerItem->m_GameServer.wSortID=pServerModify->wSortID;
				pGameServerItem->m_GameServer.wServerPort=pServerModify->wServerPort;
				pGameServerItem->m_GameServer.dwOnLineCount=pServerModify->dwOnLineCount;
				lstrcpyn(pGameServerItem->m_GameServer.szServerName,pServerModify->szServerName,CountArray(pGameServerItem->m_GameServer.szServerName));
				lstrcpyn(pGameServerItem->m_GameServer.szServerAddr,pServerModify->szServerAddr,CountArray(pGameServerItem->m_GameServer.szServerAddr));
			}

			return true;
		}
	case SUB_CS_S_SERVER_REMOVE:	//房间删除
		{
			//效验参数
			ASSERT(wDataSize==sizeof(CMD_CS_S_ServerRemove));
			if (wDataSize!=sizeof(CMD_CS_S_ServerRemove)) return false;

			//变量定义
			CMD_CS_S_ServerRemove * pServerRemove=(CMD_CS_S_ServerRemove *)pData;

			//变量定义
			m_ServerListManager.DeleteGameServer(pServerRemove->wServerID);

			return true;
		}
	case SUB_CS_S_SERVER_FINISH:	//房间完成
		{
			//清理列表
			m_ServerListManager.CleanServerItem();

			//事件处理
			CP_ControlResult ControlResult;
			ControlResult.cbSuccess=ER_SUCCESS;
			SendUIControlPacket(UI_CORRESPOND_RESULT,&ControlResult,sizeof(ControlResult));

			return true;
		}
	}

	return true;
}

//汇总事件
bool CAttemperEngineSink::OnTCPSocketMainUserCollect(WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_CS_S_COLLECT_REQUEST:	//用户汇总
		{
			//变量定义
			CMD_CS_C_UserEnter UserEnter;
			ZeroMemory(&UserEnter,sizeof(UserEnter));

			//发送用户
			WORD wIndex=0;
			do
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_ServerUserManager.EnumUserItem(wIndex++);
				if (pIServerUserItem==NULL) break;

				//设置变量
				UserEnter.dwUserID=pIServerUserItem->GetUserID();
				lstrcpyn(UserEnter.szNickName,pIServerUserItem->GetNickName(),CountArray(UserEnter.szNickName));

				//辅助信息
				UserEnter.cbGender=pIServerUserItem->GetGender();
				UserEnter.cbMemberOrder=pIServerUserItem->GetMemberOrder();
				UserEnter.cbMasterOrder=pIServerUserItem->GetMasterOrder();

				//发送数据
				ASSERT(m_pITCPSocketService!=NULL);
				m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_ENTER,&UserEnter,sizeof(UserEnter));

			} while (true);

			//汇报完成
			m_bCollectUser=true;
			m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_FINISH);

			return true;
		}
	}

	return true;
}

//登录处理
bool CAttemperEngineSink::OnTCPNetworkMainLogon(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_LOGON_USERID:		//I D 登录
		{
			return OnTCPNetworkSubLogonUserID(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_MOBILE:		//手机登录
		{
			return OnTCPNetworkSubLogonMobile(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_LOGON_ACCOUNTS:		//帐号登录
		{
			return OnTCPNetworkSubLogonAccounts(pData,wDataSize,dwSocketID);
		}
	}

	return true;
}

//用户处理
bool CAttemperEngineSink::OnTCPNetworkMainUser(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	//case SUB_GR_USER_RULE:			//用户规则
	//	{
	//		return OnTCPNetworkSubUserRule(pData,wDataSize,dwSocketID);
	//	}
	case SUB_GR_USER_LOOKON:		//用户旁观
		{
			return OnTCPNetworkSubUserLookon(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_SITDOWN:		//用户坐下
		{
			return OnTCPNetworkSubUserSitDown(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_STANDUP:		//用户起立
		{
			return OnTCPNetworkSubUserStandUp(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAT:			//用户聊天
		{
			return OnTCPNetworkSubUserChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_EXPRESSION:	//用户表情
		{
			return OnTCPNetworkSubUserExpression(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_WISPER_CHAT:		//用户私聊
		{
			return OnTCPNetworkSubWisperChat(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_WISPER_EXPRESSION:	//私聊表情
		{
			return OnTCPNetworkSubWisperExpression(pData,wDataSize,dwSocketID);
		}
	//case SUB_GR_PROPERTY_BUY:		//购买道具
	//	{
	//		return OnTCPNetworkSubPropertyBuy(pData,wDataSize,dwSocketID);
	//	}
	//case SUB_GR_PROPERTY_TRUMPET:   //喇叭道具
	//	{
	//		return OnTCPNetwordSubSendTrumpet(pData,wDataSize,dwSocketID);
	//	}
	//case SUB_GR_USER_INVITE_REQ:    //邀请用户
	//	{
	//		return OnTCPNetworkSubUserInviteReq(pData,wDataSize,dwSocketID);
	//	}
	//case SUB_GR_USER_REPULSE_SIT:   //拒绝厌友
	//	{
	//		return OnTCPNetworkSubUserRepulseSit(pData,wDataSize,dwSocketID);
	//	}
	//case SUB_GR_USER_KICK_USER:    //踢出用户
	//	{
	//		return OnTCPNetworkSubMemberKickUser(pData,wDataSize,dwSocketID);
	//	}
	//case SUB_GR_USER_INFO_REQ:     //请求用户信息
	//	{
	//		return OnTCPNetworkSubUserInfoReq(pData,wDataSize,dwSocketID);
	//	}
	case SUB_GR_USER_CHAIR_REQ:    //请求更换位置 10
		{
			return OnTCPNetworkSubUserChairReq(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_USER_CHAIR_INFO_REQ: //请求椅子用户信息 11
		{
			return OnTCPNetworkSubChairUserInfoReq(pData,wDataSize,dwSocketID);
		}
	//case SUB_GR_CLICK_BONUS:    //点击宝箱
	//	{
	//		return OnTCPNetworkSubClickBonus(pData,wDataSize,dwSocketID);
	//	}
	//case SUB_GR_APPLY_PAY:
	//	{
	//		return  OnTCPNetworkSubMobilePay(pData,wDataSize,dwSocketID);
	//	}
	//	//iPhone充值
	//case SUB_GR_APPLY_PAY_IP:
	//	{
	//		return OnTCPNetworkSubMobilePayIP(pData,wDataSize,dwSocketID);
	//	}
	}

	return false;
}

//银行处理
bool CAttemperEngineSink::OnTCPNetworkMainInsure(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_INSURE_INFO:	//银行查询
		{
			return OnTCPNetworkSubQueryInsureInfo(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_SAVE_SCORE_REQUEST:	//存款请求
		{
			return OnTCPNetworkSubSaveScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TAKE_SCORE_REQUEST:	//取款请求
		{
			return OnTCPNetworkSubTakeScoreRequest(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_TRANSFER_SCORE_REQUEST: //转账请求
		{
			return OnTCPNetworkSubTransferScoreRequest(pData,wDataSize,dwSocketID);
		}
	}

	return false;
}

//管理处理
bool CAttemperEngineSink::OnTCPNetworkMainManage(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	switch (wSubCmdID)
	{
	case SUB_GR_QUERY_OPTION:		//查询设置
		{
			return OnTCPNetworkSubQueryOption(pData,wDataSize,dwSocketID);
		}
	case SUB_GR_OPTION_SERVER:		//房间设置
		{
			return OnTCPNetworkSubOptionServer(pData,wDataSize,dwSocketID);
		}
	// add by hxh 20160627 begin
	//case SUB_GR_SEND_MESSAGE:		//发布消息
	//	{
	//		return OnTCPNetworkSubSendMessage(pData,wDataSize,dwSocketID);
	//	}
	// add by hxh 20160627 end
	}

	return false;
}

//游戏处理
bool CAttemperEngineSink::OnTCPNetworkMainGame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//处理过虑
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//消息处理 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	return pTableFrame->OnEventSocketGame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//框架处理
bool CAttemperEngineSink::OnTCPNetworkMainFrame(WORD wSubCmdID, VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//处理过虑
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	if ((wTableID==INVALID_TABLE)||(wChairID==INVALID_CHAIR)) return true;

	//消息处理 
	CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
	return pTableFrame->OnEventSocketFrame(wSubCmdID,pData,wDataSize,pIServerUserItem);
}

//I D 登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonUserID(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonUserID));
	if (wDataSize<sizeof(CMD_GR_LogonUserID)) return false;

	//处理消息
	CMD_GR_LogonUserID * pLogonUserID=(CMD_GR_LogonUserID *)pData;
	pLogonUserID->szPassword[CountArray(pLogonUserID->szPassword)-1]=0;
	pLogonUserID->szMachineID[CountArray(pLogonUserID->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT(FALSE);
		return false;
	}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonUserID->dwPlazaVersion;
	pBindParameter->dwFrameVersion=pLogonUserID->dwFrameVersion;
	pBindParameter->dwProcessVersion=pLogonUserID->dwProcessVersion;

	//大厅版本
	DWORD dwPlazaVersion=pLogonUserID->dwPlazaVersion;
	DWORD dwFrameVersion=pLogonUserID->dwFrameVersion;
	DWORD dwClientVersion=pLogonUserID->dwProcessVersion;

	if (PerformCheckVersion(dwPlazaVersion,dwFrameVersion,dwClientVersion,dwSocketID,false)==false) return true;

	//切换判断
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonUserID->dwUserID);
	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonUserID->szPassword)==true))
	{
		SwitchUserItemConnect(pIServerUserItem,pLogonUserID->szMachineID,wBindIndex);
		return true;
	}

	//变量定义
	DBR_GR_LogonUserID LogonUserID;
	ZeroMemory(&LogonUserID,sizeof(LogonUserID));

	//构造数据
	LogonUserID.dwUserID=pLogonUserID->dwUserID;
	LogonUserID.dwClientAddr=pBindParameter->dwClientAddr;
	LogonUserID.cbPlazaType = pLogonUserID->cbPlazaType;
	lstrcpyn(LogonUserID.szPassword,pLogonUserID->szPassword,CountArray(LogonUserID.szPassword));
	lstrcpyn(LogonUserID.szMachineID,pLogonUserID->szMachineID,CountArray(LogonUserID.szMachineID));
	lstrcpyn(LogonUserID.szIPAdrr,pLogonUserID->szIPAdrr,CountArray(LogonUserID.szIPAdrr));
	//OutputDebugString(pLogonUserID->szIPAdrr);

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOGON_USERID,dwSocketID,&LogonUserID,sizeof(LogonUserID));

	return true;
}

//手机登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonMobile(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonMobile));
	if (wDataSize<sizeof(CMD_GR_LogonMobile)) return false;

	//处理消息
	CMD_GR_LogonMobile * pLogonMobile=(CMD_GR_LogonMobile *)pData;
	pLogonMobile->szPassword[CountArray(pLogonMobile->szPassword)-1]=0;
	pLogonMobile->szMachineID[CountArray(pLogonMobile->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if ((pBindParameter==NULL)||(pIBindUserItem!=NULL))
	{ 
		ASSERT(FALSE);
		return false;
	}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_MOBILE;
	pBindParameter->dwProcessVersion=pLogonMobile->dwProcessVersion;
	// add by hxh 20160627
#if 1
	//pBindParameter->cbDeviceType=pLogonMobile->cbDeviceType;
	pBindParameter->wVisibleMode=pLogonMobile->wVisibleMode;
	pBindParameter->wVisibleTableRand=pLogonMobile->wVisibleTable;
#endif
	

	//大厅版本
	DWORD dwClientVersion=pLogonMobile->dwProcessVersion;
	if (PerformCheckVersion(0L,0L,dwClientVersion,dwSocketID,true)==false) return true;

	//切换判断
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonMobile->dwUserID);
	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonMobile->szPassword)==true))
	{
#ifdef USE_RS_PRINT		
		CString strlog;
		strlog.Format(L"CAttemperEngineSink::OnTCPNetworkSubLogonMobile 用户[%d][%s],szPassWord=%s",pLogonMobile->dwUserID,pIServerUserItem->GetNickName(),pLogonMobile->szPassword);
		CTraceService::TraceString(strlog,TraceLevel_Warning);
#endif
		SwitchUserItemConnect(pIServerUserItem,pLogonMobile->szMachineID,wBindIndex);
		return true;
	}

	//变量定义
	DBR_GR_LogonMobile LogonMobile;
	ZeroMemory(&LogonMobile,sizeof(LogonMobile));

	//构造数据
	LogonMobile.dwUserID=pLogonMobile->dwUserID;
	LogonMobile.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonMobile.szPassword,pLogonMobile->szPassword,CountArray(LogonMobile.szPassword));
	lstrcpyn(LogonMobile.szMachineID,pLogonMobile->szMachineID,CountArray(LogonMobile.szMachineID));
	// add by hxh 20160627
#if 1
	//LogonMobile.cbDeviceType=pLogonMobile->cbDeviceType;
	LogonMobile.wBehaviorFlags=pLogonMobile->wVisibleMode;
	LogonMobile.wPageTableCount=pLogonMobile->wVisibleTable;
#endif

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOGON_MOBILE,dwSocketID,&LogonMobile,sizeof(LogonMobile));

	return true;
}

//帐号登录
bool CAttemperEngineSink::OnTCPNetworkSubLogonAccounts(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize>=sizeof(CMD_GR_LogonAccounts));
	if (wDataSize<=sizeof(CMD_GR_LogonAccounts)) return false;

	//处理消息
	CMD_GR_LogonAccounts * pLogonAccounts=(CMD_GR_LogonAccounts *)pData;
	pLogonAccounts->szPassword[CountArray(pLogonAccounts->szPassword)-1]=0;
	pLogonAccounts->szAccounts[CountArray(pLogonAccounts->szAccounts)-1]=0;
	pLogonAccounts->szMachineID[CountArray(pLogonAccounts->szMachineID)-1]=0;

	//绑定信息
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIBindUserItem=GetBindUserItem(wBindIndex);
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//重复判断
	if (pIBindUserItem!=NULL)
	{ 
		ASSERT(FALSE);
		return false;
	}

	//版本信息
	pBindParameter->cbClientKind=CLIENT_KIND_COMPUTER;
	pBindParameter->dwPlazaVersion=pLogonAccounts->dwPlazaVersion;
	pBindParameter->dwFrameVersion=pLogonAccounts->dwFrameVersion;
	pBindParameter->dwProcessVersion=pLogonAccounts->dwProcessVersion;

	//大厅版本
	DWORD dwPlazaVersion=pLogonAccounts->dwPlazaVersion;
	DWORD dwFrameVersion=pLogonAccounts->dwFrameVersion;
	DWORD dwClientVersion=pLogonAccounts->dwProcessVersion;
	if (PerformCheckVersion(dwPlazaVersion,dwFrameVersion,dwClientVersion,dwSocketID,false)==false) return true;

	//切换判断
	IServerUserItem * pIServerUserItem=m_ServerUserManager.SearchUserItem(pLogonAccounts->szAccounts);
	if ((pIServerUserItem!=NULL)&&(pIServerUserItem->ContrastLogonPass(pLogonAccounts->szPassword)==true))
	{
		SwitchUserItemConnect(pIServerUserItem,pLogonAccounts->szMachineID,wBindIndex);
		return true;
	}

	//变量定义
	DBR_GR_LogonAccounts LogonAccounts;
	ZeroMemory(&LogonAccounts,sizeof(LogonAccounts));

	//构造数据
	LogonAccounts.dwClientAddr=pBindParameter->dwClientAddr;
	lstrcpyn(LogonAccounts.szAccounts,pLogonAccounts->szAccounts,CountArray(LogonAccounts.szAccounts));
	lstrcpyn(LogonAccounts.szPassword,pLogonAccounts->szPassword,CountArray(LogonAccounts.szPassword));
	lstrcpyn(LogonAccounts.szMachineID,pLogonAccounts->szMachineID,CountArray(LogonAccounts.szMachineID));

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LOGON_ACCOUNTS,dwSocketID,&LogonAccounts,sizeof(LogonAccounts));

	return true;
}

//用户旁观
bool CAttemperEngineSink::OnTCPNetworkSubUserLookon(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserLookon));
	if (wDataSize!=sizeof(CMD_GR_UserLookon)) return false;

	//效验数据
	CMD_GR_UserLookon * pUserLookon=(CMD_GR_UserLookon *)pData;
	if (pUserLookon->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserLookon->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	if ((wTableID==pUserLookon->wTableID)&&(wChairID==pUserLookon->wChairID)&&(cbUserStatus==US_LOOKON)) return true;

	//用户判断
	if (cbUserStatus==US_PLAYING)
	{
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_NONE);
		return true;
	}

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	//坐下处理
	CTableFrame * pTableFrame=m_TableFrameArray[pUserLookon->wTableID];
	pTableFrame->PerformLookonAction(pUserLookon->wChairID,pIServerUserItem);

	return true;
}

//用户坐下
bool CAttemperEngineSink::OnTCPNetworkSubUserSitDown(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserSitDown));
	if (wDataSize!=sizeof(CMD_GR_UserSitDown)) return false;

	//效验数据
	CMD_GR_UserSitDown * pUserSitDown=(CMD_GR_UserSitDown *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(L"CAttemperEngineSink::OnTCPNetworkSubUserSitDown 用户[%d]坐下,wTableID=[%d],wChairID=[%d]",pIServerUserItem->GetUserID(),pUserSitDown->wTableID,pUserSitDown->wChairID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif
	
	//执行坐下
	return PerformUserSitDown(pIServerUserItem,pUserSitDown->wTableID,pUserSitDown->wChairID);
}

//用户起立
bool CAttemperEngineSink::OnTCPNetworkSubUserStandUp(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_UserStandUp));
	if (wDataSize!=sizeof(CMD_GR_UserStandUp)) return false;

	//效验数据
	CMD_GR_UserStandUp * pUserStandUp=(CMD_GR_UserStandUp *)pData;
	if (pUserStandUp->wChairID>=m_pGameServiceAttrib->wChairCount) return false;
	if (pUserStandUp->wTableID>=(WORD)m_TableFrameArray.GetCount()) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(L"CAttemperEngineSink::OnTCPNetworkSubUserStandUp 用户[%d]起立,wTableID=[%d][%d],wChairID=[%d][%d],cbForceLeave=[%d]",pIServerUserItem->GetUserID(),pIServerUserItem->GetTableID(),pUserStandUp->wTableID,pIServerUserItem->GetChairID(),pUserStandUp->wChairID,pUserStandUp->cbForceLeave);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif

	//参数校验
	if ((pIServerUserItem->GetTableID()!=pUserStandUp->wTableID)||(pIServerUserItem->GetChairID()!=pUserStandUp->wChairID)) return true;

	//执行起立
	return PerformUserStandUp(pIServerUserItem,pUserStandUp->cbForceLeave);
}

//用户聊天
bool CAttemperEngineSink::OnTCPNetworkSubUserChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//变量定义
	CMD_GR_C_UserChat * pUserChat=(CMD_GR_C_UserChat *)pData;

	//效验参数
	ASSERT(wDataSize<=sizeof(CMD_GR_C_UserChat));
	ASSERT(wDataSize>=(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0])));

	//效验参数
	if (wDataSize>sizeof(CMD_GR_C_UserChat)) return false;
	if (wDataSize<(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_C_UserChat)-sizeof(pUserChat->szChatString)+pUserChat->wChatLength*sizeof(pUserChat->szChatString[0]))) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=NULL;
	if (pUserChat->dwTargetUserID!=0)
	{
		pIRecvUserItem=m_ServerUserManager.SearchUserItem(pUserChat->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;
	}

	//状态判断
	if ((CServerRule::IsForfendRoomChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户大厅聊天！"),SMT_CHAT);
		return true;
	}

	//权限判断
	if (CUserRight::CanRoomChat(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//构造消息
	CMD_GR_S_UserChat UserChat;
	ZeroMemory(&UserChat,sizeof(UserChat));

	//字符过滤
	lstrcpyn(UserChat.szChatString,pUserChat->szChatString,CountArray(UserChat.szChatString));
	
	//构造数据
	UserChat.dwChatColor=pUserChat->dwChatColor;
	UserChat.wChatLength=pUserChat->wChatLength;
	UserChat.dwSendUserID=pISendUserItem->GetUserID();
	UserChat.dwTargetUserID=pUserChat->dwTargetUserID;
	UserChat.wChatLength=CountStringBuffer(UserChat.szChatString);

	//转发消息
	WORD wHeadSize=sizeof(UserChat)-sizeof(UserChat.szChatString);
	//SendData(BG_ALL_CLIENT,MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));//by hxh 20160622
    SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_CHAT,&UserChat,wHeadSize+UserChat.wChatLength*sizeof(UserChat.szChatString[0]));

	return true;
}

//用户表情
bool CAttemperEngineSink::OnTCPNetworkSubUserExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_UserExpression));
	if (wDataSize!=sizeof(CMD_GR_C_UserExpression)) return false;

	//变量定义
	CMD_GR_C_UserExpression * pUserExpression=(CMD_GR_C_UserExpression *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=NULL;
	if (pUserExpression->dwTargetUserID!=0)
	{
		pIRecvUserItem=m_ServerUserManager.SearchUserItem(pUserExpression->dwTargetUserID);
		if (pIRecvUserItem==NULL) return true;
	}

	//状态判断
	if ((CServerRule::IsForfendRoomChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		//SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户大厅聊天！"),SMT_CHAT);
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户大厅聊天！"),SMT_CHAT);
		return true;
	}

	//权限判断
	if (CUserRight::CanRoomChat(pISendUserItem->GetUserRight())==false)
	{
//		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有大厅发言的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//构造消息
	CMD_GR_S_UserExpression UserExpression;
	ZeroMemory(&UserExpression,sizeof(UserExpression));

	//构造数据
	UserExpression.wItemIndex=pUserExpression->wItemIndex;
	UserExpression.dwSendUserID=pISendUserItem->GetUserID();
	UserExpression.dwTargetUserID=pUserExpression->dwTargetUserID;

	//转发消息
	//SendData(BG_ALL_CLIENT,MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));//by hxh 20160622
    SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_EXPRESSION,&UserExpression,sizeof(UserExpression));

	return true;
}

//用户私聊
bool CAttemperEngineSink::OnTCPNetworkSubWisperChat(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//变量定义
	CMD_GR_C_WisperChat * pWisperChat=(CMD_GR_C_WisperChat *)pData;

	//效验参数
	ASSERT(wDataSize>=(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)));
	ASSERT(wDataSize==(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0])));

	//效验参数
	if (wDataSize<(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString))) return false;
	if (wDataSize!=(sizeof(CMD_GR_C_WisperChat)-sizeof(pWisperChat->szChatString)+pWisperChat->wChatLength*sizeof(pWisperChat->szChatString[0]))) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=m_ServerUserManager.SearchUserItem(pWisperChat->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//状态判断
	if ((CServerRule::IsForfendWisperChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户私聊！"),SMT_CHAT);
		return true;
	}

	//同桌判断
	if ((CServerRule::IsForfendWisperOnGame(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		//变量定义
		bool bForfend=true;
		WORD wTableIDSend=pISendUserItem->GetTableID();
		WORD wTableIDRecv=pIRecvUserItem->GetTableID();

		//规则判断
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pISendUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&((wTableIDSend==INVALID_TABLE)||(wTableIDSend!=wTableIDRecv))) bForfend=false;

		//提示消息
		if (bForfend==true)
		{
			SendRoomMessage(pISendUserItem,TEXT("抱歉，此游戏房间不允许在游戏中与同桌的玩家私聊！"),SMT_EJECT|SMT_CHAT);
			return true;
		}
	}

	//权限判断
	if (CUserRight::CanWisper(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有发送私聊的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//变量定义
	CMD_GR_S_WisperChat WisperChat;
	ZeroMemory(&WisperChat,sizeof(WisperChat));

	//字符过滤
	lstrcpyn(WisperChat.szChatString,pWisperChat->szChatString,CountArray(WisperChat.szChatString));

	//构造数据
	WisperChat.dwChatColor=pWisperChat->dwChatColor;
	WisperChat.wChatLength=pWisperChat->wChatLength;
	WisperChat.dwSendUserID=pISendUserItem->GetUserID();
	WisperChat.dwTargetUserID=pIRecvUserItem->GetUserID();
	WisperChat.wChatLength=CountStringBuffer(WisperChat.szChatString);

	//转发消息
	WORD wHeadSize=sizeof(WisperChat)-sizeof(WisperChat.szChatString);
	SendData(pISendUserItem,MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));
	SendData(pIRecvUserItem,MDM_GR_USER,SUB_GR_WISPER_CHAT,&WisperChat,wHeadSize+WisperChat.wChatLength*sizeof(WisperChat.szChatString[0]));

	return true;
}

//用户表情
bool CAttemperEngineSink::OnTCPNetworkSubWisperExpression(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_WisperExpression));
	if (wDataSize!=sizeof(CMD_GR_C_WisperExpression)) return false;

	//变量定义
	CMD_GR_C_WisperExpression * pWisperExpression=(CMD_GR_C_WisperExpression *)pData;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pISendUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pISendUserItem!=NULL);
	if (pISendUserItem==NULL) return false;

	//寻找用户
	IServerUserItem * pIRecvUserItem=m_ServerUserManager.SearchUserItem(pWisperExpression->dwTargetUserID);
	if (pIRecvUserItem==NULL) return true;

	//状态判断
	if ((CServerRule::IsForfendWisperChat(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，当前此游戏房间禁止用户私聊！"),SMT_CHAT);
		return true;
	}

	//同桌判断
	if ((CServerRule::IsForfendWisperOnGame(m_pGameServiceOption->dwServerRule)==true)&&(pISendUserItem->GetMasterOrder()==0))
	{
		//变量定义
		bool bForfend=true;
		WORD wTableIDSend=pISendUserItem->GetTableID();
		WORD wTableIDRecv=pIRecvUserItem->GetTableID();

		//规则判断
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetMasterOrder()!=0)) bForfend=false;
		if ((bForfend==true)&&(pISendUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&(pIRecvUserItem->GetUserStatus()!=US_PLAYING)) bForfend=false;
		if ((bForfend==true)&&((wTableIDSend==INVALID_TABLE)||(wTableIDSend!=wTableIDRecv))) bForfend=false;

		//提示消息
		if (bForfend==true)
		{
			SendRoomMessage(pISendUserItem,TEXT("抱歉，此游戏房间不允许在游戏中与同桌的玩家私聊！"),SMT_EJECT|SMT_CHAT);
			return true;
		}
	}

	//权限判断
	if (CUserRight::CanWisper(pISendUserItem->GetUserRight())==false)
	{
		SendRoomMessage(pISendUserItem,TEXT("抱歉，您没有发送私聊的权限，若需要帮助，请联系游戏客服咨询！"),SMT_EJECT|SMT_CHAT);
		return true;
	}

	//变量定义
	CMD_GR_S_WisperExpression WisperExpression;
	ZeroMemory(&WisperExpression,sizeof(WisperExpression));

	//构造数据
	WisperExpression.wItemIndex=pWisperExpression->wItemIndex;
	WisperExpression.dwSendUserID=pISendUserItem->GetUserID();
	WisperExpression.dwTargetUserID=pWisperExpression->dwTargetUserID;

	//转发消息
	SendData(pISendUserItem,MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));
	SendData(pIRecvUserItem,MDM_GR_USER,SUB_GR_WISPER_EXPRESSION,&WisperExpression,sizeof(WisperExpression));

	return true;
}

//请求更换位置
bool CAttemperEngineSink::OnTCPNetworkSubUserChairReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) 
	{
		OutputDebugString(L"Here OnTCPNetworkSubUserChairReq pIServerUserItem");

		return false;
	}

	// add by hxh 20160708
#if 1
	//获取参数
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
    if(!pBindParameter) return false;
#endif


	//用户状态
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		//失败
		//m_TableFrameArray[0]->
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_NONE);//<by hxh>
		return true;
	}

	//动态加入
	bool bDynamicJoin=true;
	if (m_pGameServiceAttrib->cbDynamicJoin==FALSE) bDynamicJoin=false;
	if (CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule)==false) bDynamicJoin=false;

	//查找有人桌子
	INT nStartIndex=rand()%m_pGameServiceOption->wTableCount;
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//定义变量
		INT nTableIndex=(i+nStartIndex)%m_pGameServiceOption->wTableCount;

		//过滤同桌
		if(nTableIndex == pIServerUserItem->GetTableID())continue;

		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
		if ((pTableFrame->IsGameStarted()==true)&&(bDynamicJoin==false)) continue;
		if(pTableFrame->IsTableLocked()) continue;
		if(pTableFrame->GetChairCount()==pTableFrame->GetNullChairCount()) continue;

		//无效过滤
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//离开处理
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//定义变量
		//WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
		//WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
		WORD wTagerDeskPos = 0;
		WORD wTagerDeskCount = pBindParameter->wVisibleTableRand;

		//更新信息
		if((nTableIndex < wTagerDeskPos) ||(nTableIndex > (wTagerDeskPos+wTagerDeskCount-1)))
		{
			//pIServerUserItem->SetMobileUserDeskPos(nTableIndex/wTagerDeskCount);
		}

		//用户坐下
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//查找无人桌子
	nStartIndex=rand()%m_pGameServiceOption->wTableCount;
	for (INT_PTR i=0;i<(m_pGameServiceOption->wTableCount);i++)
	{
		//定义变量
		INT nTableIndex=(i+nStartIndex)%m_pGameServiceOption->wTableCount;

		//过滤同桌
		if(nTableIndex == pIServerUserItem->GetTableID())continue;

		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[nTableIndex];
		if ((pTableFrame->IsGameStarted()==true)&&(bDynamicJoin==false)) continue;
		if(pTableFrame->IsTableLocked()) continue;

		//无效过滤
		WORD wChairID=pTableFrame->GetRandNullChairID();
		if (wChairID==INVALID_CHAIR) continue;

		//离开处理
		if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
			if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
		}

		//定义变量
		//WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
		//WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
		WORD wTagerDeskPos = 0;
		WORD wTagerDeskCount = pBindParameter->wVisibleTableRand;

		//更新信息
		if((nTableIndex < wTagerDeskPos) ||(nTableIndex > (wTagerDeskPos+wTagerDeskCount-1)))
		{
			//pIServerUserItem->SetMobileUserDeskPos(nTableIndex/wTagerDeskCount);
		}

		//用户坐下
		pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
		return true;
	}

	//查找同桌
	if(pIServerUserItem->GetTableID() != INVALID_TABLE)
	{
		//获取桌子
		CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
		if (pTableFrame->IsGameStarted()==false && pTableFrame->IsTableLocked()==false)
		{
			//无效过滤
			WORD wChairID=pTableFrame->GetRandNullChairID();
			if (wChairID!=INVALID_CHAIR)
			{
				//离开处理
				if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
				{
					CTableFrame * pTableFrame=m_TableFrameArray[pIServerUserItem->GetTableID()];
					if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
				}

				//用户坐下
				pTableFrame->PerformSitDownAction(wChairID,pIServerUserItem);
				return true;
			}
		}
	}

	//失败
	//m_TableFrameArray[0]->
	SendRequestFailure(pIServerUserItem,TEXT("没找到可进入的游戏桌！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_NONE);
	return true;
}

//请求椅子用户信息
bool CAttemperEngineSink::OnTCPNetworkSubChairUserInfoReq(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==4/*sizeof(CMD_GR_ChairUserInfoReq)*/);
	if (wDataSize!=4/*sizeof(CMD_GR_ChairUserInfoReq)*/) 
	{
		OutputDebugString(L"Here OnTCPNetworkSubChairUserInfoReq wDataSize");

		return false;
	}


	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) 
	{
		OutputDebugString(L"Here OnTCPNetworkSubChairUserInfoReq pIServerUserItem");

		return false;
	}

	//变量定义
#if 1
	WORD wTableID=*((WORD *)pData);
	WORD wChairID=*((WORD *)pData+1);
	if(wTableID == INVALID_TABLE) return true;
	if(wTableID >= m_pGameServiceOption->wTableCount)return true;
#else
	CMD_GR_ChairUserInfoReq * pUserInfoReq = (CMD_GR_ChairUserInfoReq *)pData;
	if(pUserInfoReq->wTableID == INVALID_TABLE) return true;
	if(pUserInfoReq->wTableID >= m_pGameServiceOption->wTableCount)return true;
#endif

#ifdef USE_RS_PRINT
	CString strlog;
	strlog.Format(L"CAttemperEngineSink::OnTCPNetworkSubChairUserInfoReq 用户[%d],wTableID=[%d][%d],wChairID=[%d][%d]",pIServerUserItem->GetUserID(),pIServerUserItem->GetTableID(),wTableID,pIServerUserItem->GetChairID(),wChairID);
	CTraceService::TraceString(strlog,TraceLevel_Normal);
#endif

	//发送消息
	WORD wChairCout = m_TableFrameArray[wTableID]->GetChairCount();
	for(WORD wIndex = 0; wIndex < wChairCout; wIndex++)
	{
		//获取用户
		if(wChairID != INVALID_CHAIR && wIndex != wChairID)continue;
		IServerUserItem * pTagerIServerUserItem=m_TableFrameArray[wTableID]->GetTableUserItem(wIndex);
		if(pTagerIServerUserItem==NULL)continue;

		//变量定义
		BYTE cbBuffer[SOCKET_TCP_PACKET]={0};
		//tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
		tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
		CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));
		tagUserInfo *pUserInfo = pTagerIServerUserItem->GetUserInfo();

		//用户属性
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		//pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		//pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//用户属性
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//用户状态
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//用户局数
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//用户成绩
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pTagerIServerUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pTagerIServerUserItem->GetFrozenedScore();

		//叠加信息
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//发送消息
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

//查询银行
bool CAttemperEngineSink::OnTCPNetworkSubQueryInsureInfo(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	DBR_GR_QueryInsureInfo QueryInsureInfo;
	ZeroMemory(&QueryInsureInfo,sizeof(QueryInsureInfo));

	//构造数据
	QueryInsureInfo.dwUserID=pIServerUserItem->GetUserID();
	QueryInsureInfo.dwClientAddr=pIServerUserItem->GetClientAddr();

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_QUERY_INSURE_INFO,dwSocketID,&QueryInsureInfo,sizeof(QueryInsureInfo));

	return true;
}

//存款请求
bool CAttemperEngineSink::OnTCPNetworkSubSaveScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_SaveScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_SaveScoreRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GR_C_SaveScoreRequest * pSaveScoreRequest=(CMD_GR_C_SaveScoreRequest *)pData;

	//效验参数
	ASSERT(pSaveScoreRequest->lSaveScore>0L);
	if (SCORE_LESS_EQUAL(pSaveScoreRequest->lSaveScore,0L)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	SCORE lConsumeQuota=0L;
	SCORE lUserWholeScore=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();

	//获取限额
	if (pIServerUserItem->GetTableID()!=INVALID_TABLE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		lConsumeQuota=m_TableFrameArray[wTableID]->QueryConsumeQuota(pIServerUserItem);
	}
	else
	{
		lConsumeQuota=pIServerUserItem->GetUserScore()+pIServerUserItem->GetTrusteeScore();
	}

	//限额判断
	if (pSaveScoreRequest->lSaveScore>lConsumeQuota)
	{
		if (SCORE_LESS(lConsumeQuota,lUserWholeScore))
		{
			//构造提示
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("由于您正在游戏中，游戏币可存入额度为 %I64d，存入操作失败！"),lConsumeQuota);

			//发送数据
			SendInsureFailure(pIServerUserItem,szDescribe,0L);
		}
		else
		{
			//发送数据
			SendInsureFailure(pIServerUserItem,TEXT("您的游戏币余额不足，存入操作失败！"),0L);
		}

		return true;
	}

	//锁定积分
	if (pIServerUserItem->FrozenedUserScore(pSaveScoreRequest->lSaveScore)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	DBR_GR_UserSaveScore UserSaveScore;
	ZeroMemory(&UserSaveScore,sizeof(UserSaveScore));

	//构造数据
	UserSaveScore.dwUserID=pIServerUserItem->GetUserID();
	UserSaveScore.lSaveScore=pSaveScoreRequest->lSaveScore;
	UserSaveScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserSaveScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserSaveScore.szMachineID));

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_USER_SAVE_SCORE,dwSocketID,&UserSaveScore,sizeof(UserSaveScore));

	return true;
}

//取款请求
bool CAttemperEngineSink::OnTCPNetworkSubTakeScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_C_TakeScoreRequest));
	if (wDataSize!=sizeof(CMD_GR_C_TakeScoreRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GR_C_TakeScoreRequest * pTakeScoreRequest=(CMD_GR_C_TakeScoreRequest *)pData;
	pTakeScoreRequest->szInsurePass[CountArray(pTakeScoreRequest->szInsurePass)-1]=0;

	//效验参数
	ASSERT(SCORE_GREATER(pTakeScoreRequest->lTakeScore,0L));
	if (SCORE_EQUAL(pTakeScoreRequest->lTakeScore,0L)) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	DBR_GR_UserTakeScore UserTakeScore;
	ZeroMemory(&UserTakeScore,sizeof(UserTakeScore));

	//构造数据
	UserTakeScore.dwUserID=pIServerUserItem->GetUserID();
	UserTakeScore.lTakeScore=pTakeScoreRequest->lTakeScore;
	UserTakeScore.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(UserTakeScore.szPassword,pTakeScoreRequest->szInsurePass,CountArray(UserTakeScore.szPassword));
	lstrcpyn(UserTakeScore.szMachineID,pIServerUserItem->GetMachineID(),CountArray(UserTakeScore.szMachineID));

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_USER_TAKE_SCORE,dwSocketID,&UserTakeScore,sizeof(UserTakeScore));

	return true;
}


//转账请求
bool CAttemperEngineSink::OnTCPNetworkSubTransferScoreRequest(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GP_C_TransferScoreRequest));
	if (wDataSize!=sizeof(CMD_GP_C_TransferScoreRequest)) return false;

	//房间判断
	ASSERT((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)!=0);
	if ((m_pGameServiceOption->wServerType&GAME_GENRE_GOLD)==0) return false;

	//变量定义
	CMD_GP_C_TransferScoreRequest * pTransferScoreRequest=(CMD_GP_C_TransferScoreRequest *)pData;
	pTransferScoreRequest->szAccounts[CountArray(pTransferScoreRequest->szAccounts)-1]=0;
	pTransferScoreRequest->szLogonPass[CountArray(pTransferScoreRequest->szLogonPass)-1]=0;
	pTransferScoreRequest->szInsurePass[CountArray(pTransferScoreRequest->szInsurePass)-1]=0;

	//效验参数
	ASSERT(pTransferScoreRequest->lTransferScore>0.00f);
	if (pTransferScoreRequest->lTransferScore<=0.00f) return false;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	DBR_GR_UserTransferScore UserTransferScore;
	ZeroMemory(&UserTransferScore,sizeof(UserTransferScore));

	//构造结构	
	UserTransferScore.dwUserID = pIServerUserItem->GetUserID();
	UserTransferScore.lTransferScore = pTransferScoreRequest->lTransferScore;
	UserTransferScore.dwClientAddr = pIServerUserItem->GetClientAddr();
	CopyMemory(UserTransferScore.szAccounts,pTransferScoreRequest->szAccounts,sizeof(UserTransferScore.szAccounts));
	CopyMemory(UserTransferScore.szLogonPass,pTransferScoreRequest->szLogonPass,sizeof(UserTransferScore.szLogonPass));
	CopyMemory(UserTransferScore.szInsurePass,pTransferScoreRequest->szInsurePass,sizeof(UserTransferScore.szInsurePass));
	CopyMemory(UserTransferScore.szMachineID,pIServerUserItem->GetMachineID(),sizeof(UserTransferScore.szMachineID));

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_USER_TRANSFER_SCORE,dwSocketID,&UserTransferScore,sizeof(UserTransferScore));

	return true;
}

//查询设置
bool CAttemperEngineSink::OnTCPNetworkSubQueryOption(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//变量定义
	CMD_GR_OptionCurrent OptionCurrent;
	ZeroMemory(&OptionCurrent,sizeof(OptionCurrent));

	//挂接属性
	OptionCurrent.ServerOptionInfo.wKindID=m_pGameServiceOption->wKindID;
	OptionCurrent.ServerOptionInfo.wSortID=m_pGameServiceOption->wSortID;

	//房间配置
	OptionCurrent.ServerOptionInfo.cbRevenueRatio=m_pGameServiceOption->cbRevenueRatio;
	OptionCurrent.ServerOptionInfo.lRestrictScore=m_pGameServiceOption->lRestrictScore;
	OptionCurrent.ServerOptionInfo.lMinTableScore=m_pGameServiceOption->lMinTableScore;
	OptionCurrent.ServerOptionInfo.lMinEnterScore=m_pGameServiceOption->lMinEnterScore;
	OptionCurrent.ServerOptionInfo.lMaxEnterScore=m_pGameServiceOption->lMaxEnterScore;

	//会员限制
	OptionCurrent.ServerOptionInfo.cbMinEnterMember=m_pGameServiceOption->cbMinEnterMember;
	OptionCurrent.ServerOptionInfo.cbMaxEnterMember=m_pGameServiceOption->cbMaxEnterMember;

	//房间属性
	OptionCurrent.ServerOptionInfo.dwServerRule=m_pGameServiceOption->dwServerRule;
	lstrcpyn(OptionCurrent.ServerOptionInfo.szServerName,m_pGameServiceOption->szServerName,CountArray(OptionCurrent.ServerOptionInfo.szServerName));

	//聊天规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_CHAT;
	OptionCurrent.dwRuleMask|=SR_FORFEND_WISPER_ON_GAME;

	//房间规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_ROOM_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_ENTER;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_LOOKON;

	//银行规则
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_TAKE_IN_GAME;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_ROOM;
	OptionCurrent.dwRuleMask|=SR_FORFEND_SAVE_IN_GAME;

	//其他规则
	OptionCurrent.dwRuleMask|=SR_RECORD_GAME_TRACK;
	OptionCurrent.dwRuleMask|=SR_FORFEND_GAME_RULE;
	OptionCurrent.dwRuleMask|=SR_FORFEND_LOCK_TABLE;

	//组件规则
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_DYNAMIC_JOIN;
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_ANDROID_ATTEND;
	if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE) OptionCurrent.dwRuleMask|=SR_ALLOW_OFFLINE_TRUSTEE;

	//模式规则
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0) OptionCurrent.dwRuleMask|=SR_RECORD_GAME_SCORE;
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0) OptionCurrent.dwRuleMask|=SR_IMMEDIATE_WRITE_SCORE;

	//发送数据
	SendData(pIServerUserItem,MDM_GR_MANAGE,SUB_GR_OPTION_CURRENT,&OptionCurrent,sizeof(OptionCurrent));

	return true;
}

//房间设置
bool CAttemperEngineSink::OnTCPNetworkSubOptionServer(VOID * pData, WORD wDataSize, DWORD dwSocketID)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_GR_ServerOption));
	if (wDataSize!=sizeof(CMD_GR_ServerOption)) return false;

	//变量定义
	CMD_GR_ServerOption * pServerOption=(CMD_GR_ServerOption *)pData;
	tagServerOptionInfo * pServerOptionInfo=&pServerOption->ServerOptionInfo;

	//获取用户
	WORD wBindIndex=LOWORD(dwSocketID);
	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);

	//用户效验
	ASSERT((pIServerUserItem!=NULL)&&(pIServerUserItem->GetMasterOrder()>0));
	if ((pIServerUserItem==NULL)||(pIServerUserItem->GetMasterOrder()==0)) return false;

	//变量定义
	bool bModifyServer=false;

	//挂接类型
	if ((pServerOptionInfo->wKindID!=0)&&(m_pGameServiceOption->wKindID!=pServerOptionInfo->wKindID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wKindID=pServerOptionInfo->wKindID;
	}
	
	//挂接排序
	if ((pServerOptionInfo->wSortID!=0)&&(m_pGameServiceOption->wSortID!=pServerOptionInfo->wSortID))
	{
		bModifyServer=true;
		m_pGameServiceOption->wSortID=pServerOptionInfo->wSortID;
	}

	//房间名字
	if ((pServerOptionInfo->szServerName[0]!=0)&&(lstrcmp(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName)!=0))
	{
		bModifyServer=true;
		lstrcpyn(m_pGameServiceOption->szServerName,pServerOptionInfo->szServerName,CountArray(m_pGameServiceOption->szServerName));
	}

	//房间配置
	m_pGameServiceOption->cbRevenueRatio=pServerOptionInfo->cbRevenueRatio;
	m_pGameServiceOption->lRestrictScore=pServerOptionInfo->lRestrictScore;
	m_pGameServiceOption->lMinTableScore=pServerOptionInfo->lMinTableScore;
	m_pGameServiceOption->lMinEnterScore=pServerOptionInfo->lMinEnterScore;
	m_pGameServiceOption->lMaxEnterScore=pServerOptionInfo->lMaxEnterScore;

	//会员限制
	m_pGameServiceOption->cbMinEnterMember=pServerOptionInfo->cbMinEnterMember;
	m_pGameServiceOption->cbMaxEnterMember=pServerOptionInfo->cbMaxEnterMember;

	//聊天规则
	CServerRule::SetForfendGameChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendRoomChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperChat(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperChat(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendWisperOnGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendWisperOnGame(pServerOptionInfo->dwServerRule));

	//房间规则
	CServerRule::SetForfendRoomEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendRoomEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameEnter(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameEnter(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameLookon(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameLookon(pServerOptionInfo->dwServerRule));

	//银行规则
	CServerRule::SetForfendTakeInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendTakeInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendTakeInGame(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInRoom(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInRoom(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendSaveInGame(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendSaveInGame(pServerOptionInfo->dwServerRule));

	//其他规则
	CServerRule::SetRecordGameTrack(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameTrack(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendGameRule(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendGameRule(pServerOptionInfo->dwServerRule));
	CServerRule::SetForfendLockTable(m_pGameServiceOption->dwServerRule,CServerRule::IsForfendLockTable(pServerOptionInfo->dwServerRule));

	//动态加入
	if (m_pGameServiceAttrib->cbDynamicJoin==TRUE)
	{
		CServerRule::SetAllowDynamicJoin(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowDynamicJoin(pServerOptionInfo->dwServerRule));
	}

	//机器管理
	if (m_pGameServiceAttrib->cbAndroidUser==TRUE)
	{
		CServerRule::SetAllowAndroidAttend(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowAndroidAttend(pServerOptionInfo->dwServerRule));
	}

	//断线托管
	if (m_pGameServiceAttrib->cbOffLineTrustee==TRUE)
	{
		CServerRule::SetAllowOffLineTrustee(m_pGameServiceOption->dwServerRule,CServerRule::IsAllowOffLineTrustee(pServerOptionInfo->dwServerRule));
	}

	//记录成绩
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0)
	{
		CServerRule::SetRecordGameScore(m_pGameServiceOption->dwServerRule,CServerRule::IsRecordGameScore(pServerOptionInfo->dwServerRule));
	}

	//立即写分
	if ((m_pGameServiceOption->wServerType&(GAME_GENRE_GOLD|GAME_GENRE_MATCH))==0)
	{
		CServerRule::SetImmediateWriteScore(m_pGameServiceOption->dwServerRule,CServerRule::IsImmediateWriteScore(pServerOptionInfo->dwServerRule));
	}

	//发送修改
	if (bModifyServer==true)
	{
		//变量定义
		CMD_CS_C_ServerModify ServerModify;
		ZeroMemory(&ServerModify,sizeof(ServerModify));

		//服务端口
		CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
		ServerModify.wServerPort=pServiceUnits->m_TCPNetworkEngine->GetCurrentPort();
		//ServerModify.wServerPort=_ttoi(szServerPort);//新增从配置文件获取

		//房间信息
		ServerModify.wKindID=m_pGameServiceOption->wKindID;
		ServerModify.wSortID=m_pGameServiceOption->wSortID;
		ServerModify.dwOnLineCount=m_ServerUserManager.GetUserItemCount();
		lstrcpyn(ServerModify.szServerName,m_pGameServiceOption->szServerName,CountArray(ServerModify.szServerName));
		lstrcpyn(ServerModify.szServerAddr,m_pInitParameter->m_ServiceAddress.szAddress,CountArray(ServerModify.szServerAddr));

		//发送数据
		m_pITCPSocketService->SendData(MDM_CS_SERVICE_INFO,SUB_CS_C_SERVER_MODIFY,&ServerModify,sizeof(ServerModify));
	}

	//发送信息
	SendRoomMessage(pIServerUserItem,TEXT("当前游戏服务器房间的“运行值”状态配置数据修改成功"),SMT_CHAT|SMT_EJECT);

	//输出信息
	TCHAR szBuffer[128]=TEXT("");
	_sntprintf(szBuffer,CountArray(szBuffer),TEXT("远程修改房间配置通知 管理员 %s [ %ld ]"),pIServerUserItem->GetNickName(),pIServerUserItem->GetUserID());

	//输出信息
	CTraceService::TraceString(szBuffer,TraceLevel_Info);

	return true;
}

////发布消息
//bool CAttemperEngineSink::OnTCPNetworkSubSendMessage(VOID * pData, WORD wDataSize, DWORD dwSocketID)
//{
//	//获取用户
//	WORD wBindIndex=LOWORD(dwSocketID);
//	IServerUserItem * pIServerUserItem=GetBindUserItem(wBindIndex);
//	if (pIServerUserItem==NULL) 
//	{
//		OutputDebugString(L"Here OnTCPNetworkSubSendMessage pIServerUserItem");
//		return false;
//	}
//
//
//	//权限判断
//	ASSERT(CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==true);
//	if (CMasterRight::CanIssueMessage(pIServerUserItem->GetMasterRight())==false) 
//	{
//		OutputDebugString(L"Here OnTCPNetworkSubSendMessage CMasterRight");
//		return false;
//	}
//
//
//	//变量定义
//	CMD_GR_SendMessage *pSysMessage = (CMD_GR_SendMessage *)pData;
//
//	if(pSysMessage->cbLoop==TRUE)
//	{
//		//定于变量
//		tagSystemMessage  *pSendMessage=new tagSystemMessage;
//		ZeroMemory(pSendMessage, sizeof(tagSystemMessage));
//
//		//构造消息
//		pSendMessage->dwLastTime=(DWORD)time(NULL);
//		pSendMessage->SystemMessage.cbMessageID=100;
//		pSendMessage->SystemMessage.cbAllRoom=pSysMessage->cbAllRoom;
//		if(pSysMessage->cbRoom==TRUE && pSysMessage->cbGame==TRUE)
//			pSendMessage->SystemMessage.cbMessageType=3;
//		else if(pSysMessage->cbRoom==TRUE)
//			pSendMessage->SystemMessage.cbMessageType=2;
//		else if(pSysMessage->cbGame==TRUE)
//			pSendMessage->SystemMessage.cbMessageType=1;
//		pSendMessage->SystemMessage.dwTimeRate=pSysMessage->dwTimeRate;
//		pSendMessage->SystemMessage.tConcludeTime=pSysMessage->tConcludeTime;
//		lstrcpyn(pSendMessage->SystemMessage.szSystemMessage, pSysMessage->szSystemMessage, CountArray(pSendMessage->SystemMessage.szSystemMessage));
//
//		//记录消息
//		m_SystemMessageList.AddTail(pSendMessage);
//	}
//
//
//	//消息处理
//	return SendSystemMessage((CMD_GR_SendMessage *)pData, wDataSize);
//}

//用户登录
VOID CAttemperEngineSink::OnEventUserLogon(IServerUserItem * pIServerUserItem)
{
	//获取参数
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	bool bAndroidUser=pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//登录处理
	if (pIServerUserItem->IsMobileUser()==false)
	{
		//变量定义
		CMD_GR_LogonSuccess LogonSuccess;
		CMD_GR_ConfigServer ConfigServer;
		ZeroMemory(&LogonSuccess,sizeof(LogonSuccess));
		ZeroMemory(&ConfigServer,sizeof(ConfigServer));

		//登录成功
		LogonSuccess.dwUserRight=pIServerUserItem->GetUserRight();
		LogonSuccess.dwMasterRight=pIServerUserItem->GetMasterRight();
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_SUCCESS,&LogonSuccess,sizeof(LogonSuccess));

		//房间配置
		ConfigServer.wTableCount=m_pGameServiceOption->wTableCount;
		ConfigServer.wChairCount=m_pGameServiceAttrib->wChairCount;
		ConfigServer.bAllowDistribute=(m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW)?TRUE:FALSE;
		ConfigServer.wServerType=m_pGameServiceOption->wServerType;
		ConfigServer.dwServerRule=m_pGameServiceOption->dwServerRule;
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_SERVER,&ConfigServer,sizeof(ConfigServer));

		//列表配置
		WORD wConfigColumnHead=sizeof(m_DataConfigColumn)-sizeof(m_DataConfigColumn.ColumnItem);
		WORD wConfigColumnInfo=m_DataConfigColumn.cbColumnCount*sizeof(m_DataConfigColumn.ColumnItem[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_COLUMN,&m_DataConfigColumn,wConfigColumnHead+wConfigColumnInfo);

#ifdef USE_RS_PRINT
		//提示消息
		CString strLog;
		strLog.Format(L"PC登陆房间成功！用户[%d][%s],[%d]桌,[%d]座", pIServerUserItem->GetUserID(), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID());
		CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif

		//配置完成
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_FINISH,NULL,0);

		//登录玩家自己的数据发送给自己
		SendUserInfoPacket(pIServerUserItem,pBindParameter->dwSocketID);


		//在线用户
		WORD wUserIndex=0;
		IServerUserItem * pIServerUserItemSend=NULL;
		while (true)
		{
			pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
			if (pIServerUserItemSend==NULL) break;
			if (pIServerUserItemSend==pIServerUserItem) continue;
            //其他玩家的数据发送给自己
			SendUserInfoPacket(pIServerUserItemSend,pBindParameter->dwSocketID);
		}

		//桌子状态
		CMD_GR_TableInfo TableInfo;
		TableInfo.wTableCount=(WORD)m_TableFrameArray.GetCount();
		ASSERT(TableInfo.wTableCount<CountArray(TableInfo.TableStatusArray));
		for (WORD i=0;i<TableInfo.wTableCount;i++)
		{
			CTableFrame * pTableFrame=m_TableFrameArray[i];
			TableInfo.TableStatusArray[i].cbTableLock=pTableFrame->IsTableLocked()?TRUE:FALSE;
			TableInfo.TableStatusArray[i].cbPlayStatus=pTableFrame->IsTableStarted()?TRUE:FALSE;
		}

		//桌子状态
		WORD wHeadSize=sizeof(TableInfo)-sizeof(TableInfo.TableStatusArray);
		WORD wSendSize=wHeadSize+TableInfo.wTableCount*sizeof(TableInfo.TableStatusArray[0]);
		SendData(pBindParameter->dwSocketID,MDM_GR_STATUS,SUB_GR_TABLE_INFO,&TableInfo,wSendSize);

		//群发用户，登录玩家自己的数据发送给其他玩家
		SendUserInfoPacket(pIServerUserItem,INVALID_DWORD);

		//登录完成
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);


	
		strLog.Format(L"  ttt   PC登陆房间成功！用户[%d][%s],[%d]桌,[%d]座", pIServerUserItem->GetUserID(), pIServerUserItem->GetNickName(), pIServerUserItem->GetTableID(), pIServerUserItem->GetChairID());
		OutputDebugString(strLog);

		//欢迎消息
		if (bAndroidUser==false)
		{
			//构造提示
			TCHAR szMessage[128]=TEXT("");
			_sntprintf(szMessage,CountArray(szMessage),TEXT("欢迎您进入“%s”游戏房间，祝您游戏愉快！"),m_pGameServiceAttrib->szGameName);

			//发送消息
			SendRoomMessage(pIServerUserItem,szMessage,SMT_CHAT);
		}
	}
	else
	{

		//CTime time = CTime::GetCurrentTime();
		//if((time.GetMonth() != m_UserLogonMonth[pIServerUserItem->GetUserID()])||(time.GetDay() != m_UserLogonDay[pIServerUserItem->GetUserID()]))
		//{
		//	m_BonusCountMap[pIServerUserItem->GetUserID()] = 0;
		//	m_UserLogonMonth[pIServerUserItem->GetUserID()] = time.GetMonth();
		//	m_UserLogonDay[pIServerUserItem->GetUserID()] = time.GetDay();
		//}

		//变量定义
		CMD_GR_ConfigServer ConfigServer;
		ZeroMemory(&ConfigServer,sizeof(ConfigServer));
		////读取宝箱配置
		//m_wBonusInterval = m_pInitParameter->m_wBonusInterval;
		//m_dwTimeCount = m_pInitParameter->m_dwTimeCount;
		//m_lRewardScore = m_pInitParameter ->m_lRewardScore;

		//房间配置
		ConfigServer.wTableCount=m_pGameServiceOption->wTableCount;
		ConfigServer.wChairCount=m_pGameServiceAttrib->wChairCount;
		ConfigServer.bAllowDistribute=(m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW)?TRUE:FALSE;
		ConfigServer.wServerType=m_pGameServiceOption->wServerType;
		ConfigServer.dwServerRule=m_pGameServiceOption->dwServerRule;
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_SERVER,&ConfigServer,sizeof(ConfigServer));

		//配置完成
		SendData(pBindParameter->dwSocketID,MDM_GR_CONFIG,SUB_GR_CONFIG_FINISH,NULL,0);

		////add by hxh 20160623
		//SendUserInfoPacket(pIServerUserItem,pBindParameter->dwSocketID);

		//玩家数据<add by hxh 20160624>
		SendViewTableUserInfoPacketToMobileUser(pIServerUserItem,pIServerUserItem->GetUserID());

		//群发用户
		SendUserInfoPacket(pIServerUserItem,INVALID_DWORD);

		//立即登录<add by hxh 20160624>
		if(pIServerUserItem->GetTableID()==INVALID_TABLE)
		{
			//WORD wMobileUserRule =  pIServerUserItem->GetMobileUserRule();
			WORD wMobileUserRule =  pBindParameter->wVisibleMode;
			//if((wMobileUserRule&BEHAVIOR_LOGON_IMMEDIATELY)!=0)
			if((wMobileUserRule & 0x1000)!=0)
			{
				//MobileUserImmediately(pIServerUserItem);
                bool bRet=PerformUserSitDown(pIServerUserItem,INVALID_TABLE,INVALID_CHAIR);
				// add by hxh 20161028
#if 0
				//群发用户，登录玩家自己的数据发送给其他玩家
				SendUserInfoPacket(pIServerUserItem,INVALID_DWORD);
				//登录玩家自己的数据发送给自己
				SendUserInfoPacket(pIServerUserItem,pBindParameter->dwSocketID);
				//在线用户
				WORD wUserIndex=0;
				IServerUserItem * pIServerUserItemSend=NULL;
				while (true)
				{
					pIServerUserItemSend=m_ServerUserManager.EnumUserItem(wUserIndex++);
					if (pIServerUserItemSend==NULL) break;
					if (pIServerUserItemSend==pIServerUserItem) continue;
					//其他玩家的数据发送给自己
					SendUserInfoPacket(pIServerUserItemSend,pBindParameter->dwSocketID);
				}
#endif
			}
			else
			{
				SendViewTableUserInfoPacketToMobileUser(pIServerUserItem,INVALID_CHAIR);
			}
		}
			
		//登录完成<放在最后面>	
		SendData(pBindParameter->dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FINISH,NULL,0);
	
	}

	//网络设置
	if (bAndroidUser==false)
	{
		if (pBindParameter->cbClientKind==CLIENT_KIND_MOBILE)
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_MOBILE);
            //CTraceService::TraceString(L"不是机器人，手机连接",TraceLevel_Normal);
		}
		else
		{
			m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_COMPUTER);
            //CTraceService::TraceString(L"不是机器人，电脑连接",TraceLevel_Normal);
		}
	}
	else
	{
		m_pITCPNetworkEngine->AllowBatchSend(pBindParameter->dwSocketID,true,BG_ALL_CLIENT);
        //CTraceService::TraceString(L"是机器人",TraceLevel_Normal);
	}

	return;
}

//群发数据
bool CAttemperEngineSink::SendDataBatchToMobileUser(WORD wCmdTable, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//枚举用户
	WORD wEnumIndex=0;
	DWORD dwCount=m_ServerUserManager.GetUserItemCount();

#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(L"群发数据 在线人数[%d]",dwCount);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif

	while(wEnumIndex<dwCount)
	{
		//过滤用户
		IServerUserItem *pIServerUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIServerUserItem==NULL) continue;
		if(!pIServerUserItem->IsMobileUser()) continue;

		//获取参数
		WORD wBindIndex=pIServerUserItem->GetBindIndex();
		bool bAndroidUser=pIServerUserItem->IsAndroidUser();
		tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
        // 加上指针非空判断，否则这里会报内核异常[9,12]<by hxh 20160708>
        if(!pBindParameter) continue;

		//定义变量
	    //WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
	    WORD wMobileUserRule =  pBindParameter->wVisibleMode;
		WORD wTagerTableID = pIServerUserItem->GetTableID();
		bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
		bool bRecviceGameChat = ((wMobileUserRule&RECVICE_GAME_CHAT)!=0);
		bool bRecviceRoomChat = ((wMobileUserRule&RECVICE_ROOM_CHAT)!=0);
		bool bRecviceRoomWhisper = ((wMobileUserRule&RECVICE_ROOM_WHISPER)!=0);

		//状态过滤
		if(pIServerUserItem->GetUserStatus() >= US_SIT)
		{
			if(wTagerTableID != wCmdTable)continue;
		}

		//聊天过滤
		if(wSubCmdID==SUB_GR_USER_CHAT || wSubCmdID==SUB_GR_USER_EXPRESSION)
		{
			if(!bRecviceGameChat || !bRecviceRoomChat) continue;
		}
		if(wSubCmdID==SUB_GR_WISPER_CHAT || wSubCmdID==SUB_GR_WISPER_EXPRESSION)
		{
			if(!bRecviceRoomWhisper) continue;
		}

		//部分可视
		if(!bViewModeAll)
		{
			//消息过滤
			if(wSubCmdID==SUB_GR_USER_ENTER && wCmdTable==INVALID_TABLE) continue;
			if(wSubCmdID==SUB_GR_USER_SCORE && pIServerUserItem->GetUserStatus() < US_SIT) continue;
			
			//定义变量
			//WORD wTagerDeskPos = pIServerUserItem->GetMobileUserDeskPos();
			//WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
			WORD wTagerDeskPos = 0;
			WORD wTagerDeskCount = pBindParameter->wVisibleTableRand;

			//状态消息过滤
			if(wCmdTable < wTagerDeskPos) continue;
			if(wCmdTable > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//发送消息
		SendData(pIServerUserItem,wMainCmdID,wSubCmdID,pData,wDataSize);
	}

	return true;
}


//群发用户信息
bool CAttemperEngineSink::SendUserInfoPacketBatchToMobileUser(IServerUserItem * pIServerUserItem)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL)
	{
		OutputDebugString(L"Here SendUserInfoPacketBatchToMobileUser pIServerUserItem==NULL");
		return false;
	}

	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	//tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
    tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));

	//用户属性
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	//pUserInfoHead->dwGameID=pUserInfo->dwGameID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	//pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

	//用户属性
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

	//用户状态
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//用户局数
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;

	//用户成绩
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//叠加信息
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

	//发送数据
	WORD wHeadSize=sizeof(tagUserInfoHead);
	SendDataBatchToMobileUser(pUserInfoHead->wTableID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());

	return true;
}


//发可视用户信息
bool CAttemperEngineSink::SendViewTableUserInfoPacketToMobileUser(IServerUserItem * pIServerUserItem,DWORD dwUserIDReq)
{
	//获取参数
	WORD wBindIndex=pIServerUserItem->GetBindIndex();
	bool bAndroidUser=pIServerUserItem->IsAndroidUser();
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);
    if(!pBindParameter) return false;

	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	//tagMobileUserInfoHead * pUserInfoHead=(tagMobileUserInfoHead *)cbBuffer;
    tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	//WORD wMobileUserRule = pIServerUserItem->GetMobileUserRule();
	WORD wMobileUserRule =  pBindParameter->wVisibleMode;
	WORD wTagerTableID = pIServerUserItem->GetTableID();
	WORD wTagerDeskPos = 0;//pIServerUserItem->GetMobileUserDeskPos();
	//WORD wTagerDeskCount = pIServerUserItem->GetMobileUserDeskCount();
    WORD wTagerDeskCount = pBindParameter->wVisibleTableRand;
	bool bViewModeAll = ((wMobileUserRule&VIEW_MODE_ALL)!=0);
	if(wTagerDeskCount==0) wTagerDeskCount=1;

#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(L"发可视用户信息 wMobileUserRule=[%d] 用户[%d][%d]",wMobileUserRule,pIServerUserItem->GetUserID(),dwUserIDReq);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif

	//枚举用户
	WORD wEnumIndex=0;
	while(wEnumIndex<m_ServerUserManager.GetUserItemCount())
	{
		//过滤用户
		IServerUserItem *pIUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);
		if(pIUserItem==NULL || (dwUserIDReq==INVALID_CHAIR && pIUserItem==pIServerUserItem)) continue;
		if(dwUserIDReq != INVALID_CHAIR && pIUserItem->GetUserID() != dwUserIDReq) continue;

		//部分可视
		if(dwUserIDReq==INVALID_CHAIR && !bViewModeAll)
		{
			if(pIUserItem->GetTableID() < wTagerDeskPos) continue;
			if(pIUserItem->GetTableID() > (wTagerDeskPos+wTagerDeskCount-1)) continue;
		}

		//定义变量
		tagUserInfo * pUserInfo=pIUserItem->GetUserInfo();
		ZeroMemory(cbBuffer,sizeof(cbBuffer));
		//CSendPacketHelper SendPacket(cbBuffer+sizeof(tagMobileUserInfoHead),sizeof(cbBuffer)-sizeof(tagMobileUserInfoHead));
        CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));

		//用户属性
		pUserInfoHead->wFaceID=pUserInfo->wFaceID;
		//pUserInfoHead->dwGameID=pUserInfo->dwGameID;
		pUserInfoHead->dwUserID=pUserInfo->dwUserID;
		//pUserInfoHead->dwCustomID=pUserInfo->dwCustomID;

		//用户属性
		pUserInfoHead->cbGender=pUserInfo->cbGender;
		pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;

		//用户状态
		pUserInfoHead->wTableID=pUserInfo->wTableID;
		pUserInfoHead->wChairID=pUserInfo->wChairID;
		pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

		//用户局数
		pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
		pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
		pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
		pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
		pUserInfoHead->dwExperience=pUserInfo->dwExperience;

		//用户成绩
		pUserInfoHead->lScore=pUserInfo->lScore;
		pUserInfoHead->lScore+=pIUserItem->GetTrusteeScore();
		pUserInfoHead->lScore+=pIUserItem->GetFrozenedScore();

		//叠加信息
		SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);

		//发送数据
		//WORD wHeadSize=sizeof(tagMobileUserInfoHead);
        WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	}

	return true;
}

////发送系统消息
//bool CAttemperEngineSink::SendSystemMessage(CMD_GR_SendMessage * pSendMessage, WORD wDataSize)
//{
//	//消息处理
//	ASSERT(pSendMessage!=NULL);
//
//	//效验数据
//	ASSERT(wDataSize==sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength);
//	if (wDataSize!=sizeof(CMD_GR_SendMessage)-sizeof(pSendMessage->szSystemMessage)+sizeof(TCHAR)*pSendMessage->wChatLength) 
//	{
//		OutputDebugString(L"Here SendSystemMessage wDataSize not equal");
//		return false;
//	}
//
//	//所有房间
//	if(pSendMessage->cbAllRoom == TRUE)
//	{
//		pSendMessage->cbAllRoom=FALSE;
//		m_pITCPSocketService->SendData(MDM_CS_MANAGER_SERVICE,SUB_CS_C_SYSTEM_MESSAGE,pSendMessage,wDataSize);
//	}
//	else
//	{
//		//发送系统消息
//		if(pSendMessage->cbGame == TRUE)
//			SendGameMessage(pSendMessage->szSystemMessage,SMT_CHAT|SMT_TABLE_ROLL);
//		if(pSendMessage->cbRoom == TRUE)
//			SendRoomMessage(pSendMessage->szSystemMessage,SMT_CHAT);
//	}
//
//	return true;
//}


//用户离开
VOID CAttemperEngineSink::OnEventUserLogout(IServerUserItem * pIServerUserItem, DWORD dwLeaveReason)
{
	CString strLog;
	strLog.Format(L"CAttemperEngineSink::OnEventUserLogout dwUserID=[%d]",pIServerUserItem->GetUserID());
	CTraceService::TraceString(strLog,TraceLevel_Normal);

	//变量定义
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//提取成绩
	pIServerUserItem->QueryRecordInfo(LeaveGameServer.RecordInfo);
	pIServerUserItem->DistillVariation(LeaveGameServer.VariationInfo);

	//用户信息
	LeaveGameServer.dwLeaveReason=dwLeaveReason;
	LeaveGameServer.dwUserID=pIServerUserItem->GetUserID();
	LeaveGameServer.dwInoutIndex=pIServerUserItem->GetInoutIndex();
	LeaveGameServer.dwDBQuestID=pIServerUserItem->GetDBQuestID();
	LeaveGameServer.dwOnLineTimeCount=(DWORD)(time(NULL))-pIServerUserItem->GetLogonTime();

	//连接信息
	LeaveGameServer.dwClientAddr=pIServerUserItem->GetClientAddr();
	lstrcpyn(LeaveGameServer.szMachineID,pIServerUserItem->GetMachineID(),CountArray(LeaveGameServer.szMachineID));

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer));
	
	//汇总用户
	if (m_bCollectUser==true)
	{
		//变量定义
		CMD_CS_C_UserLeave UserLeave;
		ZeroMemory(&UserLeave,sizeof(UserLeave));

		//设置变量
		UserLeave.dwUserID=pIServerUserItem->GetUserID();

		//发送消息
		m_pITCPSocketService->SendData(MDM_CS_USER_COLLECT,SUB_CS_C_USER_LEAVE,&UserLeave,sizeof(UserLeave));
	}

	//删除分组
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);

	//删除用户
	m_ServerUserManager.DeleteUserItem(pIServerUserItem);

	return;
}


//EWIN网络 这里是分组的
//分配用户
bool CAttemperEngineSink::PerformDistribute()
{
	//人数校验
	ASSERT(m_DistributeManage.GetCount()>=m_pGameServiceOption->wMinDistributeUser);
	if(m_DistributeManage.GetCount()<m_pGameServiceOption->wMinDistributeUser) return false;					

	//分配用户
	while(true)
	{
		//人数校验
		ASSERT(m_DistributeManage.GetCount()>=m_pGameServiceOption->wMinGameUser);
		if(m_DistributeManage.GetCount()<m_pGameServiceOption->wMinGameUser) return false;

		//出错的话跳到这里就不行了   
		if(m_DistributeManage.GetRealCount()==0) return false;   //不存在真人就不分组

		//变量定义
		CDistributeInfoArray DistributeInfoArray;

        //获取用户
		WORD wRandCount = __max(m_pGameServiceOption->wMaxGameUser-m_pGameServiceOption->wMinGameUser+1,1);
		WORD wChairCount = m_pGameServiceOption->wMinGameUser+rand()%wRandCount;
		WORD wDistributeCount = m_DistributeManage.PerformDistribute(DistributeInfoArray,wChairCount);
		if(wDistributeCount < m_pGameServiceOption->wMinGameUser) break;

		//寻找位置
		CTableFrame * pCurrTableFrame=NULL;
		for (WORD i=0;i<m_TableFrameArray.GetCount();i++)
		{
			//获取对象
			ASSERT(m_TableFrameArray[i]!=NULL);
			CTableFrame * pTableFrame=m_TableFrameArray[i];

			//判断是否空
			if(pTableFrame==NULL) continue;

			//状态判断
			if (pTableFrame->GetSitUserCount()==0)
			{
				pCurrTableFrame=pTableFrame;
				break;
			}
		}

		//桌子判断
		if(pCurrTableFrame==NULL) break;

		//玩家坐下
		bool bSitSuccess=true;
		INT_PTR nSitFailedIndex=INVALID_CHAIR;
		for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
		{
			//变量定义
			WORD wChairID=pCurrTableFrame->GetNullChairID();  //获取空位

			//分配用户
			if (wChairID!=INVALID_CHAIR)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=DistributeInfoArray[nIndex].pIServerUserItem;

				//判断用户是否为空值
				if(pIServerUserItem==NULL) continue;

				//用户坐下
				if(pCurrTableFrame->PerformSitDownAction(wChairID,pIServerUserItem)==false)
				{

					bSitSuccess=false;
					nSitFailedIndex=nIndex;
					break;
				}
			}
		}

		//坐下处理
		if(bSitSuccess)
		{
			//移除结点
			while(DistributeInfoArray.GetCount()>0)
			{
				m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[0].pPertainNode);
				DistributeInfoArray.RemoveAt(0);
			}
		}
		else
		{
			for(INT_PTR nIndex=0;nIndex<DistributeInfoArray.GetCount();nIndex++)
			{
				if(nSitFailedIndex==nIndex)
					m_DistributeManage.RemoveDistributeNode((tagDistributeNode *)DistributeInfoArray[nIndex].pPertainNode);
				else
				{
					//用户起立
					if (DistributeInfoArray[nIndex].pIServerUserItem->GetTableID()!=INVALID_TABLE)
					{
						WORD wTableID=DistributeInfoArray[nIndex].pIServerUserItem->GetTableID();
						m_TableFrameArray[wTableID]->PerformStandUpAction(DistributeInfoArray[nIndex].pIServerUserItem);
					}
				}
			}
		}	
	}

	return true;
}

//EWIN网络
//玩家坐下
bool CAttemperEngineSink::PerformUserSitDown(IServerUserItem * pIServerUserItem,WORD wRequestTableID,WORD wRequestChairID)
{
	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();

	//重复判断
	if ((wRequestTableID!=INVALID_TABLE)&&(wRequestChairID!=INVALID_CHAIR))
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wRequestTableID];
		if (pTableFrame->GetTableUserItem(wRequestChairID)==pIServerUserItem) return true;
	}

	//用户判断
	if (cbUserStatus==US_PLAYING)
	{
		//SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_NONE);
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_NONE);
		return true;
	}

	//插入分组
	if ((m_pGameServiceOption->cbDistributeRule&DISTRIBUTE_ALLOW)!=0)
	{
		//变量定义
		SCORE lUserScore=pIServerUserItem->GetUserScore();
		SCORE lMinTableScore=m_pGameServiceOption->lMinTableScore;
		SCORE lMinEnterScore= m_pGameServiceOption->lMinEnterScore;		

		//积分限制
		if (((SCORE_EQUAL(lMinTableScore,0.00f)==false)&&SCORE_LESS(lUserScore,lMinTableScore))||((SCORE_EQUAL(lMinEnterScore,0.00f)==false)&&SCORE_LESS(lUserScore,lMinEnterScore)))
		{
			//构造信息
			TCHAR szDescribe[128]=TEXT("");
			_sntprintf(szDescribe,CountArray(szDescribe),TEXT("加入游戏至少需要 ") SCORE_STRING TEXT(" 的游戏币，您的游戏币不够，不能加入！"),__max(lMinTableScore,lMinEnterScore));

			//发送信息
			SendRequestFailure(pIServerUserItem,szDescribe,REQUEST_FAILURE_NOSCORE,REQUEST_FAILURE_CMD_CONCLUDE);

			return true;
		}

		return InsertDistribute(pIServerUserItem);
	}

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	//变量定义
	WORD wTempTableID = wRequestTableID; 
	WORD wTempChairID = wRequestChairID;

	//桌子调整
	if (wTempTableID>=m_TableFrameArray.GetCount())
	{
		//查询桌子
		WORD wStartTableID=0;
		if (CServerRule::IsAllowAvertCheatMode(m_pGameServiceOption->dwServerRule)==true) wStartTableID=1;

		//循环条件(查找空桌 动态加入 空闲桌子)
		bool bLoopCondition[] = {true,true,true};
		bLoopCondition[1] = CServerRule::IsAllowDynamicJoin(m_pGameServiceOption->dwServerRule);

		//寻找位置
		for(WORD dwLoopIndex=0;dwLoopIndex<CountArray(bLoopCondition);dwLoopIndex++)
		{
			//条件判断
			if(bLoopCondition[dwLoopIndex]==false) continue;

			//寻找位置
			for (WORD i=wStartTableID;i<m_TableFrameArray.GetCount();i++)
			{
				//桌子状态
				if(dwLoopIndex==0 && (m_TableFrameArray[i]->IsGameStarted()==true || m_TableFrameArray[i]->GetSitUserCount()==0)) continue;
				if(dwLoopIndex==1 && m_TableFrameArray[i]->IsGameStarted()==false) continue;
				if(dwLoopIndex==2 && m_TableFrameArray[i]->IsGameStarted()==true) continue;

				//机器判断
				if(pIServerUserItem->IsAndroidUser()==true)
				{
					//变量定义
					WORD wMinUserCount = GetGameMinUserCount();
					if(wMinUserCount==0) break;

					//机器判断
					if(wMinUserCount > 1)
					{
						//变量定义
						WORD wAndroidCount = m_TableFrameArray[i]->GetAndroidCount();
						if((wAndroidCount==m_TableFrameArray[i]->GetSitUserCount()) && (wAndroidCount>=wMinUserCount-1))								continue;
					}
				}

				//获取空位
				WORD wNullChairID=m_TableFrameArray[i]->GetNullChairID();

				//调整结果
				if (wNullChairID!=INVALID_CHAIR)
				{
					//设置变量
					wTempTableID=i;
					wTempChairID=wNullChairID;

					break;
				}
			}

			//结果判断
			if(wTempTableID!=INVALID_TABLE && wTempChairID!=INVALID_CHAIR) break;
		}

		//结果判断
		if ((wTempTableID==INVALID_CHAIR)||(wTempChairID==INVALID_CHAIR))
		{
			SendRequestFailure(pIServerUserItem,TEXT("当前游戏房间已经人满为患了，暂时没有可以让您加入的位置，请稍后再试！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_NONE);
			return true;
		}
	}

	//椅子调整
	if (wTempChairID>=m_pGameServiceAttrib->wChairCount)
	{
		//效验参数
		ASSERT(wTempTableID<m_TableFrameArray.GetCount());
		if (wTempTableID>=m_TableFrameArray.GetCount()) return false;

		//查找空位
		wTempChairID=m_TableFrameArray[wTempTableID]->GetNullChairID();

		//结果判断
		if (wTempChairID==INVALID_CHAIR)
		{
			SendRequestFailure(pIServerUserItem,TEXT("由于此游戏桌暂时没有可以让您加入的位置了，请选择另外的游戏桌！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_CONCLUDE);
			return true;
		}
	}
	
	//坐下处理
	CTableFrame * pTableFrame=m_TableFrameArray[wTempTableID];
	pTableFrame->PerformSitDownAction(wTempChairID,pIServerUserItem);
#ifdef USE_RS_PRINT
	CString strLog;
	strLog.Format(L"用户[%d]坐下,[%d]桌[%d]座",pIServerUserItem->GetUserID(),wTempTableID,wTempChairID);
	CTraceService::TraceString(strLog,TraceLevel_Normal);
#endif

	return true;
}

//玩家起立
bool CAttemperEngineSink::PerformUserStandUp(IServerUserItem * pIServerUserItem,BOOL cbForceLeave)
{
	//参数校验
	ASSERT(pIServerUserItem!=NULL);
	if(pIServerUserItem==NULL) return false;

	//消息处理
	WORD wTableID=pIServerUserItem->GetTableID();
	WORD wChairID=pIServerUserItem->GetChairID();

	//用户判断
	if ((cbForceLeave==FALSE)&&(pIServerUserItem->GetUserStatus()==US_PLAYING))
	{
		SendRequestFailure(pIServerUserItem,TEXT("您正在游戏中，暂时不能离开，请先结束当前游戏！"),REQUEST_FAILURE_NORMAL,REQUEST_FAILURE_CMD_NONE);
		return true;
	}

	//删除分组
	m_DistributeManage.RemoveDistributeNode(pIServerUserItem);

	//离开处理
	if (wTableID!=INVALID_TABLE)
	{
		CTableFrame * pTableFrame=m_TableFrameArray[wTableID];
		if (pTableFrame->PerformStandUpAction(pIServerUserItem)==false) return true;
	}

	return true;
}

//解锁金币
bool CAttemperEngineSink::PerformUnlockScore(DWORD dwUserID, DWORD dwInoutIndex, DWORD dwLeaveReason)
{
	//变量定义
	DBR_GR_LeaveGameServer LeaveGameServer;
	ZeroMemory(&LeaveGameServer,sizeof(LeaveGameServer));

	//设置变量
	LeaveGameServer.dwUserID=dwUserID;
	LeaveGameServer.dwInoutIndex=dwInoutIndex;
	LeaveGameServer.dwLeaveReason=dwLeaveReason;

	//投递请求
	m_pIKernelDataBaseEngine->PostDataBaseRequest(DBR_GR_LEAVE_GAME_SERVER,0L,&LeaveGameServer,sizeof(LeaveGameServer));

	return true;
}

//版本检查
bool CAttemperEngineSink::PerformCheckVersion(DWORD dwPlazaVersion, DWORD dwFrameVersion, DWORD dwClientVersion, DWORD dwSocketID,bool IsMobileUser)
{
	// 手机端忽略游戏版本号<add by hxh 20161031>
	if(IsMobileUser)
		return true;
	
	//变量定义
	bool bMustUpdateClient=false;
	bool bAdviceUpdateClient=false;

	//游戏版本
	if( VERSION_EFFICACY==TRUE )
	{
		if (GetSubVer(dwClientVersion)<GetSubVer(m_pGameServiceAttrib->dwClientVersion)) bAdviceUpdateClient=true;
		if (GetMainVer(dwClientVersion)!=GetMainVer(m_pGameServiceAttrib->dwClientVersion)) bMustUpdateClient=true;
		if (GetProductVer(dwClientVersion)!=GetProductVer(m_pGameServiceAttrib->dwClientVersion)) bMustUpdateClient=true;
	}
	else
	{
		if (GetSubVer(dwClientVersion)<GetSubVer(m_pGameParameter->dwClientVersion))
		{
			bAdviceUpdateClient=true;
		}
		if (GetMainVer(dwClientVersion)!=GetMainVer(m_pGameParameter->dwClientVersion))
		{
			bMustUpdateClient=true;
		}
		if (GetProductVer(dwClientVersion)!=GetProductVer(m_pGameParameter->dwClientVersion))
		{
			bMustUpdateClient=true;
		}
	}

	CString strLog;
	strLog.Format(L"BANBENHAO GetSubVer(dwPlazaVersion) = %d,GetSubVer(VERSION_PLAZA) = %d",GetSubVer(dwClientVersion),GetSubVer(m_pGameParameter->dwClientVersion));
	OutputDebugString(strLog);
	strLog.Format(L"BANBENHAO GetMainVer(dwPlazaVersion) = %d,GetMainVer(VERSION_PLAZA) = %d",GetMainVer(dwClientVersion),GetMainVer(m_pGameParameter->dwClientVersion));
	OutputDebugString(strLog);
	strLog.Format(L"BANBENHAO GetProductVer(dwPlazaVersion) = %d,GetProductVer(VERSION_PLAZA) = %d",GetProductVer(dwClientVersion),GetProductVer(m_pGameParameter->dwClientVersion));
	OutputDebugString(strLog);

	//更新通知
	if ((bMustUpdateClient==true)||(bAdviceUpdateClient==true))
	{
		//变量定义
		CMD_GR_UpdateNotify UpdateNotify;
		ZeroMemory(&UpdateNotify,sizeof(UpdateNotify));

		//变量定义
		UpdateNotify.cbMustUpdatePlaza=false;
		UpdateNotify.cbMustUpdateClient=bMustUpdateClient;
		UpdateNotify.cbAdviceUpdateClient=bAdviceUpdateClient;

		//当前版本
		UpdateNotify.dwCurrentPlazaVersion=VERSION_PLAZA;
		UpdateNotify.dwCurrentFrameVersion=VERSION_FRAME;
		UpdateNotify.dwCurrentClientVersion=m_pGameServiceAttrib->dwClientVersion;

		//发送消息
		SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_UPDATE_NOTIFY,&UpdateNotify,sizeof(UpdateNotify));

		//中止判断
		if (bMustUpdateClient==true)
		{
			m_pITCPNetworkEngine->ShutDownSocket(dwSocketID);
			return false;
		}
	}

	return true;
}

//切换连接
bool CAttemperEngineSink::SwitchUserItemConnect(IServerUserItem * pIServerUserItem, TCHAR szMachineID[LEN_MACHINE_ID], WORD wTargetIndex)
{
	//效验参数
	ASSERT((pIServerUserItem!=NULL)&&(wTargetIndex!=INVALID_WORD));
	if ((pIServerUserItem==NULL)||(wTargetIndex==INVALID_WORD)) return false;

	//断开用户
	if (pIServerUserItem->GetBindIndex()!=INVALID_WORD)
	{
		//发送通知
		LPCTSTR pszMessage=TEXT("请注意，您的帐号在另一地方进入了此游戏房间，您被迫离开！");
		SendRoomMessage(pIServerUserItem,pszMessage,SMT_CHAT|SMT_EJECT|SMT_GLOBAL|SMT_CLOSE_ROOM);

		//绑定参数
		WORD wSourceIndex=pIServerUserItem->GetBindIndex();
		tagBindParameter * pSourceParameter=GetBindParameter(wSourceIndex);

		//解除绑定
		ASSERT((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem));
		if ((pSourceParameter!=NULL)&&(pSourceParameter->pIServerUserItem==pIServerUserItem)) pSourceParameter->pIServerUserItem=NULL;

		//断开用户
		if (pIServerUserItem->IsAndroidUser()==true)
		{
			m_AndroidUserManager.DeleteAndroidUserItem(pSourceParameter->dwSocketID);
		}
		else m_pITCPNetworkEngine->ShutDownSocket(pSourceParameter->dwSocketID);
	}

	//状态切换
	if (pIServerUserItem->GetUserStatus()==US_OFFLINE)
	{
		WORD wTableID=pIServerUserItem->GetTableID();
		WORD wChairID=pIServerUserItem->GetChairID();
		pIServerUserItem->SetUserStatus(US_PLAYING,wTableID,wChairID);
	}

	//变量定义
	bool bAndroidUser=(wTargetIndex>=INDEX_ANDROID);
	tagBindParameter * pTargetParameter=GetBindParameter(wTargetIndex);

	//激活用户
	pTargetParameter->pIServerUserItem=pIServerUserItem;
	pIServerUserItem->SetUserParameter(pTargetParameter->dwClientAddr,wTargetIndex,szMachineID,bAndroidUser,false);

	//登录事件
	OnEventUserLogon(pIServerUserItem);

	return true;
}

//登录失败
bool CAttemperEngineSink::SendLogonFailure(LPCTSTR pszString, LONG lErrorCode, DWORD dwSocketID)
{
	//变量定义
	CMD_GR_LogonFailure LogonFailure;
	ZeroMemory(&LogonFailure,sizeof(LogonFailure));

	//构造数据
	LogonFailure.lErrorCode=lErrorCode;
	lstrcpyn(LogonFailure.szDescribeString,pszString,CountArray(LogonFailure.szDescribeString));

	//数据属性
	WORD wDataSize=CountStringBuffer(LogonFailure.szDescribeString);
	WORD wHeadSize=sizeof(LogonFailure)-sizeof(LogonFailure.szDescribeString);

	//发送数据
	SendData(dwSocketID,MDM_GR_LOGON,SUB_GR_LOGON_FAILURE,&LogonFailure,wHeadSize+wDataSize);

	return true;
}

//银行失败
bool CAttemperEngineSink::SendInsureFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszString, LONG lErrorCode)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	CMD_GR_S_UserInsureFailure UserInsureFailure;
	ZeroMemory(&UserInsureFailure,sizeof(UserInsureFailure));

	//构造数据
	UserInsureFailure.lErrorCode=lErrorCode;
	lstrcpyn(UserInsureFailure.szDescribeString,pszString,CountArray(UserInsureFailure.szDescribeString));

	//数据属性
	WORD wDataSize=CountStringBuffer(UserInsureFailure.szDescribeString);
	WORD wHeadSize=sizeof(UserInsureFailure)-sizeof(UserInsureFailure.szDescribeString);

	//发送数据
	SendData(pIServerUserItem,MDM_GR_INSURE,SUB_GR_USER_INSURE_FAILURE,&UserInsureFailure,wHeadSize+wDataSize);

	return true;
}

//请求失败
bool CAttemperEngineSink::SendRequestFailure(IServerUserItem * pIServerUserItem, LPCTSTR pszDescribe, BYTE cbFailureCode, BYTE cbCommandCode)
{
	//变量定义
	CMD_GR_RequestFailure RequestFailure;
	ZeroMemory(&RequestFailure,sizeof(RequestFailure));

	//设置变量
	RequestFailure.cbFailureCode=cbFailureCode;
	RequestFailure.cbCommandCode=cbCommandCode;
	lstrcpyn(RequestFailure.szDescribeString,pszDescribe,CountArray(RequestFailure.szDescribeString));

	//发送数据
	WORD wDataSize=CountStringBuffer(RequestFailure.szDescribeString);
	WORD wHeadSize=sizeof(RequestFailure)-sizeof(RequestFailure.szDescribeString);
	SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_REQUEST_FAILURE,&RequestFailure,wHeadSize+wDataSize);

	return true;
}

//发送用户
bool CAttemperEngineSink::SendUserInfoPacket(IServerUserItem * pIServerUserItem, DWORD dwSocketID)
{
	//效验参数
	ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//变量定义
	BYTE cbBuffer[SOCKET_TCP_PACKET];
	tagUserInfo * pUserInfo=pIServerUserItem->GetUserInfo();
	tagUserInfoHead * pUserInfoHead=(tagUserInfoHead *)cbBuffer;
	CSendPacketHelper SendPacket(cbBuffer+sizeof(tagUserInfoHead),sizeof(cbBuffer)-sizeof(tagUserInfoHead));

	//用户属性
	pUserInfoHead->wFaceID=pUserInfo->wFaceID;
	pUserInfoHead->dwUserID=pUserInfo->dwUserID;
	pUserInfoHead->cbGender=pUserInfo->cbGender;
	pUserInfoHead->cbMemberOrder=pUserInfo->cbMemberOrder;
	pUserInfoHead->cbMasterOrder=pUserInfo->cbMasterOrder;

	//用户状态
	pUserInfoHead->wTableID=pUserInfo->wTableID;
	pUserInfoHead->wChairID=pUserInfo->wChairID;
	pUserInfoHead->cbUserStatus=pUserInfo->cbUserStatus;

	//用户局数
	pUserInfoHead->dwWinCount=pUserInfo->dwWinCount;
	pUserInfoHead->dwLostCount=pUserInfo->dwLostCount;
	pUserInfoHead->dwDrawCount=pUserInfo->dwDrawCount;
	pUserInfoHead->dwFleeCount=pUserInfo->dwFleeCount;
	pUserInfoHead->dwExperience=pUserInfo->dwExperience;

	//用户成绩
	pUserInfoHead->lScore=pUserInfo->lScore;
	pUserInfoHead->lInsure=pUserInfo->lInsure;
	pUserInfoHead->lScore+=pIServerUserItem->GetTrusteeScore();
	pUserInfoHead->lScore+=pIServerUserItem->GetFrozenedScore();

	//叠加信息
	SendPacket.AddPacket(pUserInfo->szNickName,DTP_GR_NICK_NAME);
	SendPacket.AddPacket(pUserInfo->szAddrDescribe,DTP_GR_ADDR_DESCRIBE);


	CString strLog;
	//发送数据
	if (dwSocketID==INVALID_DWORD)
	{
		strLog.Format(L"NEW [%s][%d][%d]进入游戏了",pUserInfo->szNickName,dwSocketID,pUserInfoHead->dwUserID);
		WORD wHeadSize=sizeof(tagUserInfoHead);
		//SendData(BG_ALL_CLIENT,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());//by hxh 20160622
		SendData(BG_COMPUTER,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
		SendUserInfoPacketBatchToMobileUser(pIServerUserItem);
	}
	else
	{
		strLog.Format(L"NEW [%s][%d][%d]进入游戏了",pUserInfo->szNickName,dwSocketID,pUserInfoHead->dwUserID);
		WORD wHeadSize=sizeof(tagUserInfoHead);
		SendData(dwSocketID,MDM_GR_USER,SUB_GR_USER_ENTER,cbBuffer,wHeadSize+SendPacket.GetDataSize());
	
	}
	CTraceService::TraceString(strLog,TraceLevel_Normal);
	return true;
}

//绑定用户
IServerUserItem * CAttemperEngineSink::GetBindUserItem(WORD wBindIndex)
{
	//获取参数
	tagBindParameter * pBindParameter=GetBindParameter(wBindIndex);

	//获取用户
	if (pBindParameter!=NULL)
	{
		return pBindParameter->pIServerUserItem;
	}

	//错误断言
	ASSERT(FALSE);

	return NULL;
}

//绑定参数
tagBindParameter * CAttemperEngineSink::GetBindParameter(WORD wBindIndex)
{
	//无效连接
	if (wBindIndex==INVALID_WORD) return NULL;

	//常规连接
	if (wBindIndex<m_pGameServiceOption->wMaxPlayer)
	{
		return m_pNormalParameter+wBindIndex;
	}

	//机器连接
	if ((wBindIndex>=INDEX_ANDROID)&&(wBindIndex<(INDEX_ANDROID+MAX_ANDROID)))
	{
		return m_pAndroidParameter+(wBindIndex-INDEX_ANDROID);
	}

	//错误断言
	ASSERT(FALSE);

	return NULL;
}

//用户状况
WORD CAttemperEngineSink::GetAndroidStatus(tagAndroidStatus & AndroidStatus)
{
	//变量定义
	WORD wEnumIndex=0;
	IServerUserItem * pIServerUserItem=NULL;

	//设置变量
	ZeroMemory(&AndroidStatus,sizeof(AndroidStatus));

	//用户信息
	do
	{
		//获取用户
		pIServerUserItem=m_ServerUserManager.EnumUserItem(wEnumIndex++);

		//统计判断
		if ((pIServerUserItem!=NULL)&&(pIServerUserItem->IsAndroidUser()==true))
		{
			//用户统计
			switch (pIServerUserItem->GetUserStatus())
			{
			case US_FREE:
				{
					ASSERT(AndroidStatus.wFreeUserCount<CountArray(AndroidStatus.pServerUserFree));
					AndroidStatus.pServerUserFree[AndroidStatus.wFreeUserCount++]=pIServerUserItem;
					break;
				}
			case US_SIT:
			case US_READY:
				{
					ASSERT(AndroidStatus.wSitdownUserCount<CountArray(AndroidStatus.pServerUserSitdown));
					AndroidStatus.pServerUserSitdown[AndroidStatus.wSitdownUserCount++]=pIServerUserItem;
					break;
				}
			case US_PLAYING:
			case US_OFFLINE:
				{
					ASSERT(AndroidStatus.wPlayUserCount<CountArray(AndroidStatus.pServerUserPlay));
					AndroidStatus.pServerUserPlay[AndroidStatus.wPlayUserCount++]=pIServerUserItem;
					break;
				}
			}
		}

	} while (pIServerUserItem!=NULL);

	return AndroidStatus.wFreeUserCount+AndroidStatus.wPlayUserCount+AndroidStatus.wSitdownUserCount;
}

//桌子状况
WORD CAttemperEngineSink::GetTableStatusInfo(CTableFrame * pTableFrame, tagTableStatusInfo & TableStatusInfo)
{
	//设置变量
	ZeroMemory(&TableStatusInfo,sizeof(TableStatusInfo));

	//用户分析
	for (WORD i=0;i<m_pGameServiceAttrib->wChairCount;i++)
	{
		//获取用户
		IServerUserItem * pIServerUserItem=pTableFrame->GetTableUserItem(i);
		if (pIServerUserItem==NULL) continue;

		//用户类型
		if (pIServerUserItem->IsAndroidUser()==false)
		{
			TableStatusInfo.wTableUserCount++;
		}
		else
		{
			TableStatusInfo.wTableAndroidCount++;
		}

		//准备判断
		if (pIServerUserItem->GetUserStatus()==US_READY)
		{
			TableStatusInfo.wTableReadyCount++;
		}
	}

	//最少数目
	switch (pTableFrame->GetStartMode())
	{
	case START_MODE_ALL_READY:		//所有准备
		{
			TableStatusInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_PAIR_READY:		//配对开始
		{
			TableStatusInfo.wMinUserCount=2;
			break;
		}
	case START_MODE_TIME_CONTROL:	//时间控制
		{
			TableStatusInfo.wMinUserCount=1;
			break;
		}
	default:						//默认模式
		{
			TableStatusInfo.wMinUserCount=m_pGameServiceAttrib->wChairCount;
			break;
		}
	}

	return TableStatusInfo.wTableAndroidCount+TableStatusInfo.wTableUserCount;
}

//最少人数
WORD CAttemperEngineSink::GetGameMinUserCount()
{
	//桌子判断
	if(m_TableFrameArray.GetCount()<=0) return 0;

	//变量定义
	WORD wMinUserCount = 0;
	CTableFrame *  pTableFrame = m_TableFrameArray[0];
	ASSERT(pTableFrame!=NULL);

	//最少数目
	switch (pTableFrame->GetStartMode())
	{
	case START_MODE_ALL_READY:		//所有准备
		{
			wMinUserCount=2;
			break;
		}
	case START_MODE_PAIR_READY:		//配对开始
		{
			wMinUserCount=2;
			break;
		}
	case START_MODE_TIME_CONTROL:	//时间控制
		{
			wMinUserCount=1;
			break;
		}
	default:						//默认模式
		{
			wMinUserCount=m_pGameServiceAttrib->wChairCount;
			break;
		}
	}

	return wMinUserCount;
}

//调整参数
VOID CAttemperEngineSink::RectifyDistributeParameter()
{
	//游戏最少人数
	WORD wMinGameUser=m_pGameServiceOption->wMinGameUser;
	if(m_TableFrameArray.GetCount()>0)
	{
		CTableFrame * pTableFrame = m_TableFrameArray.GetAt(0);
		if(pTableFrame!=NULL)
		{
			//最少数目
			switch (pTableFrame->GetStartMode())
			{
			case START_MODE_ALL_READY:		//所有准备
			case START_MODE_PAIR_READY:     //配对开始
				{
					wMinGameUser = __max(wMinGameUser,2);
					break;
				}
			case START_MODE_TIME_CONTROL:	//时间控制
				{
					wMinGameUser = 1;
					break;
				}
			default:						//默认模式
				{
					wMinGameUser=m_pGameServiceAttrib->wChairCount;
					break;
				}
			}
		}
	}
	m_pGameServiceOption->wMinGameUser=wMinGameUser;

	//游戏最大人数
	if(m_pGameServiceOption->wMaxGameUser<m_pGameServiceOption->wMinGameUser)
		m_pGameServiceOption->wMaxGameUser=m_pGameServiceAttrib->wChairCount;	
	if(m_pGameServiceOption->wMaxGameUser>m_pGameServiceAttrib->wChairCount)
		m_pGameServiceOption->wMaxGameUser=m_pGameServiceAttrib->wChairCount;

	//最小分组人数
	m_pGameServiceOption->wMinDistributeUser = __max(m_pGameServiceOption->wMinDistributeUser,m_pGameServiceOption->wMinGameUser);

	//分组间隔
	WORD wDistributeTime = m_pGameServiceOption->wDistributeTimeSpace!=0?__min(m_pGameServiceOption->wDistributeTimeSpace,2*TIME_DEFINITE_TIME_GROUP):TIME_DEFINITE_TIME_GROUP;
	m_pGameServiceOption->wDistributeTimeSpace=wDistributeTime;

	return;
};

//配置机器
bool CAttemperEngineSink::InitAndroidUser()
{
	//机器参数
	tagAndroidUserParameter AndroidUserParameter;
	ZeroMemory(&AndroidUserParameter,sizeof(AndroidUserParameter));

	//配置参数
	AndroidUserParameter.pGameParameter=m_pGameParameter;
	AndroidUserParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	AndroidUserParameter.pGameServiceOption=m_pGameServiceOption;

	//服务组件
	AndroidUserParameter.pITimerEngine=m_pITimerEngine;
	AndroidUserParameter.pIServerUserManager=&m_ServerUserManager;
	AndroidUserParameter.pIGameServiceManager=m_pIGameServiceManager;
	AndroidUserParameter.pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(m_pIAttemperEngine,ITCPNetworkEngineEvent);

	//设置对象
	if (m_AndroidUserManager.InitAndroidUser(AndroidUserParameter)==false)
	{
		return false;
	}

	return true;
}

//配置桌子
bool CAttemperEngineSink::InitTableFrameArray()
{
	//桌子参数
	tagTableFrameParameter TableFrameParameter;
	ZeroMemory(&TableFrameParameter,sizeof(TableFrameParameter));

	//内核组件
	TableFrameParameter.pITimerEngine=m_pITimerEngine;
	TableFrameParameter.pIKernelDataBaseEngine=m_pIKernelDataBaseEngine;
	TableFrameParameter.pIRecordDataBaseEngine=m_pIRecordDataBaseEngine;

	//服务组件
	TableFrameParameter.pIMainServiceFrame=this;
	TableFrameParameter.pIGameServiceManager=m_pIGameServiceManager;
	TableFrameParameter.pIAndroidUserManager=&m_AndroidUserManager;

	//配置参数
	TableFrameParameter.pGameParameter=m_pGameParameter;
	TableFrameParameter.pGameServiceAttrib=m_pGameServiceAttrib;
	TableFrameParameter.pGameServiceOption=m_pGameServiceOption;

	//桌子容器
	m_TableFrameArray.SetSize(m_pGameServiceOption->wTableCount);
	ZeroMemory(m_TableFrameArray.GetData(),m_pGameServiceOption->wTableCount*sizeof(CTableFrame *));

	//创建桌子
	for (WORD i=0;i<m_pGameServiceOption->wTableCount;i++)
	{
		m_TableFrameArray[i]=new CTableFrame;
		if (m_TableFrameArray[i]->InitializationFrame(i,TableFrameParameter)==false) return false;
	}

	return true;
}

//发送请求
bool CAttemperEngineSink::SendUIControlPacket(WORD wRequestID, VOID * pData, WORD wDataSize)
{
	//发送数据
	CServiceUnits * pServiceUnits=CServiceUnits::g_pServiceUnits;
	pServiceUnits->PostControlRequest(wRequestID,pData,wDataSize);

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
