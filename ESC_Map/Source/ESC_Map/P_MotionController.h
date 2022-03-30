// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P_MotionController.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class UMotionControllerComponent;

UCLASS()
class ESC_MAP_API AP_MotionController : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent* MotionController;
															  
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* HandMesh;	 

	EControllerHand Hand;
	
public:	
	// Sets default values for this actor's properties
	AP_MotionController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform & Transform) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
