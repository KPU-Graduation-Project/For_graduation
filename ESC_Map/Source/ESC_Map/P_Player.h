// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "P_MotionController.h"
#include "P_Player.generated.h"

class AP_MotionController;
class USceneComponent;
class UCameraComponent;

UCLASS()
class ESC_MAP_API AP_Player : public APawn
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* CameraBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AP_MotionController* LeftController;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AP_MotionController* RightController;
	

public:
	// Sets default values for this pawn's properties
	AP_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
