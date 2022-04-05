// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyGameInstance.h"
#include "MyPlayerController_NetworkTest.generated.h"

/**
 * 
 */
UCLASS()
class ESC_MAP_API AMyPlayerController_NetworkTest : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	UMyGameInstance *gameInst;
	
	UPROPERTY()
	AActor *OtherPlayer;

	UPROPERTY()
	UBlueprint* test;

public:
	virtual void BeginPlay() override;
	
	AMyPlayerController_NetworkTest();
	virtual void Tick(float DeltaSeconds) override;
};
