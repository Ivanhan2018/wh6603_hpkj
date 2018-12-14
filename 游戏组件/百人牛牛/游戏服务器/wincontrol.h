#ifdef WINCONTROL_EXPORTS
#define WINCONTROL_API __declspec(dllexport)
#else
#define WINCONTROL_API __declspec(dllimport)
#endif

#ifndef		_WIN_CONTRL_H__
#define		_WIN_CONTRL_H__

#include <list>



/**
 * 说明：
 * 统计历史局数，计算玩家的总输赢。
 * 如果在一定历史局数之前，那可以认为不影响现在的陪玩作弊概率
 */

class	WINCONTROL_API	CHistoryPlayerWin            
{
private:
    int							m_iMaxCount;                    // 最大统计的局数
	int							m_iHighLimit;					// 最高限制
	int							m_iLowLimit;					// 最低限制
    std::list< __int64 >		m_arrWinMoney;					// 最近输赢的历史记录
    std::list< __int64 >		m_arrPay;						// 最近输赢的历史记录
    __int64						m_nWinMoney;					// 最近m_nTurnCount局玩家总的输赢
    __int64						m_nPay;						    // 最近m_nTurnCount局玩家总的投注
public:
	//构造函数
    CHistoryPlayerWin();

	//初始化
    void Init(int iMaxCount/*最大统计的局数*/, int nHighLimit=9950, int nLowLimit=9500);

	//判断玩家输赢是否符合要求
	//@nWin:表示本局所有玩家的输赢总和（赢=正 输=负相加得来，包括banker）
	//@nPay:表示本局所有玩家的投注相加（都是正数）
	//@nCurMinute:当前时间的分钟数
	//@return:返回值，true表示在合理范围之内。false表示输赢结果不合理，需要处理，比如重新发牌，重新开始
	bool HpGetResult(__int64 nWin/*玩家每局输赢累计*/, __int64 nPay/*每局玩家投注累计*/, int nCurMinute, int nParam);
    
	//参数同以上，区别在于这个函数并不写入，只是用于判断
	//@nParam:灵敏度，1-100， 1=非常灵敏，100=非常迟钝，建议值（10）；迟钝代表系统有可能输给玩家
	bool HpTest(__int64 nWin/*玩家每局输赢累计*/, __int64 nPay/*每局玩家投注累计*/, int nCurMinute, int nParam=10);

    // 获得最近m_nTurnCount局内玩家的输赢概率，返回万分比
    int GetWinRate();
	// 根据数据，获得新的返回比
	int GetTestWinRate(__int64 nWin,  __int64  nPay,	int nRatio=1);
	//获取赢数
	__int64	GetWinSum();

	//获取总数
	__int64	GetTotalSum();

};



#endif