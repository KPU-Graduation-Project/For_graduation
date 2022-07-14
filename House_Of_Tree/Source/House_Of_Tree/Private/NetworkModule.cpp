// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkModule.h"
#include "HoTGameInstance.h"
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
	if (gameInst->CheckSend() && gameInst->IsIngame())
	{
		UE_LOG(LogActorComponent, Display, TEXT("ShootBullet"));
		
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

void UNetworkModule::TargetSpin(AActor *actor, bool forward)
{
	if (gameInst->CheckSend() && gameInst->IsIngame() && gameInst->playerController->GetPlayerType() == PLAYERTYPE::GIRL)
	{
		const int *key = gameInst->playerController->GetActorKey(actor);
		if (key == nullptr) return;
		
		cs_object_update_packet packet;
		packet.type = CS_PACKET::CS_OBJECT_UPDATE;
		packet.size = sizeof(packet);

		packet.object_id = *key;
		packet.direction = forward? 1 : 0;

		gameInst->SocketInstance->Send(packet.size, &packet);
		UE_LOG(LogActorComponent, Display, TEXT("Send TargetSpin %d"), packet.direction);
	}
}

void UNetworkModule::CreateRoom()
{
	if (!gameInst->CheckSend()) return;

	cs_create_room_packet packet;
	packet.type = CS_PACKET::CS_CREATE_ROOM;
	packet.size = sizeof(packet);

	gameInst->SocketInstance->Send(packet.size, &packet);
	UE_LOG(LogActorComponent, Display, TEXT("Send Create Room"));
}

// SC 패킷에서도 uint32로 변경하기
void UNetworkModule::JoinRoom(int roomID)
{
	if (!gameInst->CheckSend()) return;

	cs_join_room_packet packet;
	packet.type = CS_PACKET::CS_JOIN_ROOM;
	packet.size = sizeof(packet);
	packet.room_id = roomID;

	gameInst->SocketInstance->Send(packet.size, &packet);
	UE_LOG(LogActorComponent, Display, TEXT("Send Join Room"));
}

void UNetworkModule::ReadyGame(bool isReady)
{
	if (!gameInst->CheckSend()) return;

	cs_ready_game_packet packet;
	packet.type = CS_PACKET::CS_READY_GAME;
	packet.size = sizeof(packet);
	packet.is_ready = isReady;

	gameInst->SocketInstance->Send(packet.size, &packet);
	UE_LOG(LogActorComponent, Display, TEXT("Send Ready Game"));
}

void UNetworkModule::SelectCharacter(int selected)
{
	if (!gameInst->CheckSend()) return;
	cs_change_selected_character_packet packet;
	packet.type = CS_PACKET::CS_CHANGE_SELECTED_CHARACTER;
	packet.size = sizeof(packet);
	packet.selected_character = static_cast<char>(selected);

	gameInst->SocketInstance->Send(packet.size, &packet);
	UE_LOG(LogActorComponent, Display, TEXT("Send Change Character"));
}

void UNetworkModule::ExitRoom()
{
	if (!gameInst->CheckSend()) return;
	cs_exit_room_packet packet;
	packet.type = CS_PACKET::CS_EXIT_ROOM;
	packet.size = sizeof(packet);

	gameInst->SocketInstance->Send(packet.size, &packet);
	UE_LOG(LogActorComponent, Display, TEXT("Send Exit Room"));
}

void UNetworkModule::StartGame()
{
	if (!gameInst->CheckSend()) return;
	cs_start_game_packet packet;
	packet.type = CS_PACKET::CS_START_GAME;
	packet.size = sizeof(packet);

	gameInst->SocketInstance->Send(packet.size, &packet);
	UE_LOG(LogActorComponent, Display, TEXT("Send Game Start"));
}

void UNetworkModule::DebugGame()
{
	if (!gameInst->CheckSend()) return;

	cs_debug_single_start_game packet;
	packet.type = CS_PACKET::CS_DEBUG_SINGLE_START_GAME;
	packet.size = sizeof(packet);

	gameInst->SocketInstance->Send(packet.size, &packet);
	UE_LOG(LogActorComponent, Display, TEXT("Send Debug Game"));
}