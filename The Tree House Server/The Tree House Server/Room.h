#pragma once
#include "RoomManager.h"
#include <mutex>
#include "User.h"

namespace room_state
{
	enum eSTATE { FREE = 0, IN_ROBBY = 1, IN_GAME=2 };	
}
namespace user_type
{
	enum eUSER_TYPE { HOST = 0, GUEST = 1 };
}

class cRoom
{
public:

	void StartGame();

	void SendPlayerTransform();

	void SetCharacterPosition(const unsigned short& _user_id, const short& _x, const short& _y, const short& _z);
	void SetCharacterRotation(const unsigned short& _user_id, const short& _pitch, const short& _yaw, const short& _roll);
	void SetCharacterTransform(const unsigned short& _user_id, const short& _x, const short& _y, const short& _z,
		const short& _pitch, const short& _yaw, const short& _roll);

	cRoom();
	~cRoom();

public:
	char                m_room_id;
	room_state::eSTATE  m_state;
	//mutex m_state_lock;
	cUser*              m_users[2];


	friend cRoomManager;
};




