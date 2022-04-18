#pragma once
#include <unordered_map>
#include <mutex>


class cRoom;

class cRoomManager
{
public:
	cRoomManager();
	~cRoomManager();

	void Init();
	void InitRooms();
	unsigned int CreateRoom(const unsigned int _user_id);
	bool JoinRoom(const unsigned int _user_id, const unsigned int _room_id);

public:
	static unordered_map<unsigned int, cRoom*> m_rooms;

private:
	unsigned int     m_last_id;
	CRITICAL_SECTION m_last_id_cs;
};