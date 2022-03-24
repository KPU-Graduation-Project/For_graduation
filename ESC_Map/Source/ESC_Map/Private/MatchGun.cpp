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
}

// Called every frame
void AMatchGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
		UE_LOG(LogTemp, Warning, TEXT("TEST"));
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	}
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * power, Hit.ImpactPoint);
		cmp = OtherComponent;
		vel = ProjectileMovementComponent->Velocity;
		isHit = true;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("TEST"));
}
