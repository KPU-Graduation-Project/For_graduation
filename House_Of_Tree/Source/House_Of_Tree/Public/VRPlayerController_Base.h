// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VRPlayerController_Base.generated.h"

/**
 * 
 */

class UHoTGameInstance;

UCLASS()
class HOUSE_OF_TREE_API AVRPlayerController_Base : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	UHoTGameInstance *gameInst;

	UPROPERTY()
	UBlueprint* Characters[2];

	UPROPERTY()
	APawn *MainPlayer;
	
	UPROPERTY()
	APawn *OtherPlayer;

public:
	virtual void BeginPlay() override;
	
	AVRPlayerController_Base();
	virtual void Tick(float DeltaSeconds) override;

	void PutPlayer(int PlayerType, bool IsPlayer, const FVector& Location, const FRotator& Rotation);
	void MovePawn(const FVector& Location,  const FRotator& Rotation);
};
