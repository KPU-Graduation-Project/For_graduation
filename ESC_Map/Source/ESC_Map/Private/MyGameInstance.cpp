// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	SocketInstance = nullptr;
}

UMyGameInstance::~UMyGameInstance()
{
	if (SocketInstance)
	{
		delete SocketInstance;
	}
}

void UMyGameInstance::Init()
{
	Super::Init();

	if (ConnectNetwork)
	{
		InitSocket();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Instance Initialized"));
}

void UMyGameInstance::InitSocket()
{
	if (SocketInstance != nullptr) return;
	
	SocketInstance = new ClientSocket();
}