// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkModule.h"
#include "HoTGameInstance.h"
#include "VRPlayerController_Base.h"

// Sets default values for this component's properties
UNetworkModule::UNetworkModule()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UNetworkModule::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() != nullptr && GetWorld()->GetGameInstance() != nullptr)
	{
		gameInst = Cast<UHoTGameInstance>(GetWorld()->GetGameInstance());
	}
}
