// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController_Base.h"
#include "HoTGameInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Protocol.h"	// 차후 경로 변경해야함


void AVRPlayerController_Base::BeginPlay()
{
	Super::BeginPlay();

	gameInst = Cast<UHoTGameInstance>(GetWorld()->GetGameInstance());

	MainPlayer = GetPawn();

	if (!gameInst)
	{
		gameInst->playerController = this;
		UE_LOG(LogTemp, Warning, TEXT("Get Gemainstance Error!"));
	}
}

AVRPlayerController_Base::AVRPlayerController_Base()
{
	static const ConstructorHelpers::FObjectFinder<UBlueprint> character_boy(TEXT("Blueprint'/Game/Asset/Actor/Character/BP_VRCharacter_Boy.BP_VRCharacter_Boy'"));
	static const ConstructorHelpers::FObjectFinder<UBlueprint> character_girl(TEXT("Blueprint'/Game/Asset/Actor/Character/BP_VRCharacter_Girl.BP_VRCharacter_Girl'"));

	if (character_boy.Object)
	{
		Characters[0] = character_boy.Object;
	}

	if (character_girl.Object)
	{
		Characters[1] = character_girl.Object;
	}
}

void AVRPlayerController_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (gameInst->SocketInstance)
	{
		cs_player_data_packet sendPacket;
		sendPacket.type = CS_PACKET::CS_PLAYER_DATA;
		sendPacket.size = sizeof(sendPacket);
		
		const FVector pos = MainPlayer->GetActorLocation();
		const FRotator rat = MainPlayer->GetActorRotation();

		sendPacket.x = pos.X * 100;
		sendPacket.y = pos.Y * 100;
		sendPacket.z = pos.Z * 100;
		
		// short Need Add Bias
		sendPacket.pitch = rat.Pitch;
		sendPacket.yaw = rat.Yaw;
		sendPacket.roll = rat.Roll;
		
		gameInst->SocketInstance->Send(sendPacket.size, &sendPacket);
	}
}

void AVRPlayerController_Base::PutPlayer(int PlayerType, bool IsPlayer, const FVector& Location, const FRotator& Rotation)
{
	if (Characters[PlayerType])
	{
		APawn* pawn = GetWorld()->SpawnActor<APawn>(Characters[PlayerType]->GeneratedClass);
		pawn->SetActorLocationAndRotation(Location, Rotation);

		if (IsPlayer)
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

void AVRPlayerController_Base::MovePawn(const FVector& Location, const FRotator& Rotation)
{
	OtherPlayer->SetActorLocationAndRotation(Location, Rotation);
}
