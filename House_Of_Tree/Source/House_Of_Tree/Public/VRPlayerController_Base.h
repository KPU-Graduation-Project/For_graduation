// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "HoTGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "VRPlayerController_Base.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FLOBBY, int, ID1, bool, roomMaster, int, Character, bool, isReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FID, int, ID3);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFUNC);

/**
 *
 */

class UHoTGameInstance;
class AVRCharacter_Base;
class ATestCharacter;

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

	// ID 설정
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FID DE_SetID;

	// 방 입장 이벤트
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FLOBBY DE_PlayerEnterRoom;

	// 다른 유저 입장 이벤트
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FLOBBY DE_UserEnterRoom;

	// 방 나가기
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FID DE_ExitRoom;

	// 유저의 상태 변경
	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FLOBBY DE_ChangeState;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
		FFUNC DE_QUIT;


protected:
	UPROPERTY()
		UHoTGameInstance *gameInst;

	UPROPERTY(EditDefaultsOnly, Category = "플레이어 캐릭터", DisplayName = "캐릭터")
		TArray<TSubclassOf<APawn>> Characters;

	UPROPERTY()
		AVRCharacter_Base *vrPlayer = nullptr;

	// ========================================== TestMode ========================================== //
	UPROPERTY()
		ATestCharacter *TestPlayer = nullptr;
	// ============================================================================================== //

	UPROPERTY(BlueprintReadOnly)
		TMap<int, AActor *> actorList;

	void SetPlayerCharacter(const int objectID);

	UPROPERTY()
		PLAYERTYPE playertype;

private:
	char data[BUFSIZE] = {0};
	int remainData = 0;

	// ========================================== TestMode ========================================== //
	bool TestMode = false;
	// ============================================================================================== //

protected:
	void RecvPacket();
	bool ProcessPacket(char *p);

	void SendPlayerData();

	void PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale, int meshID, int parentID);

	void LoadComplete();

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	AVRCharacter_Base *GetVRPlayer() { return vrPlayer; }
	void AddActorList(int key, AActor *actor) { actorList.Add(key, actor); }

	AActor *GetActorList(int key) { return *actorList.Find(key); }
	const int *GetActorKey(AActor *actor) { return actorList.FindKey(actor); }

	PLAYERTYPE GetPlayerType() { return playertype; }
};
