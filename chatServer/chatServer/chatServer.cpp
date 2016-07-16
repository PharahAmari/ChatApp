// chatServer.cpp : �������̨Ӧ�ó������ڵ㡣
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"
#include "windows.h"
#include "process.h"
#include <iostream>
#include <list>
#include <string>
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


//��Ϣ��Ϣ�ṹ��
struct stUserInfo
{
	SOCKET sock; 
	wstring strName;
};

list<stUserInfo> g_listUser;// �����������ߵ��û���Ϣ


bool IsExit(wstring name)
{
	for (list<stUserInfo>::iterator it = g_listUser.begin();
		it != g_listUser.end(); it++)
	{
		if (it->strName == name)
		{
			return true;
		}
	}
	return false;
}

wstring GetNewName(stUserInfo userInfo)
{
	//����µ�����
	wchar_t newName[100];
	int index = 1;
	while (1)
	{
		wsprintf(newName, L"%s%d", userInfo.strName.c_str(), index);
		if (!IsExit(newName))
		{
			break;
		}
		index++;
	}
	wstring str = newName;
	return str;
}

void SendToClient(Msg_Type msgType, SOCKET sock, wstring strName)
{
	if (msgType <= Msg_Min || msgType >= Msg_Max)
	{
		return;
	}
	wchar_t sendBuf[1024];
	switch (msgType)
	{
	case Msg_Login:
		{
			wsprintf(sendBuf, L"%d_%s", msgType, strName.c_str());
			break;
		}
	case Msg_LoginOut:
		{
			wsprintf(sendBuf, L"%d_%s", msgType, strName.c_str());
			break;
		}
	case Msg_ChatToMe:
	case Msg_ChatToOther:
		{
			wsprintf(sendBuf, L"%d_%s", msgType, strName.c_str());
			break;
		}
	}
	send(sock, (char*)sendBuf, lstrlen(sendBuf)*2, 0);
}


void SyncUserInfo(stUserInfo userInfo)
{
	//1��ͬ���Լ�������������
	//2��ͬ�����������˸���
	for (list<stUserInfo>::iterator it = g_listUser.begin();
		it != g_listUser.end(); ++it)
	{
		//���ð��Լ�ͬ�����Լ�
		if (userInfo.sock == it->sock)
		{
			continue;
		}
		SendToClient(Msg_Login, userInfo.sock, it->strName);
		SendToClient(Msg_Login, it->sock,userInfo.strName);
	}
	
}

bool GetUserByName(wstring name, stUserInfo& userInfo)
{
	for (list<stUserInfo>::iterator it = g_listUser.begin();
		it != g_listUser.end(); ++it)
	{
		if (it->strName == name)
		{
			userInfo = *it;
			return true;
		}
	}
	return false;
}

bool GetUserBySocket(SOCKET sock, stUserInfo& userInfo)
{
	for (list<stUserInfo>::iterator it = g_listUser.begin();
		it != g_listUser.end(); ++it)
	{
		if (it->sock == sock)
		{
			userInfo = *it;
			return true;
		}
	}
	return false;
}

void Chat(SOCKET sockSend, wstring nameSendTo, wstring strText)
{
	//����Ϣ���͸�nameSendTo
	stUserInfo userSendTo;
	if (!GetUserByName(nameSendTo, userSendTo))
	{
		return;
	}
	stUserInfo userSend;
	if (!GetUserBySocket(sockSend, userSend))
	{
		return;
	}

	wstring strBuf;
	strBuf = userSend.strName + L"_" + userSendTo.strName + L"_" + strText;
	//�����Լ�  ��userSendTo������һ����Ϣ
	SendToClient(Msg_ChatToMe, userSendTo.sock, strBuf);
	//���͸�������
	SendToClient(Msg_ChatToOther, userSend.sock, strBuf);
}

void RecvMessage(SOCKET sock,wstring strBuf)
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
	else  //ֻ������Ϣ����
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
			//������Ϣ ����SOCKET��Ϣ������
			stUserInfo userInfo;
			userInfo.sock = sock;
			userInfo.strName = strBuf;

			//��һ̨���������
			if (IsExit(userInfo.strName))
			{
				userInfo.strName = GetNewName(userInfo);
			}

			//����û���Ϣ��list
			g_listUser.push_back(userInfo);
			//ͬ����Ϣ
			SyncUserInfo(userInfo);

		}
	case Msg_Chat:
		{
					 //��÷���Ϣ�Ķ����������Ϣ
					 pos = strBuf.find('_');

					 if (pos == -1)
					 {
						 return;
					 }
					 wstring strName = strBuf.substr(0, pos);
					 wstring strText = strBuf.substr(pos + 1);
					 Chat(sock, strName, strText);
					 break;
		}
	}
}




void LoginOut(SOCKET sock)
{
	for (list<stUserInfo>::iterator it = g_listUser.begin();
		it != g_listUser.end(); ++it)
	{
		if (it->sock == sock)
		{
			//����Ϣ���͸�������  ����������
			for (list<stUserInfo>::iterator it2 = g_listUser.begin();
				it2 != g_listUser.end(); ++it2)
			{
				if (it2->sock = it->sock)
				{
					SendToClient(Msg_LoginOut, it2->sock, it->strName);
				}
			}
			g_listUser.erase(it);
			break;
		}
	}
}

void receive(PVOID param)
{
	SOCKET sock = *((SOCKET*)param);
	char buf[2048] = { 0 };
	int bytes;
	while (1)
	{
		//��������
		if (bytes = recv(sock, buf, sizeof(buf), 0) == SOCKET_ERROR)
		{
			_endthread();
			return;
		}
		buf[bytes] = '\0';
		wchar_t bufTest[1024];
		memcpy(bufTest, buf, bytes);
		bufTest[bytes/2] = '\0';
		RecvMessage(sock, bufTest);//������Ϣ
	}
}

void ReceiveConnectThread(void* param)
{
	SOCKET socketServer = *((SOCKET*)param);
	while (1)
	{
		SOCKET clientSocket;  //�������տͻ�������
		struct sockaddr_in clientAddress; //�׽ӵĵ�ַ���˿�
		memset(&clientAddress, 0, sizeof(clientAddress));
		int addrLen = sizeof(clientAddress);
		if (INVALID_SOCKET == (clientSocket = accept(socketServer, (sockaddr*)&clientAddress, &addrLen)))
		{
			//���տͻ�������ʧ��
			return;
		}

		//���ӳɹ��ȴ�������Ϣ
		_beginthread(receive, 0, &clientSocket);

	}
}


SOCKET StartServer()
{
	SOCKET serverSocket;
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		//�׽��ִ���ʧ��
		return -1;
	}
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(sockaddr_in));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = htons(INADDR_ANY);
	serverAddress.sin_port = htons(1900);

	//��
	if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		//��ʧ��
		return 0;
	}

	//�������Կͻ��˵�����
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		//����ʧ��
		return 0;
	}

	return serverSocket;
}


int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		//��ʼ���׽���ʧ��
		return -1;
	}

	SOCKET sock = StartServer();//����������

	_beginthread(ReceiveConnectThread, 0, &sock); //����������һ���߳̽��տͻ��˵�����

	char buf[1024];
	while (1)
	{
		gets_s(buf);
	}
	system("pause");
	return 0;
}