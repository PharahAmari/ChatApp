
// ChatClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SOCKET g_clientSock = 0;//��ʾ�˺ͷ���������

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChatClientDlg �Ի���



CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChatClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listUserCtrl);
	DDX_Control(pDX, IDC_MsgText, m_chatText);
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_EN_CHANGE(IDC_MsgInfo, &CChatClientDlg::OnEnChangeMsginfo)
	ON_BN_CLICKED(IDC_SendMsg, &CChatClientDlg::OnBnClickedSendmsg)
END_MESSAGE_MAP()


// CChatClientDlg ��Ϣ�������

BOOL CChatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	if (!ConnectServer())
	{
		MessageBox(L"���ӷ�����ʧ��",L"��ʾ",MB_OK);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CChatClientDlg::OnEnChangeEdit1()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}
//
//
//void CChatClientDlg::OnEnChangeMsginfo()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//}


void CChatClientDlg::Receive(void *p)
{
	char buf[2048] = { 0 };
	CChatClientDlg* pDlg = (CChatClientDlg*)p;  //�����ڵ�ָ��
	if (pDlg == NULL)
	{
		return;
	}

	while (1)  //һֱ���շ���������Ϣ
	{
		int bytes;
		//���������û�з���Ϣ��һֱ�ȴ�
		if ((bytes = recv(g_clientSock, buf, sizeof(buf),0)) == SOCKET_ERROR)
		{
			_endthread();
			return;
		}
		buf[bytes] = '\0';
		wchar_t bufTest[1024];
		memcpy(bufTest, buf, bytes);
		bufTest[bytes/2] = '\0';

		pDlg->RecvMessage(bufTest);
	}
	return;
}

bool CChatClientDlg::ConnectServer()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//cout<<"�����׽���ʧ��"<<endl;
		return false;
	}

	SOCKET clientSocket;
	//����socketʧ��
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		return false;
	}

	g_clientSock = clientSocket;
	if (!Connect(clientSocket))  //���ӷ�����ʧ��
	{
		return false;
	}

	_beginthread(&CChatClientDlg::Receive, 0, this);//��������   ����һ���߳�׼��������Ϣ
	
		
	//ͬʱ ������Ϣ��������  ���߷������ҵ�ip(�����ü������)
	SendToServer(Msg_Login);

	return true;
}


bool CChatClientDlg::Connect(SOCKET sock)
{
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(1900);

	//��ʼ����
	if (connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		//�ͷ���������ʧ��
		return false;
	}
	return true;
}

void CChatClientDlg::SendToServer(Msg_Type MsgType, wstring str)
{
	wchar_t sendBuf[1024];
	switch (MsgType)
	{
	case Msg_Login:
		{
			wchar_t computerName[MAX_COMPUTERNAME_LENGTH];
			DWORD len = MAX_COMPUTERNAME_LENGTH;
			GetComputerName(computerName, &len);
			wsprintf(sendBuf, L"%d_%s", Msg_Login, computerName);
			break;
		}
	case Msg_Chat:
		{
			wsprintf(sendBuf, L"%d_%s", Msg_Chat, str.c_str());
			m_chatText.SetWindowTextW(L"");  //������ؼ����
			break;
		}
		
	}
	send(g_clientSock, (char*)sendBuf, lstrlen(sendBuf) * 2, 0);
}

void CChatClientDlg::RecvMessage(wstring strBuf)
{
	//�����������Ĳ������ͺ���Ϣ����
	int pos = strBuf.find('_');
	int msgType = Msg_Min;
	if (pos != -1)
	{
		wstring bufTemp = strBuf.substr(0, pos);
		strBuf = strBuf.substr(pos + 1);//strbuf��Ϊ��Ϣ����
		char* temp = (char*)bufTemp.c_str();
		msgType = atoi(temp); // msgType
	}
	else
	{
		char* temp = (char*)strBuf.c_str();
		msgType = atoi(temp);
	}
	if (msgType <= Msg_Min || msgType >= Msg_Max)
	{
		return;
	}

	switch (msgType)
	{
	case Msg_Login:
		{
			//ĳ���û���¼��  ��ӵ�list
			m_listUser.push_back(strBuf);
			//��ʾ�������б�
			m_listUserCtrl.AddString(strBuf.c_str());

			Sleep(1);
		}
	case Msg_LoginOut:
		{
			wstring loginOutName = strBuf;
			int index = m_listUserCtrl.FindString(-1,loginOutName.c_str());
			if (index != -1)
			{
				m_listUserCtrl.DeleteString(index);
			}
			break;
		}
	case Msg_ChatToMe:
	case Msg_ChatToOther:
		{
			pos = strBuf.find('_');
			if (pos == -1)
			{
				return;
			}
			wstring strNameSend = strBuf.substr(0, pos); //������
			strBuf = strBuf.substr(pos + 1);
			pos = strBuf.find('_');
			if (pos == -1){
				return;
			}
			wstring strNameSendTo = strBuf.substr(0,pos);//������
			wstring strText = strBuf.substr(pos + 1);//��Ϣ����
			wstring msg;
			if (msgType == Msg_ChatToMe)
			{
				msg = L"��" + strNameSend + L"������˵��" + strText;
			}
			else
			{
				msg = L"��ԡ�" + strNameSendTo + L"��˵��" + strText;
			}
			m_listMsg.push_back(msg);
			UpdateMsg();
			break;
		}
		
	}
}

void CChatClientDlg::UpdateMsg()
{
	if (m_listMsg.size() > 200)
	{
		m_listMsg.pop_front();
	}
	wstring strText;
	for (list<wstring>::iterator it = m_listMsg.begin();
		it != m_listMsg.end(); ++it)
	{
		strText = strText + (*it) + L"\r\n";
	}
	SetDlgItemText(IDC_MsgInfo,strText.c_str());
}

void CChatClientDlg::OnBnClickedSendmsg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_chatText.GetWindowTextW(str);
	if (str.GetLength() == 0)
	{
		MessageBox(L"��Ϣ����Ϊ��",L"��ʾ",MB_OK);
		return;
	}
	if (str.GetLength() >= 800)
	{
		MessageBox(L"��Ϣ���ܳ���800", L"��ʾ", MB_OK);
		return;
	}

	//ȷ��Ҫ���͵���
	int index = m_listUserCtrl.GetCurSel();
	if (index == -1)
	{
		MessageBox(L"��ѡ��Ҫ���͵Ķ���", L"��ʾ", MB_OK);
		return;
	}

	CString name;
	m_listUserCtrl.GetText(index, name);

	wstring strTemp = name;
	wstring strText = str;
	strTemp = strTemp + L"_" + strText;

	//ͨ��������ת����������
	SendToServer(Msg_Chat, strTemp);

}
