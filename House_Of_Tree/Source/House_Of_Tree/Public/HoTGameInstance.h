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
class AVRCharacter_Base;

UCLASS()
class HOUSE_OF_TREE_API UHoTGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHoTGameInstance();
	virtual ~UHoTGameInstance() override;

	virtual void Init() override;

	void InitSocket();

	void SetInfo();
	UClass* GetActor(int ObjectID);

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
	UPROPERTY()
	TMap<int, UClass*> bpSet;

	// ID 리스트 파일을 새로 생성하라고 표시하는 명령어
	UPROPERTY(EditDefaultsOnly, Category="ID", DisplayName="ID 리스트 생성")
	bool makeIDList;

	// 사용하는 블루프린트 액터를 기록하는 컨테이너
	// bpset을 안쓰고 나중에 그냥 type별로 직접 분리해서 스폰하게 수정
	// 패킷 처리부분을 액터로 분리해서 사용
	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Character") // 001
	TArray<TSubclassOf<AActor>> BP_Char;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Monster") // 002
	TArray<TSubclassOf<AActor>> BP_Monster;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Bullets") // 003
	TArray<TSubclassOf<AActor>> BP_Bullet;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Dynamic Object") // 004
	TArray<TSubclassOf<AActor>> BP_DyObj;

	UPROPERTY(EditDefaultsOnly, Category="Blueprint", DisplayName="Passive Object") // 005
	TArray<TSubclassOf<AActor>> BP_PasObj;

	UPROPERTY()
	FString path;

	//*********************************************************************************************/
public:
	UPROPERTY()
	AVRPlayerController_Base* playerController;

protected:
	UPROPERTY()
	bool gameStart = false;

	UPROPERTY()
	bool allLoadComplete = false;
	
public:
	void GameStart() { gameStart = true; }
	void AllLoadComplete() { allLoadComplete = true; }

	bool IsIngame() { return gameStart; };
};
