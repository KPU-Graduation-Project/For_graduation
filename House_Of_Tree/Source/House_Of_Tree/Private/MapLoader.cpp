// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AMapLoader::AMapLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->BodyInstance.SetCollisionProfileName(TEXT("BlockAll"));
	Box->OnComponentHit.AddDynamic(this, &AMapLoader::OnHit);
}

// Called when the game starts or when spawned
void AMapLoader::BeginPlay()
{
	Super::BeginPlay();
	
	LoadPackageAsync(TEXT("Map_1_battle"), 0, PKG_ContainsMap);
}

// Called every frame
void AMapLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapLoader::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Error, TEXT("HIT"));
	UGameplayStatics::OpenLevel( this, TEXT( "Map_1_battle"));
	
}


