// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"

#pragma region Main Thread Code

ClientSocket::ClientSocket()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("TEST"));
	
	FString address = TEXT("14.36.243.158");
	int32 port = 6000;
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);
	
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	bool isConnetcted = Socket->Connect(*addr);

	if (isConnetcted)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Success!")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail")));
	}
	UE_LOG(LogTemp, Warning, TEXT("Socket Initialized"));

	// 계속 연결 요청 만들기

	Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
	
}

ClientSocket::~ClientSocket()
{
	if (Thread)
	{
		Thread->Kill();
		delete Thread;
	}
}

#pragma endregion

bool ClientSocket::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread has been initialized"));

	return true;
}

uint32 ClientSocket::Run()
{
	while(bRunThread)
	{
		UE_LOG(LogTemp, Warning, TEXT("Thread is running"));
		FPlatformProcess::Sleep(1.0f);
	}
	return 0;
}

void ClientSocket::Stop()
{
	bRunThread = false;
}

bool ClientSocket::Send(void* Packet)
{
	int32 ByteSent = 0;
	int a = Socket->Send((uint8*)reinterpret_cast<char*>(Packet), reinterpret_cast<PRT::CS_PLAYER_DATA*>(Packet)->info.size, ByteSent);
	
	UE_LOG(LogTemp, Log, TEXT("%d, %d sent"), a, reinterpret_cast<PRT::CS_PLAYER_DATA*>(Packet)->info.size);
	return true;
}
