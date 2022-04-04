// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"


// Sets default values
AMyPawn::AMyPawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Hand(TEXT("SkeletalMesh'/Game/My/Actor/Hand/MannequinHand_Right.MannequinHand_Right'"));

	VR_Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = VR_Root;

	VR_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	VR_Camera->SetupAttachment(RootComponent);
	
	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionContoller_L"));
	HandMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh_L"));
	
	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionContoller_R"));
	HandMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh_R"));
	
	Character = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chracter"));
	Character->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Character->SetOwnerNoSee(true);
	Character->SetupAttachment(RootComponent);
	
	Hand_L = EControllerHand::Left;
	Hand_R = EControllerHand::Right;

	MotionController_L->SetupAttachment(RootComponent);
	MotionController_L->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MotionController_L->SetCollisionProfileName(TEXT("BlockAll"));
	MotionController_L->SetTrackingSource(Hand_L);

	MotionController_R->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	MotionController_R->SetCollisionProfileName(TEXT("BlockAll"));
	MotionController_R->SetTrackingSource(Hand_R);

	HandMesh_L->SetupAttachment(MotionController_L);
	HandMesh_L->SetRelativeRotation(FRotator(-80.0f, 0.0f, -90.0f));
	HandMesh_L->SetRelativeScale3D(FVector(1.0f, -1.0f, 1.0f));
	HandMesh_L->SetCollisionProfileName(TEXT("NoCollision"));
	HandMesh_L->CastShadow = false;

	HandMesh_R->SetupAttachment(MotionController_R);
	HandMesh_R->SetRelativeRotation(FRotator(-80.0f, 0.0f, 90.0f));
	HandMesh_R->SetCollisionProfileName(TEXT("NoCollision"));
	HandMesh_R->CastShadow = false;
	
	if (SK_Hand.Succeeded())
	{
		HandMesh_L->SetSkeletalMesh(SK_Hand.Object);
		HandMesh_L->SetAnimationMode(EAnimationMode::AnimationBlueprint);

		HandMesh_R->SetSkeletalMesh(SK_Hand.Object);
		HandMesh_R->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

