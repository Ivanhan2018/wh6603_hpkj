#ifndef MACRO_HEAD_FILE
#define MACRO_HEAD_FILE

#include "math.h"

//////////////////////////////////////////////////////////////////////////////////
//常用常量

//无效数值
#define INVALID_BYTE				((BYTE)(0xFF))						//无效数值
#define INVALID_WORD				((WORD)(0xFFFF))					//无效数值
#define INVALID_DWORD				((DWORD)(0xFFFFFFFF))				//无效数值

//////////////////////////////////////////////////////////////////////////////////

//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//无效地址
#define INVALID_IP_ADDRESS(IPAddress) (((IPAddress==0L)||(IPAddress==INADDR_NONE)))

//////////////////////////////////////////////////////////////////////////////////

//存储长度
#ifdef _UNICODE
	#define CountStringBuffer CountStringBufferW
#else
	#define CountStringBuffer CountStringBufferA
#endif

//存储长度
#define CountStringBufferA(String) ((UINT)((lstrlenA(String)+1)*sizeof(CHAR)))
#define CountStringBufferW(String) ((UINT)((lstrlenW(String)+1)*sizeof(WCHAR)))

//////////////////////////////////////////////////////////////////////////////////

//接口释放
#define SafeRelease(pObject) { if (pObject!=NULL) { pObject->Release(); pObject=NULL; } }

//删除指针
#define SafeDelete(pData) { try { delete pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//关闭句柄
#define SafeCloseHandle(hHandle) { if (hHandle!=NULL) { CloseHandle(hHandle); hHandle=NULL; } }

//删除数组
#define SafeDeleteArray(pData) { try { delete [] pData; } catch (...) { ASSERT(FALSE); } pData=NULL; } 

//////////////////////////////////////////////////////////////////////////////////

//等于比较
#define SCORE_EQUAL(FirstScore,SecondScore) (fabs(FirstScore-SecondScore)<SCORE_ZERO)
//小于比较
#define SCORE_LESS(FirstScore,SecondScore)	(FirstScore<SecondScore)
//大于比较
#define SCORE_GREATER(FirstScore,SecondScore) (FirstScore>SecondScore)
//小于等于
#define SCORE_LESS_EQUAL(FirstScore,SecondScore) (FirstScore<=SecondScore)
//大于等于
#define SCORE_GREATER_EQUAL(FirstScore,SecondScore) (FirstScore>=SecondScore)

//四舍五入
inline DOUBLE Double_Round(DOUBLE dValue,int Retainbits,int Afterbits)	
{			
	DOUBLE TempValue=dValue;
	for(BYTE i=0;i<Afterbits;i++)								
	{															 
        LONGLONG  lValue,lScale=1;								
        for(BYTE j=0;j<Retainbits+Afterbits-1-i;j++) lScale *=  10;				
        lScale *= 2;											
        lValue = (LONGLONG)(TempValue * lScale);								
        lValue += (lValue % 2);	
		DOUBLE TrnasValue = (DOUBLE)lValue;
        TempValue = (TrnasValue/lScale);						
	}		
	return TempValue;
}

//////////////////////////////////////////////////////////////////////////////////

#endif