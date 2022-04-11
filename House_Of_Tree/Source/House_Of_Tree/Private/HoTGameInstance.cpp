// Fill out your copyright notice in the Description page of Project Settings.


#include "HoTGameInstance.h"
#include "VRPlayerController_Base.h"

UHoTGameInstance::UHoTGameInstance()
{
	SocketInstance = nullptr;
}

UHoTGameInstance::~UHoTGameInstance()
{
}

void UHoTGameInstance::Init()
{
	Super::Init();

	UE_LOG(LogTemp, Warning, TEXT("Instance Initialized"));
}

void UHoTGameInstance::OnStart()
{
	Super::OnStart();
	
	if (ConnectNetwork)
	{
		InitSocket();
	}
}

void UHoTGameInstance::InitSocket()
{
	if (SocketInstance != nullptr) return;
	
	SocketInstance = new ClientSocket(this);
}


