#include "StdAfx.h"
#include "GameLogic.h"
#include "math.h"


//////////////////////////////////////////////////////////////////////////

//静态变量
bool		CChiHuRight::m_bInit = false;
DWORD		CChiHuRight::m_dwRightMask[MAX_RIGHT_COUNT];

//构造函数
CChiHuRight::CChiHuRight()
{
	ZeroMemory( m_dwRight,sizeof(m_dwRight) );

	if( !m_bInit )
	{
		m_bInit = true;
		for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		{
			if( 0 == i )
				m_dwRightMask[i] = 0;
			else
				m_dwRightMask[i] = (DWORD(pow(2.0,i-1)))<<28;
		}
	}
}

//赋值符重载
CChiHuRight & CChiHuRight::operator = ( DWORD dwRight )
{
	DWORD dwOtherRight = 0;
	//验证权位
	if( !IsValidRight( dwRight ) )
	{
		//验证取反权位
		ASSERT( IsValidRight( ~dwRight ) );
		if( !IsValidRight( ~dwRight ) ) return *this;
		dwRight = ~dwRight;
		dwOtherRight = MASK_CHI_HU_RIGHT;
	}

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
			m_dwRight[i] = dwRight&MASK_CHI_HU_RIGHT;
		else m_dwRight[i] = dwOtherRight;
	}

	return *this;
}

//与等于
CChiHuRight & CChiHuRight::operator &= ( DWORD dwRight )
{
	bool bNavigate = false;
	//验证权位
	if( !IsValidRight( dwRight ) )
	{
		//验证取反权位
		ASSERT( IsValidRight( ~dwRight ) );
		if( !IsValidRight( ~dwRight ) ) return *this;
		//调整权位
		DWORD dwHeadRight = (~dwRight)&0xF0000000;
		DWORD dwTailRight = dwRight&MASK_CHI_HU_RIGHT;
		dwRight = dwHeadRight|dwTailRight;
		bNavigate = true;
	}

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
		{
			m_dwRight[i] &= (dwRight&MASK_CHI_HU_RIGHT);
		}
		else if( !bNavigate )
			m_dwRight[i] = 0;
	}

	return *this;
}

//或等于
CChiHuRight & CChiHuRight::operator |= ( DWORD dwRight )
{
	//验证权位
	if( !IsValidRight( dwRight ) ) return *this;

	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
	{
		if( (dwRight&m_dwRightMask[i]) || (i==0&&dwRight<0x10000000) )
		{
			m_dwRight[i] |= (dwRight&MASK_CHI_HU_RIGHT);
			break;
		}
	}

	return *this;
}

//与
CChiHuRight CChiHuRight::operator & ( DWORD dwRight )
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//与
CChiHuRight CChiHuRight::operator & ( DWORD dwRight ) const
{
	CChiHuRight chr = *this;
	return (chr &= dwRight);
}

//或
CChiHuRight CChiHuRight::operator | ( DWORD dwRight )
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//或
CChiHuRight CChiHuRight::operator | ( DWORD dwRight ) const
{
	CChiHuRight chr = *this;
	return chr |= dwRight;
}

//相等
bool CChiHuRight::operator == ( DWORD dwRight ) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)==chr;
}

//相等
bool CChiHuRight::operator == ( const CChiHuRight chr ) const
{
	for( WORD i = 0; i < CountArray( m_dwRight ); i++ )
	{
		if( m_dwRight[i] != chr.m_dwRight[i] ) return false;
	}
	return true;
}

//不相等
bool CChiHuRight::operator != ( DWORD dwRight ) const
{
	CChiHuRight chr;
	chr = dwRight;
	return (*this)!=chr;
}

//不相等
bool CChiHuRight::operator != ( const CChiHuRight chr ) const
{
	return !((*this)==chr);
}

//是否权位为空
bool CChiHuRight::IsEmpty()
{
	for( BYTE i = 0; i < CountArray(m_dwRight); i++ )
		if( m_dwRight[i] ) return false;
	return true;
}

//设置权位为空
void CChiHuRight::SetEmpty()
{
	ZeroMemory( m_dwRight,sizeof(m_dwRight) );
	return;
}

//获取权位数值
BYTE CChiHuRight::GetRightData( DWORD dwRight[], BYTE cbMaxCount )
{
	ASSERT( cbMaxCount >= CountArray(m_dwRight) );
	if( cbMaxCount < CountArray(m_dwRight) ) return 0;

	CopyMemory( dwRight,m_dwRight,sizeof(DWORD)*CountArray(m_dwRight) );
	return CountArray(m_dwRight);
}

//设置权位数值
bool CChiHuRight::SetRightData( const DWORD dwRight[], BYTE cbRightCount )
{
	ASSERT( cbRightCount <= CountArray(m_dwRight) );
	if( cbRightCount > CountArray(m_dwRight) ) return false;

	ZeroMemory( m_dwRight,sizeof(m_dwRight) );
	CopyMemory( m_dwRight,dwRight,sizeof(DWORD)*cbRightCount );
	
	return true;
}

//检查仅位是否正确
bool CChiHuRight::IsValidRight( DWORD dwRight )
{
	DWORD dwRightHead = dwRight & 0xF0000000;
	for( BYTE i = 0; i < CountArray(m_dwRightMask); i++ )
		if( m_dwRightMask[i] == dwRightHead ) return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////

//静态变量

//扑克数据
const BYTE CGameLogic::m_cbCardDataArray[MAX_REPERTORY]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,						//万子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,						//索子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,						//同子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,									//番子
	0x38,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,0x3f								//春夏秋冬梅兰菊竹
};

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
	m_cbMagicIndex = MAX_INDEX;
}

//析构函数
CGameLogic::~CGameLogic()
{
}

//混乱扑克
void CGameLogic::RandCardData(BYTE cbCardData[], BYTE cbMaxCount)
{
	//混乱准备
	BYTE cbCardDataTemp[CountArray(m_cbCardDataArray)];
	CopyMemory(cbCardDataTemp,m_cbCardDataArray,sizeof(m_cbCardDataArray));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbMaxCount-cbRandCount);
		cbCardData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbMaxCount-cbRandCount];
	} while (cbRandCount<cbMaxCount);

	return;
}

//混乱扑克
void CGameLogic::RandCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbRandData[])
{
	if( cbCardCount == 0 ) return;

	//混乱准备
	BYTE cbCardDataTemp[MAX_COUNT];
	CopyMemory(cbCardDataTemp,cbCardData,sizeof(BYTE)*cbCardCount);

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbCardCount-cbRandCount);
		cbRandData[cbRandCount++]=cbCardDataTemp[cbPosition];
		cbCardDataTemp[cbPosition]=cbCardDataTemp[cbCardCount-cbRandCount];
	} while (cbRandCount<cbCardCount);

	return;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard)
{
	//效验扑克
	ASSERT(IsValidCard(cbRemoveCard));
	ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard)]>0);

	//删除扑克
	BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard);
	if (cbCardIndex[cbRemoveIndex]>0)
	{
		cbCardIndex[cbRemoveIndex]--;
		return true;
	}

	//失败效验
	ASSERT(FALSE);

	return false;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//删除扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		//效验扑克
		ASSERT(IsValidCard(cbRemoveCard[i]));
		ASSERT(cbCardIndex[SwitchToCardIndex(cbRemoveCard[i])]>0);

		//删除扑克
		BYTE cbRemoveIndex=SwitchToCardIndex(cbRemoveCard[i]);
		if (cbCardIndex[cbRemoveIndex]==0)
		{
			//错误断言
			ASSERT(FALSE);

			//还原删除
			for (BYTE j=0;j<i;j++) 
			{
				ASSERT(IsValidCard(cbRemoveCard[j]));
				cbCardIndex[SwitchToCardIndex(cbRemoveCard[j])]++;
			}

			return false;
		}
		else 
		{
			//删除扑克
			--cbCardIndex[cbRemoveIndex];
		}
	}

	return true;
}

//删除扑克
bool CGameLogic::RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount)
{
	//检验数据
	ASSERT(cbCardCount<=MAX_COUNT);
	ASSERT(cbRemoveCount<=cbCardCount);

	//定义变量
	BYTE cbDeleteCount=0,cbTempCardData[MAX_COUNT];
	if (cbCardCount>CountArray(cbTempCardData))
		return false;
	CopyMemory(cbTempCardData,cbCardData,cbCardCount*sizeof(cbCardData[0]));

	//置零扑克
	for (BYTE i=0;i<cbRemoveCount;i++)
	{
		for (BYTE j=0;j<cbCardCount;j++)
		{
			if (cbRemoveCard[i]==cbTempCardData[j])
			{
				cbDeleteCount++;
				cbTempCardData[j]=0;
				break;
			}
		}
	}

	//成功判断
	if (cbDeleteCount!=cbRemoveCount) 
	{
		ASSERT(FALSE);
		return false;
	}

	//清理扑克
	BYTE cbCardPos=0;
	for (BYTE i=0;i<cbCardCount;i++)
	{
		if (cbTempCardData[i]!=0) 
			cbCardData[cbCardPos++]=cbTempCardData[i];
	}

	return true;
}

//有效判断
bool CGameLogic::IsValidCard(BYTE cbCardData)
{
	BYTE cbValue=(cbCardData&MASK_VALUE);
	BYTE cbColor=(cbCardData&MASK_COLOR)>>4;
	return (((cbValue>=1)&&(cbValue<=9)&&(cbColor<=2))||((cbValue>=1)&&(cbValue<=0x0f)&&(cbColor==3)));
}

//扑克数目
BYTE CGameLogic::GetCardCount(const BYTE cbCardIndex[MAX_INDEX])
{
	//数目统计
	BYTE cbCardCount=0;
	for (BYTE i=0;i<MAX_INDEX;i++) 
		cbCardCount+=cbCardIndex[i];

	return cbCardCount;
}

//获取组合
BYTE CGameLogic::GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4])
{
	//组合扑克
	switch (cbWeaveKind)
	{
	case WIK_LEFT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard+1;
			cbCardBuffer[2]=cbCenterCard+2;

			return 3;
		}
	case WIK_RIGHT:		//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-2;
			cbCardBuffer[1]=cbCenterCard-1;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_CENTER:	//上牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard-1;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard+1;

			return 3;
		}
	case WIK_PENG:		//碰牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;

			return 3;
		}
	case WIK_GANG:		//杠牌操作
		{
			//设置变量
			cbCardBuffer[0]=cbCenterCard;
			cbCardBuffer[1]=cbCenterCard;
			cbCardBuffer[2]=cbCenterCard;
			cbCardBuffer[3]=cbCenterCard;

			return 4;
		}
	default:
		{
			ASSERT(FALSE);
		}
	}

	return 0;
}

//动作等级
BYTE CGameLogic::GetUserActionRank(BYTE cbUserAction)
{
	//胡牌等级
	if (cbUserAction&WIK_CHI_HU) { return 4; }

	//杠牌等级
	if (cbUserAction&WIK_GANG) { return 3; }

	//碰牌等级
	if (cbUserAction&WIK_PENG) { return 2; }

	//上牌等级
	if (cbUserAction&(WIK_RIGHT|WIK_CENTER|WIK_LEFT)) { return 1; }

	return 0;
}

//胡牌等级
WORD CGameLogic::GetChiHuActionRank(const CChiHuRight & ChiHuRight)
{
	WORD wFanShu = 0;
	//  1番
	if( !(ChiHuRight&CHR_ZI_MO).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_DAN_DIAO_JIANG).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_KAN_ZHANG).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_BIAN_ZHANG).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_WU_ZI).IsEmpty() )
		wFanShu++;
	if( !(ChiHuRight&CHR_QUE_YI_MEN).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_MING_GANG).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_YAO_JIU).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_LAO_SHAO_JIANG).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_LIAN_LIU).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_XI_XIANG_FENG).IsEmpty() ) 
		wFanShu++;
	if( !(ChiHuRight&CHR_YI_BAN_GAO).IsEmpty() ) 
		wFanShu++;

	//	2番
	if( !(ChiHuRight&CHR_DUAN_YAO).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_AN_GANG).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_SHUANG_AN_KE).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_SHUANG_TONG_KE).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_SHI_GUI_YI).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_PING_HU).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_MEN_QI_QING).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_MEN_FENG_KE).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_QUAN_FENG_KE).IsEmpty() )
		wFanShu += 2;
	if( !(ChiHuRight&CHR_JIAN_KE).IsEmpty() )
		wFanShu += 2;

	//	4番
	if( !(ChiHuRight&CHR_JUE_ZHANG).IsEmpty() )
		wFanShu += 4;
	if( !(ChiHuRight&CHR_SHUANG_MING_GANG).IsEmpty() )
		wFanShu += 4;
	if( !(ChiHuRight&CHR_BU_QIU_REN).IsEmpty() )
		wFanShu += 4;
	if( !(ChiHuRight&CHR_QUAN_DAI_YAO).IsEmpty() )
		wFanShu += 4;

	//	6番
	if( !(ChiHuRight&CHR_SHUANG_JIAN_KE).IsEmpty() )
		wFanShu += 6;
	if( !(ChiHuRight&CHR_SHUANG_AN_GANG).IsEmpty() )
		wFanShu += 6;
	if( !(ChiHuRight&CHR_QUAN_QIU_REN).IsEmpty() )
		wFanShu += 6;
	if( !(ChiHuRight&CHR_WU_MEN_QI).IsEmpty() )
		wFanShu += 6;
	if( !(ChiHuRight&CHR_SAN_SE_BU).IsEmpty() )
		wFanShu += 6;
	if( !(ChiHuRight&CHR_HUN_YI_SE).IsEmpty() )
		wFanShu += 6;
	if( !(ChiHuRight&CHR_PENG_PENG).IsEmpty() )
		wFanShu += 6;

	//	8番
	if( !(ChiHuRight&CHR_QIANG_GANG).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_GANG_KAI).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_HAI_DI).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_MIAO_SHOU).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_WU_FAN).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_SAN_SE_JIE).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_SAN_SE_TONG).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_TUI_BU_DAO).IsEmpty() )
		wFanShu += 8;
	if( !(ChiHuRight&CHR_HUA_LONG).IsEmpty() )
		wFanShu += 8;

	//	12番
	if( !(ChiHuRight&CHR_SAN_FENG_KE).IsEmpty() )
		wFanShu += 12;
	if( !(ChiHuRight&CHR_XIAO_WU).IsEmpty() )
		wFanShu += 12;
	if( !(ChiHuRight&CHR_DA_WU).IsEmpty() )
		wFanShu += 12;

	//	16番
	if( !(ChiHuRight&CHR_SAN_AN_KE).IsEmpty() )
		wFanShu += 16;
	if( !(ChiHuRight&CHR_SAN_TONG_KE).IsEmpty() )
		wFanShu += 16;
	if( !(ChiHuRight&CHR_QUAN_DAI_WU).IsEmpty() )
		wFanShu += 16;
	if( !(ChiHuRight&CHR_YI_SE_BU).IsEmpty() )
		wFanShu += 16;
	if( !(ChiHuRight&CHR_SAN_SE_SHUANG).IsEmpty() )
		wFanShu += 16;
	if( !(ChiHuRight&CHR_QING_LONG).IsEmpty() )
		wFanShu += 16;

	//	24番
	if( !(ChiHuRight&CHR_QUAN_XIAO).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_QUAN_ZHONG).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_QUAN_DA).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_YI_SE_JIE).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_YI_SE_TONG).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_QING_YI_SE).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_QUAN_SHUANG).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_QI_XING_BU_KAO).IsEmpty() )
		wFanShu += 24;
	if( !(ChiHuRight&CHR_QI_DUI).IsEmpty() )
		wFanShu += 24;

	//	32番
	if( !(ChiHuRight&CHR_HUN_YAO).IsEmpty() )
		wFanShu += 32;
	if( !(ChiHuRight&CHR_SAN_GANG).IsEmpty() )
		wFanShu += 32;
	if( !(ChiHuRight&CHR_YI_SHI_BU).IsEmpty() )
		wFanShu += 32;

	//	48番
	if( !(ChiHuRight&CHR_YI_SHI_JIE).IsEmpty() )
		wFanShu += 48;
	if( !(ChiHuRight&CHR_YI_SHI_TONG).IsEmpty() )
		wFanShu += 48;

	//	64番
	if( !(ChiHuRight&CHR_YI_SE_SHUANG).IsEmpty() )
		wFanShu += 64;
	if( !(ChiHuRight&CHR_SI_AN_KE).IsEmpty() )
		wFanShu += 64;
	if( !(ChiHuRight&CHR_ZI_YI_SE).IsEmpty() )
		wFanShu += 64;
	if( !(ChiHuRight&CHR_XIAO_SAN_YUAN).IsEmpty() )
		wFanShu += 64;
	if( !(ChiHuRight&CHR_XIAO_SHI_XI).IsEmpty() )
		wFanShu += 64;
	if( !(ChiHuRight&CHR_QING_YAO_JIU).IsEmpty() )
		wFanShu += 64;

	//	88番
	if( !(ChiHuRight&CHR_SHI_SAN_YAO).IsEmpty() )
		wFanShu += 88;
	if( !(ChiHuRight&CHR_LIAN_QI_DUI).IsEmpty() )
		wFanShu += 88;
	if( !(ChiHuRight&CHR_SI_GANG).IsEmpty() )
		wFanShu += 88;
	if( !(ChiHuRight&CHR_JIU_LIAN_DENG).IsEmpty() )
		wFanShu += 88;
	if( !(ChiHuRight&CHR_LU_YI_SE).IsEmpty() )
		wFanShu += 88;
	if( !(ChiHuRight&CHR_DA_SAN_YUAN).IsEmpty() )
		wFanShu += 88;
	if( !(ChiHuRight&CHR_DA_SHI_XI).IsEmpty() )
		wFanShu += 88;

	return wFanShu;
}

//吃牌判断
BYTE CGameLogic::EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if ( cbCurrentCard>=0x31 || IsMagicCard(cbCurrentCard) ) 
		return WIK_NULL;

	//变量定义
	BYTE cbExcursion[3]={0,1,2};
	BYTE cbItemKind[3]={WIK_LEFT,WIK_CENTER,WIK_RIGHT};

	//吃牌判断
	BYTE cbEatKind=0,cbFirstIndex=0;
	BYTE cbCurrentIndex=SwitchToCardIndex(cbCurrentCard);
	for (BYTE i=0;i<CountArray(cbItemKind);i++)
	{
		BYTE cbValueIndex=cbCurrentIndex%9;
		if ((cbValueIndex>=cbExcursion[i])&&((cbValueIndex-cbExcursion[i])<=6))
		{
			//吃牌判断
			cbFirstIndex=cbCurrentIndex-cbExcursion[i];

			//吃牌不能包含有王霸
			if( m_cbMagicIndex != MAX_INDEX &&
				m_cbMagicIndex >= cbFirstIndex && m_cbMagicIndex <= cbFirstIndex+2 ) continue;

			if ((cbCurrentIndex!=cbFirstIndex)&&(cbCardIndex[cbFirstIndex]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+1))&&(cbCardIndex[cbFirstIndex+1]==0))
				continue;
			if ((cbCurrentIndex!=(cbFirstIndex+2))&&(cbCardIndex[cbFirstIndex+2]==0))
				continue;

			//设置类型
			cbEatKind|=cbItemKind[i];
		}
	}

	return cbEatKind;
}

//碰牌判断
BYTE CGameLogic::EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if ( IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard) ) 
		return WIK_NULL;

	//碰牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]>=2)?WIK_PENG:WIK_NULL;
}

//杠牌判断
BYTE CGameLogic::EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard)
{
	//参数效验
	ASSERT(IsValidCard(cbCurrentCard));

	//过滤判断
	if ( IsMagicCard(cbCurrentCard) || IsHuaCard(cbCurrentCard) ) 
		return WIK_NULL;

	//杠牌判断
	return (cbCardIndex[SwitchToCardIndex(cbCurrentCard)]==3)?WIK_GANG:WIK_NULL;
}

//杠牌分析
BYTE CGameLogic::AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult)
{
	//设置变量
	BYTE cbActionMask=WIK_NULL;
	ZeroMemory(&GangCardResult,sizeof(GangCardResult));

	//手上杠牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if( i == m_cbMagicIndex ) continue;
		if (cbCardIndex[i]==4)
		{
			cbActionMask|=WIK_GANG;
			GangCardResult.cbCardData[GangCardResult.cbCardCount++]=SwitchToCardData(i);
		}
	}

	//组合杠牌
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if (WeaveItem[i].cbWeaveKind==WIK_PENG)
		{
			if (cbCardIndex[SwitchToCardIndex(WeaveItem[i].cbCenterCard)]==1)
			{
				cbActionMask|=WIK_GANG;
				GangCardResult.cbCardData[GangCardResult.cbCardCount++]=WeaveItem[i].cbCenterCard;
			}
		}
	}

	return cbActionMask;
}

//吃胡分析
BYTE CGameLogic::AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight)
{
	//变量定义
	BYTE cbChiHuKind=WIK_NULL;
	CAnalyseItemArray AnalyseItemArray;

	//设置变量
	AnalyseItemArray.RemoveAll();
	ChiHuRight.SetEmpty();

	//构造扑克
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//cbCurrentCard一定不为0			!!!!!!!!!
	ASSERT( cbCurrentCard != 0 );
	if( cbCurrentCard == 0 ) return WIK_NULL;

	/*
	//	特殊番型
	*/
	//连七对
	if( IsLianQiDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
		ChiHuRight |= CHR_LIAN_QI_DUI;
	//十三幺
	if( IsShiSanYao(cbCardIndex,cbWeaveCount,cbCurrentCard) )
		ChiHuRight |= CHR_SHI_SAN_YAO;
	//七星不靠
	if( IsQiXingBuKao(cbCardIndex,cbWeaveCount,cbCurrentCard) )
		ChiHuRight |= CHR_QI_XING_BU_KAO;
	//全不靠
	if( IsQuanBuKao(cbCardIndex,cbWeaveCount,cbCurrentCard) )
		ChiHuRight |= CHR_QUAN_BU_KAO;
	//组合龙
	if( IsZuHeLong(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
		ChiHuRight |= CHR_ZU_HE_LONG;
	//七小对牌
	if( IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
		ChiHuRight |= CHR_QI_DUI;
	//三色双龙会
	if( IsSanSeShuangLong(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
		ChiHuRight |= CHR_SAN_SE_SHUANG;

	//插入扑克
	if (cbCurrentCard!=0)
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;
	//分析扑克
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

	//胡牌分析
	if (AnalyseItemArray.GetCount()>0)
	{
		//牌型分析
		for (INT_PTR i=0;i<AnalyseItemArray.GetCount();i++)
		{
			//变量定义
			tagAnalyseItem * pAnalyseItem=&AnalyseItemArray[i];

			/*
			//	判断番型
			*/
			//大四喜
			if( IsDaSiXi(*pAnalyseItem) ) 
				ChiHuRight |= CHR_DA_SHI_XI;
			//大三元
			if( IsDaSanYuan(*pAnalyseItem) ) 
				ChiHuRight |= CHR_DA_SAN_YUAN;
			//四暗刻
			if( IsSiAnKe(pAnalyseItem,WeaveItem,cbWeaveCount) ) 
				ChiHuRight |= CHR_SI_AN_KE;
			//清幺九
			if( IsQingYaoJiu(*pAnalyseItem) ) 
				ChiHuRight |= CHR_QING_YAO_JIU;
			//小四喜
			if( IsXiaoSiXi(*pAnalyseItem) ) 
				ChiHuRight |= CHR_XIAO_SHI_XI;
			//小三元
			if( IsXiaoShanYuan(*pAnalyseItem) ) 
				ChiHuRight |= CHR_XIAO_SAN_YUAN;
			//字一色牌
			if( IsZiYiSe(*pAnalyseItem) ) 
				ChiHuRight |= CHR_ZI_YI_SE;
			//一色双龙会
			if( IsYiSeHuangHuiLong(*pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_SE_SHUANG;
			//一色四同顺
			if( IsYiSeSiTongShun(*pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_SHI_TONG;
			//一色四节高
			if( IsYiSeSiJieGao(*pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_SHI_JIE;
			//一色四步高
			if( IsYiSeSiBuGao(*pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_SHI_BU;
			//混幺九
			if( IsHunYaoJiu(*pAnalyseItem) ) 
				ChiHuRight |= CHR_HUN_YAO;
			//全双
			if( IsQuanShuang(*pAnalyseItem) ) 
				ChiHuRight |= CHR_QUAN_SHUANG;
			//一色三同顺
			if( IsYiSeSanTongShun(*pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_SE_TONG;
			//一色三节高
			if( IsYiSeSanJieGao(*pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_SE_JIE;
			//一色三步高
			if( IsYiSeBu(*pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_SE_BU;
			//全带5
			if( IsQuanDaiWu(*pAnalyseItem) ) 
				ChiHuRight |= CHR_QUAN_DAI_WU;
			//三同刻
			if( IsSanTongKe(*pAnalyseItem) ) 
				ChiHuRight |= CHR_SAN_TONG_KE;
			//三暗刻
			if( IsSanAnKe(*pAnalyseItem,WeaveItem,cbWeaveCount) ) 
				ChiHuRight |= CHR_SAN_AN_KE;
			//大于五
			if( IsDaWu(pAnalyseItem) ) 
				ChiHuRight |= CHR_DA_WU;
			//小于五
			if( IsXiaoWu(pAnalyseItem) ) 
				ChiHuRight |= CHR_XIAO_WU;
			//三风刻
			if( IsSanFengKe(pAnalyseItem) ) 
				ChiHuRight |= CHR_SAN_FENG_KE;
			//花龙
			if( IsHuaLong(pAnalyseItem) ) 
				ChiHuRight |= CHR_HUA_LONG;
			//三色三同顺
			if( IsSanSeTong(pAnalyseItem) ) 
				ChiHuRight |= CHR_SAN_SE_TONG;
			//三色三节高
			if( IsSanSeJie(pAnalyseItem) ) 
				ChiHuRight |= CHR_SAN_SE_JIE;
			//碰碰和
			if( IsPengPeng(pAnalyseItem) ) 
				ChiHuRight |= CHR_PENG_PENG;
			//三色三步高
			if( IsSanSeBu(pAnalyseItem) ) 
				ChiHuRight |= CHR_SAN_SE_BU;
			//双箭刻
			if( IsShuangJianKe(pAnalyseItem) ) 
				ChiHuRight |= CHR_SHUANG_JIAN_KE;
			//全带幺
			if( IsQuanDaiYao(pAnalyseItem) ) 
				ChiHuRight |= CHR_QUAN_DAI_YAO;
			//箭刻
			if( IsJianKe(pAnalyseItem) ) 
				ChiHuRight |= CHR_JIAN_KE;
			//平胡
			if( IsPingHu(pAnalyseItem) ) 
				ChiHuRight |= CHR_PING_HU;
			//双同刻
			if( IsShuangTongKe(pAnalyseItem) ) 
				ChiHuRight |= CHR_SHUANG_TONG_KE;
			//双暗刻
			if( IsShuangAnKe(pAnalyseItem,WeaveItem,cbWeaveCount) ) 
				ChiHuRight |= CHR_SHUANG_AN_KE;
			//一般高
			if( IsYiBanGao(pAnalyseItem) ) 
				ChiHuRight |= CHR_YI_BAN_GAO;
			//喜相逢
			if( IsXiXiangFeng(pAnalyseItem) ) 
				ChiHuRight |= CHR_XI_XIANG_FENG;
			//连六
			if( IsLianLu(pAnalyseItem) ) 
				ChiHuRight |= CHR_LIAN_LIU;
			//老少副
			if( IsLaoShaoFu(pAnalyseItem) ) 
				ChiHuRight |= CHR_LAO_SHAO_JIANG;
			//幺九刻
			if( IsYaoJiuKe(pAnalyseItem) ) 
				ChiHuRight |= CHR_YAO_JIU;
			//边张
			if( IsBianZhang(pAnalyseItem,cbWeaveCount,cbCurrentCard) ) 
				ChiHuRight |= CHR_BIAN_ZHANG;
			//坎张
			if( IsKanZhang(pAnalyseItem,cbWeaveCount,cbCurrentCard) ) 
				ChiHuRight |= CHR_KAN_ZHANG;
			//单钓将
			if( IsDanDiaoJiang(pAnalyseItem,cbCurrentCard) ) 
				ChiHuRight |= CHR_DAN_DIAO_JIANG;
			//清龙
			if( IsQingLong(*pAnalyseItem) )
				ChiHuRight |= CHR_QING_LONG;
			//四归一
			if( IsSiGuiYi(pAnalyseItem,cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
				ChiHuRight |= CHR_SHI_GUI_YI;
		}

		//无番和
		ChiHuRight |= CHR_WU_FAN;

		/*
		//	判断番型
		*/
		//绿一色
		if( IsLuYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_LU_YI_SE;
		//九莲宝灯
		if( IsJiuLianBaoDeng(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_JIU_LIAN_DENG;
		//四杠
		if( IsSiGang(cbCardIndex,WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_SI_GANG;
		//双暗杠
		if( IsShuangAnGang(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_SHUANG_AN_GANG;
		//双明杠
		if( IsShuangMingGang(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_SHUANG_MING_GANG;
		//暗杠
		if( IsAnGang(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_AN_GANG;
		//明杠
		if( IsMingGang(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_MING_GANG;
		//不求人
		if( IsBuQiuRen(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_BU_QIU_REN;
		//三杠
		if( IsSanGang(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_SAN_GANG;
		//全大
		if( IsQuanDa(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_QUAN_DA;
		//全中
		if( IsQuanZhong(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_QUAN_ZHONG;
		//全小
		if( IsQuanXiao(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_QUAN_XIAO;
	}

	if( !ChiHuRight.IsEmpty() )
		cbChiHuKind = WIK_CHI_HU;

	if( cbChiHuKind == WIK_CHI_HU )
	{
		/*
		//	判断番型
		*/
		//推不倒
		if( IsTuiBuDao(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_TUI_BU_DAO;
		//清一色牌
		if( IsQingYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_QING_YI_SE;
		//门前清
		if( IsMenQianQing(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_MEN_QI_QING;
		//无字
		if( IsWuZi(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_WU_ZI;
		//缺一门
		if( IsQueYiMen(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_QUE_YI_MEN;
		//断幺
		if( IsDuanYao(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_DUAN_YAO;
		//全求人
		if( IsQuanQiuRen(WeaveItem,cbWeaveCount) )
			ChiHuRight |= CHR_QUAN_QIU_REN;
		//五门齐
		if( IsWuMenQi(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_WU_MEN_QI;
		//混一色
		if( IsHunYiSe(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
			ChiHuRight |= CHR_HUN_YI_SE;
	}

	return cbChiHuKind;
}

//听牌分析
BYTE CGameLogic::AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	//复制数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	BYTE cbCardCount = GetCardCount(cbCardIndexTemp);
	CChiHuRight chr;

	if( (cbCardCount-2)%3==0 )
	{
		for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		{
			if( cbCardIndexTemp[i] == 0 ) continue;
			cbCardIndexTemp[i]--;

			for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
			{
				BYTE cbCurrentCard = SwitchToCardData(j);
				if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
					return WIK_LISTEN;
			}

			cbCardIndexTemp[i]++;
		}
	}
	else
	{
		for( BYTE j = 0; j < MAX_INDEX-MAX_HUA_CARD; j++ )
		{
			BYTE cbCurrentCard = SwitchToCardData(j);
			if( WIK_CHI_HU == AnalyseChiHuCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,cbCurrentCard,chr) )
				return WIK_LISTEN;
		}
	}

	return WIK_NULL;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(BYTE cbCardIndex)
{
	ASSERT(cbCardIndex<MAX_INDEX);
	if( cbCardIndex < 27 )
		return ((cbCardIndex/9)<<4)|(cbCardIndex%9+1);
	else return (0x30|(cbCardIndex-27+1));
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(BYTE cbCardData)
{
	ASSERT(IsValidCard(cbCardData));
	return ((cbCardData&MASK_COLOR)>>4)*9+(cbCardData&MASK_VALUE)-1;
}

//扑克转换
BYTE CGameLogic::SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT])
{
	//转换扑克
	BYTE cbPosition=0;
	//钻牌
	if( m_cbMagicIndex != MAX_INDEX )
	{
		for( BYTE i = 0; i < cbCardIndex[m_cbMagicIndex]; i++ )
			cbCardData[cbPosition++] = SwitchToCardData(m_cbMagicIndex);
	}
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if( i == m_cbMagicIndex && m_cbMagicIndex != INDEX_REPLACE_CARD ) 
		{
			//如果财神有代替牌，则代替牌代替财神原来的位置
			if( INDEX_REPLACE_CARD != MAX_INDEX )
			{
				for( BYTE j = 0; j < cbCardIndex[INDEX_REPLACE_CARD]; j++ )
					cbCardData[cbPosition++] = SwitchToCardData(INDEX_REPLACE_CARD);
			}
			continue;
		}
		if( i == INDEX_REPLACE_CARD ) continue;
		if (cbCardIndex[i]!=0)
		{
			for (BYTE j=0;j<cbCardIndex[i];j++)
			{
				ASSERT(cbPosition<MAX_COUNT);
				cbCardData[cbPosition++]=SwitchToCardData(i);
			}
		}
	}

	return cbPosition;
}

//扑克转换
BYTE CGameLogic::SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX])
{
	//设置变量
	ZeroMemory(cbCardIndex,sizeof(BYTE)*MAX_INDEX);

	//转换扑克
	for (BYTE i=0;i<cbCardCount;i++)
	{
		ASSERT(IsValidCard(cbCardData[i]));
		cbCardIndex[SwitchToCardIndex(cbCardData[i])]++;
	}

	return cbCardCount;
}

//分析扑克
bool CGameLogic::AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)
{
	//计算数目
	BYTE cbCardCount=GetCardCount(cbCardIndex);

	//效验数目
	ASSERT((cbCardCount>=2)&&(cbCardCount<=MAX_COUNT)&&((cbCardCount-2)%3==0));
	if ((cbCardCount<2)||(cbCardCount>MAX_COUNT)||((cbCardCount-2)%3!=0))
		return false;

	//变量定义
	BYTE cbKindItemCount=0;
	tagKindItem KindItem[27*2+28];
	ZeroMemory(KindItem,sizeof(KindItem));

	//需求判断
	BYTE cbLessKindItem=(cbCardCount-2)/3;
	ASSERT((cbLessKindItem+cbWeaveCount)==MAX_WEAVE);

	//单吊判断
	if (cbLessKindItem==0)
	{
		//效验参数
		ASSERT((cbCardCount==2)&&(cbWeaveCount==MAX_WEAVE));

		//牌眼判断
		for (BYTE i=0;i<MAX_INDEX-MAX_HUA_CARD;i++)
		{
			if (cbCardIndex[i]==2 || 
				( m_cbMagicIndex != MAX_INDEX && i != m_cbMagicIndex && cbCardIndex[m_cbMagicIndex]+cbCardIndex[i]==2 ) )
			{
				//变量定义
				tagAnalyseItem AnalyseItem;
				ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

				//设置结果
				for (BYTE j=0;j<cbWeaveCount;j++)
				{
					AnalyseItem.cbWeaveKind[j]=WeaveItem[j].cbWeaveKind;
					AnalyseItem.cbCenterCard[j]=WeaveItem[j].cbCenterCard;
					CopyMemory( AnalyseItem.cbCardData[j],WeaveItem[j].cbCardData,sizeof(WeaveItem[j].cbCardData) );
				}
				AnalyseItem.cbCardEye=SwitchToCardData(i);
				if( cbCardIndex[i] < 2 || i == m_cbMagicIndex )
					AnalyseItem.bMagicEye = true;
				else AnalyseItem.bMagicEye = false;

				//插入结果
				AnalyseItemArray.Add(AnalyseItem);

				return true;
			}
		}

		return false;
	}

	//拆分分析
	BYTE cbMagicCardIndex[MAX_INDEX];
	CopyMemory(cbMagicCardIndex,cbCardIndex,sizeof(cbMagicCardIndex));
	//如果有财神
	BYTE cbMagicCardCount = 0;
	if( m_cbMagicIndex != MAX_INDEX )
	{
		cbMagicCardCount = cbCardIndex[m_cbMagicIndex];
		//如果财神有代替牌，财神与代替牌转换
		if( INDEX_REPLACE_CARD != MAX_INDEX )
		{
			cbMagicCardIndex[m_cbMagicIndex] = cbMagicCardIndex[INDEX_REPLACE_CARD];
			cbMagicCardIndex[INDEX_REPLACE_CARD] = cbMagicCardCount;
		}
	}
	if (cbCardCount>=3)
	{
		for (BYTE i=0;i<MAX_INDEX-MAX_HUA_CARD;i++)
		{
			//同牌判断
			//如果是财神,并且财神数小于3,则不进行组合
			if( cbMagicCardIndex[i] >= 3 || ( cbMagicCardIndex[i]+cbMagicCardCount >= 3 &&
				( ( INDEX_REPLACE_CARD!=MAX_INDEX && i != INDEX_REPLACE_CARD ) || ( INDEX_REPLACE_CARD==MAX_INDEX && i != m_cbMagicIndex ) ) )
				)
			{
				int nTempIndex = cbMagicCardIndex[i];
				do
				{
					ASSERT( cbKindItemCount < CountArray(KindItem) );
					BYTE cbIndex = i;
					BYTE cbCenterCard = SwitchToCardData(i);
					//如果是财神且财神有代替牌,则换成代替牌
					if( i == m_cbMagicIndex && INDEX_REPLACE_CARD != MAX_INDEX )
					{
						cbIndex = INDEX_REPLACE_CARD;
						cbCenterCard = SwitchToCardData(INDEX_REPLACE_CARD);
					}
					KindItem[cbKindItemCount].cbWeaveKind=WIK_PENG;
					KindItem[cbKindItemCount].cbCenterCard=cbCenterCard;
					KindItem[cbKindItemCount].cbValidIndex[0] = nTempIndex>0?cbIndex:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[1] = nTempIndex>1?cbIndex:m_cbMagicIndex;
					KindItem[cbKindItemCount].cbValidIndex[2] = nTempIndex>2?cbIndex:m_cbMagicIndex;
					cbKindItemCount++;

					//如果是财神,则退出
					if( i == INDEX_REPLACE_CARD || (i == m_cbMagicIndex && INDEX_REPLACE_CARD == MAX_INDEX) )
						break;

					nTempIndex -= 3;
					//如果刚好搭配全部，则退出
					if( nTempIndex == 0 ) break;

				}while( nTempIndex+cbMagicCardCount >= 3 );
			}

			//连牌判断
			if ((i<(MAX_INDEX-MAX_HUA_CARD-9))&&((i%9)<7))
			{
				//只要财神牌数加上3个顺序索引的牌数大于等于3,则进行组合
				if( cbMagicCardCount+cbMagicCardIndex[i]+cbMagicCardIndex[i+1]+cbMagicCardIndex[i+2] >= 3 )
				{
					BYTE cbIndex[3] = { cbMagicCardIndex[i],cbMagicCardIndex[i+1],cbMagicCardIndex[i+2] };
					int nMagicCountTemp = cbMagicCardCount;
					BYTE cbValidIndex[3];
					while( nMagicCountTemp+cbIndex[0]+cbIndex[1]+cbIndex[2] >= 3 )
					{
						for( BYTE j = 0; j < CountArray(cbIndex); j++ )
						{
							if( cbIndex[j] > 0 ) 
							{
								cbIndex[j]--;
								cbValidIndex[j] = (i+j==m_cbMagicIndex&&INDEX_REPLACE_CARD!=MAX_INDEX)?INDEX_REPLACE_CARD:i+j;
							}
							else 
							{
								nMagicCountTemp--;
								cbValidIndex[j] = m_cbMagicIndex;
							}
						}
						if( nMagicCountTemp >= 0 )
						{
							ASSERT( cbKindItemCount < CountArray(KindItem) );
							KindItem[cbKindItemCount].cbWeaveKind=WIK_LEFT;
							KindItem[cbKindItemCount].cbCenterCard=SwitchToCardData(i);
							CopyMemory( KindItem[cbKindItemCount].cbValidIndex,cbValidIndex,sizeof(cbValidIndex) );
							cbKindItemCount++;
						}
						else break;
					}
				}
			}
		}
	}

	//组合分析
	if (cbKindItemCount>=cbLessKindItem)
	{
		//变量定义
		BYTE cbCardIndexTemp[MAX_INDEX];
		ZeroMemory(cbCardIndexTemp,sizeof(cbCardIndexTemp));

		//变量定义
		BYTE cbIndex[MAX_WEAVE];
		for( BYTE i = 0; i < CountArray(cbIndex); i++ )
			cbIndex[i] = i;
		tagKindItem * pKindItem[MAX_WEAVE];
		ZeroMemory(&pKindItem,sizeof(pKindItem));
		tagKindItem KindItemTemp[CountArray(KindItem)];
		if( m_cbMagicIndex != MAX_INDEX )
			CopyMemory( KindItemTemp,KindItem,sizeof(KindItem) );

		//开始组合
		do
		{
			//设置变量
			CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
			cbMagicCardCount = 0;
			if( m_cbMagicIndex != MAX_INDEX )
			{
				CopyMemory( KindItem,KindItemTemp,sizeof(KindItem) );
			}

			for (BYTE i=0;i<cbLessKindItem;i++)
				pKindItem[i]=&KindItem[cbIndex[i]];

			//数量判断
			bool bEnoughCard=true;
			
			for (BYTE i=0;i<cbLessKindItem*3;i++)
			{
				//存在判断
				BYTE cbCardIndex=pKindItem[i/3]->cbValidIndex[i%3]; 
				if (cbCardIndexTemp[cbCardIndex]==0)
				{
					if( m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[m_cbMagicIndex] > 0 )
					{
						cbCardIndexTemp[m_cbMagicIndex]--;
						pKindItem[i/3]->cbValidIndex[i%3] = m_cbMagicIndex;
					}
					else
					{
						bEnoughCard=false;
						break;
					}
				}
				else cbCardIndexTemp[cbCardIndex]--;
			}

			//胡牌判断
			if (bEnoughCard==true)
			{
				//牌眼判断
				BYTE cbCardEye=0;
				bool bMagicEye = false;
				if( GetCardCount(cbCardIndexTemp) == 2 )
				{
					for (BYTE i=0;i<MAX_INDEX-MAX_HUA_CARD;i++)
					{
						if (cbCardIndexTemp[i]==2)
						{
							cbCardEye=SwitchToCardData(i);
							if( m_cbMagicIndex != MAX_INDEX && i == m_cbMagicIndex ) bMagicEye = true;
							break;
						}
						else if( i!=m_cbMagicIndex && 
							m_cbMagicIndex != MAX_INDEX && cbCardIndexTemp[i]+cbCardIndexTemp[m_cbMagicIndex]==2 )
						{
							cbCardEye = SwitchToCardData(i);
							bMagicEye = true;
						}
					}
				}

				//组合类型
				if (cbCardEye!=0)
				{
					//变量定义
					tagAnalyseItem AnalyseItem;
					ZeroMemory(&AnalyseItem,sizeof(AnalyseItem));

					//设置组合
					for (BYTE i=0;i<cbWeaveCount;i++)
					{
						AnalyseItem.cbWeaveKind[i]=WeaveItem[i].cbWeaveKind;
						AnalyseItem.cbCenterCard[i]=WeaveItem[i].cbCenterCard;
						GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,AnalyseItem.cbCardData[i] );
					}

					//设置牌型
					for (BYTE i=0;i<cbLessKindItem;i++) 
					{
						AnalyseItem.cbWeaveKind[i+cbWeaveCount]=pKindItem[i]->cbWeaveKind;
						AnalyseItem.cbCenterCard[i+cbWeaveCount]=pKindItem[i]->cbCenterCard;
						AnalyseItem.cbCardData[cbWeaveCount+i][0] = SwitchToCardData(pKindItem[i]->cbValidIndex[0]);
						AnalyseItem.cbCardData[cbWeaveCount+i][1] = SwitchToCardData(pKindItem[i]->cbValidIndex[1]);
						AnalyseItem.cbCardData[cbWeaveCount+i][2] = SwitchToCardData(pKindItem[i]->cbValidIndex[2]);
					}

					//设置牌眼
					AnalyseItem.cbCardEye=cbCardEye;
					AnalyseItem.bMagicEye = bMagicEye;

					//插入结果
					AnalyseItemArray.Add(AnalyseItem);
				}
			}

			//设置索引
			if (cbIndex[cbLessKindItem-1]==(cbKindItemCount-1))
			{
				BYTE i=0;
				for ( i=cbLessKindItem-1;i>0;i--)
				{
					if ((cbIndex[i-1]+1)!=cbIndex[i])
					{
						BYTE cbNewIndex=cbIndex[i-1];
						for (BYTE j=(i-1);j<cbLessKindItem;j++) 
							cbIndex[j]=cbNewIndex+j-i+2;
						break;
					}
				}
				if (i==0)
					break;
			}
			else
				cbIndex[cbLessKindItem-1]++;
		} while (true);

	}

	return (AnalyseItemArray.GetCount()>0);
}

//钻牌
bool CGameLogic::IsMagicCard( BYTE cbCardData )
{
	if( m_cbMagicIndex != MAX_INDEX )
		return SwitchToCardIndex(cbCardData) == m_cbMagicIndex;
	return false;
}

//花牌判断
bool CGameLogic::IsHuaCard( BYTE cbCardData )
{
	ASSERT( IsValidCard(cbCardData) );

	return cbCardData>=0x38?true:false;
}

//排序,根据牌值排序
bool CGameLogic::SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount )
{
	//数目过虑
	if (cbCardCount==0||cbCardCount>MAX_COUNT) return false;

	//排序操作
	bool bSorted=true;
	BYTE cbSwitchData=0,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if (cbCardData[i]>cbCardData[i+1])
			{
				//设置标志
				bSorted=false;

				//扑克数据
				cbSwitchData=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbSwitchData;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return true;
}

/*
// 胡法分析函数
*/

//单钓将
bool CGameLogic::IsDanDiaoJiang( const tagAnalyseItem *pAnalyseItem, BYTE cbCurrentCard )
{
	if( pAnalyseItem->cbCardEye==cbCurrentCard || (pAnalyseItem->bMagicEye&&IsMagicCard(cbCurrentCard)) ) 
		return true;
	return false;
}

//坎张
bool CGameLogic::IsKanZhang( const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//已吃碰出的组合不算
	for( BYTE i = cbWeaveCount; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤刻子
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) )
			continue;

		//检查是否有当前牌
		for( BYTE j = 0; j < 3; j++ )
		{
			if( pAnalyseItem->cbCardData[i][j] == cbCurrentCard )
			{
				if( j == 1 ||
					j == 0 && (pAnalyseItem->cbCardData[i][j]&MASK_VALUE) == 7 ||
					j == 2 && (pAnalyseItem->cbCardData[i][j]&MASK_VALUE) == 3 )
				{
					return true;
				}
			}
		}
	}
	return false;
}

//边张
bool CGameLogic::IsBianZhang( const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//已吃碰出的组合不算
	for( BYTE i = cbWeaveCount; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤刻子
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) )
			continue;

		//检查是否有当前牌
		for( BYTE j = 0; j < 3; j++ )
		{
			if( pAnalyseItem->cbCardData[i][j] == cbCurrentCard )
			{
				if( j == 0 && (pAnalyseItem->cbCenterCard[i]&MASK_VALUE) < 7 ||
					j == 2 && (pAnalyseItem->cbCenterCard[i]&MASK_VALUE) > 1 )
				{
					return true;
				}
			}
		}
	}
	return false;
}

//无字
bool CGameLogic::IsWuZi( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard )
{
	//检查当前牌
	if( cbCurrentCard >= 0x31 ) return false;

	//检查手上牌
	for( BYTE i = MAX_INDEX-MAX_HUA_CARD-7; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		if( cbCardIndex[i] > 0 ) return false;

	//检查吃碰杠
	for( BYTE i = 0; i < cbWeaveCount; i++ )
		if( WeaveItem[i].cbCenterCard > 0x31 ) return false;

	return true;
}

//缺一门
bool CGameLogic::IsQueYiMen( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard )
{
	BYTE cbColorCount[4] = {0,0,0,0};

	//当前牌
	cbColorCount[cbCurrentCard>>4]++;

	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD-7; i++ )
	{
		if( cbCardIndex[i] > 0 )
		{
			cbColorCount[i/9]++;

			i = (i/9+1)*9-1;
		}
	}

	for( BYTE i = 0; i < cbWeaveCount; i++ )
		cbColorCount[WeaveItem[i].cbCenterCard>>4]++;

	for( BYTE i = 0; i < 3; i++ )
	{
		if( cbColorCount[i] == 0 ) return true;
	}

	return false;
}

//明杠
bool CGameLogic::IsMingGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind==WIK_GANG && WeaveItem[i].cbPublicCard==TRUE )
			return true;
	}
	return false;
}

//幺九刻
bool CGameLogic::IsYaoJiuKe( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i] & (WIK_GANG|WIK_PENG) )
		{
			BYTE cbCardValue = (pAnalyseItem->cbCenterCard[i]&MASK_VALUE);
			if( cbCardValue==1 || cbCardValue==9 || pAnalyseItem->cbCenterCard[i] >= 0x31 ) return true;
		}
	}
	return false;
}

//老少副
bool CGameLogic::IsLaoShaoFu( const tagAnalyseItem *pAnalyseItem )
{
	bool bFound[3][2];
	ZeroMemory( bFound,sizeof(bFound) );
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤刻子
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;
		BYTE cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
		//1
		if( cbCardValue==1 )
			bFound[pAnalyseItem->cbCenterCard[i]>>4][0] = true;
		//7
		else if( cbCardValue==7)
			bFound[pAnalyseItem->cbCenterCard[i]>>4][1] = true;
	}

	for( BYTE i = 0; i < 3; i++ )
	{
		if( bFound[i][0] && bFound[i][1] ) return true;
	}
	return false;
}

//连六
bool CGameLogic::IsLianLu( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤刻子
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

		BYTE cbCardColor = pAnalyseItem->cbCenterCard[i]>>4;
		int nCardValue = pAnalyseItem->cbCardData[i][0]&MASK_VALUE;

		for( BYTE j = i+1; j < CountArray(pAnalyseItem->cbWeaveKind); j++ )
		{
			//过滤刻子
			if( !(pAnalyseItem->cbWeaveKind[j]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

			//过滤不同花色
			if( (pAnalyseItem->cbCenterCard[j]>>4) != cbCardColor ) continue;

			int nValue = pAnalyseItem->cbCardData[j][0]&MASK_VALUE;

			//检查相连
			if( abs(nValue-nCardValue) == 3 ) return true;
		}
	}

	return false;
}

//喜相逢
bool CGameLogic::IsXiXiangFeng( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤刻子
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

		BYTE cbCardColor = pAnalyseItem->cbCenterCard[i]>>4;
		int nCardValue = pAnalyseItem->cbCardData[i][0]&MASK_VALUE;

		for( BYTE j = i+1; j < CountArray(pAnalyseItem->cbWeaveKind); j++ )
		{
			//过滤刻子
			if( !(pAnalyseItem->cbWeaveKind[j]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

			//过滤相同花色
			if( (pAnalyseItem->cbCenterCard[j]>>4) == cbCardColor ) continue;

			int nValue = pAnalyseItem->cbCardData[j][0]&MASK_VALUE;

			//检查相同顺序
			if( nValue == nCardValue ) return true;
		}
	}
	return false;
}

//一般高
bool CGameLogic::IsYiBanGao( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤刻子
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

		BYTE cbCardColor = pAnalyseItem->cbCenterCard[i]>>4;
		int nCardValue = pAnalyseItem->cbCardData[i][0]&MASK_VALUE;

		for( BYTE j = i+1; j < CountArray(pAnalyseItem->cbWeaveKind); j++ )
		{
			//过滤刻子
			if( !(pAnalyseItem->cbWeaveKind[j]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

			//过滤不同花色
			if( (pAnalyseItem->cbCenterCard[j]>>4) != cbCardColor ) continue;

			int nValue = pAnalyseItem->cbCardData[j][0]&MASK_VALUE;

			//检查相同顺序
			if( nValue == nCardValue ) return true;
		}
	}
	return false;
}

//断幺
bool CGameLogic::IsDuanYao( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard )
{
	//检查当前牌
	BYTE cbCardValue = cbCurrentCard&MASK_VALUE;
	if( cbCurrentCard >= 0x31 || cbCardValue == 1 || cbCardValue == 9 )
		return false;

	//检查手上牌
	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
	{
		if( cbCardIndex[i] > 0 )
			return false;
		if( i%9 == 0 ) i += 7;
	}

	//检查组合
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			for(BYTE j=0;j<3;j++)
			{
				BYTE cbTempCard=WeaveItem[i].cbCardData[j];
				if(cbTempCard>=0x31 || (cbTempCard&MASK_VALUE)==1 || (cbTempCard&MASK_VALUE)==9)
					return false;
			}
		}
		else
		{
			cbCardValue = WeaveItem[i].cbCenterCard&MASK_VALUE;
			if( WeaveItem[i].cbCenterCard >= 0x31 || cbCardValue == 1 || cbCardValue == 9 )
				return false;
		}
	}

	return true;
}

//暗杠
bool CGameLogic::IsAnGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind == WIK_GANG && !WeaveItem[i].cbPublicCard )
			return true;
	}
	return false;
}

//双暗刻
bool CGameLogic::IsShuangAnKe( const tagAnalyseItem *pAnalyseItem, const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	BYTE cbAnKeCount = 0;
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( (pAnalyseItem->cbWeaveKind[i] == WIK_PENG && i >= cbWeaveCount) ||
			(pAnalyseItem->cbWeaveKind[i] == WIK_GANG && !WeaveItem[i].cbPublicCard) )
		{
			if( ++cbAnKeCount == 2 ) return true;
		}
	}
	return false;
}

//双同刻
bool CGameLogic::IsShuangTongKe( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤顺子
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_PENG|WIK_GANG)) ) continue;

		//过滤字牌
		if( pAnalyseItem->cbCenterCard[i] >= 0x31 ) continue;

		BYTE cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
		for( BYTE j = i+1; j < CountArray(pAnalyseItem->cbWeaveKind); j++ )
		{
			//过滤顺子
			if( !(pAnalyseItem->cbWeaveKind[j]&(WIK_PENG|WIK_GANG)) ) continue;

			//过滤字牌
			if( pAnalyseItem->cbCenterCard[j] >= 0x31 ) continue;

			//检查序数相同
			if( (pAnalyseItem->cbCenterCard[j]&MASK_VALUE) == cbCardValue ) return true;
		}
	}
	return false;
}

//平胡
bool CGameLogic::IsPingHu( const tagAnalyseItem *pAnalyseItem )
{
	//检查牌眼
	if( pAnalyseItem->cbCardEye >= 0x31 && !IsMagicCard(pAnalyseItem->cbCardEye) ) return false;

	//检查组合
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_PENG|WIK_GANG) ) return false;
	}
	return true;
}

//门前清
bool CGameLogic::IsMenQianQing( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind != WIK_GANG || WeaveItem[i].cbPublicCard )
			return false;
	}
	return true;
}

//全求人
bool CGameLogic::IsQuanQiuRen( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	if( cbWeaveCount != 4 ) return false;

	for( BYTE i = 0;i < cbWeaveCount; i++ )
		if( WeaveItem[i].cbWeaveKind == WIK_GANG && !WeaveItem[i].cbPublicCard )
			return false;

	return true;
}

//箭刻
bool CGameLogic::IsJianKe( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_PENG|WIK_GANG) )
		{
			BYTE cbCardData = pAnalyseItem->cbCenterCard[i];
			if( cbCardData == 0x35 || cbCardData == 0x36 || cbCardData == 0x37 )
				return true;
		}
	}
	return false;
}

//双明杠
bool CGameLogic::IsShuangMingGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	BYTE cbMingGangCount = 0;
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind == WIK_GANG && !WeaveItem[i].cbPublicCard )
			if( ++cbMingGangCount == 2 ) return true;
	}
	return false;
}

//不求人
bool CGameLogic::IsBuQiuRen( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind != WIK_GANG || WeaveItem[i].cbPublicCard )
			return false;
	}
	return true;
}

//全带幺
bool CGameLogic::IsQuanDaiYao( const tagAnalyseItem *pAnalyseItem )
{
	//检查牌眼
	if( (pAnalyseItem->cbCardEye&MASK_VALUE)!=1 && pAnalyseItem->cbCardEye<0x31 &&
		!IsMagicCard(pAnalyseItem->cbCardEye) )
		return false;

	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			BYTE cbCardValue = pAnalyseItem->cbCardData[i][0]&MASK_VALUE;
			if( cbCardValue != 1 || cbCardValue != 7 ) return false;
		}
		else
		{
			BYTE cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
			if( cbCardValue != 1 || cbCardValue != 7 || pAnalyseItem->cbCenterCard[i] < 0x31 )
				return false;
		}
	}
	return true;
}

//双箭刻
bool CGameLogic::IsShuangJianKe( const tagAnalyseItem *pAnalyseItem )
{
	BYTE cbJianCount = 0;
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_PENG|WIK_GANG) )
		{
			BYTE cbCardData = pAnalyseItem->cbCenterCard[i];
			if( cbCardData == 0x35 || cbCardData == 0x36 || cbCardData == 0x37 )
				if( ++cbJianCount == 2 ) return true;
		}
	}
	return false;
}

//双暗杠
bool CGameLogic::IsShuangAnGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	BYTE cbAnGangCount = 0;
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind == WIK_GANG && !WeaveItem[i].cbPublicCard )
			if( ++cbAnGangCount == 2 )
				return true;
	}
	return false;
}

//五门齐
bool CGameLogic::IsWuMenQi( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard )
{
	BYTE cbColor[4] = {0,0,0,0};
	BYTE cbFengJianCount[2] = {0,0};
	bool bEyeCardMagic = false;

	//检查当前牌
	cbColor[cbCurrentCard>>4]++;
	if( cbCurrentCard >= 0x35 )
		cbFengJianCount[1]++;
	else if( cbCurrentCard >= 0x31 )
		cbFengJianCount[0]++;

	//检查组合
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		BYTE cbCardColor = WeaveItem[i].cbCenterCard>>4;
		cbColor[cbCardColor]++;
		if( WeaveItem[i].cbCenterCard >= 0x35 )
			cbFengJianCount[1]++;
		else if( WeaveItem[i].cbCenterCard >= 0x31 )
			cbFengJianCount[0]++;
	}

	//检查手上牌
	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
	{
		if( cbCardIndex[i] > 0 )
		{
			cbColor[i/9]++;

			if( i >= 31 )
				cbFengJianCount[1]++;
			else if( i >= 27 )
				cbFengJianCount[0]++;

			i = (i/9+1)*9-1;
		}
	}
	
	//检查
	if( cbFengJianCount[0] == 0 || cbFengJianCount[1] == 0 ) return false;
	for( BYTE i = 0; i < CountArray(cbColor)-1; i++ )
		if( cbColor[i] == 0 ) return false;

	return true;
}

//三色三步高
bool CGameLogic::IsSanSeBu( const tagAnalyseItem *pAnalyseItem )
{
	BYTE cbValue[2][3];
	ZeroMemory( cbValue,sizeof(cbValue) );
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤碰杠牌
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

		BYTE cbCardData = pAnalyseItem->cbCenterCard[i];
		if( pAnalyseItem->cbWeaveKind[i]&WIK_CENTER ) cbCardData--;
		else if( pAnalyseItem->cbWeaveKind[i]&WIK_RIGHT ) cbCardData -= 2;

		//放入牌值数组
		BYTE cbColor = cbCardData>>4;
		if( cbValue[0][cbColor] == 0 ) cbValue[0][cbColor] = cbCardData&MASK_VALUE;
		else if( cbValue[1][cbColor] == 0 ) cbValue[1][cbColor] = cbCardData&MASK_VALUE;
		//超过两个同花色顺子,返回false
		else return false;
	}

	//检查
	if( cbValue[0][0] == 0 || cbValue[0][1] == 0 || cbValue[0][2] == 0 ) return false;
	else
	{
		//检查第二组顺子
		if( cbValue[1][0] != 0 || cbValue[1][1] != 0 || cbValue[1][2] != 0 )
		{
			if( cbValue[1][0] == 0 ) cbValue[1][0] = cbValue[0][0];
			if( cbValue[1][1] == 0 ) cbValue[1][1] = cbValue[0][1];
			if( cbValue[1][2] == 0 ) cbValue[1][2] = cbValue[0][2];
		}
	}

	//排序并检查是否间隔1
	for( BYTE i = 0; i < 2; i++ )
	{
		if( cbValue[i][0] != 0 )
		{
			SortCardList( cbValue[i],3 );
			if( cbValue[i][0]+1 == cbValue[i][1] && cbValue[i][0]+2 == cbValue[i][2] ) 
				return true;
		}
	}
	return false;
}

//混一色
bool CGameLogic::IsHunYiSe( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard )
{
	bool bHasZiPai = false;
	BYTE cbCardColor = 0xFF;

	//检查当前牌
	if( cbCurrentCard >= 0x31 ) bHasZiPai = true;
	else cbCardColor = cbCurrentCard>>4;

	//检查手上牌
	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
	{
		if( cbCardIndex[i] > 0 )
		{
			if( i >= 27 ) 
			{
				bHasZiPai = true;
				break;
			}
			if( cbCardColor == 0xFF )
			{
				cbCardColor = i/9;
				
				i = (i/9+1)*9-1;
				continue;
			}
			if( cbCardColor != i/9 ) return false;
		}
	}

	//检查吃碰杠
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbCenterCard >= 0x31 ) bHasZiPai = true;
		else 
		{
			if( cbCardColor == 0xFF ) cbCardColor = WeaveItem[i].cbCenterCard>>4;
			else if( cbCardColor != (WeaveItem[i].cbCenterCard>>4) ) return false;
		}
	}

	return (bHasZiPai&&cbCardColor!=0xFF);
}

//碰碰和
bool CGameLogic::IsPengPeng( const tagAnalyseItem *pAnalyseItem )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
			return false;
	}
	return true;
}

//三色三节高
bool CGameLogic::IsSanSeJie( const tagAnalyseItem *pAnalyseItem )
{
	BYTE cbValue[2][3];
	ZeroMemory( cbValue,sizeof(cbValue) );
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤顺牌
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_PENG|WIK_GANG)) ) continue;

		BYTE cbCardData = pAnalyseItem->cbCenterCard[i];

		//放入牌值数组
		BYTE cbColor = cbCardData>>4;
		if( cbValue[0][cbColor] == 0 ) cbValue[0][cbColor] = cbCardData&MASK_VALUE;
		else if( cbValue[1][cbColor] == 0 ) cbValue[1][cbColor] = cbCardData&MASK_VALUE;
		//超过两个同花色刻子,返回false
		else return false;
	}

	//检查
	if( cbValue[0][0] == 0 || cbValue[0][1] == 0 || cbValue[0][2] == 0 ) return false;
	else
	{
		//检查第二组刻子
		if( cbValue[1][0] != 0 || cbValue[1][1] != 0 || cbValue[1][2] != 0 )
		{
			if( cbValue[1][0] == 0 ) cbValue[1][0] = cbValue[0][0];
			if( cbValue[1][1] == 0 ) cbValue[1][1] = cbValue[0][1];
			if( cbValue[1][2] == 0 ) cbValue[1][2] = cbValue[0][2];
		}
	}

	//排序并检查是否间隔1
	for( BYTE i = 0; i < 2; i++ )
	{
		if( cbValue[i][0] != 0 )
		{
			SortCardList( cbValue[i],3 );
			if( cbValue[i][0]+1 == cbValue[i][1] && cbValue[i][0]+2 == cbValue[i][2] ) 
				return true;
		}
	}
	return false;
}

//三色三同顺
bool CGameLogic::IsSanSeTong( const tagAnalyseItem *pAnalyseItem )
{
	BYTE cbValue[2][3];
	ZeroMemory( cbValue,sizeof(cbValue) );
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤碰杠牌
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

		BYTE cbCardData = pAnalyseItem->cbCenterCard[i];
		if( pAnalyseItem->cbWeaveKind[i]&WIK_CENTER ) cbCardData--;
		else if( pAnalyseItem->cbWeaveKind[i]&WIK_RIGHT ) cbCardData -= 2;

		//放入牌值数组
		BYTE cbColor = cbCardData>>4;
		if( cbValue[0][cbColor] == 0 ) cbValue[0][cbColor] = cbCardData&MASK_VALUE;
		else if( cbValue[1][cbColor] == 0 ) cbValue[1][cbColor] = cbCardData&MASK_VALUE;
		//超过两个同花色顺子,返回false
		else return false;
	}

	//检查
	if( cbValue[0][0] == 0 || cbValue[0][1] == 0 || cbValue[0][2] == 0 ) return false;
	else
	{
		//检查第二组顺子
		if( cbValue[1][0] != 0 || cbValue[1][1] != 0 || cbValue[1][2] != 0 )
		{
			if( cbValue[1][0] == 0 ) cbValue[1][0] = cbValue[0][0];
			if( cbValue[1][1] == 0 ) cbValue[1][1] = cbValue[0][1];
			if( cbValue[1][2] == 0 ) cbValue[1][2] = cbValue[0][2];
		}
	}

	//排序并检查是否序数相同
	for( BYTE i = 0; i < 2; i++ )
	{
		if( cbValue[i][0] != 0 )
		{
			if( cbValue[i][0] == cbValue[i][1] && cbValue[i][0] == cbValue[i][2] ) 
				return true;
		}
	}
	return false;
}

//花龙
bool CGameLogic::IsHuaLong( const tagAnalyseItem *pAnalyseItem )
{
	BYTE cbValue[2][3];
	ZeroMemory( cbValue,sizeof(cbValue) );
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//过滤碰杠牌
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) continue;

		BYTE cbCardData = pAnalyseItem->cbCenterCard[i];
		if( pAnalyseItem->cbWeaveKind[i]&WIK_CENTER ) cbCardData--;
		else if( pAnalyseItem->cbWeaveKind[i]&WIK_RIGHT ) cbCardData -= 2;

		//放入牌值数组
		BYTE cbColor = cbCardData>>4;
		if( cbValue[0][cbColor] == 0 ) cbValue[0][cbColor] = cbCardData&MASK_VALUE;
		else if( cbValue[1][cbColor] == 0 ) cbValue[1][cbColor] = cbCardData&MASK_VALUE;
		//超过两个同花色顺子,返回false
		else return false;
	}

	//检查
	if( cbValue[0][0] == 0 || cbValue[0][1] == 0 || cbValue[0][2] == 0 ) return false;
	else
	{
		//检查第二组顺子
		if( cbValue[1][0] != 0 || cbValue[1][1] != 0 || cbValue[1][2] != 0 )
		{
			if( cbValue[1][0] == 0 ) cbValue[1][0] = cbValue[0][0];
			if( cbValue[1][1] == 0 ) cbValue[1][1] = cbValue[0][1];
			if( cbValue[1][2] == 0 ) cbValue[1][2] = cbValue[0][2];
		}
	}

	//排序并检查是否序数相同
	for( BYTE i = 0; i < 2; i++ )
	{
		if( cbValue[i][0] != 0 )
		{
			SortCardList( cbValue[i],3 );
			if( cbValue[i][0]+3 == cbValue[i][1] && cbValue[i][1]+3 == cbValue[i][2] ) 
				return true;
		}
	}
	return false;
}

//三风刻
bool CGameLogic::IsSanFengKe( const tagAnalyseItem *pAnalyseItem )
{
	BYTE cbFengCount = 0;
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( pAnalyseItem->cbWeaveKind[i]&(WIK_PENG|WIK_GANG) )
		{
			if( pAnalyseItem->cbCenterCard[i] >= 0x31 && 
				pAnalyseItem->cbCenterCard[i] <= 0x34 )
				if( ++cbFengCount == 3 ) return true;
		}
	}
	return false;
}

//小于五
bool CGameLogic::IsXiaoWu( const tagAnalyseItem *pAnalyseItem )
{
	//检查眼牌
	if( (pAnalyseItem->cbCardEye >= 0x31 || (pAnalyseItem->cbCardEye&MASK_VALUE) >= 5) &&
		!IsMagicCard(pAnalyseItem->cbCardEye) ) 
		return false;

	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//检查字牌
		if( pAnalyseItem->cbCenterCard[i] >= 0x31 ) return false;

		//检查序数牌
		BYTE cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
		if( pAnalyseItem->cbWeaveKind[i] & WIK_LEFT ) cbCardValue += 2;
		else if( pAnalyseItem->cbWeaveKind[i] & WIK_CENTER ) cbCardValue++;

		if( cbCardValue >= 5 ) return false;
	}
	return true;
}

//大于五
bool CGameLogic::IsDaWu( const tagAnalyseItem *pAnalyseItem )
{
	//检查眼牌
	if( (pAnalyseItem->cbCardEye >= 0x31 || (pAnalyseItem->cbCardEye&MASK_VALUE) <= 5) &&
		!IsMagicCard(pAnalyseItem->cbCardEye) ) 
		return false;

	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		//检查字牌
		if( pAnalyseItem->cbCenterCard[i] >= 0x31 ) return false;

		//检查序数牌
		BYTE cbCardValue = pAnalyseItem->cbCenterCard[i]&MASK_VALUE;
		if( pAnalyseItem->cbWeaveKind[i] & WIK_CENTER ) cbCardValue--;
		else if( pAnalyseItem->cbWeaveKind[i] & WIK_RIGHT ) cbCardValue -= 2;

		if( cbCardValue <= 5 ) return false;
	}
	return true;
}

//四暗刻
bool CGameLogic::IsSiAnKe( const tagAnalyseItem *pAnalyseItem, const tagWeaveItem WeaveItem[], BYTE cbWeaveCount )
{
	for( BYTE i = 0; i < CountArray(pAnalyseItem->cbWeaveKind); i++ )
	{
		if( !(pAnalyseItem->cbWeaveKind[i]&(WIK_PENG|WIK_GANG)) ) return false;

		if( i < cbWeaveCount && 
			( (pAnalyseItem->cbWeaveKind[i]&WIK_PENG) || WeaveItem[i].cbPublicCard==TRUE )
			)
			return false;
	}
	return true;
}

//一色三步高
bool CGameLogic::IsYiSeBu(const tagAnalyseItem & AnalyseItem)
{
	//组合首牌
	BYTE cbCardValue[3][4];
	BYTE cbCardCount[3];
	ZeroMemory( cbCardValue,sizeof(cbCardValue) );
	ZeroMemory( cbCardCount,sizeof(cbCardCount) );

	//扑克判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if (AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
		{
			BYTE cbCardData = AnalyseItem.cbCardData[i][0];
			BYTE cbCardColor = cbCardData>>4;
			cbCardValue[cbCardColor][cbCardCount[cbCardColor]++] = cbCardData&MASK_VALUE;
		}
	}

	//花色排序
	for( BYTE i = 0; i < CountArray(cbCardCount); i++ )
		SortCardList( cbCardValue[i],cbCardCount[i] );

	//检测
	for( BYTE i = 0; i < CountArray(cbCardCount); i++ )
	{
		if( cbCardCount[i] < 3 ) continue;

		if( cbCardCount[i] == 3 )
		{
			if( cbCardValue[i][1] - cbCardValue[i][0] == cbCardValue[i][2] - cbCardValue[i][1] &&
				cbCardValue[i][1] - cbCardValue[i][0] > 0 && 
				cbCardValue[i][1] - cbCardValue[i][0] <=2 )
				return true;
		}
		//有4个的情况
		else
		{
			for( BYTE cbFirst = 0; cbFirst < 2; cbFirst++ )
			{
				for( BYTE cbSecond = cbFirst+1; cbSecond < 3; cbSecond++ )
				{
					BYTE cbDistant = cbCardValue[i][cbSecond]-cbCardValue[i][cbFirst];
					if( cbDistant == 0 || cbDistant > 2 ) continue;
					for( BYTE cbThird = cbSecond+1; cbThird < 4; cbThird++ )
					{
						if( cbDistant == cbCardValue[i][cbThird]-cbCardValue[i][cbSecond] )
							return true;
					}
				}
			}
		}
	}

	return false;
}

//全带5
bool CGameLogic::IsQuanDaiWu(const tagAnalyseItem & AnalyseItem)
{
	//5做将
	BYTE bEyeCard=AnalyseItem.cbCardEye;
	if( (bEyeCard>=0x31 || (bEyeCard&MASK_VALUE)!=5) && !IsMagicCard(bEyeCard) ) return false;

	//扑克判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//提取扑克
		BYTE cbCardData[4];
		BYTE cbCardCount = GetWeaveCard(AnalyseItem.cbWeaveKind[i],AnalyseItem.cbCenterCard[i],cbCardData);

		//风牌过滤
		BYTE cbCardColor=cbCardData[0]&MASK_COLOR;
		if(cbCardColor == 0x30) return false;

		//判断该组合有无5
		bool bHave5=false;
		for(BYTE j=0;j<cbCardCount;j++)
		{
			if( (cbCardData[j]&MASK_VALUE) == 5) 
			{
				bHave5=true;
				break;
			}
		}
		if(bHave5==false) return false;
	}

	return true;
}

//三同刻
bool CGameLogic::IsSanTongKe(const tagAnalyseItem & AnalyseItem)
{
	//变量定义
	BYTE bPengGangCount=0;  //碰杠个数
	BYTE bCenterCard[4];	//碰杠牌
	ZeroMemory(bCenterCard,sizeof(bCenterCard));

	//扑克判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if (AnalyseItem.cbWeaveKind[i]&(WIK_PENG|WIK_GANG))
		{
			//过滤风牌
			if((AnalyseItem.cbCenterCard[i]&MASK_COLOR) == 0x30) continue;

			//提取扑克
			bCenterCard[bPengGangCount]=AnalyseItem.cbCenterCard[i];
			bPengGangCount++;
		}
	}

	//小于3个刻返回
	if(bPengGangCount<3) return false;

	//确定序数  先将第一个作为标准，若有2个不同则拿第2个做标准
	BYTE bValue=bCenterCard[0]&MASK_VALUE;
	if( (bCenterCard[0]&MASK_VALUE)!=(bCenterCard[1]&MASK_VALUE)&&(bCenterCard[0]&MASK_VALUE)!=(bCenterCard[2]&MASK_VALUE))	bValue=bCenterCard[1]&MASK_VALUE;

	//同刻个数
	BYTE bTongKeCount=0;
	for(BYTE i=0;i<bPengGangCount;i++)
	{
		if((bCenterCard[i]&MASK_VALUE)==bValue) bTongKeCount++;
	}

	if(bTongKeCount>=3) return true;
	return false;
}

//三暗刻
bool CGameLogic::IsSanAnKe(const tagAnalyseItem & AnalyseItem,const tagWeaveItem WeaveItem[], BYTE cbWeaveCount)
{
	BYTE bKeCount=0;
	//扑克判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if (AnalyseItem.cbWeaveKind[i]&(WIK_PENG|WIK_GANG))
		{
			bKeCount++;
		}
	}

	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if(WeaveItem[i].cbWeaveKind&(WIK_PENG)) bKeCount--;
		if((WeaveItem[i].cbWeaveKind&WIK_GANG) && WeaveItem[i].cbPublicCard==TRUE) bKeCount--;
	}

	if(bKeCount>=3) return true;
	return false;
}

//大四喜
bool CGameLogic::IsDaSiXi(const tagAnalyseItem & AnalyseItem)
{
	bool bExist[4]={false,false,false,false};
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//东南西北
		if (AnalyseItem.cbCenterCard[i]==0x31) bExist[0]=true;
		else if (AnalyseItem.cbCenterCard[i]==0x32) bExist[1]=true;
		else if (AnalyseItem.cbCenterCard[i]==0x33) bExist[2]=true;
		else if (AnalyseItem.cbCenterCard[i]==0x34) bExist[3]=true;
	}

	if ((bExist[0]==true)&&(bExist[1]==true)&&(bExist[2]==true)&&(bExist[3]==true)) return true;

	return false;
}

//大三元牌
bool CGameLogic::IsDaSanYuan(const tagAnalyseItem & AnalyseItem)
{
	bool bExist[3]={false,false,false};
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//中发白
		if (AnalyseItem.cbCenterCard[i]==0x35) bExist[0]=true;
		else if (AnalyseItem.cbCenterCard[i]==0x36) bExist[1]=true;
		else if (AnalyseItem.cbCenterCard[i]==0x37) bExist[2]=true;
	}

	if ((bExist[0]==true)&&(bExist[1]==true)&&(bExist[2]==true)) return true;

	return false;
}

//四杠
bool CGameLogic::IsSiGang(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount)
{
	//数目判断
	if(cbItemCount !=4 ) return false;

	//类型判断
	for (INT i=0;i<cbItemCount;i++)
	{
		if(WeaveItem[i].cbWeaveKind !=WIK_GANG) return false;
	}
	return true;
}

//清幺九
bool CGameLogic::IsQingYaoJiu(const tagAnalyseItem &AnalyseItem)
{
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//类型判断
		if(AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) return false;

		//数据判断
		if((AnalyseItem.cbCenterCard[i]&MASK_VALUE)!=0x01 &&(AnalyseItem.cbCenterCard[i]&MASK_VALUE)!=0x09) return false;
	}

	//眼牌判断
	if( !IsMagicCard(AnalyseItem.cbCardEye) && (AnalyseItem.cbCardEye&MASK_VALUE)!=0x01 &&
		(AnalyseItem.cbCardEye&MASK_VALUE)!=0x09 )
		return false;

	return true;
}

//小四喜
bool CGameLogic::IsXiaoSiXi(const tagAnalyseItem & AnalyseItem)
{
	BYTE cbCount = 0,cbEye = 0;

	//数目判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if ((AnalyseItem.cbCenterCard[i]>=0x31)&&(AnalyseItem.cbCenterCard[i]<=0x34)) 
			cbCount++;
	}

	//数目判断
	if(cbCount!=3) return false;

	//眼牌判断
	if( !IsMagicCard(AnalyseItem.cbCardEye) && 
		( AnalyseItem.cbCardEye < 0x31 || AnalyseItem.cbCardEye > 0x34 )
		)
		return false;

	return true;
}

// 小三元
bool CGameLogic::IsXiaoShanYuan(const tagAnalyseItem & AnalyseItem)
{
	//数目变量
	BYTE cbFengCount=0;

	//组合判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//中发白
		if ( AnalyseItem.cbCenterCard[i]>=0x35 ) 
			cbFengCount++;
	}

	//数目验证
	if(cbFengCount != 2) return false;

	//眼牌判断
	if( !IsMagicCard(AnalyseItem.cbCardEye) && AnalyseItem.cbCardEye < 0x35 )
		return false;

	//数目判断
	return true;
}

//字一色牌
bool CGameLogic::IsZiYiSe(const tagAnalyseItem & AnalyseItem)
{
	//检查眼牌
	if( !IsMagicCard(AnalyseItem.cbCardEye) && AnalyseItem.cbCardEye < 0x31 ) return false;

	//扑克判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if ((AnalyseItem.cbCenterCard[i]&MASK_COLOR)!=0x30)
			return false;
	}

	return true;
}

// 一色双龙会
bool CGameLogic::IsYiSeHuangHuiLong(const tagAnalyseItem & AnalyseItem)
{
	BYTE cbColor=(AnalyseItem.cbCenterCard[0]&MASK_COLOR);

	//眼牌判断
	if( !IsMagicCard(AnalyseItem.cbCardEye) && 
		( (AnalyseItem.cbCardEye&MASK_VALUE)!=5 || AnalyseItem.cbCardEye == 0x35 || (AnalyseItem.cbCardEye&MASK_COLOR)!=cbColor )
		)
		return false;

	BYTE cbCardData[]={0,0,0,0};

	//扑克判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if ((AnalyseItem.cbCenterCard[i]&MASK_COLOR) !=cbColor) return false;
		if ((AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_RIGHT|WIK_CENTER))==0) return false;
		cbCardData[i] = AnalyseItem.cbCardData[i][0];		
		if(((cbCardData[i]&MASK_VALUE)!=0x01)&&(((cbCardData[i]&MASK_VALUE)!=0x07))) return false;
	}

	//排列扑克
	SortCardList(cbCardData,4);

	//幺判断
	if(cbCardData[0] !=cbCardData[1]) return false;
	if((cbCardData[0]&MASK_VALUE)!=0x01) return false;

	//九判断
	if(cbCardData[2] !=cbCardData[3]) return false;
	if((cbCardData[2]&MASK_VALUE)!=0x07) return false;

	return true;
}

//一色四同顺
bool CGameLogic::IsYiSeSiTongShun(const tagAnalyseItem &AnalyseItem)
{
	if( !(AnalyseItem.cbWeaveKind[0]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) return false;

	BYTE cbCardData=AnalyseItem.cbCenterCard[0];
	if( AnalyseItem.cbWeaveKind[0]&WIK_CENTER ) cbCardData--;
	else if( AnalyseItem.cbWeaveKind[0]&WIK_RIGHT ) cbCardData-=2;

	//扑克判断
	for (BYTE i=1;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if( !(AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) return false;

		BYTE cbCard=AnalyseItem.cbCenterCard[i];
		if( AnalyseItem.cbWeaveKind[0]&WIK_CENTER ) cbCard--;
		else if( AnalyseItem.cbWeaveKind[0]&WIK_RIGHT ) cbCard-=2;

		if( cbCard != cbCardData ) return false;
	}

	return true;
}

//一色四节高
bool CGameLogic::IsYiSeSiJieGao(const tagAnalyseItem &AnalyseItem)
{
	if( !(AnalyseItem.cbWeaveKind[0]&(WIK_PENG|WIK_GANG)) ) return false;

	if( AnalyseItem.cbCenterCard[0] >= 0x31 ) return false;

	BYTE cbCardData[]={AnalyseItem.cbCenterCard[0],0,0,0};
	BYTE cbColor=(AnalyseItem.cbCenterCard[0]&MASK_COLOR);

	//扑克判断
	for (BYTE i=1;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		cbCardData[i]=AnalyseItem.cbCenterCard[i];
		if ((AnalyseItem.cbCenterCard[i]&MASK_COLOR) !=cbColor) return false;
		if ((AnalyseItem.cbWeaveKind[i]&(WIK_PENG|WIK_GANG))==0) return false;
	}   

	//排列扑克
	SortCardList(cbCardData,4);

	//递增判断
	for(BYTE i=1;i<4;i++)
	{
		if((cbCardData[0]+i)!=cbCardData[i]) return false;
	}

	return true;
}

//一色四步高
bool CGameLogic::IsYiSeSiBuGao(const tagAnalyseItem &AnalyseItem)
{
	if( !(AnalyseItem.cbWeaveKind[0]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) ) return false;

	BYTE cbCardData[]={AnalyseItem.cbCenterCard[0],0,0,0};
	BYTE cbColor=(AnalyseItem.cbCenterCard[0]&MASK_COLOR);

	//扑克判断
	for (BYTE i=1;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		cbCardData[i]=AnalyseItem.cbCardData[i][0];
		if ((AnalyseItem.cbCenterCard[i]&MASK_COLOR) !=cbColor) return false;
		if ((AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_RIGHT|WIK_CENTER))==0) return false;
	}   

	//排列扑克
	SortCardList(cbCardData,4);

	BYTE bHas1=1,bHas2=1;

	//递增判断
	for(BYTE i=1;i<4;i++)
	{
		if((cbCardData[0]+i)!=cbCardData[i]) bHas1=0;
	}

	//递增判断
	for(BYTE i=1;i<4;i++)
	{
		if((cbCardData[0]+i*2)!=cbCardData[i]) bHas2=0;
	}

	return (bHas1+bHas2==1);
}

//三杠
bool CGameLogic::IsSanGang(const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount)
{
	BYTE cbGangCount=0;

	//数目统计
	for (BYTE i=0;i<cbWeaveCount;i++)
	{
		if(WeaveItem[i].cbWeaveKind&WIK_GANG) cbGangCount++;
	}

	return (cbGangCount==3);
}

//混幺九
bool CGameLogic::IsHunYaoJiu(const tagAnalyseItem &AnalyseItem)
{
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//类型判断
		if(AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT)) return false;
		//数据判断
		if((AnalyseItem.cbCenterCard[i]&MASK_VALUE)!=0x01 &&(AnalyseItem.cbCenterCard[i]&MASK_VALUE)!=0x09 &&(AnalyseItem.cbCenterCard[i]&MASK_COLOR)!=0x30) return false;
	}

	//眼牌判断
	if( !IsMagicCard(AnalyseItem.cbCardEye) && 
		( AnalyseItem.cbCardEye<0x31 && (AnalyseItem.cbCardEye&MASK_VALUE)!=0x01 && (AnalyseItem.cbCardEye&MASK_VALUE)!=0x09 )
		)
		return false;

	return true;
}

//清一色牌
bool CGameLogic::IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard)
{
	//胡牌判断
	BYTE cbCardColor=0xFF;

	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		if(i==m_cbMagicIndex) continue;
		if (cbCardIndex[i]!=0)
		{
			//花色判断
			if (cbCardColor!=0xFF)
				return false;

			//设置花色
			cbCardColor=(SwitchToCardData(i)&MASK_COLOR);

			//设置索引
			i=(i/9+1)*9-1;
		}
	}

	//如果手上只有王霸
	if( cbCardColor == 0xFF )
	{
		ASSERT( m_cbMagicIndex != MAX_INDEX && cbCardIndex[m_cbMagicIndex] > 0 );
		//检查组合
		ASSERT( cbItemCount > 0 );
		cbCardColor = WeaveItem[0].cbCenterCard&MASK_COLOR;
	}

	if((cbCurrentCard&MASK_COLOR)!=cbCardColor && !IsMagicCard(cbCurrentCard) ) return false;

	//组合判断
	for (BYTE i=0;i<cbItemCount;i++)
	{
		BYTE cbCenterCard=WeaveItem[i].cbCenterCard;
		if ((cbCenterCard&MASK_COLOR)!=cbCardColor)	return false;
	}

	return true;
}

//七小对牌
bool CGameLogic::IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard)
{
	//组合判断
	if (cbWeaveCount!=0) return false;

	//单牌数目
	BYTE cbReplaceCount = 0;

	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入数据
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//计算单牌
	for (BYTE i=0;i<MAX_INDEX;i++)
	{
		BYTE cbCardCount=cbCardIndexTemp[i];

		//王牌过滤
		if( i == m_cbMagicIndex ) continue;

		//单牌统计
		if( cbCardCount == 1 || cbCardCount == 3 ) 	cbReplaceCount++;
	}

	//王牌不够
	if( m_cbMagicIndex != MAX_INDEX && cbReplaceCount > cbCardIndexTemp[m_cbMagicIndex] ||
		m_cbMagicIndex == MAX_INDEX && cbReplaceCount > 0 )
		return false;

	return true;

}

//全双
bool CGameLogic::IsQuanShuang(const tagAnalyseItem &AnalyseItem)
{
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//类型判断
		if( !(AnalyseItem.cbWeaveKind[i]&(WIK_PENG|WIK_GANG)) ) return false;

		//数据判断
		if((AnalyseItem.cbCenterCard[i]&MASK_VALUE)%2||(AnalyseItem.cbCenterCard[i]&MASK_COLOR)==0x30) return false;
	}

	//眼牌判断
	if( !IsMagicCard(AnalyseItem.cbCardEye) &&
		( (AnalyseItem.cbCardEye&MASK_VALUE)%2 || AnalyseItem.cbCardEye >= 0x31 )
		)
		return false;

	return true;
}

//一色三同顺
bool CGameLogic::IsYiSeSanTongShun(const tagAnalyseItem &AnalyseItem)
{
	//刻子过滤
	if((AnalyseItem.cbWeaveKind[0]&(WIK_PENG|WIK_GANG))&&(AnalyseItem.cbWeaveKind[1]&(WIK_PENG|WIK_GANG))) return false;


	BYTE cbCardData[4]={0,0,0,0};
	BYTE cbCardCount=0;
	BYTE i=0;
	for ( i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//类型判断
		if(AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
		{
			cbCardData[cbCardCount++]=AnalyseItem.cbCardData[i][0];		
		}
	}
	if(cbCardCount<3) return false;

	//排列扑克
	SortCardList(cbCardData,cbCardCount);

	i=0;
	if( cbCardData[0] != cbCardData[2] && cbCardData[1] == cbCardData[2] )
		i = 1;

	//相同判断
	BYTE cbCount = 1;
	for( BYTE j = i+1; j < 4; j++ )
	{
		if(cbCardData[i]==cbCardData[j])
			cbCount++;
	}

	return cbCount==3;
}

//一色三节高
bool CGameLogic::IsYiSeSanJieGao(const tagAnalyseItem &AnalyseItem)
{
	//顺子过滤
	if(((AnalyseItem.cbWeaveKind[0]&(WIK_PENG|WIK_GANG))==0)&&((AnalyseItem.cbWeaveKind[1]&(WIK_PENG|WIK_GANG))==0)) return false;

	BYTE cbCardData[4]={0,0,0,0};
	BYTE cbCardCount=0;

	BYTE i=0;
	for ( i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		//类型判断
		if(AnalyseItem.cbWeaveKind[i]&(WIK_GANG|WIK_PENG))
		{
			if( AnalyseItem.cbCenterCard[i] < 0x31 )
				cbCardData[cbCardCount++]=AnalyseItem.cbCenterCard[i];
		}
	}
	if(cbCardCount<3) return false;

	//排列扑克
	SortCardList(cbCardData,cbCardCount);

	i=0;
	if( cbCardData[0] != cbCardData[1]-1 && cbCardData[1] == cbCardData[2]-1 )
		i = 1;

	//递增判断
	BYTE cbLianCount = 1;
	for( BYTE j = i+1; j < 4; j++ )
	{
		if((cbCardData[i]+j-i)==cbCardData[j])
			cbLianCount++;
	}

	return cbLianCount==3;
}

//全大
bool CGameLogic::IsQuanDa(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard)
{
	//检查当前牌
	if( cbCurrentCard >= 0x31 || (cbCurrentCard&MASK_VALUE)<0x07 ) return false;

	//检查手上牌
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = SwitchToCardData( cbCardIndex,cbCardData );
	for( BYTE i = 0; i < cbCardCount; i++ )
	{
		if( cbCardData[i] >= 0x31 || (cbCardData[i]&MASK_VALUE)<0x07 ) return false;
	}

	//检查组合
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,cbCardData );
			if( cbCardData[0] >= 0x31 || (cbCardData[0]&MASK_VALUE)<0x07 ) return false;
		}
		else if( WeaveItem[i].cbCenterCard >= 0x31 || (WeaveItem[i].cbCenterCard&MASK_VALUE)<0x07 )
			return false;
	}

	return true;
}

//全中
bool CGameLogic::IsQuanZhong(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard)
{
	//检查当前牌
	if( cbCurrentCard >= 0x31 || (cbCurrentCard&MASK_VALUE)>0x06 || (cbCurrentCard&MASK_VALUE)<0x04 ) 
		return false;

	//检查手上牌
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = SwitchToCardData( cbCardIndex,cbCardData );
	for( BYTE i = 0; i < cbCardCount; i++ )
	{
		if( cbCardData[i] >= 0x31 || (cbCardData[i]&MASK_VALUE)>0x06 || (cbCardData[i]&MASK_VALUE)<0x04) 
			return false;
	}

	//检查组合
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,cbCardData );
			if( cbCardData[0] >= 0x31 || (cbCardData[0]&MASK_VALUE)>0x06 || (cbCardData[0]&MASK_VALUE)<0x04) 
				return false;
		}
		else if( WeaveItem[i].cbCenterCard >= 0x31 || (WeaveItem[i].cbCenterCard&MASK_VALUE)>0x06 ||
			(WeaveItem[i].cbCenterCard&MASK_VALUE)<0x04 )
			return false;
	}

	return true;
}

//全小
bool CGameLogic::IsQuanXiao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard)
{
	//检查当前牌
	if( cbCurrentCard >= 0x31 || (cbCurrentCard&MASK_VALUE)>0x03 ) return false;

	//检查手上牌
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = SwitchToCardData( cbCardIndex,cbCardData );
	for( BYTE i = 0; i < cbCardCount; i++ )
	{
		if( cbCardData[i] >= 0x31 || (cbCardData[i]&MASK_VALUE)>0x03 ) return false;
	}

	//检查组合
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,cbCardData );
			if( cbCardData[0] >= 0x31 || (cbCardData[0]&MASK_VALUE)>0x03 ) return false;
		}
		else if( WeaveItem[i].cbCenterCard >= 0x31 || (WeaveItem[i].cbCenterCard&MASK_VALUE)>0x03 )
			return false;
	}

	return true;
}

//推不倒
bool CGameLogic::IsTuiBuDao( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//复制数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//提取吃碰杠
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		BYTE cbCardBuffer[4];
		BYTE cbCardCount = GetWeaveCard( WeaveItem[i].cbWeaveKind,WeaveItem[i].cbCenterCard,cbCardBuffer );
		for( BYTE j = 0; j < cbCardCount; j++ )
			cbCardIndexTemp[SwitchToCardIndex(cbCardBuffer[j])]++;
	}

	//检查1-9万
	for( BYTE i = 0; i < 9; i++ )
		if( cbCardIndexTemp[i] > 0 ) return false;

	//检查字牌
	for( BYTE i = 27; i < MAX_INDEX-MAX_HUA_CARD; i++ )
		if( cbCardIndexTemp[i] > 0 && i != 33 ) return false;

	//1,3,7条
	if( cbCardIndexTemp[9] > 0 || cbCardIndexTemp[11] > 0 || cbCardIndexTemp[15] > 0 )
		return false;

	//6,7筒
	if( cbCardIndexTemp[23] > 0 || cbCardIndexTemp[24] )
		return false;

	return true;
}

//组合龙
bool CGameLogic::IsZuHeLong( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	if( cbWeaveCount > 1 ) return false;

	//各种花色组合数
	bool bZuHe[3][3];
	BYTE cbZuHeCount[3];
	ZeroMemory( cbZuHeCount,sizeof(cbZuHeCount) );
	ZeroMemory( bZuHe,sizeof(bZuHe) );

	//变量定义
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );

	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD-7; i += 9 )
	{
		//1,4,7
		if( cbCardIndex[i] > 0 && cbCardIndex[i+3] > 0 && cbCardIndex[i+6] > 0 )
		{
			bZuHe[i/9][0] = true;
			cbZuHeCount[i/9]++;
			cbCardIndexTemp[i]--;
			cbCardIndexTemp[i+3]--;
			cbCardIndexTemp[i+6]--;
		}
		//2,5,8
		if( cbCardIndex[i+1] > 0 && cbCardIndex[i+4] > 0 && cbCardIndex[i+7] > 0 )
		{
			bZuHe[i/9][1] = true;
			cbZuHeCount[i/9]++;
			cbCardIndexTemp[i+1]--;
			cbCardIndexTemp[i+4]--;
			cbCardIndexTemp[i+7]--;
		}
		//3,6,9
		if( cbCardIndex[i+2] > 0 && cbCardIndex[i+5] > 0 && cbCardIndex[i+8] > 0 )
		{
			bZuHe[i/9][2] = true;
			if( ++cbZuHeCount[i/9] == 3 ) return false; 
			cbCardIndexTemp[i+2]--;
			cbCardIndexTemp[i+5]--;
			cbCardIndexTemp[i+8]--;
		}
	}

	//检查各种花色是否有组合
	if( cbZuHeCount[0] == 0 || cbZuHeCount[1] == 0 || cbZuHeCount[2] == 0 ) return false;

	//如果只有三种组合
	if( cbZuHeCount[0] + cbZuHeCount[1] + cbZuHeCount[2] == 3 )
	{
		//检查各花色是否有相同组合,如果有,返回false
		for( BYTE i = 0; i < 2; i++ )
		{
			BYTE cbCount = 0;
			for( BYTE j = 0; j < 3; j++ )
				if( bZuHe[j][i] ) cbCount++;
			if( cbCount > 1 ) return false;
		}
	}
	else
	{
		//找到有2个组合的花色
		for( BYTE i = 0; i < CountArray(cbZuHeCount); i++ )
		{
			if( cbZuHeCount[i] > 1 )
			{
				//检查是否与别的花色有重复组合,并去除与别的花色重复
				for( BYTE j = 0; j < 3; j++ )
				{
					if( bZuHe[i][j] )
					{
						BYTE cbSameCount = 0;
						for( BYTE k = 0; k < 3; k++ )
						{
							if( k == i ) continue;

							if( bZuHe[k][j] )
							{
								if( ++cbSameCount > 1 ) return false;

								//去除重复的组合
								//1,4,7
								if( j == 0 )
								{
									cbCardIndexTemp[i*9]++;
									cbCardIndexTemp[i*9+3]++;
									cbCardIndexTemp[i*9+6]++;
								}
								//2,5,8
								else if( j == 1 )
								{
									cbCardIndexTemp[i*9+1]++;
									cbCardIndexTemp[i*9+4]++;
									cbCardIndexTemp[i*9+7]++;
								}
								//3,6,9
								else
								{
									cbCardIndexTemp[i*9+2]++;
									cbCardIndexTemp[i*9+5]++;
									cbCardIndexTemp[i*9+8]++;
								}
							}
						}
					}
				}
			}
		}
	}

	tagWeaveItem wi[MAX_WEAVE];
	ZeroMemory( wi,sizeof(wi) );
	CopyMemory( wi,WeaveItem,cbWeaveCount*sizeof(tagWeaveItem) );
	BYTE cbWiCount = cbWeaveCount;
	//添加三个组合
	BYTE cbCardData[3] = { 0x01,0x02,0x03 };
	CopyMemory( wi[cbWiCount].cbCardData,cbCardData,sizeof(cbCardData) );
	wi[cbWiCount].cbCenterCard = 0x01;
	wi[cbWiCount].cbPublicCard = TRUE;
	wi[cbWiCount++].cbWeaveKind = WIK_LEFT;
	CopyMemory( wi[cbWiCount].cbCardData,cbCardData,sizeof(cbCardData) );
	wi[cbWiCount].cbCenterCard = 0x01;
	wi[cbWiCount].cbPublicCard = TRUE;
	wi[cbWiCount++].cbWeaveKind = WIK_LEFT;
	CopyMemory( wi[cbWiCount].cbCardData,cbCardData,sizeof(cbCardData) );
	wi[cbWiCount].cbCenterCard = 0x01;
	wi[cbWiCount].cbPublicCard = TRUE;
	wi[cbWiCount++].cbWeaveKind = WIK_LEFT;

	//分析剩余手上牌,如果能组合,说明可构成组合龙
	CAnalyseItemArray ai;
	AnalyseCard( cbCardIndexTemp,wi,cbWiCount,ai );

	return ai.GetCount()>0;
}

//全不靠
bool CGameLogic::IsQuanBuKao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//组合判断
	if (cbWeaveCount!=0) return false;

	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入数据
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//147,258,369,东南西北中发白的个数
	BYTE cbFoundCount = 0;
	//147,258,369组合
	bool bZuHe[3] = { false,false,false };

	//遍历牌
	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; )
	{
		//东南西北中发白
		if( i >= 27 )
		{
			if( cbCardIndexTemp[i] > 1 ) return false;
			cbFoundCount += cbCardIndexTemp[i];
			i++;
			continue;
		}
		bool bFound = false;
		//147
		BYTE cbCount = 0;
		if( cbCardIndexTemp[i] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+3] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+6] == 1 )
			cbCount++;
		if( cbCount > 0 )
		{
			bFound = true;
			//如果已有组合,返回FALSE
			if( bZuHe[0] ) return false;
			cbFoundCount += cbCount;
			bZuHe[0] = true;
		}
		//258
		cbCount = 0;
		if( cbCardIndexTemp[i+1] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+4] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+7] == 1 )
			cbCount++;
		if( cbCount > 0 )
		{
			if( bFound ) return false;
			bFound = true;
			//如果已有组合,返回FALSE
			if( bZuHe[1] ) return false;
			cbFoundCount += cbCount;
			bZuHe[1] = true;
		}
		//369
		cbCount = 0;
		if( cbCardIndexTemp[i+2] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+5] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+8] == 1 )
			cbCount++;
		if( cbCount > 0 )
		{
			if( bFound ) return false;
			bFound = true;
			//如果已有组合,返回FALSE
			if( bZuHe[2] ) return false;
			cbFoundCount += cbCount;
			bZuHe[2] = true;
		}

		i += 9;
	}

	return cbFoundCount==MAX_COUNT;
}

//三色双龙会
bool CGameLogic::IsSanSeShuangLong( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );
	cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//提取吃碰杠
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			for( BYTE j = 0; j < 3; j++ )
			{
				cbCardIndexTemp[SwitchToCardIndex(WeaveItem[i].cbCardData[j])]++;
			}
		}
		else 
		{
			BYTE cbCount = WeaveItem[i].cbWeaveKind==WIK_GANG?4:3;
			cbCardIndexTemp[SwitchToCardIndex(WeaveItem[i].cbCenterCard)] += cbCount;
		}
	}

	//检查老少副,5
	BYTE cbLauShaoCount = 0;
	bool bJiang5 = false;
	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD-7; i += 9 )
	{
		//5
		if( cbCardIndexTemp[i+4] > 0 ) 
		{
			if( bJiang5 || cbCardIndexTemp[i+4] != 2 ) return false;
			bJiang5 = true;
			continue;
		}

		//老少副
		if( cbCardIndexTemp[i] == 1 && cbCardIndexTemp[i+1] == 1 && cbCardIndexTemp[i+2] == 1 &&
			cbCardIndexTemp[i+6] == 1 && cbCardIndexTemp[i+7] == 1 && cbCardIndexTemp[i+8] == 1 )
			cbLauShaoCount++;
	}

	return cbLauShaoCount==2;
}

//清龙
bool CGameLogic::IsQingLong( const tagAnalyseItem & AnalyseItem )
{
	//变量定义
	bool bWeaveFlags[3][3];
	ZeroMemory(&bWeaveFlags,sizeof(bWeaveFlags));

	//扑克判断
	for (BYTE i=0;i<CountArray(AnalyseItem.cbWeaveKind);i++)
	{
		if (AnalyseItem.cbWeaveKind[i]&(WIK_LEFT|WIK_CENTER|WIK_RIGHT))
		{
			//获取属性
			BYTE cbCardValue=AnalyseItem.cbCardData[i][0]&MASK_VALUE;
			BYTE cbCardColor=AnalyseItem.cbCardData[i][0]&MASK_COLOR;
			if(cbCardColor == 0x30) continue;

			//设置变量
			ASSERT((cbCardColor>>4)<CountArray(bWeaveFlags));
			if ((cbCardValue==1)||(cbCardValue==4)||(cbCardValue==7))
				bWeaveFlags[cbCardColor>>4][cbCardValue/3]=true;
		}
	}

	//牌型判断
	BYTE cbColorCount=0;
	for (BYTE i=0;i<CountArray(bWeaveFlags);i++)
	{
		BYTE j=0;
		for ( j=0;j<CountArray(bWeaveFlags[0]);j++)
		{
			if (bWeaveFlags[i][j]==false) 
				break;
		}
		if (j==CountArray(bWeaveFlags[0]))
			return true;
	}

	return false;
}

//七星不靠
bool CGameLogic::IsQiXingBuKao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//组合判断
	if (cbWeaveCount!=0) return false;

	//临时数据
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));

	//插入数据
	BYTE cbCurrentIndex = SwitchToCardIndex(cbCurrentCard);
	cbCardIndexTemp[cbCurrentIndex]++;

	//147,258,369,东南西北中发白的个数
	BYTE cbFoundCount = 0;
	//147,258,369组合
	bool bZuHe[3] = { false,false,false };

	//遍历牌
	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; )
	{
		//东南西北中发白
		if( i >= 27 )
		{
			if( cbCardIndexTemp[i] != 1 ) return false;
			cbFoundCount++;
			i++;
			continue;
		}
		bool bFound = false;
		//147
		BYTE cbCount = 0;
		if( cbCardIndexTemp[i] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+3] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+6] == 1 )
			cbCount++;
		if( cbCount > 0 )
		{
			bFound = true;
			//如果已有组合,返回FALSE
			if( bZuHe[0] ) return false;
			cbFoundCount += cbCount;
			bZuHe[0] = true;
		}
		//258
		cbCount = 0;
		if( cbCardIndexTemp[i+1] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+4] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+7] == 1 )
			cbCount++;
		if( cbCount > 0 )
		{
			if( bFound ) return false;
			bFound = true;
			//如果已有组合,返回FALSE
			if( bZuHe[1] ) return false;
			cbFoundCount += cbCount;
			bZuHe[1] = true;
		}
		//369
		cbCount = 0;
		if( cbCardIndexTemp[i+2] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+5] == 1 )
			cbCount++;
		if( cbCardIndexTemp[i+8] == 1 )
			cbCount++;
		if( cbCount > 0 )
		{
			if( bFound ) return false;
			bFound = true;
			//如果已有组合,返回FALSE
			if( bZuHe[2] ) return false;
			cbFoundCount += cbCount;
			bZuHe[2] = true;
		}

		i += 9;
	}

	return cbFoundCount==MAX_COUNT;
}

//十三幺
bool CGameLogic::IsShiSanYao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//检查吃碰杠
	if( cbWeaveCount != 0 ) return false;

	//变量定义
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );
	cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//扑克判断
	bool bCardEye=false;

	//一九判断
	for (BYTE i=0;i<MAX_INDEX-MAX_HUA_CARD-7;i+=9)
	{
		//无效判断
		if (cbCardIndexTemp[i]==0) return false;
		if (cbCardIndexTemp[i+8]==0) return false;

		//牌眼判断
		if ((bCardEye==false)&&(cbCardIndexTemp[i]==2)) bCardEye=true;
		if ((bCardEye==false)&&(cbCardIndexTemp[i+8]==2)) bCardEye=true;
	}

	//番子判断
	for (BYTE i=MAX_INDEX-MAX_HUA_CARD-7;i<MAX_INDEX-MAX_HUA_CARD;i++)
	{
		if (cbCardIndexTemp[i]==0) return false;
		if ((bCardEye==false)&&(cbCardIndexTemp[i]==2)) bCardEye=true;
	}

	return bCardEye;
}

//九莲宝灯
bool CGameLogic::IsJiuLianBaoDeng( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	BYTE cbCardColor = cbCurrentCard>>4;
	if( cbCardColor >= 3 ) return false;

	for( BYTE i = cbCardColor*9; i < cbCardColor*9+9; i++ )
	{
		if( ((i%9)==0 || (i%9)==8) && cbCardIndex[i] != 3 ) return false;
		else if( cbCardIndex[i] != 1 ) return false;
	}

	return true;
}

//绿一色
bool CGameLogic::IsLuYiSe( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//检查手上牌
	BYTE cbCardData[MAX_COUNT];
	BYTE cbCardCount = SwitchToCardData( cbCardIndex,cbCardData );
	cbCardData[cbCardCount++] = cbCurrentCard;

	for( BYTE i = 0; i < cbCardCount; i++ )
	{
		BYTE cbColor = cbCardData[i]&MASK_COLOR;
		if( cbColor != 0x10 || cbCardData[i] != 0x36 ) return false;

		if( cbCardData[i] != 0x36 )
		{
			BYTE cbCardValue = cbCardData[i]&MASK_VALUE;
			if( cbCardValue == 1 || cbCardValue == 5 || cbCardValue == 7 ||
				cbCardValue == 9 )
				return false;
		}
	}

	return true;
}

//四归一
bool CGameLogic::IsSiGuiYi( const tagAnalyseItem *pAnalyseItem, const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//变量定义
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );
	cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

	//提取组合
	for( BYTE i = 0; i < cbWeaveCount; i++ )
	{
		if( WeaveItem[i].cbWeaveKind&(WIK_LEFT|WIK_CENTER|WIK_RIGHT) )
		{
			for( BYTE j = 0; j < 3; j++ )
				cbCardIndexTemp[SwitchToCardIndex(WeaveItem[i].cbCardData[j])]++;
		}
		else cbCardIndexTemp[SwitchToCardIndex(WeaveItem[i].cbCenterCard)] += (WeaveItem[i].cbWeaveKind==WIK_GANG?4:3);
	}

	//遍历牌
	for( BYTE i = 0; i < MAX_INDEX-MAX_HUA_CARD; i++ )
	{
		if( cbCardIndexTemp[i] == 4 )
		{
			//检查是否有杠
			BYTE j=0;
			for(  j = 0; j < cbWeaveCount; j++ )
			{
				if( WeaveItem[j].cbWeaveKind == WIK_GANG && 
					WeaveItem[j].cbCenterCard == SwitchToCardData(i) )
					break;
			}
			if( j < cbWeaveCount ) return true;
		}
	}

	return false;
}

//连七对
bool CGameLogic::IsLianQiDui( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard )
{
	//检查是否七小对
	if( IsQiXiaoDui(cbCardIndex,WeaveItem,cbWeaveCount,cbCurrentCard) )
	{
		BYTE cbCardColor = cbCurrentCard>>4;
		BYTE cbCardIndexTemp[MAX_INDEX];
		CopyMemory( cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp) );
		cbCardIndexTemp[SwitchToCardIndex(cbCurrentCard)]++;

		BYTE i = cbCardColor*9;
		while( cbCardIndexTemp[i] == 0 && i < MAX_INDEX-MAX_HUA_CARD ) i++;
		ASSERT( i < MAX_INDEX-MAX_HUA_CARD );
		if( (i+6)/9 != i/9 ) return false;

		//检查连牌
		BYTE cbEndIndex = i+6;
		for( ; i <= cbEndIndex; i++ )
			if( cbCardIndexTemp[i] == 0 ) return false;

		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
