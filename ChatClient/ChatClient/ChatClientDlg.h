
// ChatClientDlg.h : ͷ�ļ�
//

#pragma once
#include <string>
#include <list>
#include "afxwin.h"
using namespace std;

#pragma comment(lib,"ws2_32.lib")

enum Msg_Type
{
	Msg_Min,
	Msg_Login,
	Msg_LoginOut,
	Msg_Chat,
	Msg_ChatToMe,
	Msg_ChatToOther,
	Msg_Max,
};

// CChatClientDlg �Ի���
class CChatClientDlg : public CDialogEx
{
// ����
public:
	CChatClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CHATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	bool Connect(SOCKET sock);
	static void Receive(void *p);
	bool ConnectServer();
	void SendToServer(Msg_Type MsgType, wstring str = L"");
	void RecvMessage(wstring bufStr);
	void UpdateMsg();

	list<wstring> m_listUser;

	list<wstring> m_listMsg;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeMsginfo();
	CListBox m_listUserCtrl;
	afx_msg void OnBnClickedSendmsg();
	CEdit m_chatText;
};
