#pragma once
#include "RoomManager.h"
#include <unordered_map>
#include <atomic>
//#include <User.h>

namespace room_state
{
	enum eSTATE { FREE = 0, IN_ROBBY_CREATED = 1, IN_ROBBY_FULLED = 2, LOADING = 3, INGAME = 4, WAITING_FOR_RESET = 5 };
}
namespace user_type
{
	enum eUSER_TYPE { HOST = 0, GUEST = 1 };
}



class cRoom
{
	friend class cRoomManager;

public:
	cRoom() {};
	cRoom(unsigned int _id, room_state::eSTATE _state, const unsigned int _host_id) :
		m_id(_id), m_state(_state) {
		m_user_id[user_type::HOST] = _host_id;
	};
	~cRoom() {};

	void InitObjects();
	void StartGame();

	void UserLoadingComplete(const unsigned int _user_id);
	void SendOtherPlayerTransform();
	void SendAllObjectData();
	void ShootBullet(const unsigned int& _user_id, iVector3 _source_position, sRotation3 _rotation);

	void Broadcast(int _size, void* _mess);

	void StateLock() { EnterCriticalSection(&m_state_cs); }
	void StateUnlock() { LeaveCriticalSection(&m_state_cs); }
	

protected:
	unsigned int        m_id;
	room_state::eSTATE  m_state;
	CRITICAL_SECTION    m_state_cs;

public:
	unsigned int        m_user_id[2] = { MAX_USER,MAX_USER };
	//cUser*              m_users[2];

	unordered_map <unsigned int, cGameObject*> m_game_objects;

private:
	static int          m_last_object_id;
};




