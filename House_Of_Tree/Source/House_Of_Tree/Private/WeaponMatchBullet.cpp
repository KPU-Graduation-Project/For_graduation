// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponMatchBullet.h"

#include "HoTGameInstance.h"
#include "Protocol.h"
#include "VRCharacter_Base.h"
#include "VRPlayerController_Base.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWeaponMatchBullet::AWeaponMatchBullet()
{
	InitialLifeSpan = 5.0f;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("MatchBullet"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AWeaponMatchBullet::OnHit);
	CollisionComponent->SetSphereRadius(15.0f);

	RootComponent = CollisionComponent;

	// ProjectileMovementComponent 를 사용하여 이 발사체의 운동을 관장합니다.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AWeaponMatchBullet::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld() != nullptr && GetWorld()->GetGameInstance() != nullptr)
	{
		gameInst = Cast<UHoTGameInstance>(GetWorld()->GetGameInstance());
	}
	
	Speed = ProjectileMovementComponent->InitialSpeed;
}

// Called every frame
void AWeaponMatchBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ProjectileMovementComponent->MaxSpeed < Speed)
		return;

	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(Speed + Acceleration, 0, 0));
	Speed = Speed + Acceleration;
}

void AWeaponMatchBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0, 0, 0));
		Speed = 0;
		Acceleration = 0;
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

		if (gameInst->CheckSend() && gameInst->playerController->GetPlayerType() == PLAYERTYPE::GIRL)
		{
			cs_bullet_hit_packet packet;
			packet.type = CS_PACKET::CS_BULLET_HIT;
			packet.size = sizeof(packet);
			
			const int* key = gameInst->playerController->GetActorKey(OtherActor);
			if (key == nullptr)
				packet.object_id = 0;
			else
				packet.object_id = *key;

			const int* actorID = gameInst->playerController->GetActorKey(this);
			if (actorID == nullptr) return;
			packet.bullet_id = *actorID;
			
			FVector location = GetTransform().GetLocation();
			FRotator rotation = GetTransform().GetRotation().Rotator();
		
			packet.x = location.X * 100;
			packet.y = location.Y * 100;
			packet.z = location.Z * 100;

			packet.pitch = rotation.Pitch * 100;
			packet.yaw = rotation.Yaw * 100;
			packet.roll = rotation.Roll * 100;

			UE_LOG(LogTemp, Error, TEXT("Send onHit"));
			gameInst->SocketInstance->Send(packet.size, &packet);
		}
	}
}