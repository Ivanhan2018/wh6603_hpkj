#ifndef CONTROL_WND_HEAD_FILE
#define CONTROL_WND_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//消息定义
#define IDM_USER_ACTION				(WM_USER+300)						//用户动作
#define IDM_USER_MOVE				(WM_USER+301)						//按钮盘旋
//////////////////////////////////////////////////////////////////////////

//控制窗口
class CControlWnd : public CWnd
{
	//控件变量
protected:
	CSkinButton						m_btPeng;							//控制按钮
	CSkinButton						m_btGang;							//控制按钮
	//CSkinButton						m_btHear;							//控制按钮
	CSkinButton						m_btChiHu;							//控制按钮
	CSkinButton						m_btGiveUp;							//控制按钮
	CSkinButton						m_btLeftChi;						//吃头按钮
	CSkinButton						m_btCenterChi;						//吃中按钮
	CSkinButton						m_btRightChi;						//吃尾按钮

	//资源变量
protected:
	CBitImage						m_ImageControlBack;					//窗口背景

	//函数定义
public:
	//构造函数
	CControlWnd();
	//析构函数
	virtual ~CControlWnd();

	//功能函数
public:
	//设置状态
	VOID SetControlInfo(BYTE cbAcitonMask);

	//内部函数
protected:
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//按钮消息
protected:
	//碰牌按钮
	VOID OnBnClickedPeng();
	//杆牌按钮
	VOID OnBnClickedGang();
	//听牌按钮
	VOID OnBnClickedHear();
	//吃胡按钮
	VOID OnBnClickedChiHu();
	//放弃按钮
	VOID OnBnClickedGiveUp();
	//吃牌按钮
	VOID OnBnClickedLeftChi();
	//吃牌按钮
	VOID OnBnClickedCenterChi();
	//吃牌按钮
	VOID OnBnClickedRightChi();

public:
	//鼠标离开
	virtual HRESULT __cdecl OnEventMouseLeft(UINT uControlID, WPARAM wParam, LPARAM lParam);
	//鼠标移动
	virtual HRESULT __cdecl OnEventMouseMove(UINT uControlID, WPARAM wParam, LPARAM lParam);
	//鼠标按钮
	virtual HRESULT __cdecl OnEventLButtonUp(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}
	//鼠标按钮
	virtual HRESULT __cdecl OnEventLButtonDown(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}
	//鼠标按钮
	virtual HRESULT __cdecl OnEventRButtonUp(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}
	//鼠标按钮
	virtual HRESULT __cdecl OnEventRButtonDown(UINT uControlID, WPARAM wParam, LPARAM lParam){return TRUE;}

	//消息映射
protected:
	//重画函数
	VOID OnPaint();
	//建立消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif