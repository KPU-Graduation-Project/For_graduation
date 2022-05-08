// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMatchBullet.generated.h"

UCLASS()
class HOUSE_OF_TREE_API AWeaponMatchBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponMatchBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class UHoTGameInstance *gameInst;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	// Destory 함수를 전적으로 블루프린트화 시킴
	UFUNCTION(BlueprintImplementableEvent)
	void CallDestory();

	// 부딪히면 충격량
	UPROPERTY(EditAnywhere, Category = Projectile)
	float power = 1.f;

	// 가속도
	UPROPERTY(EditAnywhere, Category = Projectile)
	float Acceleration = 0.f;

	// 속도 증가용 내부변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile)
	float Speed;
};
