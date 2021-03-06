#pragma once
#include <unordered_map>
#include <mutex>
#include "ObjectPool.h"
#include "GameObject.h"
#include "Character.h"
#include "Bullet.h"
#include "User.h"
#include "Monster.h"


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
	// 메모리풀 이용해서 통일 무조건 필요
	static cObjectPool<cGameObject>       m_object_pool;
	static cObjectPool<cCharacter>        m_character_pool;
	static cObjectPool<cBullet>           m_bullet_pool;
	static cObjectPool<cMonster>          m_monster_pool;

	static atomic <unsigned int>          m_last_object_id;
private:
	
};

