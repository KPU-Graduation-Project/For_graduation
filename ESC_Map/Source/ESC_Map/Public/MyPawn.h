// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "MotionControllerComponent.h"
#include "MyPawn.generated.h"

UCLASS()
class ESC_MAP_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* VR_Root;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent *MotionController_L;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent *MotionController_R;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent *HandMesh_L;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent *HandMesh_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* VR_Camera;

	EControllerHand Hand_L, Hand_R;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
