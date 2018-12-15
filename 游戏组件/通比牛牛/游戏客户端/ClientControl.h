#pragma once


//游戏控制基类
class IClientControlDlg : public CSkinDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CSkinDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

	//设置房间库存
	virtual void SetRoomStorage(LONGLONG lRoomStartStorage, LONGLONG lRoomCurrentStorage) = NULL;
	virtual void SetStorageInfo(CMD_S_ADMIN_STORAGE_INFO *pStorageInfo) = NULL;
	//查询用户结果
	virtual void RequestQueryResult(CMD_S_RequestQueryResult *pQueryResult) = NULL;
	//房间用户控制结果
	virtual void RoomUserControlResult(CMD_S_UserControl *pUserControl) = NULL;
	//用户控制完成
	virtual void UserControlComplete(CMD_S_UserControlComplete *pUserControlComplete) = NULL;
	//操作记录
	virtual void UpdateOperationRecord(CMD_S_Operation_Record *pOperation_Record) = NULL;
};
