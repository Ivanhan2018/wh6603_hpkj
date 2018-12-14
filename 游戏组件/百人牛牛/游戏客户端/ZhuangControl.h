#pragma once

class CZhuangControl
{
public:
	CZhuangControl(void);
	~CZhuangControl(void);

	//资源
public:
	CBitmap							m_ImageZhuang;				//图片资源
	//数据
private:
	int								m_CurrentIndex;				//当前帧
	CPoint							m_pointBase;				//基准位置
	CPoint							m_pointOrigin;				//画原点
	bool							m_bVisbale;					//是否可见
	CSize							m_sizeRECT;					//显示范围
	CSize							m_FrameSize;				//帧大小
	static const int				ZHUANG_FLAME_COUNT = 41;	///总帧数
	CDC								memDC;
	int								m_nBeginIndex;				///先发给谁。  2 东   3 南   4 西  5 北  1 庄
	int								m_nPerFrameCount;			///每个动画8帧

	//函数
public:
	///设置发牌起始
	void SetBeginIndex(int nIndex){m_nBeginIndex = nIndex;};
	///得到发牌起始
	int GetBeginIndex(){return m_nBeginIndex;};
	//设置点数
	void SetSiceValue(BYTE nValue);

	//设置帧索引
	void SetFrameIndex(int nIndex);
	//获取帧索引
	int GetFrameIndex();
	///一个动画8帧的计数器
	void AddFrame(){m_nPerFrameCount++;};
	///判断一个动画是否播放完成
	BOOL isPerFrameFinish(){if(m_nPerFrameCount%4==0)
								return TRUE;
							else
								return FALSE;};
	///重置每个动画计数器
	void ResetPerFrame(){m_nPerFrameCount = 1;};
	//设置基准位置
	void SetPointBase(int x, int y);
	//设置基准位置
	void SetPointBase(CPoint pos);

	//显示完成
	bool IsFinish();

	//设置显示范围
	void SetSize(int w, int h);
	//设置显示范围
	void SetSize(CSize size);

	//是否显示
	bool IsVisable()
	{
		return m_bVisbale;
	}
	//是否显示
	void SetVisable(bool bVisbale)
	{
		m_bVisbale = bVisbale;
	}

	//绘画扑克
	VOID DrawCardControl(CDC * pDC);
};
