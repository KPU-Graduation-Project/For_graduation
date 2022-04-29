// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController_Base.h"

#include <queue>

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

void AVRPlayerController_Base::ProcessPacket()
{
	char data[256] = {};
	char* p =  data;
	
	int dataSize = bufferSize.load();
	bufferSize -= dataSize;

	for (int i = 0; i<dataSize; ++i)
	{
		buffer.try_pop(data[i]);
	}

	while(dataSize > 0)
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
				UE_LOG(LogTemp, Warning, TEXT("putobject"));
				sc_put_object_packet* packet = reinterpret_cast<sc_put_object_packet*>(p);
				PutObject(packet->id, packet->object_type,
					FVector(packet->x/100, packet->y/100, packet->z/100),
					FRotator(packet->pitch/100, packet->yaw/100, packet->roll/100),
					FVector(packet->scale_x / 100, packet->scale_y / 100, packet->scale_z / 100));
			}
			break;

		case SC_PACKET::SC_REMOVE_OBJECT:
			break;

		case SC_PACKET::SC_PLAYER_DATA:
			UE_LOG(LogTemp, Warning, TEXT("SC_PLAYER_DATA"));
			break;
		
		default:
			break;
		}

		p += packetSize;
		dataSize -= packetSize;
	}
}

void AVRPlayerController_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ProcessPacket();

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


void AVRPlayerController_Base::PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale)
{
	UE_LOG(LogTemp, Error, TEXT("Actor ID: %d, ObjectID: %d"), actorID, objectID);
	FTransform transform;
	transform.SetLocation(location);
	transform.SetRotation(rotation.Quaternion());
	transform.SetScale3D(scale);
	FActorSpawnParameters SpawnParams;
	
	actorList.Add(actorID, GetWorld()->SpawnActor<AActor>(gameInst->GetActor(objectID), transform, SpawnParams));

	if (playerID == actorID)
	{
		SetPlayerCharacter();
	}
}

void AVRPlayerController_Base::SetPlayerCharacter()
{
	Possess(Cast<APawn>(actorList[playerID]));
	vrPlayer = Cast<AVRCharacter_Base>(GetCharacter());
}