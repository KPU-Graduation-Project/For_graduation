// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#pragma region Main Thread Code

ClientSocket::ClientSocket(): Thread()
{
	if (ConnectServer())
	{
		bRunThread = true;
		Thread = FRunnableThread::Create(this, TEXT("Network Thread"));
	}
}

ClientSocket::~ClientSocket()
{
	if (Thread)
	{
		bRunThread = false; 
		closesocket(Socket);
		WSACleanup();
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
	char buff[512];
	while (bRunThread)
	{
		int RecvLen = recv(Socket, &buff[0], 512, 0);
		UE_LOG(LogTemp, Warning, TEXT("Input"));
	}
	
	return 0;
}

bool ClientSocket::ConnectServer()
{
	WSADATA wsaData;
	// 윈속 버전을 2.2로 초기화
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (nRet != 0) {
		return false;
	}

	// TCP 소켓 생성	
	Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Socket == INVALID_SOCKET) {
		return false;
	}

	SOCKADDR_IN stServerAddr;

	stServerAddr.sin_family = AF_INET;
	// 접속할 서버 포트 및 IP
	stServerAddr.sin_port = htons(6000);
	stServerAddr.sin_addr.s_addr = inet_addr("14.36.243.158");
	//stServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

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
	int nSendLen = send(Socket, reinterpret_cast<char*>(Packet), reinterpret_cast<PACKET_DATA*>(Packet)->info.size, 0);
	
	UE_LOG(LogTemp, Log, TEXT("%d Data, %d sent"), reinterpret_cast<PACKET_DATA*>(Packet)->info.size, nSendLen);
	
	return true;
}
