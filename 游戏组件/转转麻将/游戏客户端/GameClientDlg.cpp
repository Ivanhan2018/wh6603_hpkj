#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"

//////////////////////////////////////////////////////////////////////////

//游戏定时器
//#define IDI_START_GAME				200									//开始定时器
//#define IDI_OPERATE_CARD			201									//操作定时器
#define IDI_START_GAME				200									//开始定时器
#define IDI_OPERATE_CARD			202									//操作定时器
#define IDI_OPERATE_HAI_DI			203									//海底定时器
#define IDI_USER_TING_CARD			204									//听牌定时器
#define IDI_OTHER_USEROUTCARD		205									//其实出牌定时器
#define IDI_GAME_END                206                                 //结束定时器
#define IDI_SHOW_SZ                 207                                 //骰子定时器
#define IDI_HUANGZHUANG             208                                 //荒庄定时器
#define IDI_SOUND                   209                                 //声音定时器
#define IDI_SZSTART                 210                                 //启动筛子定时器
#define IDI_SHOW_DIRECT             211                                 //方为动画
#define	IDI_SELECT_OUT_CARD		    212								    //选择出牌
#define IDI_REPLACE_CARD			301									//补牌定时器

//游戏定时器
//#define TIME_START_GAME				30									//开始定时器
//#define TIME_OPERATE_CARD			15									//操作定时器	
#define TIME_START_GAME				15									//开始定时器
#define TIME_HEAR_STATUS			15									//出牌定时器
#define TIME_OPERATE_CARD			10									//操作定时器
#define TIME_OPERATE_HAI_DI			10									//操作定时器
#define TIME_USERTINGCARD			5									//操作定时器
#define TIME_GAME_END				3500								//结束框显示时间
#define TIME_SHOW_SZ				100								    //骰子时间
#define TIME_SHOW_DIRECT            100                                 //方为动画时间 
#define TIME_SELECT_OUT_CARD	  5										//选牌时间
#define TIME_REPLACE_CARD			1000								//补牌定时器

#define MAX_TIME_OUT				3									//最大超时次数

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameEngine)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
    ///ON_MESSAGE(IDM_TING_CARD,OnTingCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
    //ON_MESSAGE(IDM_OPERATE_HAI_DI,OnOperateHaiDi)
	ON_MESSAGE(IDM_TRUSTEE_CONTROL,OnStusteeControl)
	ON_MESSAGE(IDM_SICE_TWO,OnSiceTwo)
	ON_MESSAGE(IDM_SICE_FINISH,OnSiceFinish)
	ON_MESSAGE(IDM_OUT_CARD_FINISH,OnOutCardFinish)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
	ON_MESSAGE(IDM_USER_ACTION,OnUserAction)
	ON_MESSAGE(IDM_USER_MOVE,OnUserMove)
	ON_MESSAGE(IDM_SOUND,OnSound)
	ON_MESSAGE(IDM_SHOW_SZ,OnShowSZ)
	ON_MESSAGE(IDM_MOPAI,OnMoPai)
	ON_MESSAGE(IDM_SZSTART,OnSZStart)
	ON_MESSAGE(IDM_SZ_BUTTON, OnSZFlashBT)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameClientDlg::CGameClientDlg()
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
	m_bMustWanTong=true;


	//禁牌信息
	m_cbEnjoinCardCount = 0;
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));

	//托管变量
	m_bStustee=false;
	m_wTimeOutCount =0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));


	return;
}

//析构函数
CGameClientDlg::~CGameClientDlg()
{
}

//初始函数
bool CGameClientDlg::OnInitGameEngine()
{
	//设置图标
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
    m_pIClientKernel->SetGameAttribute(KIND_ID,GAME_PLAYER,VERSION_CLIENT,hIcon,GAME_NAME);//by hxh 20170308
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//加载资源
	g_CardResource.LoadResource();

	//加载声音
	m_DirectSound[0].Create( TEXT("OUT_CARD") );
	m_DirectSound[1].Create( TEXT("SEND_CARD") );
	m_DirectSound[2].Create( TEXT("SELECT") );


	/*WORD wWinner=0;
	m_wBankerUser=1;
	LONGLONG lScore[GAME_PLAYER];
	ZeroMemory(lScore,sizeof(lScore));
	LONGLONG lAllScore[]={0,10,20,10};

	for (WORD i=0;i<4;i++)
	{
		if(i==wWinner) continue;
		for (WORD j=0;j<4;j++)
		{
			if(j==i || j==wWinner)  continue;

			if(i==m_wBankerUser||j==m_wBankerUser)
			{
				lScore[i] +=(lAllScore[i]-lAllScore[j]);
			}
			else
			{
				LONGLONG  lTempScore=(lAllScore[i]-lAllScore[j])/2;
				if(lTempScore%10)
				{
					if(lTempScore>0)
					{
						lTempScore -= lTempScore%10;
						lTempScore += 10;
					}
					else
					{
						lTempScore -= lTempScore%10;
						lTempScore += -10;

					}
				}
				lScore[i] +=lTempScore;
			}

		}
	}*/
	



	//test
	/*BYTE cbCardData[] = {
		0x11,0x11,0x11,0x17,0x17,0x22,0x02,0x31,0x31,
		0x31,0x27,0x28,0x29
	};
	BYTE cbCardCount = 13;
	BYTE cbCurrentCard = 0x17;
	
	m_GameLogic.SetMagicIndex( m_GameLogic.SwitchToCardIndex(0x02) );

	BYTE cbCardIndex[MAX_INDEX];
	m_GameLogic.SwitchToCardIndex( cbCardData,cbCardCount,cbCardIndex );

	CChiHuRight chr;
	DWORD dw1 = GetTickCount();
	BYTE cbHu=0;
	
	BYTE byResult = m_GameLogic.AnalyseChiHuCard( cbCardIndex,NULL,0,cbCurrentCard,chr,cbHu);
	*/
	//DWORD dw2 = GetTickCount();
	//DWORD dw = dw2-dw1;
	//TCHAR szBuffer[32] = TEXT("");
	//_sntprintf( szBuffer,CountArray(szBuffer),TEXT("%d"),dw );
	//MessageBox( szBuffer );
	//end test

	/*BYTE byCard[] = {
		0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,					
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
			0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09
	};
	BYTE cbHuaCard[]={0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f};
	BYTE cbGangCard[]={0x31,0x31,0x31,0x31};
	BYTE byCardCount = 14;

		m_GameClientView.m_HandCardControl.SetDisplayItem(true);
		m_GameClientView.m_HandCardControl.SetPositively(true);
		m_GameClientView.m_HandCardControl.SetCardData(byCard,MAX_COUNT-1-3-3-3-3,byCard[MAX_COUNT-1]);
		for( BYTE i = 0; i < GAME_PLAYER; i++ )
		{
			m_GameClientView.m_HeapCard[i].SetHeapCardInfo(5,5);
			//m_TableCard[i].SetCardData(byCard,byCardCount-3-3-3-3);
			m_GameClientView.m_DiscardCard[i].SetCardData(byCard,15);

			m_GameClientView.m_WeaveCard[i][0].SetUserAction(WIK_GANG);
			m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(false);
			m_GameClientView.m_WeaveCard[i][0].SetCardData(cbGangCard,4);

			m_GameClientView.m_WeaveCard[i][1].SetUserAction(WIK_GANG);
			m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(false);
			m_GameClientView.m_WeaveCard[i][1].SetCardData(cbGangCard,4 );

			m_GameClientView.m_WeaveCard[i][2].SetUserAction( WIK_GANG );
			m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
			m_GameClientView.m_WeaveCard[i][2].SetCardData( cbGangCard,4 );

			m_GameClientView.m_WeaveCard[i][3].SetUserAction(WIK_GANG );
			m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
			m_GameClientView.m_WeaveCard[i][3].SetCardData( cbGangCard,4 );
			
			



			m_GameClientView.m_WindCard[i].SetCardData(cbHuaCard,8);
			if( i < 3 )
				m_GameClientView.m_TableCard[i].SetCardData(cbHuaCard,8);
		}*/
		


	return true;
}

//重置框架
bool CGameClientDlg::OnResetGameEngine()
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
	m_bMustWanTong=true;

	//禁牌信息
	m_cbEnjoinCardCount = 0;
	ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));

	
	//托管变量
	m_bStustee=false;
	m_wTimeOutCount =0;

	//组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//扑克变量
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	return true;
}

//游戏设置
void CGameClientDlg::OnGameOptionSet()
{
	////构造数据
	//CGameOption GameOption;
	//GameOption.m_bEnableSound=IsEnableSound();
	//GameOption.m_bAllowLookon = IsAllowUserLookon();
	//GameOption.m_bEanbleAnimate = m_GameClientView.IsEnableAnimate();
	//
	////配置数据
	//if (GameOption.DoModal()==IDOK)
	//{
	//	EnableSound(GameOption.m_bEnableSound);
	//	AllowUserLookon(0,GameOption.m_bAllowLookon);
	//	m_GameClientView.EnableAnimate(GameOption.m_bEanbleAnimate);
	//	m_DirectSound[0].EnableSound(GameOption.m_bEnableSound);
	//	m_DirectSound[1].EnableSound(GameOption.m_bEnableSound);
	//	m_DirectSound[2].EnableSound(GameOption.m_bEnableSound);

	//}

	return;
}

//时间消息
bool CGameClientDlg::OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD nTimerID)
{
    //zz
	if (IsLookonMode())
	{
		return true;
	}

	switch (nTimerID)
	{
	case IDI_SZSTART:     ///启动筛子定时器
		{
			if (nElapse==0)
			{
				WORD wMeChairID=GetMeChairID();
				if (wChairID == m_wBankerUser && wMeChairID==m_wBankerUser)
				{
					AfxGetMainWnd()->SendMessage(IDM_SZ_BUTTON, 0, 0);
				}
				return false;
			}

			return true;
		}
	case IDI_START_GAME:		//开始游戏
		{
			if( m_bStustee && nElapse < TIME_START_GAME )
			{
				OnStart(0,0);
				return true;
			}
			if (nElapse==0)
			{
				PostMessage(WM_CLOSE);
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
				/*	if( m_cbListenStatus == 0 && m_bStustee==false && ++m_wTimeOutCount>=MAX_TIME_OUT )
					{
						m_wTimeOutCount = 0;
						OnStusteeControl(TRUE,0);
						m_pIStringMessage->InsertSystemString(TEXT("由于您多次超时，切换为“系统托管”模式."));
					}
					*/

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
                            BYTE i=0;
							for (i=0;i<MAX_INDEX;i++)
							{
								//出牌效验
								if (m_cbCardIndex[i]==0) continue;
								if (VerdictOutCard(m_GameLogic.SwitchToCardData(i))==false) 
									continue;

								//设置变量
								cbCardData=m_GameLogic.SwitchToCardData(i);
							}
							if(i==MAX_INDEX)
							{
								m_pIStringMessage->InsertSystemString(_T("此牌型非法！"));
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
	case IDI_OTHER_USEROUTCARD:		///其它出牌定时器
		{
			//超时判断
			if ((nElapse==0)||(m_bStustee==true))
			{
				//获取位置
				WORD wMeChairID=GetMeChairID();

				//动作处理
				if (wChairID==wMeChairID && wMeChairID==m_wCurrentUser)
				{
					if((m_bStustee==false)/*&&(m_bHearStatus==false)*/)
					{
						m_wTimeOutCount++;
						if(m_wTimeOutCount==3)
						{
							//InsertSystemString(TEXT("由于您多次超时，切换为“系统托管”模式."));
							OnStusteeControl(0,0);
						}
					}
					if (m_wCurrentUser==wMeChairID)
					{
						BYTE cbCardData=m_GameClientView.m_HandCardControl.GetCurrentCard();
						OnOutCard(cbCardData,cbCardData);
					}
					return true;
				}
			}

			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
			{
				PlaySound(_T("GAME_WARN"),false);
			}

			return true;
		}
	case IDI_USER_TING_CARD:		///听牌定时器
		{
			if (0==nElapse || m_bStustee==true)
			{
				if (GetMeChairID()==m_wCurrentUser)
				{
					//发送不听消息
					CMD_UserTingOperate TingOperate;
					TingOperate.bTingCard = false;
					SendSocketData(SUB_C_USERTINGCARD,&TingOperate,sizeof(TingOperate));
					//m_GameClientView.m_btTingCard.ShowWindow(SW_HIDE);
					//m_GameClientView.m_btBuTing.ShowWindow(SW_HIDE);
				}
			}
			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
			{
				PlaySound(_T("GAME_WARN"),false);
			}
			return true;
		}
	case IDI_SELECT_OUT_CARD:	/// 选牌定时器
		{
			if (0==nElapse || m_bStustee==true)
			{
				SendCardCommand(m_byCanSelectType, m_byCanSelectCard[0]);
			}

			//播放声音
			if ((nElapse<=3)&&(wChairID==GetMeChairID())) 
			{
				PlaySound(_T("GAME_WARN"),false);
			}

			return true;
		}
	}

	return true;
}

bool CGameClientDlg::OnEventGameClockKill(WORD t)
{
	return true;
}

//旁观状态
bool CGameClientDlg::OnEventLookonMode(void * pBuffer, WORD wDataSize)
{
	//扑克控制
	m_GameClientView.m_HandCardControl.SetDisplayItem(IsAllowLookon());
	m_GameClientView.InvalidGameView(0,0,0,0);

	return true;
}

//网络消息
bool CGameClientDlg::OnEventGameMessage(WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_C_SZSTART:			//启动色子动画
		{
			return OnSubStartSZFlash();
		}
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
		//case SUB_S_OPERATE_HAI_DI:	//海底操作
		//	{
		//		return OnSubOperateHaiDi(pBuffer,wDataSize);
		//	}
	case SUB_S_GAME_END:		//游戏结束
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	case SUB_S_TRUSTEE:			//用户托管
		{
			return OnSubTrustee(pBuffer,wDataSize);
		}
		//case SUB_S_TING_CARD:      //听牌标记
		//	{
		//		return OnSubTingCard(pBuffer,wDataSize);			
		//	}
		//case SUB_S_USER_TINGCARD_RESULT:	//听牌的结果
		//	{
		//		return OnSubTingCardResult(pBuffer, wDataSize);
		//	}
	}

	return true;

	//switch (wSubCmdID)
	//{
	//case SUB_S_GAME_START:		//游戏开始
	//	{
	//		m_GameClientView.StopMoveCard();
	//		return OnSubGameStart(pBuffer,wDataSize);
	//	}
	//case SUB_S_OUT_CARD:		//用户出牌
	//	{
	//		return OnSubOutCard(pBuffer,wDataSize);
	//	}
	//case SUB_S_SEND_CARD:		//发牌消息
	//	{
	//		return OnSubSendCard(pBuffer,wDataSize);
	//	}
	//case SUB_S_OPERATE_NOTIFY:	//操作提示
	//	{
	//		return OnSubOperateNotify(pBuffer,wDataSize);
	//	}
	//case SUB_S_OPERATE_RESULT:	//操作结果
	//	{
	//		return OnSubOperateResult(pBuffer,wDataSize);
	//	}
	//case SUB_S_GAME_END:		//游戏结束
	//	{
	//		m_GameClientView.StopMoveCard();
	//		return OnSubGameEnd(pBuffer,wDataSize);
	//	}
	//case SUB_S_TRUSTEE:			//用户托管
	//	{
	//		return OnSubTrustee(pBuffer,wDataSize);
	//	}
	//case SUB_S_LISTEN:			//玩家听牌
	//	{
	//		return OnSubListen( pBuffer,wDataSize );
	//	}
	//case SUB_S_REPLACE_CARD:	//补花
	//	{
	//		m_GameClientView.StopMoveCard();
	//		return OnSubReplaceCard( pBuffer,wDataSize );
	//	}
	//case SUB_S_WANTONG:		//是否万同	
	//	{
 //           return OnSubWanTong(pBuffer,wDataSize);
	//	}
	//}

	//return true;
}

//游戏场景
bool CGameClientDlg::OnEventSceneMessage(BYTE cbGameStation, bool bLookonOther, void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//空闲状态
		{
			//效验数据
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			////是否启用声音
			//if( !IsEnableSound() )
			//{
			//	m_DirectSound[0].EnableSound(FALSE);
			//	m_DirectSound[1].EnableSound(FALSE);
			//	m_DirectSound[2].EnableSound(FALSE);
			//}

			//设置数据
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);
			m_GameClientView.m_HandCardControl.SetDisplayItem(true);
			//托管设置
			for (int i=0;i<GAME_PLAYER;i++)
			{
				m_GameClientView.SetTrustee(SwitchViewChairID(i),pStatusFree->bTrustee[i]);
			}

			//设置控件
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				//m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
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

			////是否启用声音
			//if( !IsEnableSound() )
			//{
			//	m_DirectSound[0].EnableSound(FALSE);
			//	m_DirectSound[1].EnableSound(FALSE);
			//	m_DirectSound[2].EnableSound(FALSE);
			//}

			//辅助变量
			WORD wMeChairID=GetMeChairID();
			WORD wViewChairID[GAME_PLAYER]={0,0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);

			//设置变量
			m_wBankerUser=pStatusPlay->wBankerUser;
			m_wCurrentUser=pStatusPlay->wCurrentUser;
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
			m_cbListenStatus = pStatusPlay->cbHearStatus/*[wMeChairID]*/;
			//m_wReplaceUser = pStatusPlay->wReplaceUser;
			//m_bMustWanTong=pStatusPlay->bMustWanTong;

			//用户名称
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				//const tagUserData * pUserData=GetUserData(i);
                IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
				ASSERT( pUserData != NULL );
				if( pUserData == NULL ) 
				{
					m_szAccounts[i][0] = 0;
					continue;
				}
				lstrcpy( m_szAccounts[i],pUserData->GetNickName());
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
			/*if(pStatusPlay->bTrustee[wMeChairID])
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
			else
				m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
				*/

			//丢弃效果
			if(pStatusPlay->wOutCardUser != INVALID_CHAIR)
				m_GameClientView.SetDiscUser(SwitchViewChairID(pStatusPlay->wOutCardUser));

			//扑克变量
			//m_cbEnjoinCardCount = pStatusPlay->cbEnjoinCardCount;
			//CopyMemory(m_cbEnjoinCardData,pStatusPlay->cbEnjoinCardData,sizeof(m_cbEnjoinCardData));
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);

			//界面设置
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);

			//设置牌距
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<4;j++)
				{
					if(m_GameClientView.m_WeaveCard[i][j].GetCardCount()==4)
					{
						for (BYTE t=j+1;t<4;t++)
						{
							m_GameClientView.m_WeaveCard[i][t].OffPoint(i==2?true:false);
						}
					}

				}
			}
			

			//组合扑克
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wOperateViewID = SwitchViewChairID(i);
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;
					BYTE cbWeaveCardCount=(cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT|WIK_PENG))?3:4;
					
					BYTE cbWeaveCard[4];
					//CopyMemory(cbWeaveCard,m_WeaveItemArray[i][j].cbCardData,cbWeaveCardCount);

					//m_GameLogic.SortCardList(cbWeaveCard,cbWeaveCardCount);

					m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					if ((cbWeaveKind&WIK_GANG)&&(m_WeaveItemArray[i][j].wProvideUser==i)) 
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetDisplayItem(false);
					WORD wProviderViewID = SwitchViewChairID(m_WeaveItemArray[i][j].wProvideUser);
					m_GameClientView.m_WeaveCard[wOperateViewID][j].SetUserAction(cbWeaveKind);

				}
			}

			//用户扑克
			if (m_wCurrentUser==wMeChairID)
			{
				//SetHandCardControl(m_cbCardIndex,pStatusPlay->cbSendCardData);
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
				//m_GameClientView.m_WindCard[wViewChairID[i]].SetCardData(pStatusPlay->cbHuaCardData[i],pStatusPlay->cbHuaCardCount[i]);

			}

			//控制设置
			if (IsLookonMode()==false) 
			{
				m_GameClientView.m_HandCardControl.SetPositively(true);
				m_GameClientView.m_HandCardControl.SetDisplayItem(true);
				//m_GameClientView.m_btStusteeControl.EnableWindow(TRUE);
			}

			//堆立信息
			//m_GameClientView.SetHeapCardInfo( SwitchViewChairID(pStatusPlay->wHeapHand),SwitchViewChairID(pStatusPlay->wHeapTail) );

			//堆立界面
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				//m_GameClientView.m_HeapCard[wViewChairID[i]].SetHeapCardInfo(pStatusPlay->cbHeapCardInfo[i][0],pStatusPlay->cbHeapCardInfo[i][1]);
			}

			//听牌状态
			for( WORD i = 0; i < GAME_PLAYER; i++ )
			{
				//if( pStatusPlay->cbHearStatus[i] > 0 )
				//	m_GameClientView.SetUserListenStatus( wViewChairID[i],true );
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
				SetGameClock(m_wCurrentUser,TIME_OPERATE_CARD,wTimeCount);
			}
			else if( m_wReplaceUser == wMeChairID && !IsLookonMode() )
				SetTimer( IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL );

			//设置圈风
			//m_GameClientView.SetFengQuan( pStatusPlay->cbQuanFeng );

			//更新界面
			m_GameClientView.InvalidGameView(0,0,0,0);

			return true;
		}
	}

	return false;
}

//是否开始骰子动画
bool CGameClientDlg::OnSubStartSZFlash()
{
	KillGameClock(IDI_SZSTART);
	KillTimer(IDI_SHOW_SZ);
	SetTimer(IDI_SHOW_SZ,TIME_SHOW_SZ,NULL);
	m_GameClientView.InvalidGameView(0,0,0,0);
	return true;
}

//游戏开始
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//PlayWaitFlash(false);

	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) 
		return false;

	//变量定义
	CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
	if (NULL == pGameStart)
	{
		return false;
	}

	//用户名称
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		//const tagUserData * pUserData=GetUserData(i);
        IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(i);
		ASSERT( pUserData != NULL );
		if( pUserData == NULL ) 
		{
			m_szAccounts[i][0] = 0;
			continue;
		}
		lstrcpy( m_szAccounts[i],pUserData->GetNickName());
	}

	//zz清空所有定时器
	ClearAllTimer();
	m_bIsMeCanOutCard = false;
	m_bStustee = false;

	//设置状态
	SetGameStatus(GS_MJ_PLAY);

	//zz方位动画
	KillTimer(IDI_SHOW_DIRECT);
	SetTimer(IDI_SHOW_DIRECT,TIME_SHOW_DIRECT,NULL);

	//zz设置变量
	//m_GameClientView.PlaySZFlashBT(false);
	::memset(&m_GameStart, 0, sizeof(CMD_S_GameStart));
	::memcpy(&m_GameStart, pGameStart, sizeof(CMD_S_GameStart));
	//CopyMemory( &m_GameStart,pGameStart,sizeof(m_GameStart) );
	//m_wBankerUser = m_GameStart.wBankerUser;
	//m_bMustWanTong=m_GameStart.bMustWanTong;
	//m_bHearStatus=false;
	m_wBankerUser=pGameStart->wBankerUser;
	m_wCurrentUser=pGameStart->wCurrentUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;

	m_GameClientView.m_nDiceCount1 =HIBYTE( pGameStart->wSiceCount)-1;
	m_GameClientView.m_nDiceCount2 =LOBYTE( pGameStart->wSiceCount)-1;

	//zz出牌信息
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//zz组合扑克
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//zz设置扑克
	BYTE byMeChairID = GetMeChairID();
	BYTE cbCardCount=(byMeChairID==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//zz堆立扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
		m_GameClientView.m_HeapCard[SwitchViewChairID(i)].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);
	}

	//旁观界面
	if (IsLookonMode()==true)
	{
		//设置变量
		m_cbListenStatus = 0;
		m_bWillHearStatus = false;

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
			m_GameClientView.m_WindCard[i].SetCardData(NULL,0);

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
	//m_GameClientView.SetFengQuan( pGameStart->cbQuanFeng );
	//if( WORD(pGameStart->lSiceCount>>16) == 0 )
	//	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	//zz设置界面
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));
	//m_ScoreView.ShowWindow(SW_HIDE);
	//SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
	m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);

	//m_GameClientView.m_nKingCardIndex=pGameStart->wMPIndex;

	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//摇色子动画
	//m_GameClientView.m_DrawSiceWnd.SetSiceInfo(m_GameClientView.GetDC(),200,(WORD)(pGameStart->lSiceCount>>16),(WORD)pGameStart->lSiceCount);
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_SHOW);

	return true;
}

//用户出牌
bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//消息处理
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//变量定义
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;

	m_GameClientView.SetCurrentUser( INVALID_CHAIR );
	if( IsLookonMode() || pOutCard->wOutCardUser != wMeChairID )
	{
		//环境设置
		KillGameClock(IDI_OPERATE_CARD); 

		//播放声音
		PlayCardSound(pOutCard->wOutCardUser,pOutCard->cbOutCardData);

		//出牌界面
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);
		
		//设置扑克
		if (wOutViewChairID==MYSELF_VIEW_ID)
		{
			//删除扑克
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData) )
			{
				ASSERT( FALSE );
				return false;
			}

			SetHandCardControl( m_cbCardIndex,0x00 );
		}
		else
		{
			WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
		}

		//构造出牌动画
		tagMoveCardItem mci;
		mci.bReplace=false;
		mci.bOutCard = true;
		mci.cbCardData = pOutCard->cbOutCardData;
		mci.wChairId = pOutCard->wOutCardUser;
		mci.wViewChairId = SwitchViewChairID( pOutCard->wOutCardUser );
		m_GameClientView.OnUserMoveCard( mci );
		//开始发牌动画
		m_GameClientView.BeginMoveCard();
	}

	//扑克设置
	if (GetMeChairID()==pOutCard->wOutCardUser)
	{
		m_cbEnjoinCardCount=0;
		ZeroMemory(m_cbEnjoinCardData,sizeof(m_cbEnjoinCardData));

	}
	return true;
}

//发牌消息
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//复制数据
	CopyMemory( &m_SendCard,pBuffer,sizeof(m_SendCard) );

	//效验发牌数据
	ASSERT( m_SendCard.cbCardData != 0 );
	if( m_SendCard.cbCardData == 0 ) return false;

	//构造发牌动画
	tagMoveCardItem mci;
	mci.bReplace=false;
	mci.bOutCard = false;
	//mci.bSendTail = m_SendCard.bTail;
	mci.cbCardData = m_SendCard.cbCardData;
	//mci.wChairId = m_SendCard.wSendCardUser;
	//mci.wViewChairId = SwitchViewChairID(m_SendCard.wSendCardUser);
	m_GameClientView.OnUserMoveCard( mci );
	//开始发牌动画
	m_GameClientView.BeginMoveCard();

	//播放发牌声音
	m_DirectSound[1].Play();

	return true;
}

//操作提示
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) 
		return false;

	//变量定义
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

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

	return true;
}

//操作结果
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//效验消息
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//消息处理
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//变量定义
	BYTE cbPublicCard=TRUE;
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard[3];
	//CopyMemory( cbOperateCard,pOperateResult->cbOperateCard,sizeof(cbOperateCard) );
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
				//m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[3] = cbCenterCard;
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
			//for( BYTE i = 0; i < 4; i++ ) m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData[i] = cbOperateCard[0];

			m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetUserAction(WIK_GANG);
		}

		//组合界面
		BYTE cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=4;
		//m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,
		//	cbWeaveCardCount);
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


		for (BYTE j=0;j<4;j++)
		{
			if(m_GameClientView.m_WeaveCard[wOperateViewID][j].GetCardCount()==4 && m_GameClientView.m_WeaveCard[wOperateViewID][j].GetCardData(3)==cbOperateCard[0])
			{
				for (BYTE t=j+1;t<4;t++)
				{
					m_GameClientView.m_WeaveCard[wOperateViewID][t].OffPoint(wOperateViewID==2?true:false);
				}
			}

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
		//CopyMemory( m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCardData,cbOperateCard,sizeof(cbOperateCard) );

		//组合界面
		BYTE cbWeaveCard[3],cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=3;
		CopyMemory( cbWeaveCard,cbOperateCard,sizeof(cbWeaveCard) );
		//m_GameLogic.SortCardList(cbWeaveCard,sizeof(cbWeaveCard));
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetUserAction(cbWeaveKind);

		//删除扑克
		if (GetMeChairID()==wOperateUser)
		{
			VERIFY( m_GameLogic.RemoveCard(cbWeaveCard,cbWeaveCardCount,&cbOperateCard[0],1) );
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1) )
			{
				ASSERT( FALSE );
				return false;
			}
			//禁止脱牌
			if(cbWeaveKind == WIK_LEFT)  //左吃
			{
				//禁止吃牌数据
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0];
				m_cbEnjoinCardCount++;

				if((cbOperateCard[0]&MASK_VALUE)<7)
				{
					m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0]+3;

					m_cbEnjoinCardCount++;
				}

				/*CString str;
				for (BYTE tt=0;tt<m_cbEnjoinCardCount;tt++)
				{
					str.Format("禁止吃牌：%d\n",m_cbEnjoinCardData[tt]);
					m_pIStringMessage->InsertSystemString(LPCSTR(str));

				}*/
			}
			else if (cbWeaveKind == WIK_CENTER) //中吃
			{
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0];

				m_cbEnjoinCardCount++;

				/*CString str;
				for (BYTE tt=0;tt<m_cbEnjoinCardCount;tt++)
				{
					str.Format("禁止吃牌：%d\n",m_cbEnjoinCardData[tt]);
					m_pIStringMessage->InsertSystemString(LPCSTR(str));

				}*/
			}
			else  if(cbWeaveKind == WIK_RIGHT)//右吃
			{
				m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0];
				m_cbEnjoinCardCount++;
				if((cbOperateCard[0]&MASK_VALUE)>3)
				{
					m_cbEnjoinCardData[m_cbEnjoinCardCount] =cbOperateCard[0]-3;
					m_cbEnjoinCardCount++;
				}

				/*CString str;
				for (BYTE tt=0;tt<m_cbEnjoinCardCount;tt++)
				{
					str.Format("禁止吃牌：%d\n",m_cbEnjoinCardData[tt]);
					m_pIStringMessage->InsertSystemString(LPCSTR(str));

				}*/
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

			//m_cbActionMask = m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId]);

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

	return true;
}

//游戏结束
bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	//消息处理
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;

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
	//ScoreInfo.cbProvideCard=pGameEnd->cbProvideCard;

	//设置积分
	CString strTemp ,strEnd = TEXT("本局结束,成绩统计:\r\n");
	for( WORD i = 0; i < GAME_PLAYER; i++ )
	{
		strTemp.Format(_T("%s: %I64d\n"),m_szAccounts[i],pGameEnd->lGameScore[i]);
		strEnd += strTemp;
	}

	//for( WORD i = 0; i < GAME_PLAYER; i++ )
	//{
	//	strTemp.Format(_T("%s: %I64d胡\n"),m_szAccounts[i],pGameEnd->lHu[i]);
	//	strEnd += strTemp;
	//}
	//消息积分
	m_pIStringMessage->InsertSystemString((LPCTSTR)strEnd);

	CChiHuRight chr;
	//chr.SetRightData( pGameEnd->dwChiHuRight,CountArray(pGameEnd->dwChiHuRight) );
	if( pGameEnd->wProvideUser != INVALID_CHAIR )
	{
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			//胡牌类型
			//ScoreInfo.dwChiHuKind[i]=pGameEnd->dwChiHuKind[i];

			//设置成绩
			ScoreInfo.lGameScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(ScoreInfo.szUserName[i],m_szAccounts[i],CountArray(ScoreInfo.szUserName[i]));

			//胡牌扑克
			if ((ScoreInfo.cbCardCount==0)/*&&(pGameEnd->dwChiHuKind[i]!=WIK_NULL)*/)
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
					//CopyMemory( WeaveInfo.cbCardData[j],m_WeaveItemArray[i][j].cbCardData,sizeof(m_WeaveItemArray[i][j].cbCardData) );
				}

				//设置扑克
				ScoreInfo.cbCardCount=pGameEnd->cbCardCount[i];
				CopyMemory(ScoreInfo.cbCardData,&pGameEnd->cbCardData[i],ScoreInfo.cbCardCount*sizeof(BYTE));

				//提取胡牌
				for (BYTE j=0;j<ScoreInfo.cbCardCount;j++)
				{
					//if (ScoreInfo.cbCardData[j]==pGameEnd->cbProvideCard)
					{
						MoveMemory(&ScoreInfo.cbCardData[j],&ScoreInfo.cbCardData[j+1],(ScoreInfo.cbCardCount-j-1)*sizeof(BYTE));
						//ScoreInfo.cbCardData[ScoreInfo.cbCardCount-1]=pGameEnd->cbProvideCard;
						break;
					}
				}
			}
		}

		//ScoreInfo.cbFanCount = pGameEnd->cbFanCount;
		//ScoreInfo.cbHuaCardCount = pGameEnd->cbHuaCardCount;
		//成绩界面
		m_GameClientView.m_GameScore.SetScoreInfo(ScoreInfo,WeaveInfo,chr);
	}
	else m_GameClientView.SetHuangZhuang(true);

	//用户扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		//if (pGameEnd->dwChiHuKind[i]!=WIK_NULL) m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
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
			//if( pGameEnd->dwChiHuKind[i] != WIK_NULL )
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
	m_GameClientView.UpdateWindow();

	return true;
}

//用户托管
bool CGameClientDlg::OnSubTrustee(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Trustee));
	if (wDataSize!=sizeof(CMD_S_Trustee)) return false;

	//消息处理
	CMD_S_Trustee * pTrustee=(CMD_S_Trustee *)pBuffer;
	m_GameClientView.SetTrustee(SwitchViewChairID(pTrustee->wChairID),pTrustee->bTrustee);
	if ((IsLookonMode()==true)||(pTrustee->wChairID!=GetMeChairID()))
	{
		//const tagUserData * pUserData=GetUserData(pTrustee->wChairID);
        IClientUserItem * pUserData = GetTableUserItem(pTrustee->wChairID);
		TCHAR szBuffer[256];
		if(pTrustee->bTrustee==true)
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]选择了托管功能."),pUserData->GetNickName());
		else
			_sntprintf(szBuffer,sizeof(szBuffer),TEXT("玩家[%s]取消了托管功能."),pUserData->GetNickName());
		//InsertSystemString(szBuffer);
        m_pIStringMessage->InsertSystemString(szBuffer);
	}

	return true;
}

//用户听牌
bool CGameClientDlg::OnSubListen(const void * pBuffer,WORD wDataSize)
{
	//效验数据
	ASSERT(wDataSize==sizeof(CMD_S_Listen));
	if (wDataSize!=sizeof(CMD_S_Listen)) return false;

	CMD_S_Listen *pListen = (CMD_S_Listen *)pBuffer;
	WORD wViewId = SwitchViewChairID( pListen->wChairId );

	//设置听牌状态
	if( IsLookonMode() || pListen->wChairId != GetMeChairID() )
	{
		m_GameClientView.SetUserListenStatus( wViewId,true );
	}

	//播放声音
	PlayActionSound( pListen->wChairId,WIK_LISTEN );

	m_GameClientView.SetOutCardInfo( INVALID_CHAIR,0 );
	m_GameClientView.SetUserAction( wViewId,WIK_LISTEN );

	//设置状态
	m_GameClientView.SetStatusFlag( (IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),m_wCurrentUser!=GetMeChairID() );

	return true;
}
//是否万同
bool CGameClientDlg::OnSubWanTong(const void * pBuffer, WORD wDataSize)
{
	ASSERT(wDataSize==0);
	m_bMustWanTong=false;

	return true;

}

void CGameClientDlg::PlaySound(CString szSoundRes, bool bLoop)
{
	if (!szSoundRes.IsEmpty())
	{
		TCHAR szResourcePath[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH,szResourcePath);
		CString strPath(szResourcePath);
		strPath.Append(_T("\\SparrowZZ\\Sound\\"));
		strPath.Append(szSoundRes);
		strPath.Append(_T(".mp3"));

		//PlayDirSound(strPath, true, bLoop); //播放游戏音效
	}
}

//播放出牌声音
void CGameClientDlg::PlayCardSound(WORD wChairID, BYTE cbCardData)
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
	//const tagUserData* pUserData = GetUserData(wChairID);
    IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(wChairID);
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
void CGameClientDlg::PlayActionSound(WORD wChairID,BYTE cbAction)
{
	//判断性别
	//const tagUserData* pUserData = GetUserData(wChairID);
	IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(wChairID);
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
	//case WIK_LISTEN:	//听牌
	//	{
	//		if(bBoy)
	//			PlayGameSound(AfxGetInstanceHandle(),TEXT("BOY_TING"));
	//		else
	//			PlayGameSound(AfxGetInstanceHandle(),TEXT("GIRL_TING"));		
	//		break;
	//	}
	}

	return;
}

//出牌判断
bool CGameClientDlg::VerdictOutCard(BYTE cbCardData)
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
		for (i=0;i<MAX_INDEX;i++)
		{
			//胡牌分析
			BYTE cbCurrentCard=m_GameLogic.SwitchToCardData(i);
			BYTE cbHu=0;
			//BYTE cbHuCardKind=m_GameLogic.AnalyseChiHuCard(cbCardIndexTemp,m_WeaveItemArray[wMeChairID],cbWeaveCount,cbCurrentCard,chr,cbHu);

			////结果判断
			//if (cbHuCardKind!=WIK_NULL)
			//{
			//	break;
			//}
		}

		//听牌判断
		return (i!=MAX_INDEX);
	}
	if(cbCardData>=0x38 && cbCardData<=0x3f)
		return true;



	if(m_bMustWanTong==false)
	{
		if(m_cbEnjoinCardCount>0)
		{
			int nTemp=0;
			for(nTemp=0;nTemp<m_cbEnjoinCardCount;nTemp++)
			{
				if(cbCardData == m_cbEnjoinCardData[nTemp])
				{
					break;
				}
			}
			if(nTemp == m_cbEnjoinCardCount)
				return true;
			else
			{
				return false;
			}
		}
		else
			return true;
	}
	else
	{
		//如果无万筒，什么牌都可以出
		BYTE cbNo=0;
		for(BYTE i=0;i<9;i++)
		{
			cbNo +=m_cbCardIndex[i];
		}
		for(BYTE i=18;i<27;i++)
		{
			cbNo +=m_cbCardIndex[i];

		}
		if(cbNo==0) return true;

		if((cbCardData&MASK_COLOR)==0x00 ||(cbCardData&MASK_COLOR)==0x20)
			return true;
		else
			return false;
	}
	return false;
}

//开始按钮
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
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
		m_GameClientView.m_WindCard[i].SetCardData(NULL,0);

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

	//发送消息
	SendUserReady(NULL,0);

	return 0;
}

//出牌操作
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//出牌判断
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID()))
		return 0;

	//听牌判断
	if (((m_cbListenStatus>0)||(m_bWillHearStatus==true))||(VerdictOutCard((BYTE)wParam)==false))
	{
		m_pIStringMessage->InsertSystemString(TEXT("出此牌不符合游戏规则!"));
		return 0;
	}
	

	KillGameClock(IDI_OPERATE_CARD);

	//设置变量
	m_wCurrentUser=INVALID_CHAIR;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	BYTE cbOutCardData=(BYTE)wParam;
	if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbOutCardData) )
	{
		ASSERT( FALSE );
		return 0;
	}

	//设置扑克
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount=m_GameLogic.SwitchToCardData(m_cbCardIndex,cbCardData);
	ASSERT( (cbCardCount-1)%3==0 );
	m_GameClientView.m_HandCardControl.SetCardData(cbCardData,cbCardCount,0);

	//设置界面
	m_GameClientView.InvalidGameView(0,0,0,0);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(2,cbOutCardData);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	if( m_GameClientView.m_DlgListen.m_hWnd )
		m_GameClientView.m_DlgListen.EndDialog(IDCANCEL);

	//构造出牌动画
	tagMoveCardItem mci;
	mci.bOutCard = true;
	mci.bReplace=false;
	mci.cbCardData = cbOutCardData;
	mci.wChairId = GetMeChairID();
	mci.wViewChairId = MYSELF_VIEW_ID;
	m_GameClientView.OnUserMoveCard( mci );
	//开始发牌动画
	m_GameClientView.BeginMoveCard();

	//发送数据
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendSocketData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));

	//播放声音
	PlayCardSound(GetMeChairID(),cbOutCardData);

	return 0;
}

//扑克操作
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
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
	cbOperateCode = si.cbActionMask;
	cbOperateCard[0] = si.cbActionCard;
	CopyMemory( &cbOperateCard[1],si.cbCardData,sizeof(BYTE)*2 );

	m_cbActionMask = 0;
	m_cbActionCard = 0;

	//取消操作模式
	m_GameClientView.m_HandCardControl.SetSelectMode( false );

	//如果是听牌
	if( m_bWillHearStatus && (cbOperateCode&WIK_LISTEN) )
	{
		m_bWillHearStatus = false;
		return OnOutCard( cbOperateCard[0],0 );
	}

	//环境设置
	m_GameClientView.SetStatusFlag(false,true);
	
	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	//CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

//拖管控制
LRESULT CGameClientDlg::OnStusteeControl(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_wTimeOutCount=0;

	//设置状态
	if (m_bStustee==true)
	{
		m_bStustee=false;
		//m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_START_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam ) 
			m_pIStringMessage->InsertSystemString(_T("您取消了托管功能."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = false;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}
	else
	{
		m_bStustee=true;
		//m_GameClientView.m_btStusteeControl.SetButtonImage(IDB_BT_STOP_TRUSTEE,AfxGetInstanceHandle(),false,false);
		if( FALSE == (BOOL)wParam )
			m_pIStringMessage->InsertSystemString(_T("您选择了托管功能."));
		CMD_C_Trustee Trustee;
		Trustee.bTrustee = true;
		SendSocketData(SUB_C_TRUSTEE,&Trustee,sizeof(Trustee));

	}

	return 0;
}

//第二次摇色子消息
LRESULT CGameClientDlg::OnSiceTwo(WPARAM wParam, LPARAM lParam)
{
	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("DRAW_SICE"));

	//设置界面
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);
	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	return 0;
}

//摇色子结束消息
LRESULT CGameClientDlg::OnSiceFinish(WPARAM wParam, LPARAM lParam)
{
	//设置界面
	m_GameClientView.m_DrawSiceWnd.ShowWindow(SW_HIDE);

	bool bPlayerMode = (IsLookonMode()==false);

	//设置扑克
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
	m_GameLogic.SwitchToCardIndex(m_GameStart.cbCardData,cbCardCount,m_cbCardIndex);

	//设置变量
	m_wCurrentUser=m_GameStart.wCurrentUser;
	//m_wReplaceUser = m_GameStart.wReplaceUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
	m_cbActionMask = m_GameStart.cbUserAction;
	//m_lSiceCount = m_GameStart.lSiceCount;

	WORD wMeChairID=GetMeChairID();

	//设置堆牌头尾
	//m_GameClientView.SetHeapCardInfo( SwitchViewChairID(m_GameStart.wHeapHand),SwitchViewChairID(m_GameStart.wHeapTail) );
	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);

		//堆立扑克
		//m_GameClientView.m_HeapCard[wViewChairID].SetHeapCardInfo(m_GameStart.cbHeapCardInfo[i][0],m_GameStart.cbHeapCardInfo[i][1]);

		//用户扑克
		if (wViewChairID!=MYSELF_VIEW_ID)
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(MAX_COUNT-1,(i==m_wBankerUser));
		}
		else
		{
			SetHandCardControl(m_cbCardIndex,0x00);
		}
	}
	
	m_GameClientView.m_HandCardControl.SetPositively(bPlayerMode);

	//补牌操作
	if ((bPlayerMode==true)&&(m_wReplaceUser==wMeChairID))
	{
		//设置时间
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
	}

	//出牌提示,动作处理
	if ((bPlayerMode==true)&&(m_wCurrentUser==wMeChairID))
	{
		m_GameClientView.SetStatusFlag(true,false);
		
		//m_cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,NULL,0);

		if( m_cbActionMask != WIK_NULL )
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
	}
	else if( m_wCurrentUser != wMeChairID && m_wCurrentUser != INVALID_CHAIR )
		m_GameClientView.SetStatusFlag(false,true);

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	//激活框架
	if (bPlayerMode==true)
		ActiveGameFrame();

	//设置时间
	if (m_wCurrentUser!=INVALID_CHAIR)
	{
		m_GameClientView.SetCurrentUser(SwitchViewChairID(m_wCurrentUser));
		SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	//环境处理
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));

	//清0
	ZeroMemory( &m_GameStart,sizeof(m_GameStart) );

	return 0;
}

//补牌消息
bool CGameClientDlg::OnSubReplaceCard(const void * pBuffer, WORD wDataSize)
{
	//效验参数
	ASSERT(wDataSize==sizeof(CMD_S_ReplaceCard));
	if (wDataSize!=sizeof(CMD_S_ReplaceCard)) return false;

	//变量定义
	CMD_S_ReplaceCard * pReplaceCard=(CMD_S_ReplaceCard *)pBuffer;

	//丢弃扑克
	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(pReplaceCard->wReplaceUser);

	//其他用户
	if ((IsLookonMode()==true)||(pReplaceCard->wReplaceUser!=wMeChairID))
	{
		//播放声音
		PlayActionSound(pReplaceCard->wReplaceUser,WIK_REPLACE);

		//设置动作
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetUserAction( wViewChairID,WIK_REPLACE );

		//设置扑克
		if (wViewChairID==MYSELF_VIEW_ID)
		{
			//删除扑克
			if( !m_GameLogic.RemoveCard(m_cbCardIndex,pReplaceCard->cbReplaceCard) )
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
			WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
			if( m_GameClientView.m_UserCard[wUserIndex].IsHaveCurrentCard() )
				m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(false);
			else
			{
				WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
				m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount-1,false);
			}
		}

		//构造出牌动画
		tagMoveCardItem mci;
		mci.bOutCard = true;
		mci.bReplace=true;
		mci.cbCardData = pReplaceCard->cbReplaceCard;
		mci.wChairId = pReplaceCard->wReplaceUser;
		mci.wViewChairId = wViewChairID;
		m_GameClientView.OnUserMoveCard( mci );
		//开始出牌动画
		m_GameClientView.BeginMoveCard();
	}

	return true;
}

//设置扑克
void CGameClientDlg::SetHandCardControl(BYTE cbCardIndex[MAX_INDEX], BYTE cbAdvanceCard)
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
VOID CGameClientDlg::OnTimer(UINT nIDEvent)
{
	__super::OnTimer(nIDEvent);
	switch (nIDEvent)
	{
	////方位动画
	//case IDI_SHOW_DIRECT:
	//{
	//	//环境处理
	//	KillTimer(IDI_SHOW_DIRECT);
	//	m_GameClientView.PlayDirectFlash();
	//	PlaySound("EffectDirect", false);
	//	//PlaySound("GAME_START",false);
	//	return;
	//}

	////骰子定时器
	//case IDI_SHOW_SZ:
	//{
	//	//环境处理
	//	KillTimer(IDI_SHOW_SZ);
	//	m_GameClientView.SetDiceEffect(true);
	//	PlaySound("dice", false);
	//	return;
	//}

	////结束定时器
	//case IDI_SOUND:
	//{
	//	KillTimer(IDI_SOUND);
	//	PlayGameSoundLanguage(m_cbGender,m_InfoSound);
	//	return;
	//}

	////结束定时器
	//case IDI_HUANGZHUANG:
	//{
	//	KillTimer(IDI_HUANGZHUANG);
	//	m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	//	return;
	//}

	////结束定时器
	//case IDI_GAME_END:
	//{
	//	KillTimer(IDI_START_GAME);
	//	KillTimer(IDI_GAME_END);
	//	PlayWaitFlash(false);
	//	//m_ScoreView.ShowWindow(SW_SHOW);
	//	SafeShowOrHideWnd(&m_ScoreView, SW_SHOW);
	//	m_ScoreView.SetCountDown(true);
	//	return;
	//}
	//case IDI_REPLACE_CARD:	//用户补牌
	//	{
	//		//删除时间
	//		KillTimer(IDI_REPLACE_CARD);

	//		//设置界面
	//		m_GameClientView.SetUserAction( INVALID_CHAIR,0 );
	//		//寻找扑克
	//		for (BYTE i=MAX_INDEX-MAX_HUA_CARD;i<MAX_INDEX;i++)
	//		{
	//			if (m_cbCardIndex[i]>0)
	//			{
	//				//变量定义
	//				BYTE cbReplaceCard=m_GameLogic.SwitchToCardData(i);
	//				if( !m_GameLogic.RemoveCard(m_cbCardIndex,cbReplaceCard) )
	//				{
	//					ASSERT( FALSE );
	//					return;
	//				}

	//				//设置扑克
	//				SetHandCardControl(m_cbCardIndex,0x00);

	//				//播放声音
	//				PlayActionSound(GetMeChairID(),WIK_REPLACE);

	//				//设置界面
	//				m_GameClientView.SetUserAction( MYSELF_VIEW_ID,WIK_REPLACE );

	//				//构造出牌动画
	//				tagMoveCardItem mci;
	//				mci.bOutCard = true;
	//				mci.bReplace=true;
	//				mci.cbCardData = cbReplaceCard;
	//				mci.wChairId = GetMeChairID();
	//				mci.wViewChairId = MYSELF_VIEW_ID;
	//				m_GameClientView.OnUserMoveCard( mci );
	//				
	//				//开始出牌动画
	//				m_GameClientView.BeginMoveCard();

	//				//发送数据
	//				CMD_C_ReplaceCard ReplaceCard;
	//				ReplaceCard.cbCardData=cbReplaceCard;
	//				SendSocketData(SUB_C_REPLACE_CARD,&ReplaceCard,sizeof(ReplaceCard));

	//				break;
	//			}
	//		}
	//	
	//		return;
	//	}
	}

	//__super::OnTimer(nIDEvent);
}

//出牌动画完成
LRESULT CGameClientDlg::OnOutCardFinish( WPARAM wParam, LPARAM lParam )
{
	WORD wViewId = (WORD)wParam;
	if( IsLookonMode() || wViewId != MYSELF_VIEW_ID )
		m_GameClientView.SetDiscUser( wViewId );

	//播放声音
	m_DirectSound[0].Play();

	return 0;
}

//发牌动画完成
LRESULT CGameClientDlg::OnSendCardFinish( WPARAM wParam, LPARAM lParam )
{
	//发牌处理
	WORD wViewChairID=(WORD)wParam;
	WORD wChairId = (WORD)(wParam>>16);

	//取牌界面
	if (wViewChairID!=MYSELF_VIEW_ID)
	{
		WORD wUserIndex=(wViewChairID>2)?2:wViewChairID;
		WORD wCardCount = m_GameClientView.m_UserCard[wUserIndex].GetCardCount();
		wCardCount += m_cbWeaveCount[wChairId]*3;
		if( wCardCount == MAX_COUNT-1 )
			m_GameClientView.m_UserCard[wUserIndex].SetCurrentCard(true);
		else
			m_GameClientView.m_UserCard[wUserIndex].SetCardData(wCardCount+1,false);
	}
	else
	{
		//插入扑克
		BYTE cbCardData = (BYTE)lParam;
		m_cbCardIndex[m_GameLogic.SwitchToCardIndex(cbCardData)]++;

		//设置扑克
		SetHandCardControl(m_cbCardIndex,cbCardData);
	}

	//如果还在发牌,必须等待发牌结束
	if( m_GameClientView.IsMovingCard() ) 
	{
		m_GameClientView.InvalidGameView(0,0,0,0);
		return 0;
	}

	//设置变量
	WORD wMeChairID=GetMeChairID();
	m_wCurrentUser=m_SendCard.wCurrentUser;

	//当前用户
	if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
	{
		//激活框架
		ActiveGameFrame();

		if( m_cbListenStatus == 0 )
		{
			//m_SendCard.cbActionMask |= m_GameLogic.AnalyseTingCard(m_cbCardIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID]);
		}

		m_cbActionMask = m_SendCard.cbActionMask;
		m_cbActionCard = m_SendCard.cbCardData;

		//动作处理
		if (m_SendCard.cbActionMask!=WIK_NULL)
		{
			//设置界面
			m_GameClientView.m_ControlWnd.SetControlInfo(m_cbActionMask);
		}
	}

	//补牌操作
	if ((IsLookonMode()==false)/*&&(m_SendCard.wReplaceUser==wMeChairID)*/)
	{
		//设置时间
		SetTimer(IDI_REPLACE_CARD,TIME_REPLACE_CARD,NULL);
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

	return 0;
}

//玩家操作控件
LRESULT CGameClientDlg::OnUserAction( WPARAM wParam, LPARAM lParam )
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
		m_GameClientView.m_HandCardControl.SetSelectMode(false);
		if( m_wCurrentUser == INVALID_CHAIR )
		{
			//删除定时器
			KillGameClock( IDI_OPERATE_CARD );
			//发送消息
			CMD_C_OperateCard OperateCard;
			OperateCard.cbOperateCode = WIK_NULL;
			//ZeroMemory( OperateCard.cbOperateCard,sizeof(OperateCard.cbOperateCard) );
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
			cbOperateCode = si.cbActionMask;
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
		m_cbActionMask = WIK_NULL;
		m_cbActionCard = 0;

		CMD_C_OperateCard OperateCard;
		OperateCard.cbOperateCode = cbOperateCode;
		//CopyMemory( OperateCard.cbOperateCard,cbOperateCard,sizeof(cbOperateCard) );
		SendSocketData( SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard) );
	}

	return 0;
}

//获取操作信息
BYTE CGameClientDlg::GetSelectCardInfo( WORD wOperateCode, tagSelectCardInfo SelectInfo[MAX_WEAVE] )
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
				BYTE cbHu=0;
				//if( WIK_CHI_HU == m_GameLogic.AnalyseChiHuCard( cbCardIndex,m_WeaveItemArray[wMeChairId],m_cbWeaveCount[wMeChairId],
				//	cbCurrentCard,chr,cbHu) )
				//{
				//	cbCardData[cbCardCount++] = m_GameLogic.SwitchToCardData(i);
				//	break;
				//}
			}
			cbCardIndex[i]++;
		}
		for( BYTE i = 0; i < cbCardCount; i++ )
		{
			SelectInfo[cbSelectCount].cbActionCard = cbCardData[i];
			SelectInfo[cbSelectCount].cbActionMask = WIK_LISTEN;
			SelectInfo[cbSelectCount].cbCardCount = 1;
			SelectInfo[cbSelectCount++].cbCardData[0] = cbCardData[i];
		}
	}
	//吃牌
	else if( wOperateCode&(WIK_LEFT) )
	{
		//效验
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_LEFT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].cbActionMask = WIK_LEFT;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard+1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+2;
		}
	}
	else if( wOperateCode&(WIK_CENTER) )
	{
		//效验
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;
		if( m_cbActionMask & WIK_CENTER )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].cbActionMask = WIK_CENTER;
			SelectInfo[cbSelectCount].cbCardCount = 2;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard-1;
			SelectInfo[cbSelectCount++].cbCardData[1] = m_cbActionCard+1;
		}
	}
	else if( wOperateCode&(WIK_RIGHT) )
	{
		//效验
		ASSERT( m_cbActionCard != 0 );
		if( m_cbActionCard == 0 ) return 0;

		if( m_cbActionMask & WIK_RIGHT )
		{
			SelectInfo[cbSelectCount].cbActionCard = m_cbActionCard;
			SelectInfo[cbSelectCount].cbActionMask = WIK_RIGHT;
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
		SelectInfo[cbSelectCount].cbActionMask = WIK_PENG;
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
				SelectInfo[cbSelectCount].cbActionMask = WIK_GANG;				
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
			SelectInfo[cbSelectCount].cbActionMask = WIK_GANG;
			SelectInfo[cbSelectCount].cbCardCount = 3;
			SelectInfo[cbSelectCount].cbCardData[0] = m_cbActionCard;
			SelectInfo[cbSelectCount].cbCardData[1] = m_cbActionCard;
			SelectInfo[cbSelectCount++].cbCardData[2] = m_cbActionCard;
		}
	}

	return cbSelectCount;
}

void CGameClientDlg::ClearAllTimer()
{
	KillGameClock(IDI_START_GAME);
	KillGameClock(IDI_OPERATE_CARD);
	KillGameClock(IDI_OPERATE_HAI_DI);
	KillGameClock(IDI_USER_TING_CARD);
	KillGameClock(IDI_OTHER_USEROUTCARD);
	KillGameClock(IDI_SZSTART);
	KillGameClock(IDI_SELECT_OUT_CARD);
}

void CGameClientDlg::SendCardCommand(BYTE byOperatorCode, BYTE byOperatorCard)
{
	KillGameClock(IDI_SELECT_OUT_CARD);

	//发送命令
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=byOperatorCode;
	OperateCard.cbOperateCard=byOperatorCard;
	if (byOperatorCode==9 && 0!=byOperatorCard)
	{
		OperateCard.cbOperateCode=WIK_GANG;
		OperateCard.cbOperateCard=byOperatorCard;
	}
	//else if (byOperatorCode==10 && 0!=byOperatorCard)
	//{
	//	OperateCard.cbOperateCode=WIK_TING;
	//	OperateCard.cbOperateCard=byOperatorCard;
	//}
	SendSocketData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(CMD_C_OperateCard));

	memset(m_byUserAction, 0, sizeof(m_byUserAction));

	//m_GameClientView.SetStatusFlag(false,false);
	if (0==byOperatorCode)
	{
		if (m_bIsMeCanOutCard)
		{//自己还可以出牌.
			m_byUserAction[GetMeChairID()] = WIK_NULL;
			//出牌
			m_wCurrentUser = GetMeChairID();
			memset(m_byUserAction, 0, sizeof(m_byUserAction));
			SetGameClock(GetMeChairID(),IDI_OTHER_USEROUTCARD,TIME_HEAR_STATUS-10);
			//出牌提示
			//m_GameClientView.SetStatusFlag(true,false);
		}
	}

	m_byCanSelectType = WIK_NULL;
	m_byCanSelectCount = 0;
	ZeroMemory(m_byCanSelectCard,sizeof(m_byCanSelectCard));
	//m_GameClientView.m_HandCardControl.SetSelectStatus(false);
	m_GameClientView.InvalidGameView(0,0,0,0);
}


//玩家操作控件
LRESULT CGameClientDlg::OnUserMove( WPARAM wParam, LPARAM lParam )
{
	//变量定义
	BYTE cbOperateCode=(BYTE)(wParam);

	if(cbOperateCode==WIK_NULL)
	{
		m_GameClientView.m_HandCardControl.SetSelectMode( false );
		m_GameClientView.m_HandCardControl.SetAllCardGray( false );
		m_GameClientView.UpdateWindow();

		return 0;
	}

	//获取选择组合
	tagSelectCardInfo sci[MAX_WEAVE];
	BYTE cbInfoCount = GetSelectCardInfo( cbOperateCode,sci );

	//设置操作事件
	m_GameClientView.m_HandCardControl.OnEventUserAction( sci,cbInfoCount );
	
	return 0;
}
//点牌声音
LRESULT CGameClientDlg::OnSound(WPARAM wParam,LPARAM lParam)
{
	m_DirectSound[2].Play();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//点击启动筛子动画后
LRESULT CGameClientDlg::OnSZStart(WPARAM wParam, LPARAM lParam)
{
	//m_GameClientView.PlaySZFlashBT(false);
	KillGameClock(IDI_SZSTART);
	SetGameClock(m_wBankerUser,IDI_SZSTART,5);
	BYTE byMeChairID = GetMeChairID();
	if (byMeChairID == m_wBankerUser)
	{
		//m_GameClientView.PlaySZFlashBT(true);
	}

	if (m_wBankerUser == GetMeChairID())
	{
		//const tagUserData *pUserData = GetUserData(m_wBankerUser);//用户信息
        IClientUserItem * pUserData=m_GameClientView.GetClientUserItem(m_wBankerUser);
		//PlayGameSoundLanguage(pUserData->GetGender()/*cbGender*/,TEXT("Banker"));
	}

	//char cbData[128]={0};
	//sprintf(cbData,"CGameClientDlg::OnSZStart-->庄家:%d 本人:%d", m_wBankerUser, byMeChairID);
	//InsertSystemString(cbData);

	m_GameClientView.InvalidGameView(0,0,0,0);
	return 0;
}

//摸牌动画播完后
LRESULT CGameClientDlg::OnMoPai(WPARAM wParam, LPARAM lParam)
{
	bool bPlayerMode=IsLookonMode();
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);
	//SafeShowOrHideWnd(&m_ScoreView, SW_HIDE);
	memset(m_byUserAction, 0 , sizeof(m_byUserAction));

	// 托管按钮隐藏
	if(bPlayerMode)
	{
		m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_HIDE);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
		///m_GameClientView.SetHaiDiCard(0);
		//m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);	
	}
	else
	{
		m_GameClientView.m_btAutoPlayOn.ShowWindow(SW_SHOW);
		m_GameClientView.m_btAutoPlayOff.ShowWindow(SW_HIDE);
	}

	//堆立扑克
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_cbHeapCardInfo[i][0]=0;
		m_cbHeapCardInfo[i][1]=HEAP_FULL_COUNT-(((i==m_wBankerUser)||((i+2)%4==m_wBankerUser))?28:26);
	}

	//分发扑克
	BYTE cbTakeCount=(MAX_COUNT-1)*GAME_PLAYER+1;    //53张
	BYTE cbSiceFirst=HIBYTE(m_GameStart.wSiceCount);
	BYTE cbSiceSecond=LOBYTE(m_GameStart.wSiceCount);
	BYTE cbSiceCount=__min(cbSiceFirst,cbSiceSecond);
	WORD wTakeChairID=(m_wBankerUser-cbSiceFirst-cbSiceSecond+1+GAME_PLAYER*GAME_PLAYER)%GAME_PLAYER;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//计算数目
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?cbSiceCount*2:0);
		BYTE cbRemoveCount=__min(cbValidCount,cbTakeCount);

		//提取扑克
		cbTakeCount-=cbRemoveCount;
		m_cbHeapCardInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;

		//完成判断
		if (0==cbTakeCount)
		{
			m_wHeapHead=wTakeChairID;
			m_wHeapTail=(m_wBankerUser-cbSiceFirst-cbSiceSecond+1+GAME_PLAYER*GAME_PLAYER/*+7-cbSiceFirst*/)%GAME_PLAYER;
			break;
		}

		//切换索引
		wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
	}

	//绘画王牌
	//m_cbTempKing[0]=m_GameStart.cbKingCard;
	m_GameClientView.m_HandCardControl.SetCardData(NULL,0,0/*,NULL*/);

	//扑克设置
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//变量定义
		WORD wViewChairID=SwitchViewChairID(i);

		m_GameClientView.m_TableCard[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][0].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][1].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][2].SetCardData(NULL,0);
		m_GameClientView.m_WeaveCard[wViewChairID][3].SetCardData(NULL,0);

		//组合界面
		m_GameClientView.m_WeaveCard[i][0].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][1].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][2].SetDisplayItem(true);
		m_GameClientView.m_WeaveCard[i][3].SetDisplayItem(true);
		//m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);

		if (i == GetMeChairID())
		{
			BYTE cbBankerCard = 0;
			if (i == m_wBankerUser)
			{
				cbBankerCard=m_GameStart.cbCardData[MAX_COUNT-1];
				tagCardItem CardItem;
				CardItem.cbCardData = cbBankerCard;
				CardItem.bShoot = false;
				//CardItem.bKingFlag = false;//(cbBankerCard == m_cbTempKing[0])?true:false;
				m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);
			}
			m_GameClientView.m_HandCardControl.SetCardData(m_GameStart.cbCardData,MAX_COUNT-1,cbBankerCard/*,m_cbTempKing[0]*/);
		}
		else
		{
			WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
			m_GameClientView.m_UserCard[wIndex].SetCardData(MAX_COUNT-1,(i==m_wBankerUser));
		}

		////用户扑克
		//if (wViewChairID!=2)
		//{
		//	WORD wIndex=(wViewChairID>=3)?2:wViewChairID;
		//	m_GameClientView.m_UserCard[wIndex].SetCardData(MAX_COUNT-1,(i==m_wBankerUser));
		//}
		//else
		//{
		//	BYTE cbBankerCard=(i==m_wBankerUser)?m_GameStart.cbCardData[MAX_COUNT-1]:0;
		//	tagCardItem CardItem;
		//	CardItem.cbCardData = cbBankerCard;
		//	CardItem.bShoot = false;
		//	CardItem.bKingFlag = (cbBankerCard == m_cbTempKing[0])?true:false;
		//	m_GameClientView.m_HandCardControl.SetCurrentCard(CardItem);
		//	m_GameClientView.m_HandCardControl.SetCardData(m_GameStart.cbCardData,MAX_COUNT-1,cbBankerCard,m_cbTempKing[0]);
		//}
	}

	////出牌提示
	//if ((bPlayerMode==false)&&(m_wCurrentUser!=INVALID_CHAIR))
	//{
	//	WORD wMeChairID=GetMeChairID();
	//	if (m_wCurrentUser==wMeChairID)
	//	{
	//		m_GameClientView.SetStatusFlag(true,false);
	//	}
	//}

	////动作处理
	//if ((bPlayerMode==false)&&(m_GameStart.cbUserAction!=WIK_NULL))
	//{
	//	ShowOperateControl(m_GameStart.cbUserAction);
	//	SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	//}

	m_bIsMeCanOutCard = false;
	m_wCurrentUser = INVALID_CHAIR;
	m_byUserAction[GetMeChairID()] = WIK_NULL;
	WORD wMeChairID=GetMeChairID();
	if (!bPlayerMode && m_wBankerUser==wMeChairID)
	{
		m_wCurrentUser =  wMeChairID; 
		//m_GameClientView.SetStatusFlag(true,false);
		if (WIK_NULL != m_GameStart.cbUserAction)
		{
			m_byUserAction[wMeChairID] = m_GameStart.cbUserAction;
			//ShowOperateControl(m_GameStart.cbUserAction,m_GameStart.cbCardData[MAX_COUNT-1]);
			m_bIsMeCanOutCard = true;
			SetGameClock(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
		else
		{
			SetGameClock(m_wCurrentUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
		}
	}
	else
	{
		SetGameClock(m_wBankerUser,IDI_OPERATE_CARD,TIME_HEAR_STATUS);
	}

	//托管设置
	m_bStustee = false;
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.SetTrustee(SwitchViewChairID(i),m_GameStart.bTrustee[i]);
	}

	//激活框架
	if (bPlayerMode==false) ActiveGameFrame();

	//更新界面
	m_GameClientView.InvalidGameView(0,0,0,0);

	return 0;
}

//骰子动画
LRESULT CGameClientDlg::OnShowSZ(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_nDiceCount1 = wParam;
	m_GameClientView.m_nDiceCount2 = lParam;

	//环境处理
	//m_GameClientView.SetDiceEffect(true);

	return 0;
}

//听牌按钮
LRESULT CGameClientDlg::OnTingCard(WPARAM wParam, LPARAM lParam)
{
	KillGameClock(IDI_USER_TING_CARD);
	//m_GameClientView.m_btTingCard.ShowWindow(SW_HIDE);
	//m_GameClientView.m_btBuTing.ShowWindow(SW_HIDE);
	BYTE byTingTag = (BYTE)wParam;
	CMD_UserTingOperate TingOperate;
	//设置状态
	if (1 == byTingTag)
	{//听牌
		TingOperate.bTingCard = true;
	}
	else
	{//不听
		TingOperate.bTingCard = false;
	}
	SendSocketData(SUB_C_USERTINGCARD,&TingOperate,sizeof(TingOperate));
	return 0;
}

//色子动画按钮
LRESULT CGameClientDlg::OnSZFlashBT(WPARAM wParam, LPARAM lParam)
{
	KillGameClock(IDI_SZSTART);
	//m_GameClientView.PlaySZFlashBT(false);
	SendSocketData(SUB_C_SZSTART);
	return 0;
}