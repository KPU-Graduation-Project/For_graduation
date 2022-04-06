// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController_NetworkTest.h"

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

		
		FTransform tmp_transform = gameInst->MainPlayer->GetTransform();
		FVector pos = tmp_transform.GetLocation();
		FRotator rat = tmp_transform.GetRotation().Rotator();

		packet.x = pos.X;
		packet.y = pos.Y;
		packet.z = pos.Z;
	
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
		UE_LOG(LogTemp, Warning, TEXT("Get Gemainstance Error!"));
	}
	

	// 로그인 입력을 받으면 선택해서 스폰을 할 수 있게 수정해야함
	// 임시로 남자 캐릭터를 다른 플레이어에 배치하도록 되어있음
	if (Characters)
	{
		gameInst->MainPlayer = GetPawn();
		
		gameInst->OtherPlayer = GetWorld()->SpawnActor<APawn>(Characters[0]->GeneratedClass);
		gameInst->OtherPlayer->SetActorLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0,0));
	}
}
