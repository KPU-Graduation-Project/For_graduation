// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <concurrent_queue.h>
#include <atomic>

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
class UHoTGameInstance;

class HOUSE_OF_TREE_API ClientSocket : public FRunnable
{
public:
	ClientSocket(UHoTGameInstance* Inst);
	virtual ~ClientSocket() override;

	SOCKET Socket;

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	bool ConnectServer();

	bool Send(const int SendSize, void* SendData);

private:
	FRunnableThread* Thread;
	FThreadSafeCounter StopTaskCounter;

	UHoTGameInstance* gameInst;

	// Network system
public:
	Concurrency::concurrent_queue<char> buffer;
};
