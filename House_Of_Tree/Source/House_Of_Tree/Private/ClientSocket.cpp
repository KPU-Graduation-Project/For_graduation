// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#include "HoTGameInstance.h"
#include "VRPlayerController_Base.h"
//#include "..\..\..\..\ServerPrototype/ServerPrototype/protocol.h"
#include "Protocol.h"

#pragma region Main Thread Code

ClientSocket::ClientSocket(UHoTGameInstance* Inst): StopTaskCounter(0)
{
	gameInst = Inst;
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
	char RecvBuff[BUFSIZE];
	while (StopTaskCounter.GetValue() == 0)
	{
		int RecvLen = recv(Socket, reinterpret_cast<char*>(RecvBuff), BUFSIZE, 0);
		if (RecvLen != SOCKET_ERROR)
		{
			//ProcessPacket(RecvLen, RecvBuff);
			memcpy(gameInst->playerController->data, RecvBuff, RecvLen);
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
	if (nRet != 0)
	{
		return false;
	}

	Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Socket == INVALID_SOCKET)
	{
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		                                 FString::Printf(TEXT("%i.%i"), wsaData.wVersion >> 8, wsaData.wVersion & 0xFF));
		UE_LOG(LogTemp, Warning, TEXT("Socket Initialized"));
		return true;
	}
}

bool ClientSocket::Send(const int SendSize, void* SendData)
{
	char buff[BUFSIZE];
	memcpy_s(buff, BUFSIZE, SendData, SendSize);

	int nSendLen = send(Socket, buff, buff[0], 0);

	return true;
}

void ClientSocket::ProcessPacket(const int RecvSize, char* RecvData)
{
	int packetSize = RecvSize;
	char packetType = RecvData[1];
	UE_LOG(LogTemp, Warning, TEXT("%d"), packetType);

	while (packetSize > 0)
	{
		switch (packetType)
		{
		case SC_PACKET::SC_LOGINOK:
			{
				UE_LOG(LogTemp, Warning, TEXT("loginOK"));
				// 플레이어 입력 패킷인데 일단 비워두기
				sc_loginok_packet* packet = reinterpret_cast<sc_loginok_packet*>(RecvData);

				gameInst->SetPlayerID(packet->id);
			}
			break;

		case SC_PACKET::SC_CREATE_ROOM:
			break;
		case SC_PACKET::SC_JOIN_ROOM:
			break;
		case SC_PACKET::SC_USER_JOIN_ROOM:
			break;

		// SC 어디감;
		case SC_PACKET::USER_EXIT_ROOM:
			break;

		case SC_PACKET::SC_USER_READY_GAME:
			break;

		case SC_PACKET::SC_USER_CHANGE_SELECTED_CHARACTER:
			break;

		case SC_PACKET::SC_START_GAME:
			UE_LOG(LogTemp, Warning, TEXT("SC_START_GAME"));

		// 로비에서 인게임 로비 캠으로 전환
			break;

		case SC_PACKET::SC_ALL_USERS_LOADING_COMPLETE:
			UE_LOG(LogTemp, Warning, TEXT("SC_ALL_USERS_LOADING_COMPLETE"));

			gameInst->AllLoadComplete = true;
			break;

		case SC_PACKET::SC_PUT_OBJECT:
			{
				UE_LOG(LogTemp, Warning, TEXT("putobject"));
				sc_put_object_packet* packet = reinterpret_cast<sc_put_object_packet*>(RecvData);
				gameInst->PutObject(packet->id, packet->object_type, FVector(packet->x, packet->y, packet->z),
				                    FRotator(packet->pitch, packet->yaw, packet->roll), FVector(0, 0, 0));
			}
			break;

		case SC_PACKET::SC_REMOVE_OBJECT:
			break;

		case SC_PACKET::SC_PLAYER_DATA:
			UE_LOG(LogTemp, Warning, TEXT("SC_PLAYER_DATA"));
			break;

		default:
			break;
		}

		packetSize -= RecvData[0];
	}
}
