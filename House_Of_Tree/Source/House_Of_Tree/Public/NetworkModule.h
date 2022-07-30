// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NetworkModule.generated.h"

class UHoTGameInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HOUSE_OF_TREE_API UNetworkModule : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNetworkModule();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UHoTGameInstance *gameInst;

public:
	UFUNCTION(BlueprintCallable, Category="Network")
	void ShootBullet(FVector location, FRotator rotation);

	UFUNCTION(BlueprintCallable, Category="Network")
	void TargetSpin(AActor *actor , bool forward);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void CreateRoom();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void JoinRoom(int roomID);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void ReadyGame(bool isReady);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SelectCharacter(int selected);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void ExitRoom();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void StartGame();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void DebugGame();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void ObjectData(AActor *actor);
};