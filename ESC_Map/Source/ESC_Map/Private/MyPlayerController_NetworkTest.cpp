// Fill out your copyright notice in the Description page of Project Settings.
#include "MyPlayerController_NetworkTest.h"
#include "MyGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MyProtocol.h"
#include "Engine/BlueprintGeneratedClass.h"

AMyPlayerController_NetworkTest::AMyPlayerController_NetworkTest()
{
	ConstructorHelpers::FObjectFinder<UBlueprint> character_boy(TEXT("Blueprint'/Game/My/Blueprint/Character/Boy/BP_FPSCharacter_Boy.BP_FPSCharacter_Boy'"));
	ConstructorHelpers::FObjectFinder<UBlueprint> character_girl(TEXT("Blueprint'/Game/My/Blueprint/Character/Girl/BP_FPSCharacter_Girl.BP_FPSCharacter_Girl'"));
	
	if (character_boy.Object)
	{
		Characters[0] = character_boy.Object;
	}

	if (character_girl.Object)
	{
		Characters[1] = character_girl.Object;
	}
}

void AMyPlayerController_NetworkTest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (gameInst->SocketInstance)
	{
		cs_player_data_packet packet;
		
		FVector pos = MainPlayer->GetActorLocation();
		FRotator rat = MainPlayer->GetActorRotation();

		packet.x = pos.X * 100;
		packet.y = pos.Y * 100;
		packet.z = pos.Z * 100;
	
	    // short
		packet.pitch = rat.Pitch;
		packet.yaw = rat.Yaw;
		packet.roll = rat.Roll;
	
		packet.type = CS_PACKET::CS_PLAYER_DATA;
		packet.size = sizeof(packet);
		
		UE_LOG(LogTemp, Error, TEXT("Send! Type %d Position: %d %d %d"), packet.type, packet.x, packet.y, packet.z);
		gameInst->SocketInstance->Send(&packet);
	}
}

void AMyPlayerController_NetworkTest::BeginPlay()
{
	Super::BeginPlay();
	
	gameInst = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	if (!gameInst)
	{
		gameInst->playerController = this;
		UE_LOG(LogTemp, Warning, TEXT("Get Gemainstance Error!"));
	}
}

void AMyPlayerController_NetworkTest::PutPlayer(int playerType, bool isPlayer, const FVector& Location, const FRotator& Rotation)
{
	if (Characters[playerType])
	{
		APawn* pawn = GetWorld()->SpawnActor<APawn>(Characters[playerType]->GeneratedClass);
		pawn->SetActorLocationAndRotation(Location, Rotation);

		if (isPlayer)
		{
			MainPlayer = pawn;
			Possess(pawn);
		}
		else
		{
			OtherPlayer = pawn;
		}
	}
}

void AMyPlayerController_NetworkTest::MovePawn(const FVector& Location, const FRotator& Rotation)
{
	OtherPlayer->SetActorLocationAndRotation(Location, Rotation);
}
