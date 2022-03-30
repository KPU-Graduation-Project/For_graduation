// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Camera/CameraComponent.h"
#include "MyMotionController.h"
#include "HeadMountedDisplay.h"
#include "HeadMountedDisplayFunctionLibrary.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SomWorks :D // Create Components Initialize
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	CameraBase = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBase->SetRelativeLocation(FVector(0.f, 0.f, 60.f));

	RootComponent = RootScene;
	CameraBase->SetupAttachment(RootComponent);
	VRCamera->SetupAttachment(CameraBase);
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	
	const FTransform SpawnTransform = FTransform(FRotator(0.0f, 0.0f, 0.0f), FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f));

	if (GetWorld())
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
		
		LeftController = GetWorld()->SpawnActorDeferred<AMyMotionController>(AMyMotionController::StaticClass(), SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (LeftController)
		{
			
			LeftController->Hand = EControllerHand::Left;
			LeftController->FinishSpawning(SpawnTransform);
			LeftController->AttachToComponent(CameraBase, AttachRules);
		}

		RightController = GetWorld()->SpawnActorDeferred<AMyMotionController>(AMyMotionController::StaticClass(), SpawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		if (RightController)
		{
			RightController->Hand = EControllerHand::Right;
			RightController->FinishSpawning(SpawnTransform);
			RightController->AttachToComponent(CameraBase, AttachRules);
		}
	}
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

