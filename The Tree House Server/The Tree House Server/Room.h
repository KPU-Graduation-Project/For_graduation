#include "RoomManager.h"

enum E_ROOM_STATE { FREE = 0, IN_ROBBY = 1 };
class CRoom
{
public:
	CRoom();
	~CRoom();

private:


	friend CRoomManager;
};