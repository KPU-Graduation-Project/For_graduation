// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
namespace PRT
{
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

	struct CS_PLAYER_DATA
	{
		PacketInfo info;
		SVector3 player_position;
		SVector3 player_rotation;
	};
}

