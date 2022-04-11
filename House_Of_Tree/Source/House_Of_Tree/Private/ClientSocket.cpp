// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#include "HoTGameInstance.h"
#include "VRPlayerController_Base.h"
//#include "..\..\..\..\ServerPrototype/ServerPrototype/protocol.h"
#include "Protocol.h"

#pragma region Main Thread Code

ClientSocket::ClientSocket(UHoTGameInstance* inst): StopTaskCounter(0)
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
	stServerAddr.sin_port = htons(gameInst->Port);
	
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
	char sendBuff[BUFSIZE];
	strcpy_s(sendBuff, reinterpret_cast<char*>(Packet));
	
	int nSendLen = send(Socket, sendBuff, sendBuff[0], 0);
	
	UE_LOG(LogTemp, Log, TEXT("%d Data, %d sent"), sendBuff[0], nSendLen);
	
	return true;
}

void ClientSocket::ProcessPacket(const int RecvLen, char* RecvBuff)
{
	UE_LOG(LogTemp, Warning, TEXT("Input %d"), RecvLen);

	int packetSize = RecvLen;
	char packet_type = RecvBuff[1];

	while(packetSize > 0)
	{
		switch (packet_type)
		{
		case SC_PACKET::SC_LOGINOK:
			{
				// 플레이어 입력 패킷인데 일단 비워두기
				sc_loginok_packet* packet = reinterpret_cast<sc_loginok_packet*>(RecvBuff);
				
				//gameInst->playerController->PutPlayer(packet->type, true,
				//	FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
			}
			break;
		
		case SC_PACKET::SC_PUT_PLAYER:
			{
				sc_put_player_packet* packet = reinterpret_cast<sc_put_player_packet*>(RecvBuff);
	
				FVector location(packet->x / 100, packet->y / 100, packet->z / 100);
				FRotator rotation(packet->pitch, packet->yaw, packet->roll);
				gameInst->playerController->PutPlayer(packet->character_type, false, location, rotation);
			}
			break;
		
		case SC_PACKET::SC_PUT_OBJECT:
			{
				sc_put_object_packet* packet = reinterpret_cast<sc_put_object_packet*>(RecvBuff);
	
				// 아직 안만듬
			}
			break;
	
		case SC_PACKET::SC_REMOVE_OBJECT:
			{
				sc_remove_object_packet* packet = reinterpret_cast<sc_remove_object_packet*>(RecvBuff);
	
				// 아직 안만듬
			}
			break;
		
		case SC_PACKET::SC_PLAYER_DATA:
			{
				sc_player_data_packet* packet = reinterpret_cast<sc_player_data_packet*>(RecvBuff);
	
				FVector location(packet->x / 100, packet->y / 100, packet->z / 100);
				FRotator rotation(packet->pitch, packet->yaw, packet->roll);
				
				gameInst->playerController->MovePawn(location, rotation);
			}
			break;
		
		default:
			break;
		}
	
		packetSize -= RecvBuff[0];
	}
}