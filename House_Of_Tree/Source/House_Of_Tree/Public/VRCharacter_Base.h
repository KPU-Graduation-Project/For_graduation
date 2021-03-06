// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "VRCharacter_Base.generated.h"

UCLASS()
class HOUSE_OF_TREE_API AVRCharacter_Base : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter_Base();

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
	USceneComponent *VR_Root;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Speed;

public:
	inline FTransform GetHeadTransform() const {return VR_Camera->GetComponentTransform();}
	inline FTransform GetRHTransform() const {return MotionController_R->GetComponentTransform();}
	inline FTransform GetLHTransform() const {return MotionController_L->GetComponentTransform();}

	inline float GetSpeed() { return Speed; }
	inline void SetSpeed(float speed) { Speed = speed; }

	void SetLocationAndRotation(const FVector &location, const float &yaw);
	void SetHandLocationAndRotation(const FVector &lhLocation, const FRotator &lhRotation, const FVector &rhLocationRH, const FRotator &rhRotation);
};
