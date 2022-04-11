// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetworkModule.generated.h"

class UHoTGameInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOUSE_OF_TREE_API UNetworkModule : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetworkModule();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UHoTGameInstance *gameInst;

public:	
	UFUNCTION(BlueprintCallable, Category="Network")
	void SendPacket();
};
