#pragma once
#include "ExpOver.h"
#include "UserManager.h"

enum E_USER_STATE { FREE = 0, IN_ROBBY = 1, IN_MATCHING_ROOM = 2, IN_GAME = 3 };

class CUser
{
protected:
	CUser();
	~CUser();

	unsigned short GetID();
	E_USER_STATE GetState();
	unsigned short GetRoomNum();

	void SetID(const unsigned short _id);
	void SetState(const E_USER_STATE _state);
	void SetRoomNum(const unsigned short _room_num);

private:
	SOCKET             m_socket;
	CEXP_OVER          m_recv_over;
	int		           m_prev_size;

	unsigned short     m_id;
	E_USER_STATE       m_state;
	unsigned short     m_room_num;

	//mutex              m_state_lock;

	friend CUserManager;
};