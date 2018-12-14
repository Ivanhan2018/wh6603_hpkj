#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////

//用于财神的转换，如果有牌可以代替财神本身牌使用，则设为该牌索引，否则设为MAX_INDEX. 注:如果替换牌是序数牌,将出错.
#define	INDEX_REPLACE_CARD					MAX_INDEX

//////////////////////////////////////////////////////////////////////////
//逻辑掩码

#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//////////////////////////////////////////////////////////////////////////
//动作定义

//动作标志
#define WIK_NULL					0x00								//没有类型
#define WIK_LEFT					0x01								//左吃类型
#define WIK_CENTER					0x02								//中吃类型
#define WIK_RIGHT					0x04								//右吃类型
#define WIK_PENG					0x08								//碰牌类型
#define WIK_GANG					0x10								//杠牌类型
#define WIK_LISTEN					0x20								//吃牌类型
#define WIK_CHI_HU					0x40								//吃胡类型
#define WIK_REPLACE					0x80								//花牌替换

//////////////////////////////////////////////////////////////////////////
//胡牌定义

/*
//	1番
*/
#define CHR_ZI_MO					0x00000001									//自摸
#define CHR_DAN_DIAO_JIANG			0x00000002									//单钓将
#define CHR_KAN_ZHANG				0x00000004									//坎张
#define CHR_BIAN_ZHANG				0x00000008									//边张
#define CHR_WU_ZI					0x00000010									//无字
#define CHR_QUE_YI_MEN				0x00000020									//缺一门
#define CHR_MING_GANG				0x00000040									//明杠
#define CHR_YAO_JIU					0x00000080									//幺九刻
#define CHR_LAO_SHAO_JIANG			0x00000100									//老少副
#define CHR_LIAN_LIU				0x00000200									//连六
#define CHR_XI_XIANG_FENG			0x00000400									//喜相逢
#define CHR_YI_BAN_GAO				0x00000800									//一般高

/*
//	2番
*/
#define CHR_DUAN_YAO				0x00001000									//断幺
#define CHR_AN_GANG					0x00002000									//暗杠
#define CHR_SHUANG_AN_KE			0x00004000									//双暗刻
#define CHR_SHUANG_TONG_KE			0x00008000									//双同刻
#define CHR_SHI_GUI_YI				0x00010000									//四归一
#define CHR_PING_HU					0x00020000									//平和
#define CHR_MEN_QI_QING				0x00040000									//门前清
#define CHR_MEN_FENG_KE				0x00080000									//门风刻
#define CHR_QUAN_FENG_KE			0x00100000									//圈风刻
#define CHR_JIAN_KE					0x00200000									//箭刻

/*
//	4番
*/
#define CHR_JUE_ZHANG				0x00400000									//绝张
#define CHR_SHUANG_MING_GANG		0x00800000									//双明杠
#define CHR_BU_QIU_REN				0x01000000									//不求人
#define CHR_QUAN_DAI_YAO			0x02000000									//全带幺

/*
//	6番
*/
#define CHR_SHUANG_JIAN_KE			0x04000000									//双箭刻
#define CHR_SHUANG_AN_GANG			0x08000000									//双暗杠
#define CHR_QUAN_QIU_REN			0x10000001									//全求人
#define CHR_WU_MEN_QI				0x10000002									//五门齐
#define CHR_SAN_SE_BU				0x10000004									//三色三步高
#define CHR_HUN_YI_SE				0x10000008									//混一色
#define CHR_PENG_PENG				0x10000010									//碰碰和

/*
//	8番
*/
#define CHR_QIANG_GANG				0x10000020									//抢杠和
#define CHR_GANG_KAI				0x10000040									//杠上开花
#define CHR_HAI_DI					0x10000080									//海底捞月
#define CHR_MIAO_SHOU				0x10000100									//妙手回春
#define CHR_WU_FAN					0x10000200									//无番和
#define CHR_SAN_SE_JIE				0x10000400									//三色三节高
#define CHR_SAN_SE_TONG				0x10000800									//三色三同顺 
#define CHR_TUI_BU_DAO				0x10001000									//推不倒
#define CHR_HUA_LONG				0x10002000									//花龙

/*
//	12番
*/
#define CHR_SAN_FENG_KE				0x10004000									//三风刻
#define CHR_XIAO_WU					0x10008000									//小于五
#define CHR_DA_WU					0x10010000									//大于五
#define CHR_ZU_HE_LONG				0x10020000									//组合龙
#define CHR_QUAN_BU_KAO				0x10040000									//全不靠

/*
//	16番
*/
#define CHR_SAN_AN_KE				0x10080000									//三暗刻
#define CHR_SAN_TONG_KE				0x10100000									//三同刻
#define CHR_QUAN_DAI_WU				0x10200000									//全带五
#define CHR_YI_SE_BU				0x10400000									//一色三步高
#define CHR_SAN_SE_SHUANG			0x10800000									//三色双龙会
#define CHR_QING_LONG				0x11000000									//清龙

/*
//	24番
*/
#define CHR_QUAN_XIAO				0x12000000									//全小
#define CHR_QUAN_ZHONG				0x14000000									//全中
#define CHR_QUAN_DA					0x18000000									//全大
#define CHR_YI_SE_JIE				0x20000001									//一色三节高
#define CHR_YI_SE_TONG				0x20000002									//一色三同顺
#define CHR_QING_YI_SE				0x20000004									//清一色
#define CHR_QUAN_SHUANG				0x20000008									//全双
#define CHR_QI_XING_BU_KAO			0x20000010									//七星不靠
#define CHR_QI_DUI					0x20000020									//七对

/*
//	32番
*/
#define CHR_HUN_YAO					0x20000040									//混幺九
#define CHR_SAN_GANG				0x20000080									//三杠
#define CHR_YI_SHI_BU				0x20000100									//一色四步高

/*
//	48番
*/
#define CHR_YI_SHI_JIE				0x20000200									//一色四节高
#define CHR_YI_SHI_TONG				0x20000400									//一色四同顺

/*
//	64番
*/
#define CHR_YI_SE_SHUANG			0x20000800									//一色双龙会
#define CHR_SI_AN_KE				0x20001000									//四暗刻
#define CHR_ZI_YI_SE				0x20002000									//字一色
#define CHR_XIAO_SAN_YUAN			0x20004000									//小三元
#define CHR_XIAO_SHI_XI				0x20008000									//小四喜
#define CHR_QING_YAO_JIU			0x20010000									//清幺九

/*
//	88番
*/
#define CHR_SHI_SAN_YAO				0x20020000									//十三幺
#define CHR_LIAN_QI_DUI				0x20040000									//连七对
#define CHR_SI_GANG					0x20080000									//四杠
#define CHR_JIU_LIAN_DENG			0x20100000									//九莲宝灯
#define CHR_LU_YI_SE				0x20200000									//绿一色
#define CHR_DA_SAN_YUAN				0x20400000									//大三元
#define CHR_DA_SHI_XI				0x20800000									//大四喜

//////////////////////////////////////////////////////////////////////////

//类型子项
struct tagKindItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbValidIndex[3];					//实际扑克索引
};

//组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户
	BYTE							cbCardData[4];						//
};

//杠牌结果
struct tagGangCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[MAX_WEAVE];				//扑克数据
};

//分析子项
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//牌眼扑克
	bool                            bMagicEye;                          //牌眼是否是王霸
	BYTE							cbWeaveKind[MAX_WEAVE];				//组合类型
	BYTE							cbCenterCard[MAX_WEAVE];			//中心扑克
	BYTE                            cbCardData[MAX_WEAVE][4];           //实际扑克
};

//////////////////////////////////////////////////////////////////////////


#define MASK_CHI_HU_RIGHT			0x0fffffff

/*
//	权位类。
//  注意，在操作仅位时最好只操作单个权位.例如
//  CChiHuRight chr;
//  chr |= (chr_zi_mo|chr_peng_peng)，这样结果是无定义的。
//  只能单个操作:
//  chr |= chr_zi_mo;
//  chr |= chr_peng_peng;
*/
class CChiHuRight
{	
	//静态变量
private:
	static bool						m_bInit;
	static DWORD					m_dwRightMask[MAX_RIGHT_COUNT];

	//权位变量
private:
	DWORD							m_dwRight[MAX_RIGHT_COUNT];

public:
	//构造函数
	CChiHuRight();

	//运算符重载
public:
	//赋值符
	CChiHuRight & operator = ( DWORD dwRight );

	//与等于
	CChiHuRight & operator &= ( DWORD dwRight );
	//或等于
	CChiHuRight & operator |= ( DWORD dwRight );

	//与
	CChiHuRight operator & ( DWORD dwRight );
	CChiHuRight operator & ( DWORD dwRight ) const;

	//或
	CChiHuRight operator | ( DWORD dwRight );
	CChiHuRight operator | ( DWORD dwRight ) const;

	//相等
	bool operator == ( DWORD dwRight ) const;
	bool operator == ( const CChiHuRight chr ) const;

	//不相等
	bool operator != ( DWORD dwRight ) const;
	bool operator != ( const CChiHuRight chr ) const;

	//功能函数
public:
	//是否权位为空
	bool IsEmpty();

	//设置权位为空
	void SetEmpty();

	//获取权位数值
	BYTE GetRightData( DWORD dwRight[], BYTE cbMaxCount );

	//设置权位数值
	bool SetRightData( const DWORD dwRight[], BYTE cbRightCount );

private:
	//检查权位是否正确
	bool IsValidRight( DWORD dwRight );
};

//////////////////////////////////////////////////////////////////////////

//数组说明
typedef CWHArray<tagAnalyseItem,tagAnalyseItem &> CAnalyseItemArray;

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_cbCardDataArray[MAX_REPERTORY];	//扑克数据
	BYTE							m_cbMagicIndex;						//钻牌索引

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//控制函数
public:
	//混乱扑克
	void RandCardData(BYTE cbCardData[], BYTE cbMaxCount);
	//混乱扑克
	void RandCardData(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbRandData[]);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, const BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//设置财神
	void SetMagicIndex( BYTE cbMagicIndex ) { m_cbMagicIndex = cbMagicIndex; }
	//财神判断
	bool IsMagicCard( BYTE cbCardData );
	//花牌判断
	bool IsHuaCard( BYTE cbCardData );

	//辅助函数
public:
	//有效判断
	bool IsValidCard(BYTE cbCardData);
	//扑克数目
	BYTE GetCardCount(const BYTE cbCardIndex[MAX_INDEX]);
	//组合扑克
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//等级函数
public:
	//动作等级
	BYTE GetUserActionRank(BYTE cbUserAction);
	//胡牌等级
	WORD GetChiHuActionRank(const CChiHuRight & ChiHuRight);

	//动作判断
public:
	//吃牌判断
	BYTE EstimateEatCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//碰牌判断
	BYTE EstimatePengCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//杠牌判断
	BYTE EstimateGangCard(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);

	//动作判断
public:
	//杠牌分析
	BYTE AnalyseGangCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, tagGangCardResult & GangCardResult);
	//吃胡分析
	BYTE AnalyseChiHuCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, CChiHuRight &ChiHuRight);
	//听牌分析
	BYTE AnalyseTingCard( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );

	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(const BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//扑克转换
	BYTE SwitchToCardIndex(const BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

	//胡法分析
protected:
	//单钓将
	bool IsDanDiaoJiang( const tagAnalyseItem *pAnalyseItem, BYTE cbCurrentCard );
	//坎张
	bool IsKanZhang( const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount, BYTE cbCurrentCard );
	//边张
	bool IsBianZhang( const tagAnalyseItem *pAnalyseItem, BYTE cbWeaveCount, BYTE cbCurrentCard );
	//无字
	bool IsWuZi( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//缺一门
	bool IsQueYiMen( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//明杠
	bool IsMingGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//幺九刻
	bool IsYaoJiuKe( const tagAnalyseItem *pAnalyseItem );
	//老少副
	bool IsLaoShaoFu( const tagAnalyseItem *pAnalyseItem );
	//连六
	bool IsLianLu( const tagAnalyseItem *pAnalyseItem );
	//喜相逢
	bool IsXiXiangFeng( const tagAnalyseItem *pAnalyseItem );
	//一般高
	bool IsYiBanGao( const tagAnalyseItem *pAnalyseItem );
	//断幺
	bool IsDuanYao( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//暗杠
	bool IsAnGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//双暗刻
	bool IsShuangAnKe( const tagAnalyseItem *pAnalyseItem, const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//双同刻
	bool IsShuangTongKe( const tagAnalyseItem *pAnalyseItem );
	//平胡
	bool IsPingHu( const tagAnalyseItem *pAnalyseItem );
	//门前清
	bool IsMenQianQing( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//全求人
	bool IsQuanQiuRen( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//箭刻
	bool IsJianKe( const tagAnalyseItem *pAnalyseItem );
	//双明杠
	bool IsShuangMingGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//不求人
	bool IsBuQiuRen( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//全带幺
	bool IsQuanDaiYao( const tagAnalyseItem *pAnalyseItem );
	//双箭刻
	bool IsShuangJianKe( const tagAnalyseItem *pAnalyseItem );
	//双暗杠
	bool IsShuangAnGang( const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//五门齐
	bool IsWuMenQi( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//三色三步高
	bool IsSanSeBu( const tagAnalyseItem *pAnalyseItem );
	//混一色
	bool IsHunYiSe( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbWeaveCount,const BYTE cbCurrentCard );
	//碰碰和
	bool IsPengPeng( const tagAnalyseItem *pAnalyseItem );
	//三色三节高
	bool IsSanSeJie( const tagAnalyseItem *pAnalyseItem );
	//三色三同顺
	bool IsSanSeTong( const tagAnalyseItem *pAnalyseItem );
	//花龙
	bool IsHuaLong( const tagAnalyseItem *pAnalyseItem );
	//三风刻
	bool IsSanFengKe( const tagAnalyseItem *pAnalyseItem );
	//小于五
	bool IsXiaoWu( const tagAnalyseItem *pAnalyseItem );
	//大于五
	bool IsDaWu( const tagAnalyseItem *pAnalyseItem );
	//四暗刻
	bool IsSiAnKe( const tagAnalyseItem *pAnalyseItem, const tagWeaveItem WeaveItem[], BYTE cbWeaveCount );
	//一色三步高
	bool IsYiSeBu(const tagAnalyseItem & AnalyseItem);
	//全带5
	bool IsQuanDaiWu(const tagAnalyseItem & AnalyseItem);
	//三同刻
	bool IsSanTongKe(const tagAnalyseItem & AnalyseItem);
	//三暗刻
	bool IsSanAnKe(const tagAnalyseItem & AnalyseItem,const tagWeaveItem WeaveItem[], BYTE cbWeaveCount);
	//大四喜
	bool IsDaSiXi(const tagAnalyseItem & AnalyseItem);
	//大三元
	bool IsDaSanYuan(const tagAnalyseItem & AnalyseItem);
	//四杠
	bool IsSiGang(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount);
	//清幺九
	bool IsQingYaoJiu(const tagAnalyseItem &AnalyseItem);
	//小四喜
	bool IsXiaoSiXi(const tagAnalyseItem & AnalyseItem);
	// 小三元
	bool IsXiaoShanYuan(const tagAnalyseItem & AnalyseItem);
	//字一色牌
	bool IsZiYiSe(const tagAnalyseItem & AnalyseItem);
	// 一色双龙会
	bool IsYiSeHuangHuiLong(const tagAnalyseItem & AnalyseItem);
	//一色四同顺
	bool IsYiSeSiTongShun(const tagAnalyseItem &AnalyseItem);
	//一色四节高
	bool IsYiSeSiJieGao(const tagAnalyseItem &AnalyseItem);
	//一色四步高
	bool IsYiSeSiBuGao(const tagAnalyseItem &AnalyseItem);
	//三杠
	bool IsSanGang(const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount);
	//混幺九
	bool IsHunYaoJiu(const tagAnalyseItem &AnalyseItem);
	//清一色牌
	bool IsQingYiSe(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], const BYTE cbItemCount,const BYTE cbCurrentCard);
	//七小对牌
	bool IsQiXiaoDui(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[],const  BYTE cbWeaveCount,const BYTE cbCurrentCard);
	//全双
	bool IsQuanShuang(const tagAnalyseItem &AnalyseItem);
	//一色三同顺
	bool IsYiSeSanTongShun(const tagAnalyseItem &AnalyseItem);
	//一色三节高
	bool IsYiSeSanJieGao(const tagAnalyseItem &AnalyseItem);
	//全大
	bool IsQuanDa(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard);
	//全中
	bool IsQuanZhong(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard);
	//全小
	bool IsQuanXiao(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard);
	//推不倒
	bool IsTuiBuDao( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//组合龙
	bool IsZuHeLong( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//全不靠
	bool IsQuanBuKao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//三色双龙会
	bool IsSanSeShuangLong( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//清龙
	bool IsQingLong( const tagAnalyseItem & AnalyseItem );
	//七星不靠
	bool IsQiXingBuKao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//十三幺
	bool IsShiSanYao( const BYTE cbCardIndex[MAX_INDEX], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//九莲宝灯
	bool IsJiuLianBaoDeng( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//绿一色
	bool IsLuYiSe( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//四归一
	bool IsSiGuiYi( const tagAnalyseItem *pAnalyseItem, const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );
	//连七对
	bool IsLianQiDui( const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard );

	//内部函数
private:
	//分析扑克
	bool AnalyseCard(const BYTE cbCardIndex[MAX_INDEX], const tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);
	//排序,根据牌值排序
	bool SortCardList( BYTE cbCardData[MAX_COUNT], BYTE cbCardCount );
};

//////////////////////////////////////////////////////////////////////////

#endif
