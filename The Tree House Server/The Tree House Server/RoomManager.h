#pragma once
#include <unordered_map>
#include <mutex>
#include "ObjectPool.h"
#include "GameObject.h"
#include "Character.h"
#include "Bullet.h"
#include "User.h"



class cRoomManager
{
	friend cRoom;

public:

	cRoomManager() { Init(); };
	~cRoomManager() {};

	void Init();
	void InitRooms();
	unsigned int CreateRoom(UserRef _user);
	bool JoinRoom(UserRef _user, const unsigned int _room_id);

public:
	static unordered_map<unsigned int, RoomRef> m_rooms;

protected:
	static cObjectPool<cGameObject>       m_object_pool;
	static cObjectPool<cCharacter>        m_character_pool;
	static cObjectPool<cBullet>           m_bullet_pool;
private:
	
};

