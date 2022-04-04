// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController_NetworkTest.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "MyProtocol.h"
#include "Engine/BlueprintGeneratedClass.h"

AMyPlayerController_NetworkTest::AMyPlayerController_NetworkTest()
{

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
	
		packet.info.type = CS_PACKET::CS_PLAYER_DATA;
		packet.info.size = sizeof(packet);
		
		UE_LOG(LogTemp, Error, TEXT("Send! Type %d Position: %d %d %d"), packet.info.type, packet.player_position.x, packet.player_position.y, packet.player_position.z);
		gameInst->SocketInstance->Send(&packet);
	}
}

void AMyPlayerController_NetworkTest::BeginPlay()
{
	Super::BeginPlay();

	gameInst = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(AActor::GetWorld()));

	if (!gameInst)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get Gemainstance Error!"));
	}
	
	// UWorld* const world = GetWorld();
	// UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/My/Blueprint/BP_FPSCharacter.BP_FPSCharacter'")));
	// UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	//
	// FVector location;
	// FRotator rotation;
	// location = FVector(0, 0, 0);
	// rotation = FRotator(0, 0,0);
	// OtherPlayer = world->SpawnActor(GeneratedBP->GeneratedClass, &location, &rotation);
}
