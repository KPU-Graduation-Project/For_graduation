// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController_NetworkTest.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MyProtocol.h"
#include "Engine/BlueprintGeneratedClass.h"

AMyPlayerController_NetworkTest::AMyPlayerController_NetworkTest()
{
	ConstructorHelpers::FObjectFinder<UBlueprint> character_boy(TEXT("Blueprint'/Game/My/Blueprint/Character/Boy/BP_FPSCharacter_Boy.BP_FPSCharacter_Boy'"));
	if (character_boy.Object)
	{
		test = character_boy.Object;
	}
}

void AMyPlayerController_NetworkTest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (gameInst->SocketInstance)
	{
		cs_player_data_packet packet;

		FTransform tmp_transform = GetPawn()->GetTransform();
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
	

	if (test)
	{
		OtherPlayer = GetWorld()->SpawnActor(test->GeneratedClass);
		OtherPlayer->SetActorLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0,0));

		gameInst->OtherPlayer = OtherPlayer;
	}
}
