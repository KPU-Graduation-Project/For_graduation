// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

constexpr int NetValue = 1;

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

namespace CS_PACKET
{
	const char CS_PLAYER_DATA = 0;
};

namespace SC_PACKET
{
	const char SC_PLAYER_DATA = 0;
};

//---------------------//

struct cs_player_data
{
	PacketInfo info;
	SVector3 player_position;
	SVector3 player_rotation;
};

// Server
struct sc_player_data
{
	PacketInfo info;
	unsigned int id;
	SVector3 player_position;
	SVector3 player_rotation;
};
