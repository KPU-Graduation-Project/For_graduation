// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController_Base.h"

#include "EngineUtils.h"
#include "HoTGameInstance.h"
#include "GameFramework/Character.h"
#include "Protocol.h"	// 차후 경로 변경해야함

AVRPlayerController_Base::AVRPlayerController_Base()
{
}

void AVRPlayerController_Base::BeginPlay()
{
	Super::BeginPlay();

	gameInst = Cast<UHoTGameInstance>(GetWorld()->GetGameInstance());

	if (gameInst->ConnectNetwork == false)
	{
		MainPlayer = GetPawn();
		gameInst->SetInfo();
	}

	if (!gameInst)
	{
		gameInst->playerController = this;
		UE_LOG(LogTemp, Warning, TEXT("Get Gemainstance Error!"));
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

		// Need Rotation interpolation value
		sendPacket.pitch = rat.Pitch;
		sendPacket.yaw = rat.Yaw;
		sendPacket.roll = rat.Roll;

		gameInst->SocketInstance->Send(sendPacket.size, &sendPacket);
	}
}

void AVRPlayerController_Base::PutPlayer(int PlayerType, bool IsPlayer, const FVector& Location,
                                         const FRotator& Rotation)
{
	if (Characters[PlayerType])
	{
		APawn* pawn = GetWorld()->SpawnActor<APawn>(Characters[PlayerType]);
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
