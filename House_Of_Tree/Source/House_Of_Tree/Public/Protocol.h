#pragma once

const short SERVER_PORT = 6000;
const int   BUFSIZE = 256;


namespace CS_PACKET
{
	constexpr char CS_PLAYER_DATA = 0;
};

namespace SC_PACKET
{
	constexpr char SC_LOGINOK = 0;
	constexpr char SC_PUT_PLAYER = 1;
	constexpr char SC_PUT_OBJECT = 2;
	constexpr char SC_REMOVE_OBJECT = 3;
	constexpr char SC_PLAYER_DATA = 4;
};

#pragma pack (push, 1)
//----------cs packet------------//
//-------------------------------//

struct cs_player_data_packet
{
	unsigned char size;
	unsigned char type;

	unsigned short id;

	int x;
	int y;
	int z;
	short pitch;
	short yaw;
	short roll;
};

//----------sc packet------------//
//-------------------------------//
struct sc_loginok_packet
{
	unsigned char size;
	unsigned char type;

	unsigned short id;
};

struct sc_put_player_packet
{

	unsigned char size;
	unsigned char type;

	unsigned char character_type; 
	unsigned short id;

	int x;
	int y;
	int z;
	short pitch;
	short yaw;
	short roll;
};

struct sc_put_object_packet
{
	unsigned char size;
	unsigned char type;

	unsigned short object_type;
	unsigned short id;

	int x;
	int y;
	int z;
	short pitch;
	short yaw;
	short roll;
};

struct sc_remove_object_packet
{
	unsigned char size;
	unsigned char type;

	unsigned short id;
};

struct sc_player_data_packet
{ 
	unsigned char size;
	unsigned char type;

	unsigned short id;

	int x;
	int y;
	int z;
	short pitch;
	short yaw;
	short roll;
};
#pragma pack(pop)