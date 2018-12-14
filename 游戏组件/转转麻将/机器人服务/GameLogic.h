#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

#include <vector>

using namespace std;
//////////////////////////////////////////////////////////////////////////
//常量定义

#define MAX_WEAVE					4									//最大组合
#define MAX_INDEX					34									//最大索引
#define MAX_COUNT					14									//最大数目
#define MAX_REPERTORY				108									//最大库存
#define MAX_HUA_CARD				8									//花牌个数
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
#define WIK_FILL					0x10								//补牌类型
#define WIK_GANG					0x20								//杠牌类型
#define WIK_TING                    0x80                                //听牌类型
#define WIK_CHI_HU					0x40								//吃胡类型

//////////////////////////////////////////////////////////////////////////
//胡牌定义

//小胡牌型
#define CHK_NULL					0x0000								//非胡类型
#define CHK_JI_HU					0x0001								//鸡胡类型
#define CHK_PING_HU					0x0002								//平胡类型

//特殊胡牌
#define CHK_SIXI_HU					0x0010								//四喜胡牌
#define CHK_BANBAN_HU				0x0020								//板板胡牌
#define CHK_LIULIU_HU				0x0040								//六六顺牌
#define CHK_QUEYISE_HU				0x0080								//缺一色牌

//大胡牌型
#define CHK_PENG_PENG				0x0100								//碰碰胡牌
#define CHK_JIANG_JIANG				0x0200								//将将胡牌
#define CHK_QI_XIAO_DUI				0x0400								//七小对牌

//需将权位
#define CHR_DI						0x0001								//地胡权位
#define CHR_TIAN					0x0002								//天胡权位
#define CHR_HAI_DI					0x0004								//海底权位
#define CHR_QIANG_GANG				0x0008								//抢杆权位

//乱将权位
#define CHR_QING_YI_SE				0x0100								//清色权位
#define CHR_MEN_QIAN_QING		    0x0400								//门清权位
#define CHR_QUAN_QIU_REN			0x0200								//全求权位 (全求人就是单吊一张，其余都下坎了)

//////////////////////////////////////////////////////////////////////////

//类型子项
struct tagKindItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbCardIndex[3];						//扑克索引

	tagKindItem()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//组合子项
struct tagWeaveItem
{
	BYTE							cbWeaveKind;						//组合类型
	BYTE							cbCenterCard;						//中心扑克
	BYTE							cbPublicCard;						//公开标志
	WORD							wProvideUser;						//供应用户

	tagWeaveItem()
	{
		ZeroMemory(this, sizeof(*this));
		wProvideUser = -1;
	}
};

//胡牌结果
struct tagChiHuResult
{
	WORD							wChiHuKind;							//吃胡类型
	WORD							wChiHuRight;						//胡牌权位
	BYTE                            m_bKingCount;                         //王牌个数

	tagChiHuResult()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//杠牌结果
struct tagGangCardResult
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbCardData[4];						//扑克数据

	tagGangCardResult()
	{
		cbCardCount = 0;
		ZeroMemory(cbCardData, sizeof(cbCardData));
	}
};

//分析子项
struct tagAnalyseItem
{
	BYTE							cbCardEye;							//牌眼扑克
	BYTE							cbWeaveKind[4];						//组合类型
	BYTE							cbCenterCard[4];					//中心扑克

	tagAnalyseItem()
	{
		ZeroMemory(this, sizeof(*this));
	}
};


//杠上开花
struct tagGSKUResult
{
	bool							bIsGang;							//杠
	bool							bIsGSKU;							//杠上开花

	tagGSKUResult()
	{
		ZeroMemory(this, sizeof(*this));
	}
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
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard);
	//删除扑克
	bool RemoveCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbRemoveCard[], BYTE cbRemoveCount);
	//删除扑克
	bool RemoveCard(BYTE cbCardData[], BYTE cbCardCount, BYTE cbRemoveCard[], BYTE cbRemoveCount);

	//辅助函数
public:
	//有效判断
	static bool IsValidCard(BYTE cbCardData);
	static const char * GetMJCardName(unsigned char nCardID);//调试用
	//扑克数目
	BYTE GetCardCount(BYTE cbCardIndex[MAX_INDEX]);
	//组合扑克
	BYTE GetWeaveCard(BYTE cbWeaveKind, BYTE cbCenterCard, BYTE cbCardBuffer[4]);

	//等级函数
public:
	//动作等级
	BYTE GetUserActionRank(BYTE cbUserAction);
	//胡牌等级
	BYTE GetChiHuActionRank(tagChiHuResult & ChiHuResult);

	//动作判断
public:
	//吃胡判断
	//BYTE EstimateChiHu(BYTE cbCardIndex[MAX_INDEX]);
	//吃牌判断
	//BYTE EstimateEatCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard, BYTE cbKingCard[]);
	//碰牌判断
	BYTE EstimatePengCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//杠牌判断
	BYTE EstimateGangCard(BYTE cbCardIndex[MAX_INDEX], BYTE cbCurrentCard);
	//听牌判断
	//BYTE EstimateTingCard(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE IsGangData[MAX_INDEX],BYTE &GangCount);
	//bool EstimateTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE byOutCard);


	//动作判断
public:
	//杠牌分析
	BYTE AnalyseGangCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, tagGangCardResult & GangCardResult);
	//吃胡分析(无王牌)
	BYTE AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult);
	/// 分析杠，碰有多少组扑克
	/// [cbCardIndex in] 分析的牌数据索引
	/// [bKind in] 分析类型 WIK_PENG 碰，WIK_GANG/WIK_FILL 杠
	/// [byCardData in] 刚打出或者抓到，补的牌
	/// [byCanSelectCardData out] 能选择的牌数据
	/// [byCount out] 数目
	/// [return] 其中一组的牌数据
	BYTE AnalyseGangPengCountCard(BYTE cbCardIndex[MAX_INDEX], BYTE byKind, BYTE byCardData, BYTE byCanSelectCardData[4], BYTE &byCount);

public:	
	//王牌数目
	//BYTE GetKingCardCount(BYTE cbCardIndex[MAX_INDEX],tagWeaveItem WeaveItem[],BYTE cbItemCount,BYTE bKingCard[],BYTE bKingCount);
	//王牌判断
	//bool IsKingCard(BYTE cbCardData,BYTE bKingCard[],BYTE bKingCount);
	//听牌判断
	//bool IsCanTingCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount);

	//特殊胡牌
public:
	//四喜胡牌
	//bool IsSiXi(BYTE cbCardIndex[MAX_INDEX]);
	//缺一色牌
	//bool IsQueYiSe(BYTE cbCardIndex[MAX_INDEX]);
	//板板胡牌
	//bool IsBanBanHu(BYTE cbCardIndex[MAX_INDEX]);
	//六六顺牌
	//bool IsLiuLiuShun(BYTE cbCardIndex[MAX_INDEX]);

	//特殊胡牌
public:
	//清一色牌
	//bool IsQingYiSe(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//七小对牌
	//bool IsQiXiaoDui(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//豪华对牌
	//bool IsMenQianQing(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);
	//将将胡牌
	//bool IsJiangJiangHu(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount);

	

	//转换函数
public:
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData);
	//扑克转换
	BYTE SwitchToCardData(BYTE cbCardIndex[MAX_INDEX], BYTE cbCardData[MAX_COUNT]);
	//扑克转换
	BYTE SwitchToCardIndex(BYTE cbCardData[], BYTE cbCardCount, BYTE cbCardIndex[MAX_INDEX]);

public:
	int GetHuPaiData(BYTE cbCardData[], BYTE cbCardCount);

	//内部函数
private:
	//分析扑克
	bool AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbItemCount, CAnalyseItemArray & AnalyseItemArray);

private:
	std::vector<int>	bTempHuKind;	      //保存胡牌类型
	std::vector<BYTE>	m_cbHuCardIndex;	      //保存胡牌数据
};

//////////////////////////////////////////////////////////////////////////

#endif