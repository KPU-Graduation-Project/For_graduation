#include "stdafx.h"
#include "ProtocolManager.h"


PacketManagerFunc g_server_packet_manager[128];

bool Error(void* _packet, int _len)
{
	cout << "Invalid Packet\n";
	return false;
}

bool CSCreateRoom(void* _packet, int _len)
{
	return false;
}

bool CSJoinRoom(void* _packet, int _len)
{


	return false;
}

bool CSJoinRandomRoom(void* _packet, int _len)
{


	return false;
}

bool CSReadyGame(void* _packet, int _len)
{


	return false;
}

bool CSChangeSeletecCharacter(void* _packet, int _len)
{


	return false;
}

bool CSLoadingComplete(void* _packet, int _len)
{


	return false;
}

bool CSplayerData(void* _packet, int _len)
{


	return false;
}

bool CSShootBullet(void* _packet, int _len)
{


	return false;
}

bool CSBulletHit(void* _packet, int _len)
{


	return false;
}
