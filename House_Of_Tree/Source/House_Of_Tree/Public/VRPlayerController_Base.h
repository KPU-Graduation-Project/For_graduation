// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <concurrent_queue.h>
#include <atomic>

#include "CoreMinimal.h"
#include "HoTGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "VRPlayerController_Base.generated.h"

/**
 * 
 */

class UHoTGameInstance;
class AVRCharacter_Base;

UCLASS()
class HOUSE_OF_TREE_API AVRPlayerController_Base : public APlayerController
{
	GENERATED_BODY()
public:
	AVRPlayerController_Base();

protected:
	UPROPERTY()
	UHoTGameInstance* gameInst;

	UPROPERTY(EditDefaultsOnly, Category = "플레이어 캐릭터", DisplayName="캐릭터")
	TArray<TSubclassOf<APawn>> Characters;

	UPROPERTY()
	int playerID;

	UPROPERTY()
	AVRCharacter_Base* vrPlayer;

	UPROPERTY()
	TMap<int, AActor*> actorList;
	
	void SetPlayerCharacter();
	
	// Network system
public:
	Concurrency::concurrent_queue<char> buffer;
	std::atomic<int> bufferSize {0};

protected:
	void ProcessPacket();

	void PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale);

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

};
