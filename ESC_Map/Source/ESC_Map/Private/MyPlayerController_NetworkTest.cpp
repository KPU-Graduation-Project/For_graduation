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
		cs_player_data packet;

		FTransform tmp_transform = GetPawn()->GetTransform();
		FVector pos = tmp_transform.GetLocation();
		FRotator rat = tmp_transform.GetRotation().Rotator();

		packet.player_position.x = pos.X * NetValue;
		packet.player_position.y = pos.Y * NetValue;
		packet.player_position.z = pos.Z * NetValue;
	
		packet.player_rotation.x = rat.Roll * NetValue;
		packet.player_rotation.y = rat.Pitch * NetValue;
		packet.player_rotation.z = rat.Yaw * NetValue;
	
		packet.type = CS_PLAYER_DATA;
		packet.size = sizeof(packet);
		
		UE_LOG(LogTemp, Error, TEXT("Send! Type %d Position: %d %d %d"), packet.type, packet.player_position.x, packet.player_position.y, packet.player_position.z);
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
	
	gameInst->SocketInstance->SetGameInstance(gameInst);

	if (test)
	{
		OtherPlayer = GetWorld()->SpawnActor(test->GeneratedClass);
		OtherPlayer->SetActorLocationAndRotation(FVector(0, 0, 0), FRotator(0, 0,0));

		gameInst->OtherPlayer = OtherPlayer;
	}
}
