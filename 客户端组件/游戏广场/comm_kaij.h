#pragma once

//账户长度
#define		ACCOUNT_LENGTH	21
//密码长度
#define		PASSWORD_LENGTH	21
//IP地址长度
#define		IP_LENGTH		11
//时间长度
#define		TIME_LENGTH		51
//QQ号码长度
#define		QQ_LENGTH		12
//号码最大长度
#define		HAOMAO_LENGTH	60

//开奖信息
#define	KJ_QIHAO_LENGTH		30
#define	KJ_HAOMA_LENGTH		30
#define	KJ_SHIJIAN_LENGTH	30


//开奖数据
struct	OpenCodeInfo
{
	int nTypeID;				//开奖彩种
	int nIndexID;				//流水号
	TCHAR	szExpect[14];		//开奖期号
	TCHAR	szOpenCode[42];		//开奖号码
	VOID*	pPointer;
};


#define		FLAG_KAIJIANG_NONE		0
#define		FLAG_KAIJIANG_ING		1
#define		FLAG_KAIJIANG_OK		2

//彩票种类
typedef enum {
	CZChongQingSSC=1,		//重庆时时彩
	CZJiangxiSSC=2,			//江西时时彩
	CZ3D=3,					//福彩3D
	CZXinJiangSSC=4,		//新疆时时彩
	CZPailie3,				//排列3
	CZGD11Xuan5,			//广东11选5
	CZCQ11Xuan5,			//重庆11选5
	CZSD11Xuan5,			//山东11选5
	CZJX11Xuan5,			//江西11选5
	CZKUAILE8,				//北京快乐8
	CZ_FENFEN_CAI,			//分分彩：5分钟开奖，24小时不停。每天288期
	CZ_WUFEN_CAI,			//5分彩
	CZ_BJKUAICHE,			//北京赛车
	CZ_HK6HeCai,			//香港六和采
	CZ_SEVENSTAR,			//七星彩；15
	CZ_HLJ11Xuan5,			//黑龙江11选5
	CZ_LUCKY_28,			//幸运28
	CZ_KUAI_3,				//江苏快3		1-6
	CZ_GD_HAPPY10MIN,		//广东快乐10分	8 of 20
	CZ_SECOND,				//秒秒彩
	CZ_LUCKY_FARM,			//幸运农场：重庆快乐10分=21
	CZ_ERFEN_CAI,			//2分彩
	CZ_TIANJIN_SSC,			//天津时时彩
	CZCount
}CaiZhong;


//时时彩玩法种类
typedef enum 
{
	inValid,
	//QianSanZhiXuan,			//前三直选
	QianSanZhiXuan_FuShi=1,		//前三直选复式，也叫五星前三
	QianSanZhiXuan_DanShi,		//

	//HouSanZhiXuan,			//后三直选
	HouSanZhiXuan_FuShi,		//后三直选复式，也叫五星后三
	HouSanZhiXuan_DanShi,

	//QianSanZuXuan,			//前三组选
	QianSanZuXuan_ZuSanFuShi,
	QianSanZuXuan_ZuSanDanShi,
	QianSanZuXuan_ZuLiuFuShi,
	QianSanZuXuan_ZuLiuDanShi,

	//HouSanZuXuan,				//后三组选
	HouSanZuXuan_ZuSanFuShi,
	HouSanZuXuan_ZuSanDanShi,
	HouSanZuXuan_ZuLiuFuShi,
	HouSanZuXuan_ZuLiuDanShi,

	//WuXingQianEr,				//五星前二
	WuXingQianEr_FuShi,
	WuXingQianEr_DanShi,

	//WuXingHouEr,				//五星后二
	WuXingHouEr_FuShi,
	WuXingHouEr_DanShi,

	//QianErZuXuan,				//前二组选
	QianErZuXuan_FuShi,
	QianErZuXuan_DanShi,

	//HouErZuXuan,				//后二组选
	HouErZuXuan_FuShi,
	HouErZuXuan_DanShi,

	DaXiaoDanShuang,			//大小单双

	//BuDingWei,				//不定位
	BuDingWei_QianSan,
	BuDingWei_HouSan,

	DingWeiDan,					//定位胆
		
	RenXuan2,					//任选2,3,4
	RenXuan3,
	RenXuan4,
	
	QianSanHunhe,				//前三混合
	ZhongSanHunhe,				//中三混合
	HouSanHunhe,				//后三混合
	
	//新增直选复式玩法
	WuxingZhixuan,				//五星直选（0209新增）
	WuxingQianSi,				//五星前四
	WuxingHousi,				//五星后四
	WuxingZhongsan,				//五星中三（五星前三和五星后三见第一排）

	//新增组选玩法
	ZhongsanZusan,				//中三组三（0209新增）
	ZhongsanZuliu,				//中三组六
	
	//新增组选玩法
	Zuxuan120=37,				//组选120
	Zuxuan60,					//组选60
	Zuxuan30,					//组选30
	Zuxuan20,					//组选20
	Zuxuan10,					//组选10
	Zuxuan5,					//组选5
								
	//新增和值玩法
	QianSanZhiXuanHeZhi,		//前三直选和值
	HouSanZhiXuanHeZhi,			//后三直选和值
	ZhongSanZhiXuanHeZhi,		//中三直选和值

	//新增不定位
	BuDingWei_QianEr,			//不定位：前2
	BuDingWei_HouEr,			//不定位：后2

	//新增趣味玩法
	QW_YiFanFengShun,			//趣味：一帆风顺，一个一样一个数字就中奖
	QW_HaoShiChengShuang,		//趣味：好事成双，2个一样的数字才中奖
	QW_SanXingBaoXi,			//趣味：三星报喜，3个一样的数字才中奖
	QW_SiJiFaCai,				//趣味：四季发财，4个一样的数字才中奖
	NiuNiu_Num=52,				//牛牛：5个数字之和是10的倍数；牛1--牛10（牛牛） 1:10（无牛通吃）
	NiuNiu_None,				//牛牛：无牛 1:2
	NiuNiu_DxDs,				//牛牛：大小单双 1:2.5
	DaXiaoDanShuang_dw,			//定位胆大小单双;
	Zonghe_DXDSLh=56,			//总和大小单双：总和值大於等於23為大，小於等於22為小。
	SSC_LongHu,					//龙虎：头大于尾为龙，尾大于头为虎；和 ？
	QianSan_Quwei,				//前三豹子顺子对子半顺杂六
	ZhongSan_Quwei,				//中三豹子顺子对子半顺杂六
	HouSan_Quwei,				//后三豹子顺子对子半顺杂六
	HouErZhixuan_Hezhi=61,		//后二直选和值
	QianErZhixuan_Hezhi,		//前二直选和值
	RenSanZhixuan_Hezhi,		//任三直选和值	0
	RenErZhixuan_Hezhi,			//任二直选和值	0
	SSC_LongHuYaJun=65,			//龙虎：亚军
	Ren3Zu3_Fushi,				//任3组3复式	0
	Ren3Zu6_Fushi,				//任3组6复式	0
	Ren3Hunhe_Zuxuan,			//任3混合 组选	0
	Ren2Zuxuan_Fushi,			//任2组选 复式	0
	Ren2Zuxuan_Danshi,			//任2组选 单式	0

	WanFaCount
}SSCWanFaKind;



//11选5的玩法
typedef enum tagEnumIIXuan5
{
		IIinValid,
		IIRenXuan2,						//任选二
		IIRenXuan3,						//任选三
		IIRenXuan4,						//任选四
		IIRenXuan5,						//任选五
		IIRenXuan6,						//任选六
		IIRenXuan7,						//任选七
		IIRenXuan8,						//任选八
		QianYi,							//前一
		QianEr_ZhiXuan,					//前二 直选选号
		QianEr_ZuXuan,					//前二组选选号
		QianSan_ZhiXuan,				//前三直选选号
		QianSan_ZuXuan,					//前三组选选号
		IIWanFaCount
}IIXuan5GameKind;

//3D和排列三的玩法
typedef	enum	tagEnumTreeNumbers
{
	Invalid,
	enWF_ZhixuanFushi,					//三星直选复式,
	enWF_ZhixuanDanshi,					//三星直选单式,
	enWF_QiansanzusanFushi,				//前三组三复式,
	enWF_QiansanzusanDanshi,			//前三组三单式,
	enWF_QiansanzuliuFushi,				//前三组六复式,
	enWF_QiansanzuliuDanshi,			//前三组六单式,
	enWF_QianyizhixuanFushi,			//前一直选复式,
	enWF_QianerzhixuanFushi,			//前二直选复式,
	enWF_QianerzhixuanDanshi,			//前二直选单式,
	enWF_Budingwei,						//三星不定位,
	enWF_HouyizhixuanFushi,				//后一直选复式,
	enWF_HouerzhixuanFushi,				//后二直选复式,
	enWF_HouerzhixuanDanshi,			//后二直选单式,
	enWF_ZhixuanHezhi,					//三星直选和值,
	enWF_DaxiaoDanshuang,				//大小单双
	enWF_DingWeiDan,					//定位胆
	enWF_Count							//玩法个数
}TreeNumbersKind;


//HK六合彩定义
typedef	enum	tagEnumHkLiuHeCai
{
	invalid,
	TeMaDanShi,								//特码单式
	TeMaDanShuang,							//特码单双
	TeMaDaXiao,								//特码大小
	TeMaShengXiao,							//特码生肖
	TeMaBallSe,								//特码波色
	HkLiuHeCaiWanFaCount
};


// 	//七星彩玩法种类---lly
typedef enum {
	QiXingCai_LiangDing=1,
	QiXingCai_SanDing,
	QiXingCai_SiDing,
	QiXingCai_YiDing,
	QiXingCai_LiangZiXian,
	QiXingCai_SanZiXian,
}QiXingCaiKind;

//北京快8和 幸运28

typedef	enum	tagEnumBeijingKuaile8
{
	enBeijingKuaile8Invalid,
	enBeijingKuaile8Renxuan1,
	enBeijingKuaile8Renxuan2,
	enBeijingKuaile8Renxuan3,
	enBeijingKuaile8Renxuan4,
	enBeijingKuaile8Renxuan5,
	enBeijingKuaile8Renxuan6,
	enBeijingKuaile8Renxuan7,
	enBeijingKuaile8Renxuan8,
	enBeijingKuaile8Renxuan9,
	enBeijingKuaile8Renxuan10,
	enLucky28BigSmallSingleDouble,				//幸运28：大小单双玩法
	enLucky28Funny,								//幸运28：趣味玩法：对子，顺子，豹子
	enLucky28SpecialValue,						//幸运28：极值玩法
	enLucky28TeMa,								//幸运28：特码
	enBJK8_ShangXia=15,							//上下
	enBJK8_JiOu=16,								//奇偶/单双
	enBJK8_HeZhiDXDS=17,						//和值 大小单双
};


//通用投注内容转义解释如下：
#define		NSmallNumber		1				//小
#define		NSingleNumber		3				//单
#define		NDoubleNumber		2				//双
#define		NTigerNumber		4				//虎
#define		NPingNumber			5				//和=平
#define		NDragonNumber		6				//龙
#define		NHalfSerialNumber	7				//半顺 如540 790 706
#define		NSimpleNumber		8				//杂六：排除豹子，顺子，对子，半顺 即为中奖
#define		NBigNumber			9				//大
#define		NSmallSingleNumber	10				//小单
#define		NSmallDoubleNumber	11				//小双
#define		NBigSingleNumber	12				//大单
#define		NBigDoubleNumber	13				//大双
#define		NCoupleNumber		14				//对子
#define		NSerialNumber		15				//顺子（一条龙）
#define		NAllSame			16				//豹子（全相同）
#define		NLimitSmall			17				//极值小,0-5
#define		NLimitBig			18				//极值大：22-27
//快乐10分
#define		NSumDigitBig		19				//总尾大（总和各位数大）
#define		NSumDigitSmall		20				//总尾小（总和各位数小）
#define		NHeshuDa			21				//合数大（所有的个位和十位数字相加之和）
#define		NHeshuXiao			22				//合数小（所有的个位和十位数字相加之和）
#define		NZhongNumber		23				//中：開出之號碼為01、02、03、04、05、06、07
#define		NFacaiNumber		24				//發：開出之號碼為08、09、10、11、12、13、14
#define		NBaiNumber			25				//白：開出之號碼為15、16、17、18、19、20
#define		NEastNumber			26				//東：開出的號碼為01、05、09、13、17
#define		NSouthNumber		27				//南：開出的號碼為02、06、10、14、18
#define		NWestNumber			28				//西：開出的號碼為03、07、11、15、19
#define		NNorthNumber		29				//北：開出的號碼為04、08、12、16、20
//快乐8
#define		NFrontMore			30				//前多：前盤號碼（01-40）比後盤號碼是（41~80）個數多時
#define		NBackMore			31				//后多：後盤號碼（41-80）比前盤號碼是（01~40）個數多
#define		NEqualNumber		32				//前后和：前盘号码跟后盘号码个数相同
#define		NSingleMore			33				//单多：開出的20個號碼中，单數號碼比双數號碼個數多
#define		NDoubleMore			34				//双多：開出的20個號碼中，双數號碼比单數號碼個數多
#define		NEqualDigits		35				//单双数相同
//開出的20個號碼相加的总和值分在5個段，以金、木、水、火、土命名：
//金（210～695）、木（696～763）、水（764～855）、火（856～923）和土（924～1410）
#define		NSumGold			36				//金
#define		NSumWood			37				//木
#define		NSumWater			38				//水
#define		NSumFire			39				//火
#define		NSumEarth			40				//土

//	北京赛车PK10
typedef enum 
	{
		PKinValid,
 
		WF_GuanJun,		//冠军
		WF_GuanYaJun,	//冠亚军
		WF_YaJun,		//亚军
		WF_QianSan,		//	前三
		WF_DiSan,		//第三
		WF_QianSi,		//	前四
		WF_DiSi,		//第四
		WF_QianWu,		//	前五
		WF_DiWu,		//第五
		WF_QianLiu,		//	前六
		WF_DiLiu,		//第六
		WF_QianQi,		//	前七
		WF_DiQi,		//第七
		WF_QianBa,		//	前八
		WF_DiBa,		//第八
		WF_QianJiu,		//	前九
		WF_DiJiu,		//第九
		WF_QianShi,		//	前十
		WF_DiShi,		//第十
		WF_DXDS,		//大小单双龙虎（PK10没有和）
		WF_DWD,			//定位胆
		WF_GYHZ,		//冠亚和值：下注为大小单双
		PK10WanFaCount
	}PK10GameKind;


//快乐10分
typedef		enum 
{
	WF_Invalid,
	WF_DWD_Danma,		//定位胆；单码
	WF_DWD_DXDS,		//定位大小单双
	WF_HeZhi_DXDS,		//和值大小单双；尾大，尾小
	WF_Tail_DX,			//尾大，尾小
	
}Happy10GameKind;



//基本用户信息-登录后常在主界面显示
typedef struct _BaseUser{
	int user_id;
	TCHAR account[ACCOUNT_LENGTH];
	int type;
	int Scoretype;	//1元   2角  3分	4=厘
	double	fandian;
	double yue;
	double dongjie;
}BaseUser;

//我的账户-用户信息页面显示
typedef struct _WDZHUserInfo{
	int user_id;
	TCHAR account[ACCOUNT_LENGTH];
	double fandian;
	TCHAR	last_ip[IP_LENGTH];
	TCHAR	last_time[TIME_LENGTH];
	TCHAR current_ip[IP_LENGTH];
	TCHAR	qq[QQ_LENGTH];
	TCHAR	shangji_qq[QQ_LENGTH];
	double	yongjin;
	double yongjin_ze;
	double	kuisunyongjin;
	double kuisunyongjin_ze;
	double fenhong;
	double	fenhong_ze;
	double qipai_ye;
	//double	yingkui_ze;
	SYSTEMTIME	t_fh_time;
	SYSTEMTIME	t_ksyj_time;
	SYSTEMTIME t_yj_time;
}WDZHUserInfo;

//DingDan
typedef struct _TouZhuOrder{
	TCHAR haomao[60];
	int zhushu;
	int beishu;
	int jine;
}TouZhuOrder;

typedef struct _KjXinXi {
	TCHAR qihao[KJ_QIHAO_LENGTH];
	TCHAR haoma[KJ_HAOMA_LENGTH];
	TCHAR shijian[KJ_SHIJIAN_LENGTH];
}KjXinXi;

typedef struct _topuser {
	int t_id;
	int user_id;
	int gametype;
	int gamekind;
	double jiangjin;
}TopUser;

typedef struct _zhongjiang {
	int t_type;
	CString t_qishu;
	int zhushu;
}Zhongjiang;



#ifdef	_DEBUG

	#define		OUT_TEXT	OutputDebugString

#else
	#define	OUT_TEXT	
#endif

class LogFile
{
public:
	LogFile();
	~LogFile();

	int		LogText(LPCTSTR lpszFormat,...);
	int		LogText(CString& strLog);

	
	TCHAR*			GetWorkFolder();
	static	LogFile &instance();
private:

	TCHAR	m_szPath[255];

};

