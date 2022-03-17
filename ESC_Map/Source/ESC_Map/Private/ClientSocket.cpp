// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#include "SocketSubsystem.h"
#include "Common/UdpSocketReceiver.h"
#include "Interfaces/IPv4/IPv4Address.h"

#pragma region Main Thread Code

ClientSocket::ClientSocket(): Thread()
{
	Socket = nullptr;
	if (ConnectServer())
	{
		Thread = FRunnableThread::Create(this, TEXT("Network Thread"), 0, TPri_BelowNormal);
	}
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

struct sc_player_data
{ 
	PacketInfo info;
	unsigned int id;
	SVector3 player_position;
	SVector3 player_rotation;

};

uint32 ClientSocket::Run()
{
	while (Socket == nullptr);

	TArray<uint8> ReceivedData;
	uint32 Size;

	while(StopTaskCounter.GetValue() == 0 && Socket->HasPendingData(Size))
	{
		int32 Read;
		ReceivedData.SetNum(Size);
		bool result =  Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
		UE_LOG(LogTemp, Warning, TEXT("%s"), result? _T("True") : _T("False"));
	}
	return 0;
}

bool ClientSocket::ConnectServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));
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
		UE_LOG(LogTemp, Warning, TEXT("Socket Initialized"));
		return true;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail")));
		return false;
	}
}

bool ClientSocket::Send(void* Packet)
{
	int32 ByteSent = 0;
	bool isSent = Socket->Send((uint8*)reinterpret_cast<char*>(Packet), reinterpret_cast<PACKET_DATA*>(Packet)->info.size, ByteSent);
	
	UE_LOG(LogTemp, Log, TEXT("%d Data, %d sent"), reinterpret_cast<PACKET_DATA*>(Packet)->info.size, ByteSent);
	
	return isSent;
}
