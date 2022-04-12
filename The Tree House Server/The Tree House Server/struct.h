#pragma once
#include <iostream>
#pragma pack (push, 1)
struct PacketInfo
{
	unsigned char size;
	unsigned char type;
};

struct iVector3
{
	int x;
	int y;
	int z;
};

struct sRotation3
{
	short pitch;
	short yaw;
	short roll;
};

struct Transform
{
	iVector3   position;
	sRotation3 ratation;
};


#pragma pack(pop)