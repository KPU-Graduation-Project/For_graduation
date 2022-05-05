// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkModule.h"
#include "HoTGameInstance.h"
#include "Protocol.h"
#include "VRCharacter_Base.h"
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

void UNetworkModule::ShootBullet(FVector location, FRotator rotation)
{
	if (gameInst->CheckSend())
	{
		cs_shoot_bullet_packet packet;
		packet.type = CS_PACKET::CS_SHOOT_BULLET;
		packet.size = sizeof(packet);
		
		packet.x = location.X * 100;
		packet.y = location.Y * 100;
		packet.z = location.Z * 100;

		packet.pitch = rotation.Pitch * 100;
		packet.yaw = rotation.Yaw * 100;
		packet.roll = rotation.Roll * 100;

		gameInst->SocketInstance->Send(packet.size, &packet);
	}
}
