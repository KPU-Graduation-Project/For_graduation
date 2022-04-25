// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CannonBall.generated.h"

UCLASS()
class ESC_MAP_API ACannonBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannonBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere,Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& ShootDirection);
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = Weight)
	float weight = 10.f;

	UPROPERTY(BlueprintReadOnly, Category = Projectile)
	bool isAttached;
};
