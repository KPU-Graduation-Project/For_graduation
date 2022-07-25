// Fill out your copyright notice in the Description page of Project Settings.


#include "MapLoader.h"
#include "HoTGameInstance.h"
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

	gameInst = Cast<UHoTGameInstance>(GetWorld()->GetGameInstance());
	
	//gameInst->StreamManager.RequestAsyncLoad(map, FStreamableDelegate::CreateUObject(this, &AMapLoader::LoadComplete));
	//UE_LOG(LogTemp, Warning, TEXT("AsyncLoad Map %d"), gameInst->GetMapIndex());
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

	/*FSoftObjectPath map = gameInst->GetMap();
	if (map != nullptr)
	{
		UGameplayStatics::OpenLevel(this, map.GetAssetPathName());
		gameInst->ChangeMapIndex(1);
	}*/
	
}

void AMapLoader::LoadComplete()
{
	UE_LOG(LogTemp, Warning, TEXT("Map Load Complete"));
}