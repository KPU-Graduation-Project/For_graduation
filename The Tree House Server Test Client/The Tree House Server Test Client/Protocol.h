#pragma once
#include "Struct.h"

const short SERVER_PORT = 6000;
const int   BUFSIZE = 512;

namespace CS_PACKET
{
	const char CS_PLAYER_DATA = 0;
};

namespace SC_PACKET
{
	const char SC_PLAYER_DATA = 0;
};

struct cs_player_data
{
	PacketInfo info;
	SVector3 player_position;
	SVector3 player_rotation;

};



struct sc_player_data
{
	PacketInfo info;
	unsigned int id;
	SVector3 player_position;
	SVector3 player_rotation;

};