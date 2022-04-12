#pragma once
#include <unordered_map>
#include <mutex>
#include "User.h"


class cRoom;

class cRoomManager
{
public:
	cRoomManager();
	~cRoomManager();

	void Init();
	void InitRooms();
	unsigned char CreateRoom(const unsigned short _user_id, cUser* _user_p);
	bool JoinRoom(const unsigned short _user_id, const unsigned short _room_id, cUser* _user_p);
	//UsersInRoom GetPlayersID(const unsigned char _room_id);

public:
	unordered_map<unsigned short, cRoom*> m_rooms;

private:
	unsigned short m_last_id;
	mutex          m_last_id_lock;
	//cRoom m_rooms[MAX_ROOM];
	//cRoom* m_rooms;
};