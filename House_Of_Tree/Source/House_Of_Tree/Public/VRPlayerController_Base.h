// Fill out your copyright notice in the Description page of Project Settings.

#pragma once



#include "CoreMinimal.h"
#include "HoTGameInstance.h"
#include "Protocol.h"	// 차후 경로 변경해야함
#include "GameFramework/PlayerController.h"
#include "VRPlayerController_Base.generated.h"

/**
 * 
 */

class UHoTGameInstance;
class AVRCharacter_Base;

UENUM()
enum class PLAYERTYPE
{
	GIRL,
	BOY
};

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

	UPROPERTY(BlueprintReadOnly)
	int playerID;

	UPROPERTY()
	AVRCharacter_Base* vrPlayer;

	UPROPERTY(BlueprintReadOnly)
	TMap<int, AActor*> actorList;

	void SetPlayerCharacter(const int objectID);

	UPROPERTY()
	PLAYERTYPE playertype;

private:
	char data[BUFSIZE];
	int remainData = 0;

protected:
	void RecvPacket();
	void ProcessPacket(char* p);

	void SendPlayerData();

	void PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale, int meshID, int parentID);

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	AVRCharacter_Base* GetVRPlayer() { return vrPlayer; }
	void AddActorList(int key, AActor* actor) { actorList.Add(key, actor); }

	AActor* GetActorList(int key) { return *actorList.Find(key); }
	const int* GetActorKey(AActor* actor) { return actorList.FindKey(actor); }

	PLAYERTYPE GetPlayerType() { return playertype; }
};
