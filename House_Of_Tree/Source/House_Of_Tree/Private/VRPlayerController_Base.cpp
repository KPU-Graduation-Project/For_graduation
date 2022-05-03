// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController_Base.h"


#include "VRCharacter_Base.h"
#include "EngineUtils.h"
#include "HoTGameInstance.h"
#include "GameFramework/Character.h"
#include "Protocol.h"	// 차후 경로 변경해야함
#include "Misc/OutputDeviceNull.h"

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

// 	PutObject(100, 100002,
// 		FVector(-590.000000,-880.000000,85.998222),
// 		FRotator(0, 0, 0),
// 		FVector(1, 1, 1));
}

void AVRPlayerController_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ProcessPacket();
	SendPlayerData();
}

void AVRPlayerController_Base::SendPlayerData()
{
	if (gameInst->SocketInstance && gameInst->IsIngame())
	{
		UE_LOG(LogTemp, Error, TEXT("Send Packet"));

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
		trans = vrPlayer->GetLHTransform();
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

void AVRPlayerController_Base::ProcessPacket()
{
	// 버퍼사이즈 보다 큰 패킷덩어리가 왔을때 터지는거 막아야 함.
	char data[10000] = {};
	char* p = data;

	int dataSize = bufferSize.load();
	bufferSize -= dataSize;

	for (int i = 0; i < dataSize; ++i)
	{
		buffer.try_pop(data[i]);
	}

	while (dataSize > 0)
	{
		int packetSize = p[0];
		char packetType = p[1];

		UE_LOG(LogTemp, Warning, TEXT("%d"), packetType);

		switch (packetType)
		{
		case SC_PACKET::SC_LOGINOK:
			{
				UE_LOG(LogTemp, Warning, TEXT("loginOK"));
				// 플레이어 입력 패킷인데 일단 비워두기
				sc_loginok_packet* packet = reinterpret_cast<sc_loginok_packet*>(p);
				playerID = packet->id;
			}
			break;

		case SC_PACKET::SC_CREATE_ROOM:
			break;
		case SC_PACKET::SC_JOIN_ROOM:
			break;
		case SC_PACKET::SC_USER_JOIN_ROOM:
			break;

		// SC 어디감;
		case SC_PACKET::USER_EXIT_ROOM:
			break;

		case SC_PACKET::SC_USER_READY_GAME:
			break;

		case SC_PACKET::SC_USER_CHANGE_SELECTED_CHARACTER:
			break;

		case SC_PACKET::SC_START_GAME:
			UE_LOG(LogTemp, Warning, TEXT("SC_START_GAME"));

		// 로비에서 인게임 로비 캠으로 전환

			gameInst->GameStart();
			break;

		case SC_PACKET::SC_ALL_USERS_LOADING_COMPLETE:
			UE_LOG(LogTemp, Warning, TEXT("SC_ALL_USERS_LOADING_COMPLETE"));

			gameInst->AllLoadComplete();
			break;

		case SC_PACKET::SC_PUT_OBJECT:
			{
				sc_put_object_packet* packet = reinterpret_cast<sc_put_object_packet*>(p);
				// PutObject(packet->id, packet->object_type,
				// 	FVector(packet->x/100, packet->y/100, packet->z/100),
				// 	FRotator(packet->pitch/100, packet->yaw/100, packet->roll/100),
				// 	FVector(packet->scale_x / 100, packet->scale_y / 100, packet->scale_z / 100));
				UE_LOG(LogTemp, Warning, TEXT("putobject %s"), *gameInst->GetActor(packet->object_type)->GetName());

				FVector location, scale;
				FRotator rotation;

				location.X = static_cast<float>(packet->x) / 100;
				location.Y = static_cast<float>(packet->y) / 100;
				location.Z = static_cast<float>(packet->z) / 100;

				rotation.Pitch = static_cast<float>(packet->pitch) / 100;
				rotation.Yaw = static_cast<float>(packet->yaw) / 100;
				rotation.Roll = static_cast<float>(packet->roll) / 100;

				scale.X = static_cast<float>(packet->scale_x) / 100;
				scale.Y = static_cast<float>(packet->scale_y) / 100;
				scale.Z = static_cast<float>(packet->scale_z) / 100;
				
				PutObject(packet->id, packet->object_type, location, rotation, scale, packet->mesh_id);
			}
			break;

		case SC_PACKET::SC_REMOVE_OBJECT:
			break;

		case SC_PACKET::SC_PLAYER_DATA:
			{
				sc_player_data_packet* packet = reinterpret_cast<sc_player_data_packet*>(p);
				UE_LOG(LogTemp, Warning, TEXT("PlayerID %d SC_PLAYER_DATA %d"), playerID, packet->id);

				// 자신의 캐릭터의 정보거나 잘못된 id라면 취소
				if (packet->id == playerID || actorList.Contains(packet->id) == false)
				{
					UE_LOG(LogTemp, Warning, TEXT("It's you"));

					return;
				}
				
				AVRCharacter_Base* otherPlayer = Cast<AVRCharacter_Base>(actorList[packet->id]);

				FVector location;
				FRotator rotation;

				location.X = static_cast<float>(packet->x) / 100;
				location.Y = static_cast<float>(packet->y) / 100;
				location.Z = static_cast<float>(packet->z) / 100;

				rotation.Pitch = static_cast<float>(packet->pitch) / 100;
				rotation.Yaw = static_cast<float>(packet->yaw) / 100;
				rotation.Roll = static_cast<float>(packet->roll) / 100;

				otherPlayer->SetLocationAndRotation(location, rotation.Yaw);

				FVector lhLocation, rhLocation;
				FRotator lhRotation, rhRotation;

				lhLocation.X = static_cast<float>(packet->lh_x) / 100;
				lhLocation.Y = static_cast<float>(packet->lh_y) / 100;
				lhLocation.Z = static_cast<float>(packet->lh_z) / 100;
				lhRotation.Pitch = static_cast<float>(packet->lh_pitch) / 100;
				lhRotation.Yaw = static_cast<float>(packet->lh_yaw) / 100;
				lhRotation.Roll = static_cast<float>(packet->lh_roll) / 100;
				
				rhLocation.X = static_cast<float>(packet->rh_x) / 100;
				rhLocation.Y = static_cast<float>(packet->rh_y) / 100;
				rhLocation.Z = static_cast<float>(packet->rh_z) / 100;
				rhRotation.Pitch = static_cast<float>(packet->rh_pitch) / 100;
				rhRotation.Yaw = static_cast<float>(packet->rh_yaw) / 100;
				rhRotation.Roll = static_cast<float>(packet->rh_roll) / 100;

				otherPlayer->SetHandLocationAndRotation(lhLocation, lhRotation, rhLocation, rhRotation);
				break;
			}
		default:
			break;
		}

		p += packetSize;
		dataSize -= packetSize;
	}
}

void AVRPlayerController_Base::PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale, int meshID)
{
	UE_LOG(LogTemp, Error, TEXT("Actor ID: %d, ObjectID: %d"), actorID, objectID);
	FActorSpawnParameters SpawnParams;

	actorList.Add(actorID, GetWorld()->SpawnActor<AActor>(gameInst->GetActor(objectID), location, rotation, SpawnParams));
	actorList[actorID]->SetActorScale3D(scale);

	//char funcCallBuf[1024];
	FOutputDeviceNull ar;
	//_snprintf_s(funcCallBuf, sizeof(funcCallBuf), "%s %d", "SetMesh", meshID);
	actorList[actorID]->CallFunctionByNameWithArguments(TEXT("SetMesh ") + meshID, ar, NULL, true);

	if (playerID == actorID)
	{
		SetPlayerCharacter();
	}
}

void AVRPlayerController_Base::SetPlayerCharacter()
{
	Possess(Cast<APawn>(actorList[playerID]));
	vrPlayer = Cast<AVRCharacter_Base>(GetCharacter());

	gameInst->GameStart();
}
