// Fill out your copyright notice in the Description page of Project Settings.


#include "MatchGun.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AMatchGun::AMatchGun()
{
	InitialLifeSpan = 5.0f;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projecile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AMatchGun::OnHit);
	CollisionComponent->SetSphereRadius(15.0f);

	RootComponent = CollisionComponent;

	// ProjectileMovementComponent 를 사용하여 이 발사체의 운동을 관장합니다.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AMatchGun::BeginPlay()
{
	Super::BeginPlay();

	Speed = ProjectileMovementComponent->InitialSpeed;

}

// Called every frame
void AMatchGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(ProjectileMovementComponent->MaxSpeed < Speed)
		return;

	ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(Speed + Acceleration, 0, 0));
	Speed = Speed + Acceleration;
}

void AMatchGun::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AMatchGun::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                      FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0, 0, 0));
		Speed = 0;
		Acceleration = 0;
		//AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
		//UE_LOG(LogTemp, Warning, TEXT("Attach to actor"));
	}
}
