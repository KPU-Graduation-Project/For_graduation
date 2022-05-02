#pragma once
#include <unordered_map>
#include <mutex>
#include "ObjectPool.h"
#include "GameObject.h"
#include "Character.h"
#include "Bullet.h"
#include "User.h"

class cRoom;

class cRoomManager
{
	friend cRoom;

public:

	cRoomManager() { Init(); };
	~cRoomManager() {};

	void Init();
	void InitRooms();
	unsigned int CreateRoom(cUser* _user);
	bool JoinRoom(cUser* _user, const unsigned int _room_id);

public:
	static unordered_map<unsigned int, cRoom*> m_rooms;

protected:
	static cObjectPool<cGameObject>       m_object_pool;
	static cObjectPool<cCharacter>        m_character_pool;
	static cObjectPool<cBullet>           m_bullet_pool;
private:
	
};

extern cRoomManager g_room_manager;