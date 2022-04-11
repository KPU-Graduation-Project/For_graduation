// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientSocket.h"
#include "HoTGameInstance.generated.h"

/**
 * 
 */

class AVRPlayerController_Base;

UCLASS()
class HOUSE_OF_TREE_API UHoTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHoTGameInstance();
	virtual ~UHoTGameInstance() override;

	virtual void Init() override;
	virtual void OnStart() override;
	
	void InitSocket();
	
	ClientSocket* SocketInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Network)
	bool ConnectNetwork;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Network)
	FString ipAddr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Network)
	int Port;

	UPROPERTY()
	AVRPlayerController_Base *playerController;
};