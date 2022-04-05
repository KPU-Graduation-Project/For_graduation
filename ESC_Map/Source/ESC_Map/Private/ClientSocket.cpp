// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"
#include "MyGameInstance.h"
#include "MyProtocol.h"

#pragma region Main Thread Code

ClientSocket::ClientSocket(UMyGameInstance* inst): StopTaskCounter(0)
{
	gameInst = inst;
	if (ConnectServer())
	{
		Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
	}
}

ClientSocket::~ClientSocket()
{
	if (Thread)
	{
		// 스레드 종료
		Thread->WaitForCompletion();
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
	char buff[512];
	while (StopTaskCounter.GetValue() == 0)
	{
		int RecvLen = recv(Socket, reinterpret_cast<char*>(buff), 512, 0);
		if (RecvLen != SOCKET_ERROR)
		{
			ProcessPacket(RecvLen, buff);
		}
	}
	
	return 0;
}

void ClientSocket::Stop()
{
	StopTaskCounter.Increment();
}

void ClientSocket::Exit()
{
	if (Socket)
	{
		closesocket(Socket);
		WSACleanup();
	}
}

bool ClientSocket::ConnectServer()
{
	WSADATA wsaData;
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		return false;
	}

	Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Socket == INVALID_SOCKET) {
		return false;
	}

	SOCKADDR_IN stServerAddr;

	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(6000);
	//stServerAddr.sin_addr.s_addr = inet_addr("14.36.243.12");
	
	stServerAddr.sin_addr.s_addr = inet_addr(TCHAR_TO_ANSI(ToCStr(gameInst->ipAddr)));

	nRet = connect(Socket, (sockaddr*)&stServerAddr, sizeof(sockaddr));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	if (nRet == SOCKET_ERROR)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Fail")));
		return false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Success!")));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%i.%i"), wsaData.wVersion >> 8, wsaData.wVersion & 0xFF));
		UE_LOG(LogTemp, Warning, TEXT("Socket Initialized"));
		return true;
	}
}

bool ClientSocket::Send(void* Packet)
{
	int nSendLen = send(Socket, reinterpret_cast<char*>(Packet), reinterpret_cast<sc_player_data*>(Packet)->size, 0);
	
	UE_LOG(LogTemp, Log, TEXT("%d Data, %d sent"), reinterpret_cast<sc_player_data*>(Packet)->size, nSendLen);
	
	return true;
}

void ClientSocket::ProcessPacket(const int RecvLen, char* p)
{
	UE_LOG(LogTemp, Warning, TEXT("Input %d"), RecvLen);

	char packet_type = p[1];

	switch (packet_type)
	{
	case SC_PACKET::SC_PLAYER_DATA:
		{
			sc_player_data* packet = reinterpret_cast<sc_player_data*>(p);

			UE_LOG(LogTemp, Log, TEXT("Get Data %d %d %d"), packet->x,packet->y, packet->z);
			
			gameInst->OtherPlayer->SetActorLocation(FVector(packet->x,packet->y, packet->z));
			gameInst->OtherPlayer->SetActorRotation(FRotator(packet->pitch,packet->yaw, packet->roll));
		}
		break;
		
	default:
		break;
	}
}
