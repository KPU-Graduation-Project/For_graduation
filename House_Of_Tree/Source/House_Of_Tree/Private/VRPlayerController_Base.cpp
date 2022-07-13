// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController_Base.h"
#include "VRCharacter_Base.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "ClientSocket.h"
#include "WeaponCannonBall.h"
#include "WeaponMatchBullet.h"
#include "Misc/OutputDeviceNull.h"


AVRPlayerController_Base::AVRPlayerController_Base()
{}

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
		UE_LOG(LogPlayerController, Error, TEXT("Get GameInstance Error!"));
	}
}

void AVRPlayerController_Base::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RecvPacket();
	SendPlayerData();
}

void AVRPlayerController_Base::SendPlayerData()
{
	if (gameInst->CheckSend() && gameInst->IsIngame())
	{
		UE_LOG(LogPlayerController, Display, TEXT("Send Packet"));

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

void AVRPlayerController_Base::RecvPacket()
{
	if (gameInst->CheckSend())
	{
		int bufferSize = gameInst->SocketInstance->buffer.unsafe_size();

		while (remainData < bufferSize)
		{
			while (remainData < BUFSIZE && remainData < bufferSize)
			{
				gameInst->SocketInstance->buffer.try_pop(data[remainData++]);
			}

			while (remainData > 0 && data[0] <= remainData)
			{
				ProcessPacket(data);
				remainData -= data[0];
				bufferSize -= data[0];
				memcpy(data, data + data[0], BUFSIZE - data[0]);
			}
		}
	}
}

void AVRPlayerController_Base::ProcessPacket(char *p)
{
	UE_LOG(LogPlayerController, Display, TEXT("Packet type: %d"), p[1]);

	switch (p[1])
	{
	case SC_PACKET::SC_LOGINOK:
	{
		UE_LOG(LogPlayerController, Display, TEXT("SC_LOGINOK"));
		// 플레이어 입력 패킷인데 일단 비워두기
		sc_loginok_packet *packet = reinterpret_cast<sc_loginok_packet *>(p);
		playerID = packet->id;

		DE_SetID.Broadcast(packet->id);
	}
	break;

	case SC_PACKET::SC_CREATE_ROOM:
	{
		sc_create_room_packet *packet = reinterpret_cast<sc_create_room_packet *>(p);

		if (packet->room_id != -1)
		{
			// Change to Room UI
			DE_PlayerEnterRoom.Broadcast(packet->room_id, packet->is_host, packet->selected_character, packet->is_ready);
		}
	}
	break;

	case SC_PACKET::SC_JOIN_ROOM:
	{
		sc_join_room_packet *packet = reinterpret_cast<sc_join_room_packet *>(p);
		
		if (packet->room_id != -1)
		{
			// Change to Room UI
			DE_PlayerEnterRoom.Broadcast(packet->room_id, packet->is_host, packet->selected_character, packet->is_ready);
		}
	}
	break;

	case SC_PACKET::SC_USER_JOIN_ROOM:
	{
		sc_user_join_room_packet *packet = reinterpret_cast<sc_user_join_room_packet *>(p);

		// 여기는 상대방의 ID
		DE_UserEnterRoom.Broadcast(packet->id, packet->is_host, packet->selected_character, packet->is_ready);
	}

	case SC_PACKET::SC_USER_EXIT_ROOM:
	{
		sc_user_exit_room_packet *packet = reinterpret_cast<sc_user_exit_room_packet *>(p);

		DE_ExitRoom.Broadcast(packet->id);
	}
	break;

	case SC_PACKET::SC_CHANGE_USER_STATE:
	{
		sc_change_user_state_packet *packet = reinterpret_cast<sc_change_user_state_packet *>(p);

		DE_ChangeState.Broadcast(packet->id, packet->is_host, packet->selected_character, packet->is_ready);
	}
	break;

	case SC_PACKET::SC_START_GAME:
	case SC_PACKET::SC_DEBUG_SINGLE_START_GAME:
		UE_LOG(LogPlayerController, Display, TEXT("SC_START_GAME"));

		// 로비에서 인게임 맵으로 변경

		gameInst->GameStart();
		break;

	case SC_PACKET::SC_ALL_USERS_LOADING_COMPLETE:
		UE_LOG(LogPlayerController, Display, TEXT("SC_ALL_USERS_LOADING_COMPLETE"));

		gameInst->AllLoadComplete();
		break;

	case SC_PACKET::SC_PUT_OBJECT:
	{
		sc_put_object_packet *packet = reinterpret_cast<sc_put_object_packet *>(p);
		if (gameInst->GetActor(packet->object_type) == nullptr) return;

		UE_LOG(LogPlayerController, Display, TEXT("putobject %s"), *gameInst->GetActor(packet->object_type)->GetName());

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

		PutObject(packet->id, packet->object_type, location, rotation, scale, packet->mesh_id, packet->parent_object_id);
	}
	break;

	case SC_PACKET::SC_REMOVE_OBJECT:
	{
		sc_remove_object_packet *packet = reinterpret_cast<sc_remove_object_packet *>(p);

		AActor *removeActor;
		if (actorList.RemoveAndCopyValue(packet->id, removeActor) == false) break;
		removeActor->Destroy();
	}
	break;

	case SC_PACKET::SC_DESTROY_OBJECT:
	{
		sc_destroy_object_packet *packet = reinterpret_cast<sc_destroy_object_packet *>(p);

		AActor *removeActor;
		if (actorList.RemoveAndCopyValue(packet->id, removeActor) == false) break;

		if (Cast<AWeaponMatchBullet>(removeActor))
		{
			Cast<AWeaponMatchBullet>(removeActor)->CallDestory();
		}
		else if (Cast<AWeaponCannonBall>(removeActor))
		{
			Cast<AWeaponCannonBall>(removeActor)->CallDestory();
		}
	}
	break;

	case SC_PACKET::SC_OBJECT_DATA:
	{
		sc_object_data_packet *packet = reinterpret_cast<sc_object_data_packet *>(p);

		if (actorList.Contains(packet->id) == false) break;

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

		actorList[packet->id]->SetActorLocationAndRotation(location, rotation);
		actorList[packet->id]->SetActorScale3D(scale);
	}
	break;

	case SC_PACKET::SC_PLAYER_DATA:
	{
		sc_player_data_packet *packet = reinterpret_cast<sc_player_data_packet *>(p);
		//UE_LOG(LogTemp, Warning, TEXT("PlayerID %d SC_PLAYER_DATA %d"), playerID, packet->id);

		// 자신의 캐릭터의 정보거나 잘못된 id라면 취소
		if (packet->id == playerID || actorList.Contains(packet->id) == false)
		{
			UE_LOG(LogPlayerController, Display, TEXT("It's you"));

			return;
		}

		AVRCharacter_Base *otherPlayer = Cast<AVRCharacter_Base>(actorList[packet->id]);
		if (otherPlayer == nullptr) break;

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
	}
	break;

	case SC_PACKET::SC_SHOOT_BULLET:
	{
		sc_shoot_bullet_packet *packet = reinterpret_cast<sc_shoot_bullet_packet *>(p);
		UClass *bullet = gameInst->GetBullet(packet->bullet_type);
		if (bullet == nullptr) return;

		FVector location;
		FRotator rotation;

		location.X = static_cast<float>(packet->x) / 100;
		location.Y = static_cast<float>(packet->y) / 100;
		location.Z = static_cast<float>(packet->z) / 100;

		rotation.Pitch = static_cast<float>(packet->pitch) / 100;
		rotation.Yaw = static_cast<float>(packet->yaw) / 100;
		rotation.Roll = static_cast<float>(packet->roll) / 100;

		FActorSpawnParameters SpawnParams;
		actorList.Add(packet->id, GetWorld()->SpawnActor<AActor>(bullet, location, rotation, SpawnParams));
	}
	break;

	// Move to next map
	case SC_PACKET::SC_MOVE_SECTOR:
	{
		// ********************************** Need to Fill ********************************** //
	}
	break;

	case SC_PACKET::SC_OBJECT_UPDATE:
	{
		UE_LOG(LogPlayerController, Display, TEXT("SC_OBJECT_UPDATE"));
		sc_object_update_packet *packet = reinterpret_cast<sc_object_update_packet *>(p);

		if (!actorList.Contains(packet->object_id)) break;

		FOutputDeviceNull ar;
		FString command = FString::Printf(TEXT("SetRotation %f"), packet->direction == 1 ? -120.0 : 120.0);
		actorList[packet->object_id]->CallFunctionByNameWithArguments(*command, ar, NULL, true);
	}
	break;

	default:
		break;
	}
}

void AVRPlayerController_Base::PutObject(int actorID, int objectID, FVector location, FRotator rotation, FVector scale, int meshID,
	int parentID)
{
	if (gameInst->GetActor(objectID) == nullptr) return;

	UE_LOG(LogPlayerController, Display, TEXT("Actor ID: %d, ObjectID: %d"), actorID, objectID);
	FActorSpawnParameters SpawnParams;

	// Set Transform
	actorList.Add(actorID, GetWorld()->SpawnActor<AActor>(gameInst->GetActor(objectID), location, rotation, SpawnParams));
	actorList[actorID]->SetActorScale3D(scale);

	// Set Mesh ID
	FOutputDeviceNull ar;
	FString command = FString::Printf(TEXT("SetMesh %d"), meshID);
	actorList[actorID]->CallFunctionByNameWithArguments(*command, ar, NULL, true);

	UE_LOG(LogPlayerController, Display, TEXT("%d"), parentID);
	// Set Parent Actor
	if (parentID != 0 && actorList.Contains(parentID))
	{
		UE_LOG(LogPlayerController, Display, TEXT("your parent %s"), *actorList[parentID]->GetName());

		// 부모의 자식이 여럿이면 블루프린트에서 배열을 추가하는 방식으로
		command = FString::Printf(TEXT("SetChild %s"), *actorList[actorID]->GetName());

		actorList[parentID]->CallFunctionByNameWithArguments(*command, ar, NULL, true);
	}

	UE_LOG(LogPlayerController, Display, TEXT("player ID: %d, actor ID: %d"), playerID, actorID);

	// character Possess
	if (playerID == actorID)
	{
		SetPlayerCharacter(objectID);
	}
}

void AVRPlayerController_Base::SetPlayerCharacter(const int objectID)
{
	vrPlayer = Cast<AVRCharacter_Base>(actorList[playerID]);
	if (vrPlayer)
	{
		UnPossess();
		Possess(vrPlayer);

		if (objectID % 10 == 1)
		{
			playertype = PLAYERTYPE::GIRL;
			UE_LOG(LogPlayerController, Warning, TEXT("GIRL"));
		}
		else
		{
			playertype = PLAYERTYPE::BOY;
			UE_LOG(LogPlayerController, Warning, TEXT("BOY"));
		}

		gameInst->GameStart();
	}
}