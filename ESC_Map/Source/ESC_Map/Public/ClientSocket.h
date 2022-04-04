// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// window 기본 타입 Hide
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"

#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>


#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
/**
 * 
 */

class ESC_MAP_API ClientSocket : public FRunnable
{
public:
	ClientSocket();
	virtual ~ClientSocket() override;

	SOCKET Socket;

	virtual bool Init() override;
	virtual uint32 Run() override;

	bool ConnectServer();
	
	bool Send(void* Packet);
	void ProcessPacket(const unsigned int _uesr_id, unsigned char* p);

private:
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;

	bool bRunThread;
};
