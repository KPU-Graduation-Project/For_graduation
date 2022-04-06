// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController_NetworkTest.generated.h"

/**
 * 
 */
class UMyGameInstance;

UCLASS()
class ESC_MAP_API AMyPlayerController_NetworkTest : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	UMyGameInstance *gameInst;

	UPROPERTY()
	UBlueprint* Characters[2];

	UPROPERTY()
	APawn *MainPlayer;
	
	UPROPERTY()
	APawn *OtherPlayer;

public:
	virtual void BeginPlay() override;
	
	AMyPlayerController_NetworkTest();
	virtual void Tick(float DeltaSeconds) override;

	void PutPlayer(int playerType, bool isPlayer, const FVector& Location, const FRotator& Rotation);
	void MovePawn(const FVector& Location,  const FRotator& Rotation);
};
