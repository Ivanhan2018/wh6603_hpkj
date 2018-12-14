#pragma once


//发送牌
class CSendCard
{
public:
	CSendCard(void);
	~CSendCard(void);

public:
	//设置路径
	void SetFilePath(CString strFilePath);
	//获得玩家手牌数据
	int GetCardData(int iChairID, BYTE* pCard, int iNums);
	//获得王牌
	BYTE GetKingCard();
	//获得色子数
	bool GetSZNums(int& iNum1, int& iNum2);
	//获得下次抓的牌
	BYTE GetNextCard();
	//获得牌墙数据
	int GetCardWallData(BYTE* pCard, int iNums);

private:
	BYTE GetNum(char c);

private:
	CString			m_strFilePath;
};
