// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#include <ThirdParty/asio/1.12.2/asio/detail/socket_ops.hpp>

#include "SocketSubsystem.h"
#include "Common/UdpSocketReceiver.h"
#include "Interfaces/IPv4/IPv4Address.h"

#pragma region Main Thread Code

ClientSocket::ClientSocket(): Thread()
{
	if (ConnectServer())
	{
		Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
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

uint32 ClientSocket::Run()
{
	while(bRunThread)
	{
		UE_LOG(LogTemp, Warning, TEXT("Thread is running"));
		FPlatformProcess::Sleep(0.1f);
	}
	return 0;
}

void ClientSocket::Stop()
{
	bRunThread = false;
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

	// Have to try Reconnet?
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

void ClientSocket::Recv()
{
	Socket->Recv();
}

bool RecvMsg(FSocket *Socket, uint32 DataSize, FString& Msg)
{
	TSharedRef<FInternetAddr> targetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	TArray<uint8> ReceivedData;//Define a receiver
	uint32 Size;
	if (ListenSocket->HasPendingData(Size))
	{
		success = true;
		str = "";
		uint8 *Recv = new uint8[Size];
		int32 BytesRead = 0;
		// Adjust the array to a given number of elements. The new element will be initialized.
		ReceivedData.SetNumUninitialized(FMath::Min(Size, 65507u));
		ListenSocket->RecvFrom(ReceivedData.GetData(), ReceivedData.Num(), BytesRead, *targetAddr);
		char ansiiData[1024];
		FMemory::Memcpy(ansiiData, ReceivedData.GetData(), BytesRead);//Copy data to the receiver
		ansiiData[BytesRead] = 0; //Judge the end of the data
		FString debugData = ANSI_TO_TCHAR(ansiiData); //string conversion
		str = debugData;
	}
}
