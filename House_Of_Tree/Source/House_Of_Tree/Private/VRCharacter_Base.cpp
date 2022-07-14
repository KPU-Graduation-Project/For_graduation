// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacter_Base.h"

// Sets default values
AVRCharacter_Base::AVRCharacter_Base()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Hand(
		TEXT("SkeletalMesh'/Game/Resource/Actor/Hand/MannequinHand_Right.MannequinHand_Right'"));

	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->SetRelativeLocationAndRotation(FVector(-15, 0, -88), FRotator(0, -90 ,0));
	GetMesh()->SetRelativeScale3D(FVector(2.5, 2.5, 2.5));

	VR_Root = CreateDefaultSubobject<USceneComponent>(TEXT("VR_Root"));
	VR_Root->SetupAttachment(RootComponent);
	
	VR_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	VR_Camera->SetupAttachment(VR_Root);
	VR_Camera->SetRelativeLocation(FVector(0, 0, 80));

	MotionController_L = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionContoller_L"));
	HandMesh_L = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh_L"));

	MotionController_R = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionContoller_R"));
	HandMesh_R = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh_R"));

	Hand_L = EControllerHand::Left;
	Hand_R = EControllerHand::Right;

	MotionController_L->SetupAttachment(VR_Root);
	MotionController_L->SetRelativeLocation(FVector(0, 0, 0));
	MotionController_L->SetCollisionProfileName(TEXT("BlockAll"));
	MotionController_L->SetTrackingSource(Hand_L);

	MotionController_R->SetupAttachment(VR_Root);
	MotionController_R->SetRelativeLocation(FVector(0, 0, 0));
	MotionController_R->SetCollisionProfileName(TEXT("BlockAll"));
	MotionController_R->SetTrackingSource(Hand_R);

	HandMesh_L->SetupAttachment(MotionController_L);
	HandMesh_L->SetRelativeRotation(FRotator(-30, 0, -90));
	HandMesh_L->SetRelativeScale3D(FVector(1, -1, 1));
	HandMesh_L->SetCollisionProfileName(TEXT("NoCollision"));
	HandMesh_L->CastShadow = false;

	HandMesh_R->SetupAttachment(MotionController_R);
	HandMesh_R->SetRelativeRotation(FRotator(-30, 0, 90));
	HandMesh_R->SetCollisionProfileName(TEXT("NoCollision"));
	HandMesh_R->CastShadow = false;


	if (SK_Hand.Succeeded())
	{
		HandMesh_L->SetSkeletalMesh(SK_Hand.Object);
		HandMesh_L->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		HandMesh_L->SetOnlyOwnerSee(true);

		HandMesh_R->SetSkeletalMesh(SK_Hand.Object);
		HandMesh_R->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		HandMesh_R->SetOnlyOwnerSee(true);
	}
}

// Called when the game starts or when spawned
void AVRCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVRCharacter_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AVRCharacter_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AVRCharacter_Base::SetLocationAndRotation(const FVector& location, const float& yaw)
{
	UE_LOG(LogActor, Display, TEXT("%lf, %lf, %lf"), location.X, location.Y, location.Z);

	FRotator rotation = GetActorRotation();
	rotation.Yaw = yaw;
	SetActorLocationAndRotation(location, rotation);
}

void AVRCharacter_Base::SetHandLocationAndRotation(const FVector& lhLocation, const FRotator& lhRotation,
                                                   const FVector& rhLocationRH, const FRotator& rhRotation)
{
	MotionController_L->SetWorldLocationAndRotation(lhLocation, lhRotation);
	MotionController_R->SetWorldLocationAndRotation(rhLocationRH, rhRotation);
}
