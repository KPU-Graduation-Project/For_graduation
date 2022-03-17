// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProtocol.h"

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
	virtual ~ClientSocket();

	SOCKET Socket;

	virtual bool Init() override;
	virtual uint32 Run() override;

	bool ConnectServer();
	
	bool Send(void* Packet);

private:
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;

	bool bRunThread;
};
