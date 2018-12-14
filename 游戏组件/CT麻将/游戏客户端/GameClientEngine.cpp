#include "Stdafx.h"
#include "GameClient.h"
////#include "GameOption.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////

//游戏定时器
#define IDI_START_GAME				200									//开始定时器
#define IDI_OPERATE_CARD			201									//操作定时器
#define IDI_REPLACE_CARD			301									//补牌定时器

//游戏定时器
#define TIME_START_GAME				30									//开始定时器
#define TIME_OPERATE_CARD			20									//操作定时器		
#define TIME_REPLACE_CARD			1000								//补牌定时器

#define MAX_TIME_OUT				3									//最大超时次数

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientEngine, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
	ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
	ON_MESSAGE(IDM_USER_ACTION_FINISH,OnUserActionFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientEngine::CGameClientEngine() : CGameFrameEngine()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;

	//托管变量
	m_bStustee=false;
	m_wTimeOutCount =0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;
	ZeroMemory( m_cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	return;
}

//析构函数
CGameClientEngine::~CGameClientEngine()
{
}

//初始函数
bool CGameClientEngine::OnInitGameEngine()
{


	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

//////////////
	m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);


	//////加载资源
	////g_CardResource.LoadResource();

	//加载声音
	m_DirectSound[0].Create( TEXT("OUT_CARD") );
	m_DirectSound[1].Create( TEXT("SEND_CARD") );
	m_DirectSound[2].Create( TEXT("GAME_START") );


	//test
	//BYTE cbCardData[] = {
	//	0x13,0x12,0x16,0x15,0x22,0x23,0x25,0x26,0x05,
	//	0x03,0x03,0x03,0x03
	//};
	//BYTE cbCardCount = 13;
	//BYTE cbCurrentCard = 0x06;
	//
	//m_GameLogic.SetMagicIndex( m_GameLogic.SwitchToCardIndex(0x03) );

	//BYTE cbCardIndex[MAX_INDEX];
	//m_GameLogic.SwitchToCardIndex( cbCardData,cbCardCount,cbCardIndex );

	//CChiHuRight chr;
	//DWORD dw1 = GetTickCount();
	//BYTE byResult = m_GameLogic.AnalyseChiHuCard( cbCardIndex,NULL,0,cbCurrentCard,chr );
	//DWORD dw2 = GetTickCount();
	//DWORD dw = dw2-dw1;
	//TCHAR szBuffer[32] = TEXT("");
	//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),dw );
	//MessageBox( szBuffer );
	//end test

	return true;
}

//重置框架
bool CGameClientEngine::OnResetGameEngine()
{
	//游戏变量
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	m_cbListenStatus = 0;
	m_bWillHearStatus=false;
	m_lSiceCount = 0;
	
	//托管变量
	m_bStustee=false;
	m_wTimeOutCount =0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	return true;
}

//////游戏设置
////void CGameClientEngine::OnGameOptionSet()
////{
////	//构造数据
////	CGameOption GameOption;
////	GameOption.m_bEnableSound=IsEnableSound();
////	GameOption.m_bAllowLookon = IsAllowUserLookon();
////	GameOption.m_bEanbleAnimate = m_GameClientView.IsEnableAnimate();
////	
////	//配置数据
////	if (GameOption.DoModal()==IDOK)
////	{
////		EnableSound(GameOption.m_bEnableSound);
////		AllowUserLookon(0,GameOption.m_bAllowLookon);
////		m_GameClientView.EnableAnimate(GameOption.m_bEanbleAnimate);
////		for( BYTE i = 0; i < CountArray(m_DirectSound); i++ )
////			m_DirectSound[i].EnableSound(GameOption.m_bEnableSound);
////	}
////
////	return;
////}

//时间消息
bool CGameClientEngine::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//开始游戏
		{
			if( m_bStustee && nElapse < TIME_START_GAME )
			{
				OnStart(0,0);
				return true;
			}
			if (nElapse==0)
			{
				////PostMessage(WM_CLOSE);
				AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
				return true;
			}
			if (nElapse<=5)
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	case IDI_OPERATE_CARD:		//操作定时器
		{
			//自动出牌
			bool bAutoOutCard=false;
			if ((bAutoOutCard==true)&&(m_GameClientView.m_ControlWnd.IsWindowVisible()))
				bAutoOutCard=false;
			if((bAutoOutCard==false)&&(m_bStustee==true))
			{
				bAutoOutCard=true;
			}

			//超时判断
			if ( (IsLookonMode()==false)&&
				( nElapse==0 || bAutoOutCard || (m_cbListenStatus&&!m_bWillHearStatus&&nElapse<TIME_OPERATE_CARD) ) )
			{
				//获取位置
				WORD wMeChairID=GetMeChairID();

				//动作处理
				if (wChairID==wMeChairID)
				{
					//玩家未听牌,且未托管,则累加超时次数
					if( m_cbListenStatus == 0 && m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
					{
						m_wTimeOutCount = 0;
						OnStusteeControl(TRUE,0);
						m_pIStringMessage->InsertSystemString(TEXT("由于您多次超时，切换为“系统托管”模式."));
					}

					if (m_wCurrentUser==wMeChairID)
					{
						//获取扑克
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();

						//听牌状态
						if( m_cbListenStatus >= 2 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//如果在操作模式下
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						//出牌效验
						if (VerdictOutCard(cbCardData)==false)
						{
							for (BYTE i=0;i<MAX_INDEX;i++)
							{
								//出牌效验
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//设置变量
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
						}

						//出牌动作
						ASSERT( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)] > 0 );
						OnOutCard(cbCardData,cbCardData);
					}
					else 
					{
						//听牌状态,点炮或自摸和点炮
						if( m_cbListenStatus == 1 || m_cbListenStatus == 3 )
						{
							if( m_cbActionMask & WIK_CHI_HU )
							{
								OnUserAction( WIK_CHI_HU,0 );
								return true;
							}
						}
						//如果在操作模式下
						if( m_GameClientView.m_HandCardControl.GetSelectMode() )
						{
							OnCardOperate(0,0);
							return true;
						}
						OnUserAction(WIK_NULL,0);
					}
				}

				return true;
			}

			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) 
			{
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_WARN"));
			}

			return true;
		}
	}

	return true;
}

//旁观状态
bool CGameClientEngine::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
	//扑克控制
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//网络消息
bool CGameClientEngine::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_GAME_START:		//游戏开始
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//用户出牌
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//发牌消息
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//操作提示
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//操作结果
		{
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//用户托管
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
	case SUB_S_LISTEN:			//玩家听牌
		{
			return OnSubListen( pBuffer,wDataSize );
		}
	case SUB_S_REPLACE_CARD:	//补花
		{
			return OnSubReplaceCard( pBuffer,wDataSize );
		}
	}

	return true;
}

//游戏场景
bool CGameClientEngine::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//////是否启用声音
			////if( !IsEnableSound() )
			////{
			////	for( BYTE i = 0; i < CountArray(m_DirectSound); i++ )
			////		m_DirectSound[i].EnableSound(FALSE);
			////}

			//设置数据
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//托管设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//设置控件
			////if (IsLookonMode()==false)
			if (IsLookonMode()==false && GetMeUserItem()->GetUserStatus()!=US_READY)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
				SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			//丢弃效果
			m_GameClientView.SetDiscUser(INVALID_CHAIR);

			//更新界面
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	case GS_MJ_PLAY:	//游戏状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//////是否启用声音
			////if( !IsEnableSound() )
			////{
			////	for( BYTE i = 0; i < CountArray(m_DirectSound); i++ )
			////		m_DirectSound[i].EnableSound(FALSE);
			////}

			//辅助变量
			WORD wMeChairID=GetMeChairID();
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//设置变量
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
			m_cbListenStatus = pStatusPlay->cbHearStatus[wMeChairID];
			m_wReplaceUser = pStatusPlay->wReplaceUser;
			m_wHeapHead = pStatusPlay->wHeapHead;
			m_wHeapTail = pStatusPlay->wHeapTail;

			//用户名称
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				IClientUserItem * pUserData=GetTableUserItem(i);
				ASSERT( pUserData != NULL );
				if( pUserData == NULL ) 
				{
					m_szAccounts[i][0] = 0;
					continue;
				}
				lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
			}

			//旁观
			if( IsLookonMode()==true )
				m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());

			//托管设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusPlay->bTrustee[i]);
			}
			
			if( pStatusPlay->bTrustee[wMeChairID] )
			{
				OnStusteeControl(0,0);
			}

			m_wTimeOutCount=0;
			if(pStatusPlay->bTrustee[wMeChairID])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false, false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false, false);

			//扑克变量
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//界面设置
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//组合扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wOperateViewID = SwitchViewChairID(i);
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(m_WeaveItemArray[i][j].cbCardData,cbWeaveCardCount);
					if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i)) 
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetDirectionCardPos((BYTE)wProviderViewID);

				}
			}

			//用户扑克
			if (m_wCurrentUser==wMeChairID&&m_wReplaceUser==INVALID_CHAIR&&
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(pStatusPlay->cbSendCardData)] > 0 )
			{
				SetHandCardControl(m_cbCardIndex,pStatusPlay->cbSendCardData);
			}
			else
			{
				SetHandCardControl(m_cbCardIndex,0x00);
			}

			//扑克设置
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//用户扑克
				if (i!=wMeChairID)
				{
					BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[i]*3-1;
					WORD wUserCardIndex=(wViewChairID[i]<2)?wViewChairID[i]:2;
					m_GameClientView.m_UserCard[wUserCardIndex].SetCardData(cbCardCount,(m_wCurrentUser==i));
				}

				//丢弃扑克
				m_GameClientView.m_DiscardCard[wViewChairID[i]].SetCardData(pStatusPlay->cbDiscardCard[i],pStatusPlay->cbDiscardCount[i]);
			}

			//丢弃效果
			if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
			{
				m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));
				m_GameClientView.SetOutCardInfo( wViewChairID[pStatusPlay->wOutCardUser],pStatusPlay->cbOutCardData );
				m_GameClientView.m_DiscardCard[wViewChairID[pStatusPlay->wOutCardUser]].AddCardItem( pStatusPlay->cbOutCardData );
			}

			//控制设置
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//堆立界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.m_HeapCard[wViewChairID[i]].SetHeapCardInfo(pStatusPlay->cbHeapCardInfo[i][0],pStatusPlay->cbHeapCardInfo[i][1]);
			}

			//听牌状态
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				if( pStatusPlay->cbHearStatus[i] > 0 )
					m_GameClientView.SetUserListenStatus( wViewChairID[i],true );
			}

			//操作界面
			if ((IsLookonMode()==false)&&(pStatusPlay->cbActionMask!=WIK_NULL))
			{
				//获取变量
				m_cbActionMask=pStatusPlay->cbActionMask;
				m_cbActionCard=pStatusPlay->cbActionCard;

				//设置界面
				if (m_wCurrentUser==INVALID_CHAIR)
					SetGameClock(wMeChairID,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				if (IsLookonMode()==false) 
					m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}

			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR)
			{
				//计算时间
				WORD wTimeCount=TIME_OPERATE_CARD;

				//设置时间
				SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
			}
			else if( m_wReplaceUser == wMeChairID && !IsLookonMode() )
			{
				tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);

				pActionReplaceCard->wReplaceUser = m_wReplaceUser;

				m_UserActionArray.Add(pActionReplaceCard);

				BeginUserAction();
			}

			//设置圈风
			m_GameClientView.SetFengQuan( pStatusPlay->cbQuanFeng );

			//更新界面
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	}

	return false;
}

//游戏开始
bool CGameClientEngine::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;

	if( IsLookonMode() )
		StopUserAction();

	//用户名称
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		IClientUserItem * pUserData=GetTableUserItem(i);
		ASSERT( pUserData != NULL );
		if( pUserData == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserData->GetNickName() );
	}

	//设置状态
	SetGameStatus(GS_MJ_PLAY);

	//设置变量
	m_wBankerUser = pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_cbActionMask = pGameStart->cbUserAction;
	m_lSiceCount = pGameStart->lSiceCount;

	//设置扑克
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//设置堆牌头尾
	m_wHeapHead = pGameStart->wHeapHead;
	m_wHeapTail = pGameStart->wHeapTail;
	CopyMemory( m_cbHeapCardInfo,pGameStart->cbHeapCardInfo,sizeof(m_cbHeapCardInfo) );

	//旁观界面
	if (IsLookonMode()==true)
	{
		//设置变量
		m_cbListenStatus = 0;
		m_bWillHearStatus = false;
		m_wReplaceUser = INVALID_CHAIR;

		//组合扑克
		ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
		ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

		m_GameClientView.m_GameScore.RestorationData();
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);

		//设置界面
		m_GameClientView.SetDiscUser(INVALID_CHAIR);
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetBankerUser(INVALID_CHAIR);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);

		//扑克设置
		m_GameClientView.m_UserCard[0].SetCardData(0,false);
		m_GameClientView.m_UserCard[1].SetCardData(0,false);
		m_GameClientView.m_UserCard[2].SetCardData(0,false);
		m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

		//扑克设置
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
				m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
		}

		//堆立扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[i].ResetCardData();
			m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
		}
	}

	//设置界面
	m_GameClientView.SetFengQuan( pGameStart->cbQuanFeng );
	if( WORD(pGameStart->lSiceCount>>16) == 0 )
		m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//播放开始声音
	m_DirectSound[2].Play();

	if( pGameStart->lSiceCount != 0 )
	{
		//摇色子动画
		tagActionSice *pActionSice = (tagActionSice *)ActiveUserAction(AK_Sice);
		ASSERT(pActionSice);
		if( !pActionSice ) return false;
		pActionSice->lSiceCount = pGameStart->lSiceCount;

		m_UserActionArray.Add(pActionSice);
	}

	//发牌动作
	WORD wMeChairID = GetMeChairID();
	BYTE cbHandCardData[MAX_COUNT];
	m_GameLogic.SwitchToCardData( m_cbCardIndex,cbHandCardData );
	//混乱扑克
	m_GameLogic.RandCardData(cbHandCardData,(m_wBankerUser==wMeChairID)?MAX_COUNT:MAX_COUNT-1,cbHandCardData);
	//确定堆牌删除位置
	WORD wHeapId = m_wHeapTail;
	WORD wHeapCardIndex = m_cbHeapCardInfo[m_wHeapTail][1]-1;
	//发4圈，最后1圈发一张
	for( BYTE i = 0; i < 4; i++ )
	{
		//从庄家开始
		WORD wId = m_wBankerUser;
		for( WORD j = 0; j < GAME_PLAYER; j++ )
		{
			WORD wChairId = (wId+GAME_PLAYER-j)%GAME_PLAYER;

			//创建动画项目
			tagActionStartCard *pStartCardItem = (tagActionStartCard *)ActiveUserAction(AK_StartCard);
			ASSERT( pStartCardItem );
			if( !pStartCardItem ) return false;

			//发牌数
			pStartCardItem->cbCardCount = i<3?4:1;
			//发牌值
			if( wChairId == wMeChairID )
				CopyMemory( pStartCardItem->cbCardData,&cbHandCardData[i*4],sizeof(BYTE)*(i<3?4:1) );
			//发牌玩家
			pStartCardItem->wChairId = wChairId;
			//发牌位置
			pStartCardItem->wHeapId = wHeapId;
			//堆牌位置
			pStartCardItem->wHeapCardIndex = wHeapCardIndex;

			pStartCardItem->bLastItem = false;
			//加入动画数组
			m_UserActionArray.Add(pStartCardItem);

			//推进
			if( i < 3 )
			{
				if( wHeapCardIndex <= 3 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-(3-wHeapCardIndex)-1;
				}
				else wHeapCardIndex -= 4;
			}
			else
			{
				if( wHeapCardIndex == 0 )
				{
					wHeapId = (wHeapId+1)%GAME_PLAYER;
					wHeapCardIndex = HEAP_FULL_COUNT-1;
				}
				else wHeapCardIndex--;
			}
		}
	}
	//最后一张，发给庄家
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)ActiveUserAction(AK_StartCard);
	ASSERT( pStartCardItem );
	if( !pStartCardItem ) return false;
	//发牌数
	pStartCardItem->cbCardCount = 1;
	//发牌值
	if( m_wBankerUser == wMeChairID )
		pStartCardItem->cbCardData[0] = cbHandCardData[MAX_COUNT-1];
	//发牌玩家
	pStartCardItem->wChairId = m_wBankerUser;
	//发牌位置
	pStartCardItem->wHeapId = wHeapId;
	//堆牌位置
	pStartCardItem->wHeapCardIndex = wHeapCardIndex;
	//最后一张
	pStartCardItem->bLastItem = true;
	//加入动画数组
	m_UserActionArray.Add(pStartCardItem);

	//补花动作
	if( !IsLookonMode() && pGameStart->wReplaceUser == GetMeChairID() )
	{
		tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);
		ASSERT(pActionReplaceCard);
		if( !pActionReplaceCard ) return false;

		pActionReplaceCard->wReplaceUser = pGameStart->wReplaceUser;

		m_UserActionArray.Add(pActionReplaceCard);
	}

	//开始动作
	BeginUserAction();

	return true;
}

//用户出牌
bool CGameClientEngine::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	if( IsLookonMode() || pOutCard->wOutCardUser != GetMeChairID() )
	{
		tagActionOutCard *pActionOutCard = (tagActionOutCard *)ActiveUserAction(AK_OutCard);
		ASSERT(pActionOutCard);
		if( !pActionOutCard ) return false;

		pActionOutCard->cbOutCardData = pOutCard->cbOutCardData;
		pActionOutCard->wOutCardUser = pOutCard->wOutCardUser;

		//添加动作
		m_UserActionArray.Add(pActionOutCard);

		//开始动作
		BeginUserAction();
	}

	return true;
}

//发牌消息
bool CGameClientEngine::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//消息处理
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	tagActionSendCard *pActionSendCard = (tagActionSendCard *)ActiveUserAction(AK_SendCard);
	ASSERT(pActionSendCard);
	if( !pActionSendCard ) return false;

	pActionSendCard->bTail = pSendCard->bTail;
	pActionSendCard->cbActionMask = pSendCard->cbActionMask;
	pActionSendCard->cbCardData = pSendCard->cbCardData;
	pActionSendCard->wCurrentUser = pSendCard->wCurrentUser;
	pActionSendCard->wSendCardUser = pSendCard->wSendCardUser;

	//添加动作
	m_UserActionArray.Add(pActionSendCard);

	if( !IsLookonMode() && pSendCard->wReplaceUser == GetMeChairID() )
	{
		tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);
		ASSERT(pActionReplaceCard);
		if( !pActionReplaceCard ) return false;

		pActionReplaceCard->wReplaceUser = pSendCard->wReplaceUser;

		m_UserActionArray.Add(pActionReplaceCard);
	}

	//开始动作
	BeginUserAction();

	return true;
}

//补牌消息
bool CGameClientEngine::OnSubReplaceCard(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_ReplaceCard));
	if (wDataSize!=sizeof(CMD_S_ReplaceCard)) return false;

	//变量定义
	CMD_S_ReplaceCard * pReplaceCard=(CMD_S_ReplaceCard *)pBuffer;

	if( IsLookonMode() || pReplaceCard->wReplaceUser != GetMeChairID() )
	{
		tagActionReplaceCard *pActionReplaceCard = (tagActionReplaceCard *)ActiveUserAction(AK_ReplaceCard);
		ASSERT(pActionReplaceCard);
		if( !pActionReplaceCard ) return false;

		pActionReplaceCard->cbReplaceCard = pReplaceCard->cbReplaceCard;
		pActionReplaceCard->wReplaceUser = pReplaceCard->wReplaceUser;

		m_UserActionArray.Add(pActionReplaceCard);

		BeginUserAction();
	}

	return true;
}

//操作提示
bool CGameClientEngine::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return false;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//添加动作
	tagActionActionNotify *pActionActionNotify = (tagActionActionNotify *)ActiveUserAction(AK_ActionNotify);
	ASSERT( pActionActionNotify );
	if( !pActionActionNotify ) return false;

	pActionActionNotify->cbActionCard = pOperateNotify->cbActionCard;
	pActionActionNotify->cbActionMask = pOperateNotify->cbActionMask;

	m_UserActionArray.Add(pActionActionNotify);

	//开始动作
	BeginUserAction();

	return true;
}

//操作结果
bool CGameClientEngine::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) 
		return false;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//添加动作
	tagActionActionResult *pActionActionResult = (tagActionActionResult *)ActiveUserAction(AK_ActionResult);
	ASSERT( pActionActionResult );
	if( !pActionActionResult ) return false;

	pActionActionResult->wOperateUser = pOperateResult->wOperateUser;
	pActionActionResult->wProvideUser = pOperateResult->wProvideUser;
	pActionActionResult->cbOperateCode = pOperateResult->cbOperateCode;
	CopyMemory( pActionActionResult->cbOperateCard,pOperateResult->cbOperateCard,
		sizeof(pOperateResult->cbOperateCard) );

	m_UserActionArray.Add(pActionActionResult);

	//开始动作
	BeginUserAction();

	return true;
}

//游戏结束
bool CGameClientEngine::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

	//创建动作
	tagActionGameEnd *pActionGameEnd = (tagActionGameEnd *)ActiveUserAction(AK_GameEnd);
	CopyMemory( pActionGameEnd->cbCardCount,pGameEnd->cbCardCount,sizeof(pGameEnd->cbCardCount) );
	CopyMemory( pActionGameEnd->cbCardData,pGameEnd->cbCardData,sizeof(pGameEnd->cbCardData) );
	pActionGameEnd->cbProvideCard = pGameEnd->cbProvideCard;
	CopyMemory( pActionGameEnd->dwChiHuKind,pGameEnd->dwChiHuKind,sizeof(pGameEnd->dwChiHuKind) );
	CopyMemory( pActionGameEnd->dwChiHuRight,pGameEnd->dwChiHuRight,sizeof(pGameEnd->dwChiHuRight) );
	pActionGameEnd->cbFanCount = pGameEnd->cbFanCount;
	CopyMemory( pActionGameEnd->lGameScore,pGameEnd->lGameScore,sizeof(pGameEnd->lGameScore) );
	pActionGameEnd->lGameTax = pGameEnd->lGameTax;
	pActionGameEnd->wProvideUser = pGameEnd->wProvideUser;
	pActionGameEnd->cbHuaCardCount = pGameEnd->cbHuaCardCount;

	m_UserActionArray.Add(pActionGameEnd);

	BeginUserAction();

	return true;
}

//用户托管
bool CGameClientEngine::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		IClientUserItem * pUserData=GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]选择了托管功能."),pUserData->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]取消了托管功能."),pUserData->GetNickName());
		m_pIStringMessage->InsertSystemString(szBuffer);
	}

	return true;
}

//用户听牌
bool CGameClientEngine::OnSubListen(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Listen));
	if (wDataSize!=sizeof(CMD_S_Listen)) return false;

	CMD_S_Listen *pListen = (CMD_S_Listen *)pBuffer;

	//添加动作
	tagActionListenCard *pActionListen = (tagActionListenCard *)ActiveUserAction(AK_ListenCard);
	ASSERT(pActionListen);
	if( !pActionListen ) return false;

	pActionListen->wChairId = pListen->wChairId;

	m_UserActionArray.Add(pActionListen);

	BeginUserAction();

	return true;
}

//播放出牌声音
void CGameClientEngine::PlayCardSound(WORD wChairID, BYTE cbCardData)
{
	if(m_GameLogic.IsValidCard(cbCardData) == false)
	{
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}

	//判断性别
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "得不到玩家信息");
		return;
	}
	bool bBoy = (pUserData->GetGender() == 2 ? false : true);
	BYTE cbType= (cbCardData & MASK_COLOR);
	BYTE cbValue= (cbCardData & MASK_VALUE);
	CString strSoundName;
	switch(cbType)
	{
	case 0X30:	//风
		{
			switch(cbValue) 
			{
			case 1:
				{
					strSoundName = _T("F_1");
					break;
				}
			case 2:
				{
					strSoundName = _T("F_2");
					break;
				}
			case 3:
				{
					strSoundName = _T("F_3");
					break;
				}
			case 4:
				{
					strSoundName = _T("F_4");
					break;
				}
			case 5:
				{
					strSoundName = _T("F_5");
					break;
				}
			case 6:
				{
					strSoundName = _T("F_6");
					break;
				}
			case 7:
				{
					strSoundName = _T("F_7");
					break;
				}
			default:
				{
					strSoundName=_T("BU_HUA");
				}

			}
			break;
		}		
	case 0X20:	//筒
		{
			strSoundName.Format(_T("T_%d"), cbValue);
			break;
		}

	case 0X10:	//索
		{
			strSoundName.Format(_T("S_%d"), cbValue);
			break;
		}
	case 0X00:	//万
		{
			strSoundName.Format(_T("W_%d"), cbValue);
			break;
		}
	}	

	if(bBoy)
	{
		strSoundName = _T("BOY_") +strSoundName;
	}
	else
	{
		strSoundName = _T("GIRL_") + strSoundName;
	}
	PlayGameSound(AfxGetInstanceHandle(), strSoundName);
}

//播放声音
void CGameClientEngine::PlayActionSound(WORD wChairID,BYTE cbAction)
{
	//判断性别
	IClientUserItem * pUserData = GetTableUserItem(wChairID);
	if(pUserData == 0)
	{
		assert(0 && "得不到玩家信息");
		return;
	}
	if(wChairID < 0 || wChairID > 3)
	{
		return;
	}
	bool bBoy = (pUserData->GetGender() == 2 ? false : true);

	switch (cbAction)
	{
	case WIK_LEFT:
	case WIK_CENTER:
	case WIK_RIGHT:		//上牌
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI"));		
			break;
		}
	case WIK_PENG:		//碰牌
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_PENG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_PENG"));	
			break;
		}
	case WIK_GANG:		//杠牌
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_GANG"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_GANG"));		
			break;
		}
	case WIK_CHI_HU:	//吃胡
		{

			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_CHI_HU"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_CHI_HU"));		
			break;
		}
	case WIK_REPLACE:	//替换花牌
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_REPLACE"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_REPLACE"));		
			break;
		}
	case WIK_LISTEN:	//听牌
		{
			if(bBoy)
				PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_TING"));
			else
				PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_TING"));		
			break;
		}
	}

	return;
}

//出牌判断
bool CGameClientEngine::VerdictOutCard(BYTE cbCardData)
{
	//听牌判断
	if ((m_cbListenStatus>0)||(m_bWillHearStatus==true))
	{
		//变量定义
		CChiHuRight chr;
		WORD wMeChairID=GetMeChairID();
		BYTE cbWeaveCount=m_cbWeaveCount[wMeChairID];

		//构造扑克
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory(cbCardIndexTemp,m_cbCardIndex,sizeof(cbCardIndexTemp));

		//删除扑克
		if( !m_GameLogic.RemoveCard(cbCardIndexTemp,cbCardData) )
		{
			ASSERT( FALSE );
			return false;
		}

		//听牌判断
		BYTE i=0;
		for ( i=0;i<MAX_INDEX;i++)
		{
			//胡牌分析
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,chr);

			//结果判断
			if (cbHuCardKind!=WIK_NULL)
			{
				break;
			}
		}

		//听牌判断
		return (i!=MAX_INDEX);
	}

	return true;
}

//开始按钮
LRESULT CGameClientEngine::OnStart(WPARAM wParam, LPARAM lParam)
{
	StopUserAction();

	//环境设置
	KillGameClock(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScore.RestorationData();
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//设置界面
	m_GameClientView.SetDiscUser(INVALID_CHAIR);
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.SetUserListenStatus(INVALID_CHAIR,false);
	
	//扑克设置
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
	}

	//堆立扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_HeapCard[i].ResetCardData();
		m_GameClientView.m_HeapCard[i].SetHeapCardInfo(0,0);
	}

	//游戏变量
	m_wCurrentUser=INVALID_CHAIR;
	m_wReplaceUser = INVALID_CHAIR;
	m_cbListenStatus = 0;
	m_bWillHearStatus = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));
	m_wHeapHead = INVALID_CHAIR;
	m_wHeapTail = INVALID_CHAIR;

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//出牌操作
LRESULT CGameClientEngine::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//出牌判断
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
		return 0;

	//听牌判断
	if (((m_cbListenStatus>0)||(m_bWillHearStatus==true))&&(VerdictOutCard((BYTE)wParam)==false))
	{
		m_pIStringMessage->InsertSystemString(TEXT("出此牌不符合游戏规则!"));
		return 0;
	}

	KillGameClock(IDI_OPERATE_CARD);

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;

	//设置界面
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if( m_GameClientView.m_DlgListen.m_hWnd )
		m_GameClientView.m_DlgListen.EndDialog(IDCANCEL);

	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//添加动作
	tagActionOutCard *pActionOutCard = (tagActionOutCard *)ActiveUserAction(AK_OutCard);
	ASSERT( pActionOutCard );
	if( !pActionOutCard ) return 0;

	pActionOutCard->cbOutCardData = cbOutCardData;
	pActionOutCard->wOutCardUser = GetMeChairID();

	m_UserActionArray.Add(pActionOutCard);

	BeginUserAction();

	return 0;
}

//扑克操作
LRESULT CGameClientEngine::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//变量定义
	BYTE cbOperateCode;
	BYTE cbOperateCard[3] = {0,0,0};

	//删除时间
	KillGameClock(IDI_OPERATE_CARD);

	//提取选择信息
	tagSelectCardInfo si;
	m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
	
	//构造操作信息
	cbOperateCode = (BYTE)si.wActionMask;
	cbOperateCard[0] = si.cbActionCard;
	CopyMemory( &cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2 );

	//取消操作模式
	m_GameClientView.m_HandCardControl.SetSelectMode( false );

	//如果是听牌
	if( m_bWillHearStatus && (cbOperateCode&WIK_LISTEN) )
	{
		m_bWillHearStatus = false;
		return OnOutCard( cbOperateCard[0],0 );
	}

	m_wCurrentUser = INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//环境设置
	m_GameClientView.SetStatusFlag(false,true);
	
	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//拖管控制
LRESULT CGameClientEngine::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_wTimeOutCount=0;

	//设置状态
	if (m_bStustee==true)
	{
		m_bStustee=false;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false, false);
		if( FALSE == (BOOL)wParam ) 
			m_pIStringMessage->InsertSystemString(_T("您取消了托管功能."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));
	}
	else
	{
		m_bStustee=true;
		m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false, false);
		if( FALSE == (BOOL)wParam )
			m_pIStringMessage->InsertSystemString(_T("您选择了托管功能."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}

	return 0;
}

//第二次摇色子消息
LRESULT CGameClientEngine::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//设置界面
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	return 0;
}

//设置扑克
void CGameClientEngine::SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard)
{
	//组合数目
	WORD wMeChairID=GetMeChairID();
	BYTE cbWeaveCardCount=m_cbWeaveCount[wMeChairID]*3;

	//转换扑克
	BYTE cbHandCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(cbCardIndex,cbHandCardData);

	//调整扑克
	if ( (cbWeaveCardCount+cbCardCount)==MAX_COUNT )
	{
		if( cbAdvanceCard != 0x00 )
		{
			//删除扑克
			BYTE cbRemoveCard[]={cbAdvanceCard};
			VERIFY( m_GameLogic.RemoveCard(cbHandCardData,cbCardCount,cbRemoveCard,1) );

			//设置扑克
			cbHandCardData[cbCardCount-1]=cbAdvanceCard;
		}
		
		m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount-1,cbHandCardData[cbCardCount-1]);
	}
	else m_GameClientView.m_HandCardControl.SetCardData(cbHandCardData,cbCardCount,0x00);

	return;
}

//定时器消息
VOID CGameClientEngine::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case IDI_REPLACE_CARD:	//用户补牌
		{
			OnTimerReplaceCard();

			return;
		}
	}

	__super::OnTimer(nIDEvent);
}

//玩家操作控件
LRESULT CGameClientEngine::OnUserAction( WPARAM wParam, LPARAM lParam )
{
	//变量定义
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard[3] = {0,0,0};

	//隐藏操作控件
	m_GameClientView.m_ControlWnd.ShowWindow( SW_HIDE );

	//状态判断
	if (cbOperateCode==WIK_NULL)
	{
		//设置变量
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;
		if( m_wCurrentUser == INVALID_CHAIR )
		{
			//删除定时器
			KillGameClock( IDI_OPERATE_CARD );
			//发送消息
			CMD_C_OperateCard OperateCard;
			OperateCard.cbOperateCode = WIK_NULL;
			ZeroMemory( OperateCard.cbOperateCard,sizeof(OperateCard.cbOperateCard) );
			SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
		}
		return 0;
	}

	if( cbOperateCode & WIK_LISTEN )
	{
		ASSERT( m_cbListenStatus == 0 );
		if( m_cbListenStatus != 0 ) return 0;

		//胡牌选项
		if( m_GameClientView.m_DlgListen.DoModal() == IDOK )
		{
			//选择自摸和抓炮
			if( m_GameClientView.m_DlgListen.m_bZhuaPao && m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 3;
			//只自摸
			else if( m_GameClientView.m_DlgListen.m_bZiMo )
				m_cbListenStatus = 2;
			//只抓炮
			else if( m_GameClientView.m_DlgListen.m_bZhuaPao )
				m_cbListenStatus = 1;
			//如果选择了,则发送听牌消息
			if( m_cbListenStatus > 0 )
			{
				//设置即将听牌标志
				m_bWillHearStatus = true;

				//设置变量
				m_cbActionMask = WIK_NULL;
				m_cbActionCard = 0;

				//设置听牌状态
				m_GameClientView.SetUserListenStatus( 2,true );

				//发送听牌命令
				CMD_C_Listen Listen;
				Listen.cbListen = m_cbListenStatus;
				SendSocketData( SUB_C_LISTEN,&Listen,sizeof(Listen) );

				//获取选择组合
				tagSelectCardInfo sci[MAX_WEAVE];
				BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );
				
				//如果只听一张牌
				if( cbInfoCount == 1 )
				{
					m_bWillHearStatus = false;
					OnOutCard( sci[0].cbActionCard,0 );
				}
				//否则由玩家选择
				else
				{
					//设置操作事件
					m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );
					//提示消息
					m_pIStringMessage->InsertSystemString(TEXT("请选择胡口牌"));
				}
			}
		}

		return 0;
	}

	bool bDone = false;
	//胡牌
	if( cbOperateCode & WIK_CHI_HU )
		bDone = true;
	else
	{
		//获取选择组合
		tagSelectCardInfo sci[MAX_WEAVE];
		BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

		//设置操作事件
		bDone = m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );

		//如果完成操作
		if( bDone )
		{
			//设置操作结果
			tagSelectCardInfo si;
			m_GameClientView.m_HandCardControl.GetUserSelectResult( si );
			cbOperateCode = (BYTE)si.wActionMask;
			cbOperateCard[0] = si.cbActionCard;
			CopyMemory( &cbOperateCard[1],si.cbCardData,2*sizeof(BYTE) );

			m_GameClientView.m_HandCardControl.SetSelectMode( false );
		}
		//否则，设置等待选择
		else m_GameClientView.SetStatusFlag( true,false );
	}

	//如果操作完成，直接发送操作命令
	if( bDone )
	{
		//删除定时器
		KillGameClock( IDI_OPERATE_CARD );

		//设置变量
		m_wCurrentUser = INVALID_CHAIR;
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		CMD_C_OperateCard OperateCard;
		OperateCard.cbOperateCode = cbOperateCode;
		CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
		SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
	}

	return 0;
}

//获取操作信息
BYTE CGameClientEngine::GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] )
{
	//初始化
	BYTE cbSelectCount = 0;

	if( wOperateCode == WIK_NULL ) return 0;

	//听牌
	if( wOperateCode & WIK_LISTEN )
	{
		//提取可丢弃牌
		BYTE cbCardData[MAX_COUNT],cbCardCount=0;
		BYTE cbCardIndex[MAX_INDEX];
		CopyMemory( cbCardIndex,m_cbCardIndex,sizeof(cbCardIndex) );
		WORD wMeChairId = GetMeChairID();
		CChiHuRight chr;
		for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		{
			if( cbCardIndex[i] == 0 ) continue;

			cbCardIndex[i]--;
			for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
			{
				BYTE cbCurrentCard = m_GameLogic.SwitchToCardData(j);
				if( WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard( cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],
					cbCurrentCard,chr) )
				{
					cbCardData[cbCardCount++] = m_GameLogic.SwitchToCardData(i);
					break;
				}
			}
			cbCardIndex[i]++;
		}
		for( BYTE i = 0; i < cbCardCount; i++ )
		{
			SelectInfo[cbSelectCount].cbActionCard = cbCardData[i];
			SelectInfo[cbSelectCount].wActionMask = WIK_LISTEN;
			SelectInfo[cbSelectCount].cbCardCount = 1;
			SelectInfo[cbSelectCount++].cbCardData[0] = cbCardData[i];
		}
	}
	//吃牌
	else if( wOperateCode&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
	{
		//效验
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_LEFT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_LEFT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard+1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+2;
		}
		if( m_cbActionMask & WIK_CENTER )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_CENTER;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+1;
		}
		if( m_cbActionMask & WIK_RIGHT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_RIGHT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-2;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard-1;
		}
	}
	//碰牌
	else if( wOperateCode & WIK_PENG )
	{
		//效验
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
		SelectInfo[cbSelectCount].wActionMask = WIK_PENG;
		SelectInfo[cbSelectCount].cbCardCount = 2;
		SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
		SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard;
	}
	//杠牌
	else if( wOperateCode & WIK_GANG )
	{
		//如果是自己杠牌
		if( m_wCurrentUser == GetMeChairID() )
		{
			//寻找是否有多个杠牌
			WORD wMeChairId = GetMeChairID();
			tagGangCardResult gcr;
			ZeroMemory( &gcr,sizeof(gcr) );
			m_GameLogic.AnalyseGangCard( m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],gcr );
			ASSERT( gcr.cbCardCount > 0 );
			for( BYTE i = 0; i < gcr.cbCardCount; i++ )
			{
				SelectInfo[cbSelectCount].cbActionCard = gcr.cbCardData[i];
				SelectInfo[cbSelectCount].wActionMask = WIK_GANG;				
				if( m_cbCardIndex[m_GameLogic.SwitchToCardIndex(gcr.cbCardData[i])] == 1 )
				{
					SelectInfo[cbSelectCount].cbCardCount = 1;
					SelectInfo[cbSelectCount].cbCardData[0] = gcr.cbCardData[i];
				}
				else
				{
					SelectInfo[cbSelectCount].cbCardCount = m_GameLogic.GetWeaveCard(WIK_GANG,gcr.cbCardData[i],
						SelectInfo[cbSelectCount].cbCardData);
				}
				cbSelectCount++;
			}
		}
		else
		{
			ASSERT( m_cbActionCard != 0 );
			if( m_cbActionCard == 0 ) return 0;
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].wActionMask = WIK_GANG;
			SelectInfo[cbSelectCount].cbCardCount = 3;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
			SelectInfo[cbSelectCount].cbCardData[1] = m_cbActionCard;
			SelectInfo[cbSelectCount++].cbCardData[2] = m_cbActionCard;
		}
	}

	return cbSelectCount;
}


//开始玩家动作
bool CGameClientEngine::BeginUserAction()
{
	if( m_UserActionArray.GetCount() == 0 )
		return false;

	//枚举动作
	switch( m_UserActionArray[0]->enActionKind )
	{
	case AK_StartCard:				//发牌
		return BeginActionStartCard();
	case AK_OutCard:				//出牌
		return BeginActionOutCard();
	case AK_SendCard:				//抓牌
		return BeginActionSendCard();
	case AK_ReplaceCard:			//补牌
		return BeginActionReplaceCard();
	case AK_ActionNotify:			//提示操作
		return BeginActionActionNotify();
	case AK_ActionResult:			//操作结果
		return BeginActionActionResult();
	case AK_ListenCard:				//听牌操作
		return BeginActionListenCard();
	case AK_Sice:					//掷色子
		return BeginActionSice();	
	case AK_GameEnd:				//结束游戏
		return BeginActionGameEnd();
	default:
		ASSERT(FALSE);
		return false;
	}

	return false;
}

//停止玩家动作
bool CGameClientEngine::StopUserAction()
{
	//停止色子动画
	m_GameClientView.m_DrawSiceWnd.StopSicing();

	if( m_GameClientView.IsMovingCard() )
		m_GameClientView.StopMoveCard();

	while( BeginUserAction() )
	{
		//判断类型
		ASSERT(m_UserActionArray.GetCount() > 0 );
		if( m_UserActionArray.GetCount() > 0 )
		{
			enmActionKind ActionKind = m_UserActionArray[0]->enActionKind;
			if( ActionKind == AK_ReplaceCard )
				OnTimerReplaceCard();
			else if( m_GameClientView.IsMovingCard() )
				m_GameClientView.StopMoveCard();
		}
	}

	return true;
}

//移除当前动作
bool CGameClientEngine::RemoveCurrentAction()
{
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//删除当前动作
	tagUserAction *pUserAction = m_UserActionArray[0];
	m_UserActionArray.RemoveAt(0);
	m_UserActionStorage.Add(pUserAction);
	pUserAction->ResetAction();

	return true;
}

//激活动作
tagUserAction *CGameClientEngine::ActiveUserAction(enmActionKind enActionKind)
{
	//变量定义
	tagUserAction *pUserAction = NULL;

	//寻找库存
	for( INT_PTR i = 0; i < m_UserActionStorage.GetCount(); i++ )
	{
		if( m_UserActionStorage[i]->enActionKind == enActionKind )
		{
			pUserAction = m_UserActionStorage[i];
			m_UserActionStorage.RemoveAt(i);
			return pUserAction;
		}
	}

	//新创建
	switch( enActionKind )
	{
	case AK_StartCard:
		{
			pUserAction = new tagActionStartCard;
			return pUserAction;
		}
	case AK_OutCard:
		{
			pUserAction = new tagActionOutCard;
			return pUserAction;
		}
	case AK_SendCard:
		{
			pUserAction = new tagActionSendCard;
			return pUserAction;
		}
	case AK_ReplaceCard:
		{
			pUserAction = new tagActionReplaceCard;
			return pUserAction;
		}
	case AK_ActionNotify:
		{
			pUserAction = new tagActionActionNotify;
			return pUserAction;
		}
	case AK_ActionResult:
		{
			pUserAction = new tagActionActionResult;
			return pUserAction;
		}
	case AK_ListenCard:
		{
			pUserAction = new tagActionListenCard;
			return pUserAction;
		}
	case AK_GameEnd:
		{
			pUserAction = new tagActionGameEnd;
			return pUserAction;
		}
	case AK_Sice:
		{
			pUserAction = new tagActionSice;
			return pUserAction;
		}
	default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	return pUserAction;
}

//发牌动作
bool CGameClientEngine::BeginActionStartCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)m_UserActionArray[0];

	pStartCardItem->bHandleStatus = TRUE;

	//起始,结束点
	CPoint ptFrom,ptTo;

	//发牌位置
	WORD wHeapId = pStartCardItem->wHeapId;
	WORD wHeapViewId = SwitchViewChairID(wHeapId);
	//堆牌位置
	WORD wHeapCardIndex = pStartCardItem->wHeapCardIndex;
	if( pStartCardItem->cbCardCount > wHeapCardIndex+1 )
	{
		ASSERT( pStartCardItem->cbCardCount == 4 );
		//删除前面的堆牌
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,wHeapCardIndex+1 );

		wHeapId = (wHeapId+1)%GAME_PLAYER;
		wHeapViewId = SwitchViewChairID(wHeapId);
		wHeapCardIndex = HEAP_FULL_COUNT-(4-(wHeapCardIndex+1))-1;

		//获取起始点
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(WORD(wHeapCardIndex-1));

		//删除堆牌
		m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( HEAP_FULL_COUNT-1,
			pStartCardItem->cbCardCount-pStartCardItem->wHeapCardIndex-1 );
	}
	else
	{
		WORD wHeapPos = wHeapCardIndex;
		if( pStartCardItem->cbCardCount == 4 )
			wHeapPos -= 3;
		else
		{
			ASSERT( pStartCardItem->cbCardCount == 1 );
			if( wHeapPos%2 != 0 ) wHeapPos--;
			else wHeapPos++;
		}
		//获取起始点
		ptFrom = m_GameClientView.m_HeapCard[wHeapViewId].GetDispatchCardPos(wHeapPos);

		//删除堆牌
		if( pStartCardItem->cbCardCount == 4 )
			m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex,4 );
		else
		{
			ASSERT( pStartCardItem->cbCardCount == 1 );
			if( wHeapCardIndex%2 != 0 )
				m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex-1,pStartCardItem->cbCardCount );
			else m_GameClientView.m_HeapCard[wHeapViewId].SetCardEmpty( wHeapCardIndex+1,pStartCardItem->cbCardCount );
		}
	}

	//获取结束点
	WORD wViewChairId = SwitchViewChairID(pStartCardItem->wChairId);
	if( wViewChairId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		ptTo = m_GameClientView.m_UserCard[wUserIndex].GetDispatchCardPos();
	}

	//播放发牌声音
	m_DirectSound[1].Play();

	//构造动画
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = pStartCardItem->cbCardCount;
	mci.cbCardData = 0;
	mci.wViewChairId = wViewChairId;
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//出牌动作
bool CGameClientEngine::BeginActionOutCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionOutCard *pOutCardItem = (tagActionOutCard *)m_UserActionArray[0];

	pOutCardItem->bHandleStatus = TRUE;

	//定义变量
	WORD wMeChairId = GetMeChairID();
	WORD wOutCardUser = pOutCardItem->wOutCardUser;
	WORD wViewOutCardUser = SwitchViewChairID(wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->cbOutCardData;

	//环境设置
	KillGameClock(IDI_OPERATE_CARD); 

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//计算起始,结束点
	CPoint ptFrom,ptTo;
	if( wOutCardUser == wMeChairId )
		ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wViewId = wViewOutCardUser>=3?2:wViewOutCardUser;
		ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
	}
	ptTo = m_GameClientView.m_DiscardCard[wViewOutCardUser].GetDispatchCardPos();

	//播放声音
	PlayCardSound(wOutCardUser,cbOutCardData);

	//设置界面
	m_GameClientView.SetCurrentUser( INVALID_CHAIR );
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(wViewOutCardUser,cbOutCardData);
	m_GameClientView.SetStatusFlag(false,false);

	//设置扑克
	if (wViewOutCardUser==MYSELF_VIEW_ID)
	{
		//删除扑克
		if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
		{
			ASSERT( FALSE );
			return false;
		}

		SetHandCardControl( m_cbCardIndex,0x00 );
	}
	else
	{
		WORD wUserIndex=(wViewOutCardUser>2)?2:wViewOutCardUser;
		m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
	}

	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = 1;
	mci.cbCardData = cbOutCardData;
	mci.wViewChairId = wViewOutCardUser;
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//发牌动作
bool CGameClientEngine::BeginActionSendCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionSendCard *pSendCardItem = (tagActionSendCard *)m_UserActionArray[0];

	pSendCardItem->bHandleStatus = TRUE;

	WORD wSendUserViewId = SwitchViewChairID(pSendCardItem->wSendCardUser);

	//计算起始,结束点
	CPoint ptFrom,ptTo;
	if( !pSendCardItem->bTail )
	{
		ASSERT( m_wHeapHead != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
		//删除牌头
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) )
		{
			m_wHeapHead = (m_wHeapHead+1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].GetDispatchCardPos(false);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapHead)].DeductionHeapCard(false) );
		}
	}
	else
	{
		ASSERT( m_wHeapTail != INVALID_CHAIR );
		ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
		//删除牌尾
		if( !m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) )
		{
			m_wHeapTail = (m_wHeapTail+GAME_PLAYER-1)%GAME_PLAYER;
			ptFrom = m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].GetDispatchCardPos(true);
			VERIFY( m_GameClientView.m_HeapCard[SwitchViewChairID(m_wHeapTail)].DeductionHeapCard(true) );
		}
	}
	if( wSendUserViewId == MYSELF_VIEW_ID )
		ptTo = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
	else
	{
		WORD wViewId = wSendUserViewId>=3?2:wSendUserViewId;
		ptTo = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
	}

	//播放发牌声音
	m_DirectSound[1].Play();

	//构造动画
	tagMoveCardItem mci;
	mci.ptFrom = ptFrom;
	mci.ptTo = ptTo;
	mci.cbCardCount = 1;
	mci.cbCardData = 0;
	mci.wViewChairId = SwitchViewChairID(pSendCardItem->wSendCardUser);
	m_GameClientView.OnMoveCardItem( mci );

	return true;
}

//补花动作
bool CGameClientEngine::BeginActionReplaceCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionReplaceCard *pReplaceCardItem = (tagActionReplaceCard *)m_UserActionArray[0];

	pReplaceCardItem->bHandleStatus = TRUE;

	//设置变量
	WORD wMeChairId = GetMeChairID();
	m_wReplaceUser = pReplaceCardItem->wReplaceUser;
	BYTE cbReplaceCard = pReplaceCardItem->cbReplaceCard;
	WORD wReplaceUserViewId = SwitchViewChairID(m_wReplaceUser);

	//补牌操作
	if ((IsLookonMode()==false)&&(m_wReplaceUser==wMeChairId))
	{
		//设置时间
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	if( IsLookonMode() || m_wReplaceUser != wMeChairId )
	{
		//播放声音
		PlayActionSound(m_wReplaceUser,WIK_REPLACE);

		//计算起始,结束点
		CPoint ptFrom,ptTo;
		if( m_wReplaceUser == wMeChairId )
			ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
		else
		{
			WORD wViewId = wReplaceUserViewId>=3?2:wReplaceUserViewId;
			ptFrom = m_GameClientView.m_UserCard[wViewId].GetDispatchCardPos();
		}
		ptTo = m_GameClientView.m_DiscardCard[wReplaceUserViewId].GetDispatchCardPos();

		//设置动作
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction( wReplaceUserViewId,WIK_REPLACE );

		//设置扑克
		if (wReplaceUserViewId==MYSELF_VIEW_ID)
		{
			//删除扑克
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
			{
				ASSERT( FALSE );
				return false;
			}

			//设置扑克
			SetHandCardControl(m_cbCardIndex,0x00);
		}
		else
		{
			//设置扑克
			WORD wUserIndex=(wReplaceUserViewId>2)?2:wReplaceUserViewId;
			if( m_GameClientView.m_UserCard[wUserIndex].IsHaveCurrentCard() )
				m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
			else
			{
				WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
				m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount-1,false);
			}
		}

		tagMoveCardItem mci;
		mci.ptFrom = ptFrom;
		mci.ptTo = ptTo;
		mci.cbCardCount = 1;
		mci.cbCardData = cbReplaceCard;
		mci.wViewChairId = wReplaceUserViewId;
		m_GameClientView.OnMoveCardItem( mci );
	}

	return true;
}

//提示操作
bool CGameClientEngine::BeginActionActionNotify()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionActionNotify *pOperateNotify = (tagActionActionNotify *)m_UserActionArray[0];

	pOperateNotify->bHandleStatus = TRUE;

	//用户界面
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		//获取变量
		WORD wMeChairID=GetMeChairID();
		m_cbActionMask = pOperateNotify->cbActionMask;
		m_cbActionCard = pOperateNotify->cbActionCard;

		//如果手上牌属于操作模式,则取消
		if( m_GameClientView.m_HandCardControl.GetSelectMode() )
			m_GameClientView.m_HandCardControl.SetSelectMode(false);

		//设置界面
		ActiveGameFrame();
		m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);

		//设置时间
		m_GameClientView.SetCurrentUser(INVALID_CHAIR);
		SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return OnActionActionNotify();
}

//操作结果
bool CGameClientEngine::BeginActionActionResult()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionActionResult *pOperateResult = (tagActionActionResult *)m_UserActionArray[0];

	pOperateResult->bHandleStatus = TRUE;

	//删除定时器
	KillGameClock(IDI_OPERATE_CARD);

	//变量定义
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard[3];
	CopyMemory( cbOperateCard,pOperateResult->cbOperateCard,sizeof(cbOperateCard) );
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);
	WORD wProviderViewID = SwitchViewChairID(pOperateResult->wProvideUser);

	//设置变量
	m_wCurrentUser=pOperateResult->wOperateUser;

	//设置组合
	if ((pOperateResult->cbOperateCode&WIK_GANG)!=0)
	{
		//设置变量
		m_wCurrentUser=INVALID_CHAIR;

		//组合扑克
		BYTE cbWeaveIndex=0xFF;
		for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
		{
			BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
			BYTE cbCenterCard=m_WeaveItemArray[wOperateUser][i].cbCenterCard;
			if ((cbCenterCard==cbOperateCard[0])&&(cbWeaveKind==WIK_PENG))
			{
				cbWeaveIndex=i;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[3] = cbCenterCard;
				break;
			}
		}

		//组合扑克
		if (cbWeaveIndex==0xFF)
		{
			//暗杠判断
			cbPublicCard=(pOperateResult->wProvideUser==wOperateUser)?FALSE:TRUE;

			//设置扑克
			cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=cbPublicCard;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
			m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
			for( BYTE i = 0; i < 4; i++ ) m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[i] = cbOperateCard[0];

			m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);
		}

		//组合界面
		BYTE cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=4;
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,
			cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicCard==TRUE)?true:false);

		//扑克设置
		if (GetMeChairID()==wOperateUser)
		{
			m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbOperateCard[0])]=0;
		}

		//设置扑克
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,false);
		}
	}
	else if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		//设置组合
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard[0];
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
		CopyMemory( m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,cbOperateCard,sizeof(cbOperateCard) );

		//组合界面
		BYTE cbWeaveCard[3],cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=3;
		CopyMemory( cbWeaveCard,cbOperateCard,sizeof(cbWeaveCard) );
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetDirectionCardPos((BYTE)wProviderViewID);

		//删除扑克
		if (GetMeChairID()==wOperateUser)
		{
			VERIFY( m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1) );
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1) )
			{
				ASSERT( FALSE );
				return false;
			}
		}

		//设置扑克
		if (GetMeChairID()==wOperateUser)
		{
			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOperateViewID>=3)?2:wOperateViewID;
			BYTE cbCardCount=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(cbCardCount-1,true);
		}
	}

	//设置界面
	//如果提供玩家不是自己,则去除提供玩家的已出牌
	if( pOperateResult->wProvideUser != wOperateUser )
	{
		if( m_GameClientView.m_DiscardCard[wProviderViewID].GetCardCount() > 0 )
			m_GameClientView.m_DiscardCard[wProviderViewID].RemoveCard(1);
	}
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
	//如果手上牌属于操作模式,则取消
	if( m_GameClientView.m_HandCardControl.GetSelectMode() )
		m_GameClientView.m_HandCardControl.SetSelectMode(false);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//环境设置
	PlayActionSound(wOperateUser,pOperateResult->cbOperateCode);

	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		//听牌判断
		if( !IsLookonMode() && m_cbListenStatus == 0 && m_wCurrentUser==GetMeChairID() )
		{
			WORD wMeChairId = GetMeChairID();

			m_cbActionMask = m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId]);

			if( m_cbActionMask != WIK_NULL )
			{
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
			}
		}

		//计算时间
		WORD wTimeCount=TIME_OPERATE_CARD;

		//设置时间
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}

	return OnActionActionResult();
}

//听牌操作
bool CGameClientEngine::BeginActionListenCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionListenCard *pListen = (tagActionListenCard *)m_UserActionArray[0];

	pListen->bHandleStatus = TRUE;

	WORD wViewId = SwitchViewChairID( pListen->wChairId );

	//设置听牌状态
	m_GameClientView.SetUserListenStatus( wViewId,true );

	//播放声音
	PlayActionSound( pListen->wChairId,WIK_LISTEN );

	m_GameClientView.SetOutCardInfo( INVALID_CHAIR,0 );
	m_GameClientView.SetUserAction( wViewId,WIK_LISTEN );

	//设置状态
	m_GameClientView.SetStatusFlag( (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),m_wCurrentUser!=GetMeChairID() );

	return OnActionListenCard();
}

//掷色子
bool CGameClientEngine::BeginActionSice()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionSice *pActionSice = (tagActionSice *)m_UserActionArray[0];

	pActionSice->bHandleStatus = TRUE;

	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//摇色子动画
	m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(pActionSice->lSiceCount>>16),(WORD)pActionSice->lSiceCount);
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);

	return true;
}

//游戏结束
bool CGameClientEngine::BeginActionGameEnd()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;
	if( m_UserActionArray[0]->bHandleStatus ) return true;

	//变量定义
	tagActionGameEnd *pGameEnd = (tagActionGameEnd *)m_UserActionArray[0];

	pGameEnd->bHandleStatus = TRUE;

	//设置状态
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//删除定时器
	KillGameClock(IDI_OPERATE_CARD);

	//设置控件
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HandCardControl.SetPositively(false);
	m_GameClientView.SetCurrentUser(INVALID_CHAIR);

	//设置扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		for( BYTE j = 0; j < CountArray(m_GameClientView.m_WeaveCard[i]); j++ )
			m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
	}

	//变量定义
	tagScoreInfo ScoreInfo;
	tagWeaveInfo WeaveInfo;
	ZeroMemory(&ScoreInfo,sizeof(ScoreInfo));
	ZeroMemory(&WeaveInfo,sizeof(WeaveInfo));

	//成绩变量
	ScoreInfo.wProvideUser=pGameEnd->wProvideUser;
	ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;

	//设置积分
	CString strTemp ,strEnd = TEXT("本局结束,成绩统计:\r\n");
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		strTemp.Format(TEXT("%s: %+I64d\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}
	//消息积分
	m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);

	CChiHuRight chr;
	chr.SetRightData( pGameEnd->dwChiHuRight,CountArray(pGameEnd->dwChiHuRight) );
	if( pGameEnd->wProvideUser != INVALID_CHAIR )
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//胡牌类型
			ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];

			//设置成绩
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));

			//胡牌扑克
			if ((ScoreInfo.cbCardCount==0)&&(pGameEnd->dwChiHuKind[i]!=WIK_NULL))
			{
				ScoreInfo.wChiHuUser = i;
				//组合扑克
				WeaveInfo.cbWeaveCount=m_cbWeaveCount[i];
				for (BYTE j=0;j<WeaveInfo.cbWeaveCount;j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					WeaveInfo.cbPublicWeave[j]=m_WeaveItemArray[i][j].cbPublicCard;
					WeaveInfo.cbCardCount[j]=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					CopyMemory( WeaveInfo.cbCardData[j],m_WeaveItemArray[i][j].cbCardData,sizeof(m_WeaveItemArray[i][j].cbCardData) );
				}

				//设置扑克
				ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

				//提取胡牌
				for (BYTE j=0;j<ScoreInfo.cbCardCount;j++)
				{
					if (ScoreInfo.cbCardData[j]==pGameEnd->cbProvideCard)
					{
						MoveMemory(&ScoreInfo.cbCardData[j],&ScoreInfo.cbCardData[j+1],(ScoreInfo.cbCardCount-j-1)*sizeof(BYTE));
						ScoreInfo.cbCardData[ScoreInfo.cbCardCount-1]=pGameEnd->cbProvideCard;
						break;
					}
				}
			}
		}

		ScoreInfo.cbFanCount = pGameEnd->cbFanCount;
		ScoreInfo.cbHuaCardCount = pGameEnd->cbHuaCardCount;
		//成绩界面
		m_GameClientView.m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
	}
	else m_GameClientView.SetHuangZhuang(true);

	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		if (pGameEnd->dwChiHuKind[i]!=WIK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//设置扑克
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0);

	//播放声音
	if( !chr.IsEmpty() )
	{
		for( WORD i = 0; i < GAME_PLAYER; i++ )
		{
			if( pGameEnd->dwChiHuKind[i] != WIK_NULL )
			{
				PlayActionSound( i,WIK_CHI_HU );
				break;
			}
		}
	}
	else 
	{
		PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_END"));
	}

	//设置界面
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
		SetGameClock(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}

	//取消托管
	if(m_bStustee)
		OnStusteeControl(0,0);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return OnActionGameEnd();
}

//发牌完成
bool CGameClientEngine::OnActionStartCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//变量定义
	tagActionStartCard *pStartCardItem = (tagActionStartCard *)m_UserActionArray[0];

	//设置变量
	WORD wMeChairId = GetMeChairID();
	WORD wChairId = pStartCardItem->wChairId;
	WORD wViewChairId = SwitchViewChairID(wChairId);

	//添加扑克
	if( wViewChairId == MYSELF_VIEW_ID )
		m_GameClientView.m_HandCardControl.AddCardData( pStartCardItem->cbCardData,pStartCardItem->cbCardCount );
	else
	{
		WORD wUserIndex = wViewChairId>2?2:wViewChairId;
		if( pStartCardItem->bLastItem )
			m_GameClientView.m_UserCard[wUserIndex].AddCardData(true);
		else
		{
			for( BYTE i = 0; i < pStartCardItem->cbCardCount; i++ )
				m_GameClientView.m_UserCard[wUserIndex].AddCardData(false);
		}
	}

	//播放声音
	m_DirectSound[1].Play();

	//判断是否最后一张牌
	if( pStartCardItem->bLastItem )
	{
		//激活框架
		if (!IsLookonMode())
			ActiveGameFrame();

		//堆立扑克
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetHeapCardInfo(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1]);
		}

		//手上扑克
		SetHandCardControl(m_cbCardIndex,0x00);

		m_GameClientView.m_HandCardControl.SetPositively(!IsLookonMode());

		//出牌提示,动作处理
		if (!IsLookonMode()&&(m_wCurrentUser==wMeChairId))
		{
			m_GameClientView.SetStatusFlag(true,false);

			m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);

			if( m_cbActionMask != WIK_NULL )
				m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
		else if( m_wCurrentUser != wMeChairId && m_wCurrentUser != INVALID_CHAIR )
			m_GameClientView.SetStatusFlag(false,true);

		//设置时间
		if (m_wCurrentUser!=INVALID_CHAIR)
		{
			m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
			SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
	}

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//出牌完成
bool CGameClientEngine::OnActionOutCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//变量定义
	tagActionOutCard *pOutCardItem = (tagActionOutCard *)m_UserActionArray[0];

	WORD wOutUserViewId = SwitchViewChairID(pOutCardItem->wOutCardUser);
	BYTE cbOutCardData = pOutCardItem->cbOutCardData;

	//添加到已丢弃牌
	m_GameClientView.m_DiscardCard[wOutUserViewId].AddCardItem( cbOutCardData );

	//设置出牌指示
	m_GameClientView.SetDiscUser( wOutUserViewId );

	//播放声音
	m_DirectSound[0].Play();

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//抓牌完成
bool CGameClientEngine::OnActionSendCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//变量定义
	tagActionSendCard *pSendCardItem = (tagActionSendCard *)m_UserActionArray[0];

	//设置变量
	WORD wMeChairID = GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pSendCardItem->wSendCardUser);
	WORD wChairId = pSendCardItem->wSendCardUser;
	BYTE cbSendCardData = pSendCardItem->cbCardData;
	m_wCurrentUser = pSendCardItem->wCurrentUser;

	//取牌界面
	if (wViewChairID!=MYSELF_VIEW_ID)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		BYTE cbCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount()+m_cbWeaveCount[wChairId]*3;
		m_GameClientView.m_UserCard[wUserIndex].AddCardData(cbCardCount==MAX_COUNT-1?true:false);
	}
	else
	{
		//插入扑克
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbSendCardData)]++;

		//设置扑克
		SetHandCardControl(m_cbCardIndex,cbSendCardData);
	}

	//当前用户
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//激活框架
		ActiveGameFrame();

		m_cbActionMask = pSendCardItem->cbActionMask;
		if( m_cbListenStatus == 0 )
		{
			m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID]);
		}
		m_cbActionCard = pSendCardItem->cbCardData;

		//动作处理
		if (m_cbActionMask!=WIK_NULL)
		{
			//设置界面
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
	}

	//出牌提示
	m_GameClientView.SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//计算时间
	WORD wTimeCount=TIME_OPERATE_CARD;

	//设置时间
	if( m_wCurrentUser != INVALID_CHAIR )
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,wTimeCount);
	}
	else m_GameClientView.SetCurrentUser( INVALID_CHAIR );

	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//补花完成
bool CGameClientEngine::OnActionReplaceCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//变量定义
	tagActionReplaceCard *pReplaceCardItem = (tagActionReplaceCard *)m_UserActionArray[0];

	WORD wReplaceUserViewId = SwitchViewChairID(pReplaceCardItem->wReplaceUser);
	BYTE cbReplaceCardData = pReplaceCardItem->cbReplaceCard;

	//添加到已丢弃牌
	m_GameClientView.m_DiscardCard[wReplaceUserViewId].AddCardItem( cbReplaceCardData );

	//设置出牌指示
	m_GameClientView.SetDiscUser( wReplaceUserViewId );

	//播放声音
	m_DirectSound[0].Play();

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//设置变量
	m_wReplaceUser = INVALID_CHAIR;

	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//提示完成
bool CGameClientEngine::OnActionActionNotify()
{
	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//操作完成
bool CGameClientEngine::OnActionActionResult()
{
	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//听牌操作
bool CGameClientEngine::OnActionListenCard()
{
	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//掷色子完成
bool CGameClientEngine::OnActionSice()
{
	//隐藏窗口
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//游戏结束
bool CGameClientEngine::OnActionGameEnd()
{
	//删除动作
	RemoveCurrentAction();

	//下个动作
	BeginUserAction();

	return true;
}

//操作完成
LRESULT CGameClientEngine::OnUserActionFinish(WPARAM wParam, LPARAM lParam)
{
	//校验
	ASSERT( m_UserActionArray.GetCount() > 0 );		
	if( m_UserActionArray.GetCount() == 0 ) return 0;

	//枚举动作
	switch( m_UserActionArray[0]->enActionKind )
	{
	case AK_StartCard:				//发牌完成
		OnActionStartCard();
		break;
	case AK_OutCard:				//出牌完成
		OnActionOutCard();
		break;
	case AK_SendCard:				//抓牌完成
		OnActionSendCard();
		break;
	case AK_ReplaceCard:			//补牌完成
		OnActionReplaceCard();
		break;
	case AK_Sice:					//掷色子完成
		OnActionSice();
		break;
	default:
		ASSERT(FALSE);
		return 0;
	}

	return 0;
}

//补花事件
bool CGameClientEngine::OnTimerReplaceCard()
{
	//效验
	ASSERT( m_UserActionArray.GetCount() > 0 );
	if( m_UserActionArray.GetCount() == 0 ) return false;

	//变量定义
	tagActionReplaceCard *pReplaceCardItem = (tagActionReplaceCard *)m_UserActionArray[0];

	//删除时间
	KillTimer(IDI_REPLACE_CARD);

	ASSERT( m_wReplaceUser == GetMeChairID() );
	if( m_wReplaceUser != GetMeChairID() ) return false;

	//设置界面
	m_GameClientView.SetUserAction( INVALID_CHAIR,0 );
	//寻找扑克
	for (BYTE i=MAX_INDEX-MAX_HUA_CARD;i<MAX_INDEX;i++)
	{
		if (m_cbCardIndex[i]>0)
		{
			//变量定义
			BYTE cbReplaceCard=m_GameLogic.SwitchToCardData(i);
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
			{
				ASSERT( FALSE );
				return false;
			}

			//发送数据
			CMD_C_ReplaceCard ReplaceCard;
			ReplaceCard.cbCardData=cbReplaceCard;
			SendSocketData(SUB_C_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

			//设置扑克
			SetHandCardControl(m_cbCardIndex,0x00);

			//播放声音
			PlayActionSound(GetMeChairID(),WIK_REPLACE);

			//设置界面
			m_GameClientView.SetUserAction( MYSELF_VIEW_ID,WIK_REPLACE );

			pReplaceCardItem->cbReplaceCard = cbReplaceCard;

			//构造出牌动画
			tagMoveCardItem mci;
			mci.cbCardCount = 1;
			mci.cbCardData = cbReplaceCard;
			mci.wViewChairId = MYSELF_VIEW_ID;
			mci.ptFrom = m_GameClientView.m_HandCardControl.GetDispatchCardPos();
			mci.ptTo = m_GameClientView.m_DiscardCard[MYSELF_VIEW_ID].GetDispatchCardPos();
			m_GameClientView.OnMoveCardItem( mci );

			break;
		}
	}

	return true;
}


///////////
bool CGameClientEngine::OnEventGameClockKill(WORD t)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////
