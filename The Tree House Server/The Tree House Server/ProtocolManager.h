#pragma once
#include <iostream>
#include <functional>
#include "Protocol.h"

using PacketManagerFunc = std::function<bool(void* _buff, int _len)>;
extern PacketManagerFunc g_server_packet_manager[128];

bool Error(void* _packet, int _len);
bool CSCreateRoom(void* _packet, int _len);
bool CSJoinRoom(void* _packet, int _len);
bool CSJoinRandomRoom(void* _packet, int _len);
bool CSReadyGame(void* _packet, int _len);
bool CSChangeSeletecCharacter(void* _packet, int _len);
bool CSLoadingComplete(void* _packet, int _len);
bool CSplayerData(void* _packet, int _len);
bool CSShootBullet(void* _packet, int _len);
bool CSBulletHit(void* _packet, int _len);


class cPacketManager
{
public:
	static void Init()
	{
		g_server_packet_manager[CS_PACKET::NONE] = Error;
		g_server_packet_manager[CS_PACKET::CS_CREATE_ROOM] = CSCreateRoom;
		g_server_packet_manager[CS_PACKET::CS_JOIN_ROOM] = CSJoinRoom;
		g_server_packet_manager[CS_PACKET::CS_JOIN_RANDOM_ROOM] = CSJoinRandomRoom;
		g_server_packet_manager[CS_PACKET::CS_READY_GAME] = CSReadyGame;
		g_server_packet_manager[CS_PACKET::CS_CHANGE_SELECTED_CHARACTER] = CSChangeSeletecCharacter;
		g_server_packet_manager[CS_PACKET::CS_LOADING_COMPLETE] = CSLoadingComplete;
		g_server_packet_manager[CS_PACKET::CS_PLAYER_DATA] = CSplayerData;
		g_server_packet_manager[CS_PACKET::CS_SHOOT_BULLET] = CSShootBullet;
		g_server_packet_manager[CS_PACKET::CS_BULLET_HIT] = CSBulletHit;
	}
	static bool ProcessPacket(void* _packet, int _len)
	{
		unsigned char packet_type = reinterpret_cast<packet_header*>(_packet)->type;
		return g_server_packet_manager[packet_type](_packet, _len);
	}
};
