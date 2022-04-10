// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "MyGameInstance.h"
#include "MyPlayerController_NetworkTest.h"

UMyGameInstance::UMyGameInstance()
{
	SocketInstance = nullptr;
}

UMyGameInstance::~UMyGameInstance()
{
}

void UMyGameInstance::Init()
{
	Super::Init();
	
	UE_LOG(LogTemp, Warning, TEXT("Instance Initialized"));
}

void UMyGameInstance::OnStart()
{
	Super::OnStart();

	if (ConnectNetwork)
	{
		InitSocket();
	}
}

void UMyGameInstance::InitSocket()
{
	if (SocketInstance != nullptr) return;
	
	SocketInstance = new ClientSocket(this);
}
