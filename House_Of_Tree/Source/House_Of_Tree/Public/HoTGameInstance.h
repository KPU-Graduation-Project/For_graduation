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

	UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="서버 연결")
	bool ConnectNetwork;
	
	UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="IP 주소", meta = (EditCondition = "ConnectNetwork"))
	FString ipAddr;

	UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="포트번호", meta = (EditCondition = "ConnectNetwork"))
	int Port;

	UPROPERTY()
	AVRPlayerController_Base *playerController;

	UPROPERTY()
	int playerID;
};