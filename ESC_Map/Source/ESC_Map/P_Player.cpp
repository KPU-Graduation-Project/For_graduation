// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Player.h"
#include "Camera/CameraComponent.h"
#include "P_MotionController.h"
#include "HeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AP_Player::AP_Player()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	CameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBase->SetRelativeLocation(FVector(0.f, 0.f, 60.f));

	RootComponent = RootScene;
	CameraBase->SetupAttachment(RootComponent);
	VRCamera->SetupAttachment(CameraBase);


}

// Called when the game starts or when spawned
void AP_Player::BeginPlay()
{
	Super::BeginPlay();


	// 위치값 수정
	//UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
	RootScene->SetRelativeLocation(FVector(-14.0f, 1461.0f, 92.000008f));

	if (GetWorld())
	{
		const FTransform SpawnTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));

		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		
		LeftController = GetWorld()->SpawnActorDeferred<AP_MotionController>(AP_MotionController::StaticClass(), SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (LeftController)
		{

			LeftController->Hand = EControllerHand::Left;
			LeftController->FinishSpawning(SpawnTransform);
			LeftController->AttachToComponent(CameraBase, AttachRules);
		}

		RightController = GetWorld()->SpawnActorDeferred<AP_MotionController>(AP_MotionController::StaticClass(), SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (RightController)
		{
			RightController->Hand = EControllerHand::Right;
			RightController->FinishSpawning(SpawnTransform);
			RightController->AttachToComponent(CameraBase, AttachRules);
		}
	}
}

// Called every frame
void AP_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AP_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

