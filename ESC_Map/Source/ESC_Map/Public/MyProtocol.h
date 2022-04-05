// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

constexpr int NetValue = 1;

struct SVector3
{
	short x;
	short y;
	short z;
};

constexpr char CS_PLAYER_DATA = 0;
constexpr char SC_PLAYER_DATA = 1;

//---------------------//

struct cs_player_data
{
	unsigned char size;
	unsigned char type;
	SVector3 player_position;
	SVector3 player_rotation;
};

// Server
struct sc_player_data
{
	unsigned char size;
	unsigned char type;
	unsigned int id;
	SVector3 player_position;
	SVector3 player_rotation;
};
