#pragma once
#include "RoomManager.h"
#include <unordered_map>
#include <atomic>
#include "User.h"


enum class ROOM_STATE
{
	FREE = 0, IN_ROBBY_CREATED = 1, IN_ROBBY_FULLED = 2, LOADING = 3, INGAME = 4, WAITING_FOR_RESET = 5,

	DISCONNECTING = 99
};

constexpr char HOST = 0;
constexpr char GUEST = 1;


class cRoom
{
	friend class cRoomManager;

public:
	cRoom() {};
	cRoom(unsigned int _id, ROOM_STATE _state, UserRef _user)
	{
		m_id = _id;
		m_state = _state;
		m_users[HOST] = _user;
	};
	~cRoom() {};

	void Init();

	void InitObjects();
	void StartGame();

	void UserLoadingComplete(const unsigned int _user_id);
	void SendPlayerData();
	void SendAllObjectData();
	void ShootBullet(UserRef _user, iVector3 _source_position, sRotation3 _rotation);

	void Broadcast(int _size, void* _mess);

	void StateLock() { EnterCriticalSection(&m_state_cs); }
	void StateUnlock() { LeaveCriticalSection(&m_state_cs); }
	
	void Disconnect(UserRef _user);

protected:
	unsigned int        m_id;
	ROOM_STATE          m_state;
	CRITICAL_SECTION    m_state_cs;

public:
	//unsigned int        m_user_id[2] = { MAX_USER,MAX_USER };
	UserRef m_users[2] = { nullptr,nullptr };

	unordered_map <unsigned int, cGameObject*> m_game_objects;

private:
	static int          m_last_object_id;
};




