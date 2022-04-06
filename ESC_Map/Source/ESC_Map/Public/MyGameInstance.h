// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ClientSocket.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ESC_MAP_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();
	~UMyGameInstance();

	virtual void Init() override;
	virtual void OnStart() override;
	
	void InitSocket();
	
	ClientSocket* SocketInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Network)
	bool ConnectNetwork;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Network)
    FString ipAddr;

	UPROPERTY(BlueprintReadOnly, Category=Character)
	APawn *MainPlayer;
	
	UPROPERTY(BlueprintReadOnly, Category=Character)
	APawn *OtherPlayer;
};
