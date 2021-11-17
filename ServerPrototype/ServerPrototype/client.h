#pragma once
#include <iostream>
#include <mutex>
#include "protocol.h"
#include "exp_over.h"
#include "player.h"

using namespace std;

//client state
enum STATE { ST_FREE, ST_ACCEPT, ST_INGAME };

class CClient
{
public:	
	void InitRecvOver();
	void DoSend(int num_bytes, void* mess);
	void DoRecv();
	void CloseSocket();
	
	void Lock();
	void Unlock();

	void SetState(STATE state);
	void SetID(const short id);
	void SetName(const char* name);
	void SetRoomNum(const short room_num);
	void SetChar(const short room_num);   // true: 1P  false: 2P
	void SetPrevSize(const int prev_size);
	void SetSocket(const SOCKET& socket);
	

	STATE GetState();
	int  GetID();
	short GetRoomNum();
	bool GetChar();                 // true: 1P  false: 2P
	int GetPrevSize();	

	void ErrorDisplay(int err_no);

	explicit CClient();
	explicit CClient(const char* name, short id);
	~CClient();

private:
	STATE                 m_state = ST_FREE;
	short		              m_id;                 // �÷��̾� �ĺ�Ű
	char	              m_name[MAX_NAME_SIZE];// �÷��̾� �г���
	short                 m_room_num = -1;      // ���� ��Ī�� �� ��ȣ, ���� ��Ī ������ �� = -1. ���߸�Ī �������� ���ȣ 0����
	bool                  m_is_char1 = true;    // true: 1P  false: 2P
	
	mutex                 m_state_lock;
	CEXP_OVER             m_recv_over;
	SOCKET                m_socket;
	int		              m_prev_size;

	CPlayer               m_player;
};