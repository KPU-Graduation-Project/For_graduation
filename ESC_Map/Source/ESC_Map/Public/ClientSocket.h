// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
/**
 * 
 */
class ESC_MAP_API ClientSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	FSocket* Socket;
};
