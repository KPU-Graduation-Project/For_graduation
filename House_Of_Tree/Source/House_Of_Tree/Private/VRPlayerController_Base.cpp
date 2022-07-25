// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPlayerController_Base.h"
#include "VRCharacter_Base.h"
#include "TestCharacter.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "ClientSocket.h"
#include "WeaponCannonBall.h"
#include "WeaponMatchBullet.h"
#include "Misc/OutputDeviceNull.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/SoftObjectPath.h"



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
		cs_player_data_packet sendPacket;
		sendPacket.type = CS_PACKET::CS_PLAYER_DATA;
		sendPacket.size = sizeof(sendPacket);
		sendPacket.id = gameInst->GetPlayerID();

		// ========================================== TestMode ========================================== //
		// ============================================================================================== //
		if (TestMode && TestPlayer)
		{
			// Character Transform
			FVector pos = TestPlayer->GetActorLocation();
			FRotator rat = TestPlayer->GetActorRotation();

			sendPacket.x = pos.X * 100;
			sendPacket.y = pos.Y * 100;
			sendPacket.z = pos.Z * 100;

			sendPacket.yaw = rat.Yaw * 100;

			sendPacket.rh_x = 0;
			sendPacket.rh_y = 0;
			sendPacket.rh_z = 0;

			sendPacket.rh_pitch = 0;
			sendPacket.rh_yaw = 0;
			sendPacket.rh_roll = 0;

			sendPacket.lh_x = 0;
			sendPacket.lh_y = 0;
			sendPacket.lh_z = 0;

			sendPacket.lh_pitch = 0;
			sendPacket.lh_yaw = 0;
			sendPacket.lh_roll = 0;

			sendPacket.Speed = 0;

			gameInst->SocketInstance->Send(sendPacket.size, &sendPacket);
		}
		// ============================================================================================== //
		// ============================================================================================== //

		else if (vrPlayer)
		{
			// Character Transform
			FVector pos = vrPlayer->GetActorLocation();
			FRotator rat = vrPlayer->GetMesh()->GetRelativeRotation();

			sendPacket.x = pos.X * 100;
			sendPacket.y = pos.Y * 100;
			sendPacket.z = pos.Z * 100;

			sendPacket.yaw = rat.Yaw * 100;

			// RH Transform
			FTransform trans = vrPlayer->GetRHTransform();
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

			sendPacket.Speed = vrPlayer->GetSpeed();

			gameInst->SocketInstance->Send(sendPacket.size, &sendPacket);
		}
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
				if (ProcessPacket(data) == false)
					return;
				remainData -= data[0];
				bufferSize -= data[0];
				memcpy(data, data + data[0], BUFSIZE - data[0]);
			}
		}
	}
}

bool AVRPlayerController_Base::ProcessPacket(char *p)
{
	UE_LOG(LogPlayerController, Display, TEXT("Packet type: %d"), p[1]);

	switch (p[1])
	{
	case SC_PACKET::SC_LOGINOK:
	{
		UE_LOG(LogPlayerController, Display, TEXT("SC_LOGINOK"));
		// 플레이어 입력 패킷인데 일단 비워두기
		sc_loginok_packet *packet = reinterpret_cast<sc_loginok_packet *>(p);
		gameInst->SetPlayerID(packet->id);

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
	break;

	case SC_PACKET::SC_USER_EXIT_ROOM:
	{
		UE_LOG(LogTemp, Error, TEXT("Recv Exit Room"));

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
	{
		UE_LOG(LogPlayerController, Display, TEXT("SC_START_GAME"));

		FSoftObjectPath map = gameInst->GetMap();
		if (map != nullptr)
		{
			gameInst->StreamManager.RequestAsyncLoad(map, FStreamableDelegate::CreateUObject(this, &AVRPlayerController_Base::LoadComplete));
			UE_LOG(LogTemp, Warning, TEXT("AsyncLoad Map"));
		}

		gameInst->GameStart();
	}
	break;

	case SC_PACKET::SC_DEBUG_SINGLE_START_GAME:
	{
		UE_LOG(LogPlayerController, Display, TEXT("SC_DEBUG_SINGLE_START_GAME"));

		FSoftObjectPath map = gameInst->GetMap();
		if (map != nullptr)
		{
			gameInst->StreamManager.RequestAsyncLoad(map, FStreamableDelegate::CreateUObject(this, &AVRPlayerController_Base::LoadComplete));
			UE_LOG(LogTemp, Warning, TEXT("AsyncLoad Map"));
		}

		gameInst->GameStart();
	}
	break;

	case SC_PACKET::SC_ALL_USERS_LOADING_COMPLETE:
	{
		UE_LOG(LogPlayerController, Display, TEXT("SC_ALL_USERS_LOADING_COMPLETE"));

		gameInst->AllLoadComplete();
	}
	break;


	case SC_PACKET::SC_PUT_OBJECT:
	{
		sc_put_object_packet *packet = reinterpret_cast<sc_put_object_packet *>(p);
		if (gameInst->GetActor(packet->object_type) == nullptr)
			break;

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

		// 자신의 캐릭터의 정보거나 잘못된 id라면 취소
		if (packet->id == gameInst->GetPlayerID() || actorList.Contains(packet->id) == false)
		{
			UE_LOG(LogPlayerController, Display, TEXT("It's you"));
			break;
		}

		if (!TestMode)
		{
			AVRCharacter_Base *otherPlayer = Cast<AVRCharacter_Base>(actorList[packet->id]);
			if (otherPlayer == nullptr) break;

			FVector location;
			float yaw;

			location.X = static_cast<float>(packet->x) / 100;
			location.Y = static_cast<float>(packet->y) / 100;
			location.Z = static_cast<float>(packet->z) / 100;

			yaw = static_cast<float>(packet->yaw) / 100;

			otherPlayer->SetLocationAndRotation(location, yaw);

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

			otherPlayer->SetSpeed(static_cast<float>(packet->Speed) / 100);
		}
	}
	break;

	case SC_PACKET::SC_SHOOT_BULLET:
	{
		sc_shoot_bullet_packet *packet = reinterpret_cast<sc_shoot_bullet_packet *>(p);
		UClass *bullet = gameInst->GetBullet(packet->bullet_type);
		if (bullet == nullptr)
			break;

		FVector location;
		FRotator rotation;

		location.X = static_cast<float>(packet->x) / 100;
		location.Y = static_cast<float>(packet->y) / 100;
		location.Z = static_cast<float>(packet->z) / 100;

		rotation.Pitch = static_cast<float>(packet->pitch) / 100;
		rotation.Yaw = static_cast<float>(packet->yaw) / 100;
		rotation.Roll = static_cast<float>(packet->roll) / 100;

		// need scale

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

	case SOCKET_ERROR:
		UE_LOG(LogNet, Error, TEXT("Socket Error!!"));
		UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
		return false;

	default:
		UE_LOG(LogNet, Error, TEXT("Recv Wrong Pacekt!!"));
		//UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
		//return false;
		break;
	}
	return true;
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

	UE_LOG(LogPlayerController, Display, TEXT("player ID: %d, actor ID: %d"), gameInst->GetPlayerID(), actorID);

	// character Possess
	if (gameInst->GetPlayerID() == actorID)
	{
		SetPlayerCharacter(objectID);
	}
}

void AVRPlayerController_Base::SetPlayerCharacter(const int objectID)
{
	vrPlayer = Cast<AVRCharacter_Base>(actorList[gameInst->GetPlayerID()]);
	if (vrPlayer)
	{
		TestMode = false;

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

	// ========================================== TestMode ========================================== //
	// ============================================================================================== //
	else if (Cast<ATestCharacter>(actorList[gameInst->GetPlayerID()]) != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("TEST PLAYER MODE")));

		TestMode = true;
		TestPlayer = Cast<ATestCharacter>(actorList[gameInst->GetPlayerID()]);

		UnPossess();
		Possess(TestPlayer);

		if (objectID % 10 == 3)
		{
			playertype = PLAYERTYPE::GIRL;
			UE_LOG(LogPlayerController, Warning, TEXT("TEST GIRL"));
		}
		else
		{
			playertype = PLAYERTYPE::BOY;
			UE_LOG(LogPlayerController, Warning, TEXT("TEST BOY"));
		}

		gameInst->GameStart();
	}
	// ============================================================================================== //
	// ============================================================================================== //
}

void AVRPlayerController_Base::LoadComplete()
{
	cs_loading_complete_packet sendPacket;
	sendPacket.type = CS_PACKET::CS_LOADING_COMPLETE;
	sendPacket.size = sizeof(sendPacket);

	gameInst->SocketInstance->Send(sendPacket.size, &sendPacket);
	UE_LOG(LogLoad, Display, TEXT("Send Load Complete"));


	FSoftObjectPath map = gameInst->GetMap();
	if (map != nullptr)
	{
		UGameplayStatics::OpenLevel(this, TEXT("GameMap"));
	}
}