// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCannonBall.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AWeaponCannonBall::AWeaponCannonBall()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponet"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("CannonBall"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AWeaponCannonBall::OnHit);
	CollisionComponent->SetSphereRadius(15.0f);
	
	// ProjectileMovementComponent 를 사용하여 이 발사체의 운동을 관장합니다.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 2.5f;
}

// Called when the game starts or when spawned
void AWeaponCannonBall::BeginPlay()
{
	Super::BeginPlay();
	isAttached = false;
}

// Called every frame
void AWeaponCannonBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponCannonBall::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AWeaponCannonBall::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0, 0, 0));
		AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);
	}
}

