#include "stdafx.h"
#include "Global.h"
#include "RoomManager.h"


cRoomManager* g_room_manager=nullptr;


class cGlobal
{
public:
	cGlobal() 
	{
		g_room_manager = new cRoomManager();
	
	};

	~cGlobal()
	{
		delete g_room_manager;
	};

private:


};


cGlobal g_global;