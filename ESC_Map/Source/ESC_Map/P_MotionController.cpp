// Fill out your copyright notice in the Description page of Project Settings.


#include "P_MotionController.h"
#include "MotionControllerComponent.h"

// Sets default values
AP_MotionController::AP_MotionController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Hand(TEXT("SkeletalMesh'/Game/My/Actor/Hand/MannequinHand_Right.MannequinHand_Right'"));

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionContoller"));
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));

	RootComponent = RootScene;

	MotionController->SetupAttachment(RootComponent);
	MotionController->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MotionController->SetCollisionProfileName(TEXT("BlockAll"));

	HandMesh->SetupAttachment(MotionController);
	HandMesh->SetRelativeLocation(FVector(-12.8f, -0.03f, -1.8f));
	HandMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 90.0f));
	HandMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HandMesh->CastShadow = false;
	if (SK_Hand.Succeeded())
	{
		HandMesh->SetSkeletalMesh(SK_Hand.Object);
		HandMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}

	Hand = EControllerHand::Left;
}
void AP_MotionController::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);

	MotionController->SetTrackingSource(Hand);
}


// Called when the game starts or when spawned
void AP_MotionController::BeginPlay()
{
	Super::BeginPlay();

	if (Hand == EControllerHand::Left)
	{
		HandMesh->SetWorldScale3D(FVector(1.0f, 1.0f, -1.0f));
	}
}

// Called every frame
void AP_MotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

