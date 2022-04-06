// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Character.h"
#include "MotionControl.generated.h"

UCLASS()
class ESC_MAP_API AMotionControl : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMotionControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent* MotionController_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMotionControllerComponent* MotionController_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* HandMesh_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* HandMesh_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCameraComponent* VR_Camera;

	EControllerHand Hand_L, Hand_R;
};
