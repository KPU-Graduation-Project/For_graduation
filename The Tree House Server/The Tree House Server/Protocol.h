#pragma once
#include "struct.h"

const short SERVER_PORT = 6000;
const int   BUFSIZE = 512;


struct CS_PLAYER_DATA
{
	PacketInfo info;
	SVector3 player_position;
	SVector3 player_rotation;

};
