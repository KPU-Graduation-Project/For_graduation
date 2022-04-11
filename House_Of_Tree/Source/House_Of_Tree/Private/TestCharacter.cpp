// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 일인칭 카메라 컴포넌트를 생성합니다.
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// 카메라 컴포넌트를 캡슐 컴포넌트에 붙입니다.
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());

	// 카메라 위치를 눈 살짝 위쪽으로 잡습니다.
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	// 폰의 카메라 로테이션 제어를 허용합니다.
	FPSCameraComponent->bUsePawnControlRotation = true;

	// 일인칭 메시 컴포넌트입니다.
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// 소유 플레이어만 이 메시를 볼 수 있습니다.
	FPSMesh->SetOnlyOwnerSee(true);
	// FPS 메시를 FPS 카메라에 붙입니다.
	FPSMesh->SetupAttachment(FPSCameraComponent);
	// 일부 환경 섀도잉을 꺼 메시가 하나인 듯 보이는 느낌을 유지합니다.
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// 자신 이외 모두가 일반 몸통 메시를 볼 수 있습니다.
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &ATestCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &ATestCharacter::AddControllerPitchInput);
}

