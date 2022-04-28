// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <unordered_map>

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

	void SetInfo();

	ClientSocket* SocketInstance;

private:
	UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="서버 연결")
	bool ConnectNetwork;

public:
	UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="IP 주소", meta = (EditCondition = "ConnectNetwork"))
	FString ipAddr;

	UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="포트번호", meta = (EditCondition = "ConnectNetwork"))
	int Port;

	//*********************************************************************************************/

private:
	// ID 리스트 파일을 새로 생성하라고 표시하는 명령어
	UPROPERTY(EditDefaultsOnly, Category="ID", DisplayName="ID 리스트 생성")
	bool makeIDList;

	// 사용하는 블루프린트 액터를 기록하는 컨테이너
	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Character") // 001
	TArray<UBlueprint*> BP_Char;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Monster") // 002
	TArray<UBlueprint*> BP_Monster;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Bullets") // 003
	TArray<UBlueprint*> BP_Bullet;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Dynamic Object") // 004
	TArray<UBlueprint*> BP_DyObj;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Passive Object") // 005
	TArray<UBlueprint*> BP_PasObj;

	UPROPERTY()
	FString path;

	//*********************************************************************************************/
public:
	UPROPERTY()
	AVRPlayerController_Base* playerController;

protected:
	UPROPERTY()
	TMap<int, AActor*> actorList;

	UPROPERTY(EditAnywhere)
	TMap<int, UClass*> bpSet;

	UPROPERTY()
	bool gameStart = true;

public:
	void SetPlayerID(int playerId);

	UPROPERTY()
	bool AllLoadComplete = false;

	
	void PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale);

	bool IsIngame() {return AllLoadComplete;};
};
