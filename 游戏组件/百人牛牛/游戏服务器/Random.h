#ifndef _HP_RANDOM_H_
#define _HP_RANDOM_H_


#include "WinControl.h"

#define _IN_
#define _OUT_


//自定义随机数类
//@功能：获取随机数
//@功能：洗牌算法
class WINCONTROL_API	IRandom
{
public:
	/// 取[iMin, iMax]中的随机数，iMin和iMax一定要计算精确，如果没有特殊需要，
	///	不要再对取出来的值进行取模之类的计算
	virtual unsigned int Rand(unsigned int iMin=0, unsigned int iMax=32767) = 0;

	/// 洗牌算法
	virtual void	Shuffle(_IN_ _OUT_ unsigned char* pData, unsigned int nDataLen) = 0;
	virtual void	Shuffle(_IN_ _OUT_ unsigned short* pData, unsigned int nDataLen) = 0;
	virtual void	Shuffle(_IN_ _OUT_ unsigned int* pData, unsigned int nDataLen) = 0;
	static IRandom*		GetSingleton();
};

//使用案例
/*
	//首先在所有游戏代码中去掉 srand()函数，然后将原随机数函数 rand()更换为 IRandom::GetSingleton()->Rand();
	//并且不需要再进行取模算法

	//获取随机数
	int nRnd = IRandom::GetSingleton()->Rand(0, 200);

	//洗牌算法
	unsigned char szData[54];
	for(BYTE n=0; n<54; n++)
		szData[n] = '0' + n%13;
	IRandom::GetSingleton()->Shuffle(szData, sizeof(szData));

*/


#undef _IN_
#undef _OUT_

#endif // _RANDOM_H_