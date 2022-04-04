// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameInstance.h"
#include "NetworkModule.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESC_MAP_API UNetworkModule : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetworkModule();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UMyGameInstance *gameInst;

public:
	UFUNCTION(BlueprintCallable, Category="Network")
	void SendPacket();
};
