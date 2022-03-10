// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController_NetworkTest.h"

#include "Kismet/GameplayStatics.h"

AMyPlayerController_NetworkTest::AMyPlayerController_NetworkTest()
{

}

void AMyPlayerController_NetworkTest::BeginPlay()
{
	Super::BeginPlay();
	
	gameInst = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(AActor::GetWorld()));
	
	if (!gameInst)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get Gemainstance Error!"));
	}
	
	PRT::CS_PLAYER_DATA packet;
	
	packet.player_position.x = 12;
	packet.player_position.y = 24;
	packet.player_position.z = 36;
	
	packet.player_rotation.x = 1;
	packet.player_rotation.y = 2;
	packet.player_rotation.z = 3;
	
	packet.info.type = 1;
	packet.info.size = sizeof(packet);
	
	
	UE_LOG(LogTemp, Error, TEXT("Send!"));
	gameInst->SocketInstance->Send(&packet);
}