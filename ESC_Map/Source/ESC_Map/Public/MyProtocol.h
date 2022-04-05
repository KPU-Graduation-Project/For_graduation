// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

constexpr int NetValue = 1;

namespace CS_PACKET
{
	const char CS_PLAYER_DATA = 0;
};

namespace SC_PACKET
{
	const char SC_PLAYER_DATA = 0;
};

#pragma pack (push, 1)
//----------cs packet------------//
//-------------------------------//

struct cs_player_data_packet
{
	unsigned char size;
	unsigned char type;

	//unsigned short id;
	short x;
	short y;
	short z;
	short pitch;
	short yaw;
	short roll;
};

//----------sc packet------------//
//-------------------------------//
struct sc_player_loginok_packet
{
	unsigned char size;
	unsigned char type;

	unsigned short id;
};

struct sc_player_disconnect
{
	unsigned char size;
	unsigned char type;
};

struct sc_put_object_packet
{
	unsigned char size;
	unsigned char type;

	unsigned short id;
	short x;
	short y;
	short z;	
	short pitch;
	short yaw;
	short roll;
};

struct sc_player_data
{ 
	unsigned char size;
	unsigned char type;

	unsigned short id;
	short x;
	short y;
	short z;
	short pitch;
	short yaw;
	short roll;
};
#pragma pack(pop)