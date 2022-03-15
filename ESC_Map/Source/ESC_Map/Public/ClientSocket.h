// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sockets.h"
#include "MyProtocol.h"
/**
 * 
 */
class ESC_MAP_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	FSocket* Socket;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	bool ConnectServer();
	
	bool Send(void* Packet);
	void Recv();

private:
	FRunnableThread* Thread;

	bool bRunThread;
};
