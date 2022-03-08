// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientSocket.h"

#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"

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
}

ClientSocket::~ClientSocket()
{
}

