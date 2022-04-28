// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController_Base.h"

#include <strmif.h>

#include "VRCharacter_Base.h"
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

	if (gameInst)
	{
		gameInst->playerController = this;
		gameInst->InitSocket();
		gameInst->SetInfo();
	}
	
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Get GameInstance Error!"));
	}
}

void AVRPlayerController_Base::SetPlayerCharacter(AActor* playerActor)
{
	Possess(Cast<APawn>(playerActor));
	vrPlayer = Cast<AVRCharacter_Base>(GetCharacter());
}

void AVRPlayerController_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (gameInst->SocketInstance && gameInst->IsIngame())
	{
		cs_player_data_packet sendPacket;
		sendPacket.type = CS_PACKET::CS_PLAYER_DATA;
		sendPacket.size = sizeof(sendPacket);
		sendPacket.id = playerID;

		// Character Transform
		FTransform trans = vrPlayer->GetTransform();
		FVector pos = trans.GetLocation();
		FRotator rat = trans.GetRotation().Rotator();

		sendPacket.x = pos.X * 100;
		sendPacket.y = pos.Y * 100;
		sendPacket.z = pos.Z * 100;

		sendPacket.pitch = rat.Pitch * 100;
		sendPacket.yaw = rat.Yaw * 100;
		sendPacket.roll = rat.Roll * 100;

		// Head Transform
		trans = vrPlayer->GetHeadTransform();
		pos = trans.GetLocation();
		rat = trans.GetRotation().Rotator();

		sendPacket.head_x = pos.X * 100;
		sendPacket.head_y = pos.Y * 100;
		sendPacket.head_z = pos.Z * 100;

		sendPacket.head_pitch = rat.Pitch * 100;
		sendPacket.head_yaw = rat.Yaw * 100;
		sendPacket.head_roll = rat.Roll * 100;

		// RH Transform
		trans = vrPlayer->GetRHTransform();
		pos = trans.GetLocation();
		rat = trans.GetRotation().Rotator();

		sendPacket.rh_x = pos.X * 100;
		sendPacket.rh_y = pos.Y * 100;
		sendPacket.rh_z = pos.Z * 100;

		sendPacket.rh_pitch = rat.Pitch * 100;
		sendPacket.rh_yaw = rat.Yaw * 100;
		sendPacket.rh_roll = rat.Roll * 100;

		// LH Transform
		trans = vrPlayer->GetRHTransform();
		pos = trans.GetLocation();
		rat = trans.GetRotation().Rotator();

		sendPacket.lh_x = pos.X * 100;
		sendPacket.lh_y = pos.Y * 100;
		sendPacket.lh_z = pos.Z * 100;
				   
		sendPacket.lh_pitch = rat.Pitch * 100;
		sendPacket.lh_yaw = rat.Yaw * 100;
		sendPacket.lh_roll = rat.Roll * 100;

		
		gameInst->SocketInstance->Send(sendPacket.size, &sendPacket);
	}
}