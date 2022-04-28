// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UPROPERTY()
	UHoTGameInstance* gameInst;

	UPROPERTY(EditDefaultsOnly, Category = "플레이어 캐릭터", DisplayName="캐릭터")
	TArray<TSubclassOf<APawn>> Characters;

	UPROPERTY()
	int playerID;

	UPROPERTY()
	AVRCharacter_Base* vrPlayer;

public:
	char data[265];
	void ProcessPacket();

public:
	void SetPlayerID(int id) { playerID = id; }
	int GetPlayerID() const { return playerID; }

	void SetPlayerCharacter(AActor* playerActor);

	virtual void BeginPlay() override;

	AVRPlayerController_Base();
	virtual void Tick(float DeltaSeconds) override;
};
