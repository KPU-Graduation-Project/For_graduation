// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#include "HoTGameInstance.h"
#include "VRPlayerController_Base.h"
#include <queue>

#pragma region Main Thread Code

ClientSocket::ClientSocket(UHoTGameInstance* Inst)
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
	UE_LOG(LogNet, Warning, TEXT("Thread has been initialized"));

	return true;
}

uint32 ClientSocket::Run()
{
	char RecvBuff[BUFSIZE];
	while (1)
	{
		int RecvLen = recv(Socket, reinterpret_cast<char*>(RecvBuff), BUFSIZE, 0);
		if (RecvLen != SOCKET_ERROR)
		{
			for (int i = 0; i < RecvLen; ++i)
				buffer.push(RecvBuff[i]);
		}
		else
		{
			buffer.push(2);
			buffer.push(SOCKET_ERROR);
			return 0;
		}
	}

	return 0;
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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, gameInst->ipAddr);
		return false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Success!")));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		                                 FString::Printf(TEXT("%i.%i"), wsaData.wVersion >> 8, wsaData.wVersion & 0xFF));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, gameInst->ipAddr);

		UE_LOG(LogNet, Warning, TEXT("Socket Initialized"));
		return true;
	}
}

bool ClientSocket::Send(const int SendSize, void* SendData)
{
	char buff[BUFSIZE];
	memcpy(buff, SendData, SendSize);

	int nSendLen = send(Socket, buff, buff[0], 0);
	UE_LOG(LogNet, Display, TEXT("Send Packet SIZE %d"), nSendLen);

	return true;
}
