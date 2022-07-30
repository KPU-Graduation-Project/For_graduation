// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientSocket.h"
#include "Engine/StreamableManager.h"
#include "HoTGameInstance.generated.h"

/**
 * 
 */

constexpr int BUFSIZE = 1024;

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
	UClass* GetActor(int ObjectID) { return *bpSet.Find(ObjectID); }

	UClass* GetBullet(int index) {return BP_Bullet.IsValidIndex(index - 1)? BP_Bullet[index - 1].Get() : nullptr;}

	ClientSocket* SocketInstance;

	FStreamableManager StreamManager;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Network, DisplayName = "서버 연결")
	bool ConnectNetwork;

	UPROPERTY()
	FString ipAddr;

	UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="포트번호", meta = (EditCondition = "ConnectNetwork"))
	int Port;

	//UPROPERTY(EditDefaultsOnly, Category=Network, DisplayName="TEST", meta = (EditCondition = "ConnectNetwork"))
	//int TEST;

private:
	FString ipPath;

	//*********************************************************************************************/

private:
	UPROPERTY(EditDefaultsOnly, Category = NavMesh, DisplayName = "내비 메쉬 데이터 생성")
		bool ExportNavMesh;

	//*********************************************************************************************/
private:
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

	UPROPERTY(EditDefaultsOnly, Category = "Map", DisplayName = "Map")
	TSoftObjectPtr<UWorld> Map;

	FString path;

	//*********************************************************************************************/
public:
	UPROPERTY()
	AVRPlayerController_Base* playerController;

protected:
	UPROPERTY(BlueprintReadOnly)
	int playerID;

	UPROPERTY()
	bool gameStart = false;

	UPROPERTY()
	bool allLoadComplete = false;

public:
	inline void SetPlayerID(int id) { playerID = id; }
	inline int GetPlayerID() { return playerID; }

	inline void GameStart() { gameStart = true; }
	inline void AllLoadComplete() { allLoadComplete = true; }

	inline bool IsIngame() { return gameStart; };
	
	inline bool CheckSend() { return SocketInstance != nullptr; }

	const FSoftObjectPath GetMap()
	{
		return Map.ToStringReference();
	}
};