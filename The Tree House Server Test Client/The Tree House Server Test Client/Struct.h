#pragma once

constexpr int MAX_USER = 10;
constexpr int MAX_NPC = 10;

struct PacketInfo
{
	unsigned char size;
	unsigned char type;
};

struct SVector3
{
	short x;
	short y;
	short z;
};

struct UsersInRoom
{
	unsigned int user0;
	unsigned int user1;
};